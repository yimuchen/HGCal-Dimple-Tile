import ROOT
from ROOT import TCanvas, TGraphErrors
from ROOT import gROOT
from ROOT import TFile, TNtuple
from array import array
Arr = array('f', [-1.2, -0.7, -0.3, 0.0, 0.3, 0.7])
Arr.append(1.2)
detxs = array('f', [22, 24, 21, 18, 23, 23, 27])
errxs = array('f', [6.5, 3.32, 3.321, 2.404, 2.393, 1.903, 3.822])
detys = array('f', [24, 21, 22, 18, 22, 25, 27])
errys = array('f', [7.23, 4.11, 3.361, 2.893, 2.938, 1.233, 5.432])
err = array('f', [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
nxs = 7
#xs = TFile( 'xs.root', 'create' )
#xs.cd()
c1 = TCanvas('c1', 'Y', 200, 10, 700, 500)
gxs = TGraphErrors(nxs, Arr, detxs, err, errxs)
gxs.SetTitle('Y=0 Detected Photons')
gxs.Draw('ALP')
c1.Print("xs.png")
#gxs.SetName("gxs")
#gxs.Write( 'xs.root' )
#ys = TFile( 'ys.root', 'create' )
#ys.cd()
c2 = TCanvas('c1', 'Y', 200, 10, 700, 500)
gys = TGraphErrors(nxs, Arr, detys, err, errys)
gys.SetTitle('X=0 Detected Photons')
gys.Draw('ALP')
c2.Print("ys.png")
#gys.SetName("gys")
#gys.Write( 'ys.root' )