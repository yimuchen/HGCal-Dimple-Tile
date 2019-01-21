#!/usr/bin/python
import os, sys, string
import shlex 
import subprocess
from datetime import datetime, date
import time
import shutil
import time
from random import randint
import random
import math
import ROOT
from ROOT import TFile, TNtuple
from ROOT import TCanvas, TGraphErrors
from ROOT import gROOT
from array import array

deft = math.sqrt((2*(4.5)-1.6)*1.6)
##################################################
## Set Dimple Parameters ##
Rad = 3.0*deft  #Dimple radius mm (default 3.4409)
Din = 1.1*1.6	#Dimple depth in mm (defaut 1.6)
## Set Source ##
Source = 0	#0 ->Trigger (spread inside area), 1 ->Point Source
##################################################
if Din > Rad:
	print("Depth cannot exceed radius")
	exit()
if Din > 2.98:
	print("Depth cannot exceed thickness of tile")
	exit()
DCfile = open("/data/users/ahorst/hgcal_tile/TILE/src/LYSimDetectorConstruction.cc")
for line in DCfile:
	if "Din" in line:
		dinline = line.replace("    Din = ","")
		dinline = dinline.replace("*mm;","")
		dinline = eval(dinline)
		dinline = float(dinline)
		if dinline == Din:
			print("Depth matches source")
		else:
			print("Depth does not match source! Quitting..")
			exit()
	elif "Rad" in line and "*mm;" in line:
		radline = line.replace("    Rad = ","")
		radline = radline.replace("*deft*mm;//3.4409*mm","")
		radline = eval(radline)
		radline = float(radline)
		if deft*radline == Rad:
			print("Radius matches source")
		else:
			print("Radius does not match source! Quitting..")
			exit()
	elif "DimpleType" in line:
		Dimpleline = line.replace("    DimpleType = ","")
		Dimpleline = Dimpleline.replace("; //0: Normal, 1: Pyramid, 2: Parabolic","")
		Dimpleline = float(Dimpleline)
		if Dimpleline == 0:
			norm=1
			pyramid=0
			para=0
		if Dimpleline == 1:
			pyramid=1
			norm=0
			para=0
		if Dimpleline == 2:
			para=1
			norm=0
			pyramid=0
	elif "G4VPhysicalVolume* LYSimDetectorConstruction::ConstructDetector()" in line:
		break

JobTime = datetime.now()
fTag = JobTime.strftime("%Y%m%d_%H%M%S")
sTag = "DataCollectionTile"
dirname = "jobs/%s_%s"%(sTag,fTag)
DetType = "0" #Tile
logFile = "0607.log"

try:
    os.makedirs(dirname)
except:
    pass

ProdTag = "Run4_20180730"
OutDir  = "/data/users/ahorst/hgcal_tile/build/Absdata"
WorkDir = "/data/users/ahorst/hgcal_tile/build"

try:
    os.makedirs(OutDir)
except:
    pass

try:
    os.makedirs(OutDir+"/"+ProdTag)
except:
    pass

try:
   os.makedirs(OutDir+"/"+ProdTag+"/logs")
except:
    pass


#array that contains the different points being tested                    
Arr = [-1.2, -0.7, -0.3, 0.0, 0.3, 0.7, 1.2]
#Arr2 = [-1.4, -1.3, -1.2, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4]
Arr2 = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4]
Arr3 = [0.0]

def random_N_digit(n):
    range_start = 10**(n-1)
    range_end = (10**n)-1
    return randint(range_start, range_end)

