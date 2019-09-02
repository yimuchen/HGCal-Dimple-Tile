#!/usr/bin/python
import os, sys
import shlex
import subprocess
from datetime import datetime, date
import time
import shutil
import time
from random import randint
#sys.path.append(os.path.abspath(os.path.curdir))

JobTime = datetime.now()
fTag = JobTime.strftime("%Y%m%d_%H%M%S")
sTag = "DataCollectionTile"
dirname = "jobs/%s_%s" % (sTag, fTag)
DetType = "0"  #Tile
logFile = "0607.log"

try:
  os.makedirs(dirname)
except:
  pass

ProdTag = "Run4_20180730"
OutDir = "/data/users/ahorst/hgcal_tile/build/Absdata"
WorkDir = "/data/users/ahorst/hgcal_tile/build"

try:
  os.makedirs(OutDir)
except:
  pass

try:
  os.makedirs(OutDir + "/" + ProdTag)
except:
  pass

try:
  os.makedirs(OutDir + "/" + ProdTag + "/logs")
except:
  pass

######################%(WORKDIR)s %(INPUT)s %(FILENAME)s %(DETTYPE)s###################
# %(OUTDIR)s/%(MYPREFIX)s/

#array that contains the different abs lengths being tested
Arr = [-1.3, -0.5, 0.0, 0.5, 1.3]
Arr2 = [-1.3, -0.5, 0.0, 0.5, 1.3]


def random_N_digit(n):
  range_start = 10**(n - 1)
  range_end = (10**n) - 1
  return randint(range_start, range_end)


#Random Numbers to start with (1: 9 digit, 2: 8 digit) make sure these are in photontest too
RandS1 = 498333355
RandS2 = 72626948

q = 1
a = False
for x in range(len(Arr)):
  for y in range(len(Arr)):
    if q <= 25:
      #Replacement Random Numbers
      RandF1 = random_N_digit(9)
      RandF2 = random_N_digit(8)

      print("%s %s" % (Arr[x], Arr[y]))
      # Creating new file
      shutil.copy2('photontestbeams.mac', 'photontest.mac')

      # Reading in the file
      with open('photontest.mac', 'r') as file:
        filedata = file.read()

  # Replacing the target string
      a = " "
      j = str(Arr[x]) + a + str(Arr[y]) + a + str(0.0)
      i = str(0.0) + a + str(0.0) + a + str(0.0)
      filedata = filedata.replace(i, j)

      # Writing out the new file
      with open('photontest.mac', 'w') as file:
        file.write(filedata)

  # Reading in the file
      with open('photontest.mac', 'r') as file:
        filedata = file.read()
    # Replacing the target string
      a = " "
      j = str(RandF1) + a + str(RandF2)
      i = str(RandS1) + a + str(RandS2)
      filedata = filedata.replace(i, j)

      # Writing out the new file
      with open('photontest.mac', 'w') as file:
        file.write(filedata)

      if q == 13:
        # Reading in the file
        with open('photontest.mac', 'r') as file:
          filedata = file.read()
    # Replacing the target string
        a = " "
        j = str(3143)
        i = str(6736)
        filedata = filedata.replace(i, j)

        # Writing out the new file
        with open('photontest.mac', 'w') as file:
          file.write(filedata)

  # Defining the infile
      InFile = 'photontest.mac'

      condorcmd = "condor_submit condor-jobs.jdl"
      print 'condorcmd: ', condorcmd
      print('Executing condorcmd %s' % str(q))

      p = subprocess.Popen(condorcmd, shell=True)
      p.wait()

      time.sleep(0.05)
      q += 1

      #Make Sure Correct Numbers Are Replaced Next Loop
      RandF1 = RandS1
      RandF2 = RandS2

#time.sleep(20)
#command = ["condor_q ahorst"]
#p = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
#text= p.stdout.readlines()
#abort_after = 60 * 60
#start = time.time()
#while any(" 0 running" not in s for s in text):
#	time.sleep(10)
#	delta = time.time() - start
#	command = ["condor_q ahorst"]
#	p = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
#	text = p.stdout.readlines()
#	if any(" 0 running" in s for s in text):
    #		replacefile = ["mv Analysis.root Analysis[%s][%s].root" % (Arr[x], Arr[y])]
    #		p=subprocess.Popen(replacefile, shell=True)
    #		p.wait()
    #		remove = "rm photontest.mac"
    #		p = subprocess.Popen(remove, shell=True)
    #		p.wait()
    #          	break
    #	if delta >= abort_after:
    #          	break
    #continue
    else:
      q += 1
