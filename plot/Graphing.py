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
from ROOT import TCanvas, TGraphErrors, TLegend
from ROOT import gROOT
from array import array
#ifn = os.path.expandvars("FinalShallow.csv")
#ofn = 'FinalShallow.root'
#print ('opening file'), ifn, '...'
#infile = open( ifn, 'r' )
#lines  = infile.read().splitlines()
#title  = "Mean Detected Photons Per Muon"
#labels = lines[0].split(",")
#print ('writing file'), ofn, '...'
#outfile = TFile( ofn, 'RECREATE', 'ROOT file with an NTuple' )
#ntuple  = TNtuple( 'ntuple', title, string.join( labels, ':') )
#for line in lines[1:]:
#    words = line.split(",")
#    row = map( float, words )
#    apply( ntuple.Fill, row )
#outfile.Write()
#print(labels)
#print ('done')
#TH2F* h3 = new TH2F("Ndetect", "Mean Detected Photons Per Muon", 20, -2 , 2, 20, -2, 2) use this to define a 2d histogram with 20 bins in x and y axis (range -6 to 6)
#ntuple->Draw("x:y>>Ndetect","det","SURF3") use this to fill the histogram using the ntuple created in this .py, using 'avg' as n for the histogram, SURF3 is a draw option
#make sure in the fill function above, the arguments (x,y,avg in this case) match the printed 'labels', otherwise the histogram will not fill correctly
#30x30
Arr1 = array( 'f', [-1.8, -1.5, -1.4, -1.3, -1.2, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.8])
det1s = array( 'f', [0.0, 5.632, 11.198, 11.222, 11.038, 11.254, 11.072, 11.621, 11.952, 11.884, 12.170, 13.022, 13.651, 13.989, 13.917, 14.700, 15.548, 14.842, 13.962, 13.635, 13.255, 12.652, 12.212, 11.831, 11.650, 11.670, 11.527, 11.264, 11.117, 10.975, 10.466, 5.463, 0.0])
err1s = array( 'f', [0.0, 0.313, 0.121, 0.097, 0.094, 0.096, 0.099, 0.104, 0.104, 0.106, 0.116, 0.114, 0.154, 0.152, 0.166, 0.192, 0.232, 0.184, 0.133, 0.173, 0.102 , 0.121, 0.102, 0.104, 0.113, 0.118, 0.101, 0.107, 0.111, 0.092, 0.097, 0.317, 0.0])
err1 = array( 'f', [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
#30x30 2X
Arr2x = array( 'f', [-1.8, -1.5, -1.4, -1.3, -1.2, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.8])
det2xs = array( 'f', [0.0, 10.232, 22.218, 22.432, 22.538, 22.654, 22.472, 23.221, 23.952, 23.844, 24.170, 26.022, 26.907, 28.955, 33.096, 39.075, 40.084, 38.295, 34.259, 28.991, 26.957, 25.152, 24.312, 23.831, 23.250, 23.170, 23.027, 22.364, 22.147, 21.775, 20.766, 10.513, 0.0])
err2xs = array( 'f', [0.0, 0.313, 0.121, 0.097, 0.094, 0.096, 0.099, 0.104, 0.104, 0.106, 0.116, 0.114, 0.154, 0.152, 0.166, 0.192, 0.232, 0.184, 0.133, 0.173, 0.102 , 0.121, 0.102, 0.104, 0.113, 0.118, 0.101, 0.107, 0.111, 0.092, 0.097, 0.317, 0.0])
err2x = array( 'f', [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
#15-weak
Arr2 = array( 'f', [-1.8, -1.5, -1.4, -1.3, -1.2, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.8])
det2s = array( 'f', [0.0, 6.539, 12.659, 13.166, 12.897, 13.327, 13.165, 13.433, 13.453, 14.613, 14.000, 14.925, 15.253, 15.234, 15.931, 16.240, 16.422, 16.165, 15.679, 15.519, 15.368, 15.071, 14.101, 14.838, 14.418, 13.440, 12.064, 7.864, 6.040, 3.526, 2.820, 0.0, 0.0])
err2s = array( 'f', [0.0, 0.163, 0.138, 0.112, 0.108, 0.111, 0.106, 0.104, 0.092, 0.106, 0.118, 0.113, 0.151, 0.163, 0.154, 0.227, 0.178, 0.182, 0.159, 0.164, 0.154, 0.124, 0.110, 0.102, 0.113, 0.114, 0.115, 0.115, 0.096, 0.092, 0.114, 0.283, 0.0])
err2 = array( 'f', [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
#15-tight
Arr3 = array( 'f', [-1.8, -1.5, -1.4, -1.3, -1.2, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.8])
det3s = array( 'f', [0.0, 6.822, 13.259, 13.273, 13.867, 14.111, 13.787, 13.603, 14.320, 14.959, 14.699, 15.176, 15.382, 15.504, 15.117, 17.046, 16.224, 15.923, 15.371, 17.166, 17.052, 15.785, 15.333, 14.89, 14.756, 14.345, 13.024, 7.950, 6.528, 4.168, 3.142, 0.0, 0.0])
err3s = array( 'f', [0.0, 0.269, 0.130, 0.105, 0.110, 0.102, 0.105, 0.104, 0.106, 0.113, 0.111, 0.153, 0.165, 0.176, 0.159, 0.174, 0.183, 0.129, 0.174, 0.157, 0.115, 0.112, 0.114, 0.107, 0.109, 0.145, 0.176, 0.232, 0.272, 0.336, 0.342, 0.294, 0.0])
err3 = array( 'f', [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
#30x20
Arr4 = array( 'f', [-1.8, -1.5, -1.4, -1.3, -1.2, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.8])
det4s = array( 'f', [0.0, 8.193, 16.529, 15.620, 16.147, 16.854, 16.436, 16.290, 17.201, 16.839, 18.028, 18.266, 18.563, 18.782, 18.922, 19.104, 19.436, 19.194, 18.532, 17.764, 16.422, 14.992, 8.748, 4.734, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
err4s = array( 'f', [0.0, 0.318, 0.138, 0.111, 0.102, 0.107, 0.127, 0.112, 0.114, 0.122, 0.131, 0.105, 0.123, 0.134, 0.153, 0.124, 0.102, 0.143, 0.125, 0.098, 0.115, 0.143, 0.232, 0.328, 0.395, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
err4 = array( 'f', [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
#20x20
Arr5 = array( 'f', [-1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0])
det5s = array( 'f', [10.532, 23.169, 24.020, 24.086, 24.410, 24.671, 22.142, 18.479, 16.205, 17.676, 18.475, 17.173, 16.702, 18.884, 21.872, 24.723, 24.314, 24.122, 23.831, 23.692, 10.672])
err5s = array( 'f', [0.251, 0.172, 0.153, 0.142, 0.155, 0.156, 0.253, 0.273, 0.259, 0.267, 0.343, 0.287, 0.248, 0.251, 0.255, 0.154, 0.148, 0.150, 0.148, 0.182, 0.239])
err5 = array( 'f', [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
#40x40
Arr6 = array( 'f', [-2.0, -1.8, -1.6, -1.4, -1.2, -1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0])
det6s = array( 'f', [3.002, 6.100, 6.024, 6.364, 6.701, 6.940, 7.706, 7.920, 8.293, 10.141, 11.660, 10.456, 8.198, 7.676, 7.642, 6.966, 6.677, 6.378, 6.243, 6.122, 3.024])
err6s = array( 'f', [0.102, 0.070, 0.073, 0.088, 0.078, 0.076, 0.080, 0.081, 0.100, 0.155, 0.189, 0.180, 0.107, 0.101, 0.086, 0.083, 0.076, 0.080, 0.075, 0.080, 0.110])
err6 = array( 'f', [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
n4s = 21
nxs = 33
n2s = 21
c1 = TCanvas( 'c1', 'Y', 200, 10, 1400, 1000 )

g1s = TGraphErrors(nxs, Arr1, det1s, err1, err1s)
g1s.SetTitle( 'Y=0 Detected PE' )
g1s.SetMarkerColor( 4 )
g1s.SetLineColor( 4 )
g1s.SetMarkerStyle( 21 )
g1s.Draw('APC')
g1s.GetYaxis().SetRangeUser(0,45)
g1s.GetYaxis().SetTitle("Hits [PE]")
g1s.GetXaxis().SetTitle("X [cm]")
g2xs = TGraphErrors(nxs, Arr2x, det2xs, err2x, err2xs)
g2xs.SetTitle( 'Y=0 Detected PE' )
g2xs.SetMarkerColor( 9 )
g2xs.SetLineColor( 9 )
g2xs.SetMarkerStyle( 5 )
g2xs.Draw('PC SAME')
#c1.Print("xs.png")
#c2 = TCanvas( 'c1', 'Y', 200, 10, 700, 500 )
'''
g2s = TGraphErrors(nxs, Arr2, det2s, err2, err2s)
g2s.SetTitle( 'Y=0 Detected PE' )
g2s.SetMarkerColor( 800 )
g2s.SetLineColor( 800 )
g2s.SetMarkerSize( 2 )
g2s.SetMarkerStyle( 22 )
g2s.Draw('PC SAME')
g3s = TGraphErrors(nxs, Arr3, det3s, err3, err3s)
g3s.SetTitle( 'Y=0 Detected PE' )
g3s.SetMarkerColor( 15 )
g3s.SetLineColor( 15 )
g3s.SetMarkerSize( 2 )
g3s.SetMarkerStyle( 29 )
g3s.Draw('PC SAME')
g4s = TGraphErrors(nxs, Arr4, det4s, err4, err4s)
g4s.SetTitle( 'Y=0 Detected PE' )
g4s.SetMarkerColor( 401 )
g4s.SetLineColor( 401 )
g4s.SetMarkerSize( 2 )
g4s.SetMarkerStyle( 34 )
g4s.Draw('PC SAME')

g5s = TGraphErrors(n2s, Arr5, det5s, err5, err5s)
g5s.SetTitle( 'Y=0 Detected PE' )
g5s.SetMarkerColor( 9 )
g5s.SetLineColor( 9 )
g5s.SetMarkerStyle( 5 )
g5s.Draw('PC SAME')
g5s.GetYaxis().SetRangeUser(0,30)
g6s = TGraphErrors(n4s, Arr6, det6s, err6, err6s)
g6s.SetTitle( 'Y=0 Detected Photons' )
g6s.SetMarkerColor( 8 )
g6s.SetLineColor( 8 )
g6s.SetMarkerStyle( 26 )
g6s.Draw('PC SAME')
g6s.GetYaxis().SetRangeUser(0,30)
'''
#gys.GetYaxis().SetRangeUser(0,30)
#gys.GetYaxis().SetTitle("Hits [PE]")
#gys.GetXaxis().SetTitle("Y [cm]")
#c2.Print("ys.png")
l = TLegend(0.6,0.7,0.85,0.9)
l.AddEntry('g1s','30x30','p')
l.AddEntry('g2s','15-weak','p')
l.AddEntry('g3s','15-tight','p')
l.AddEntry('g4s','30x20','p')
c1.Print("newTilestwo.png")