#Random Numbers to start with (1: 9 digit, 2: 8 digit) make sure these are in photontest too
RandS1 = 825332965
RandS2 = 72622768
random.seed(datetime.now())
q = 0
a = False
for x in range(len(Arr2)):
	for y in range(len(Arr3)):
		q+=1
		if q in range(0,40):
			for w in range(1,11):
				for n in range (1, 1001):
					#Replacement Random Numbers
    					RandF1 = random_N_digit(9)
					RandF2 = random_N_digit(8)
					if n == 1:
						print("%s %s %s" % (Arr2[x], Arr3[y], str(q)))
 	               			# Creating new file                                                              
 		              		shutil.copy2('photontestbeams.mac', 'photontest.mac')

					### Write rand position of source 1###
		       			# Reading in the file                                                            
		       			with open('photontest.mac', 'r') as file:
		       				filedata = file.read()
		
					# Random Number inside source area
					if Source == 0:
						RandPosx = round(random.uniform(Arr2[x]-0.15,Arr2[x]+0.15), 4)
						RandPosy = round(random.uniform(Arr3[y]-0.15,Arr3[y]+0.15), 4)

		        		# Replacing the target string   
		        		a =" "
					if Source == 0:
		       				j = str(RandPosx)+a+str(RandPosy)+a+str(0.0)
					elif Source ==1:
						j = str(Arr2[x])+a+str(Arr3[y])+a+str(0.0)
	        			i = str(0.0)+a+str(0.0)+a+str(0.0)
	        			filedata = filedata.replace(i, j)

		        		# Writing out the new file                                                       
		        		with open('photontest.mac', 'w') as file:
		       				file.write(filedata)
				
		       			# Reading in the file                                                            
		       			with open('photontest.mac', 'r') as file:
		       				filedata = file.read()		
					# Replacing the target string
					a =" "
					j = str(RandF1)+a+str(RandF2)
					i = str(RandS1)+a+str(RandS2)
					filedata = filedata.replace(i, j)


		        		# Writing out the new file                                                       
		        		with open('photontest.mac', 'w') as file:
		       				file.write(filedata)
			
					#dforcond = math.sqrt(RandPosx**2+RandPosy**2)
			
					dforcond = math.sqrt(Arr2[x]**2+Arr3[y]**2)
				
					# Shallow Dimple Conditional for Fewer Source Photons
						#.3 for other dimple
					ra = Rad/10
					dep = Din/10
					big_rad = 0.5*((ra*ra)/dep + dep)
					#if (-ra <= Arr2[x] <= ra and -ra <= Arr3[y] <= ra):
					if dforcond <= ra:
	        				# Reading in the file                                                            
	        				with open('photontest.mac', 'r') as file:
		        				filedata = file.read()		
						# Replacing the target string
						#dfromcenter = math.sqrt(RandPosx**2+RandPosy**2)
						dfromcenter = math.sqrt(Arr2[x]**2+Arr3[y]**2)
					
						if norm == 1:
							thick = round((0.298 - (math.sqrt(big_rad**2 - dfromcenter**2) - (big_rad-dep))), 4)
						elif para == 1:
							thick = 0.298-abs(1.35*dfromcenter**2-dep)
						elif pyramid == 1:
							if abs(Arr3[y]) <= abs(Arr2[x]):
								thick = round((0.298 - (dep-(dep/ra)*(abs(Arr2[x])))), 4)
							elif abs(Arr2[y]) < abs(Arr3[x]):
								thick = round((0.298 - (dep-(dep/ra)*(abs(Arr3[y])))), 4)
						middle = -(0.298 - thick)/2

						a =" "
						j = a+str(thick/2)+a
						i = a+str(0.149)+a
						filedata = filedata.replace(i, j)
					
		        			# Writing out the new file                                                       
		        			with open('photontest.mac', 'w') as file:
		       					file.write(filedata)

						a =" "
						if Source==0:
							r = str(RandPosx)+a+str(RandPosy)+a+str(middle)
							e = str(RandPosx)+a+str(RandPosy)+a+str(0.0)
						elif Source==1:
							r = str(Arr2[x])+a+str(Arr3[y])+a+str(middle)
							e = str(Arr2[x])+a+str(Arr3[y])+a+str(0.0)
	        				filedata = filedata.replace(e, r)
	
		        			# Writing out the new file                                                       
		        			with open('photontest.mac', 'w') as file:
		        				file.write(filedata)
	

						numphoton = int(math.floor((6677/0.3)*thick)) 
						a =" "
						j = a+str(numphoton)
						i = a+str(6677)
						#6736 (10,000) 7410 (11,000)
						filedata = filedata.replace(i, j)


		       				# Writing out the new file                                                       
		       				with open('photontest.mac', 'w') as file:
		       					file.write(filedata)

					# Shallow Dimple Conditional for Fewer Source Photons
			
		        		# Defining the infile                                                            
		        		InFile = 'photontest.mac' 

  		       			condorcmd = "condor_submit condor-jobs.jdl"
	               	
					#if w == 1:
					#	print ('%s' % str(w))
	
	                		p=subprocess.Popen(condorcmd, shell=True)
	                		p.wait()
                
                	
	        			#Make Sure Correct Numbers Are Replaced Next Loop
	       				RandF1 = RandS1
	       				RandF2 = RandS2

					if n == 1000:
	               				command = ["condor_q ahorst"]
	               				p = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
	               				text= p.stdout.readlines()
	               				abort_after = 60 * 60
	       					start = time.time()
	               				while any("0 idle, 0 running, 0 held" not in s for s in text):
 	           					time.sleep(1)
 		               				delta = time.time() - start
	                				command = ["condor_q ahorst"]
	                 				p = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
	                 				text = p.stdout.readlines()
	               					if any("0 idle, 0 running, 0 held" in s for s in text):
								errroots = 'find . -name "Analysis*.root" -size -3k -delete'
								p = subprocess.Popen(errroots, shell=True)
								p.wait()
								hadd = ["hadd Part[%d][%s][%s].root Analysis*.root" % (w, Arr2[x], Arr3[y])]
								p=subprocess.Popen(hadd, shell=True)
	 	       						p.wait()
								if q == 300:
									movefile = ["mv Analysis*.root HitData/RootsForTestPoint"]
									p=subprocess.Popen(movefile, shell=True)
	                						p.wait()
								else:
									removefiles = ["rm Analysis*.root"]
									p=subprocess.Popen(removefiles, shell=True)
 		               						p.wait()
								remove = "rm photontest.mac photontest_*.mac Analysis*.txt *.condor *.stdout *.stderr"
								p = subprocess.Popen(remove, shell=True)
								p.wait()
                 	 	         			break
	                 				if delta >= abort_after:
	                 		  	     	  	end = "condor_rm ahorst"
								p = subprocess.Popen(end, shell=True)
								p.wait()
				if w == 10:
					hadd = ["hadd Total[%s][%s].root Part*.root" % (Arr2[x], Arr3[y])]
					p=subprocess.Popen(hadd, shell=True)
	 	               		p.wait()
					f = ROOT.TFile("Total[%s][%s].root" % (Arr2[x], Arr3[y]))
					t = f.Get("Ndetect")
					meant = t.GetMean()
					meant = meant
					rms = t.GetRMS()
					ent = t.GetEntries()
					err = rms/math.sqrt(ent)
					with open('FinalShallow.csv', 'a') as file:
						file.write("\n%f,%f,%f,%f" % (Arr2[x], Arr3[y], meant, err))
					rmparts = ["rm Part*.root"]
					p=subprocess.Popen(rmparts, shell=True)
 		               		p.wait()

