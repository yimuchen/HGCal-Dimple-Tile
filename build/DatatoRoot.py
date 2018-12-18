# This script grabs data from FinalShallow.csv printed out py SubmitJobs.py at the end of a run
# it creates root files for each set of data separated by a string.

import sys, string, os
from ROOT import TFile, TNtuple
from ROOT import TCanvas, TGraphErrors, TLegend
from ROOT import gROOT
from array import array
ifn = os.path.expandvars("FinalShallowGraph.csv")
ofn = 'FinalShallowDec17.root'
print ('opening file'), ifn, '...'
infile = open( ifn, 'r' )
lines  = infile.read().splitlines()
title  = "Mean Detected Photons Per Muon"
print ('writing file'), ofn, '...'
outfile = TFile( ofn, 'RECREATE', 'ROOT file with an NTuple' )
i=0
Arrx = {}
Det = {}
Errs = {}
Err = {}
labels = []
for line in lines:
	words = line.split(",")
	if len(words) == 1:
		labels.append(words)
		i+=1
		if i > 1:
			print("Would you like to make a histogram of %s? [y/n]" % (labels[i-2]))
			yes = ('yes','y','ye')
			no = ('no','n')
			name = labels[i-2]
			hist = str(labels[i-2])
			hist = hist.strip("['']")
			print(hist)
			choice = raw_input()
			if choice in yes:
				name = TGraphErrors(len(Arrx[i-1]), Arrx[i-1], Det[i-1], Err[i-1], Errs[i-1])
				name.SetTitle(" ;X [CM];Hits [PE]")
				name.SetMarkerColor(i*3)
				name.SetLineColor(i*3)
				#name.SetYTitle("Hits [PE]")
				name.Write("%s" % (hist))
			elif choice in no:
				p = True
			else:
				sys.stdout.write("Please respond with 'yes' or 'no'")
		Arrx[i] = array( 'f', [])
		Det[i] = array( 'f', [])
		Errs[i] = array( 'f', [])
		Err[i] = array( 'f', [])
	else:
		Arrx[i].append(float(words[0]))
		Det[i].append(float(words[2]))
		Errs[i].append(float(words[3]))
		Err[i].append(float(words[1]))
print ('done')