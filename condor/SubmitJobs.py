#!/usr/bin/python2
import os, sys, string
import subprocess
from datetime import datetime, date
import time
import time
from random import randint
import random
import math
from array import array

## Defining global variables.
USER_NAME = os.environ['USER']
BASE_DIR = os.path.dirname(os.path.abspath(__file__)) + '/../'
DATA_DIR = BASE_DIR + '/data/photontest/'

## These parameters should match those in Tile/src/LYSimDetectorConstruction.cc
DIMPLE_DEFT = math.sqrt((2 * (4.5) - 1.6) * 1.6)
DIMPLE_RADIUS = 3.0 * DIMPLE_DEFT
DIMPLE_INDENT = 1.1 * 1.6
SOURCE = 0  # 0 for randomized source, 1 for detailed.
DIMPLE_TYPE = 0  # 0:Normal, 1: Pyramid, 2: Parabolic

PHOTON_TESTBEAM_MACRO_TEMPLATE = """
/control/execute {0}/mac/init.mac
/tracking/verbose 0
/gps/source/intensity 1
/gps/particle opticalphoton
/gps/pos/type Volume
/gps/pos/shape Cylinder
/gps/pos/radius 0.001 mm
/gps/pos/halfz {3} cm #thickness position
/gps/pos/centre {1} {2} 0.0 mm # Beam position
/gps/ang/type iso
/gps/ene/type Arb
/gps/hist/type arb
/gps/hist/point 3.128e-6 0.0000
/gps/hist/point 3.119e-6 0.0008
/gps/hist/point 3.108e-6 0.0065
/gps/hist/point 3.096e-6 0.0089
/gps/hist/point 3.085e-6 0.0185
/gps/hist/point 3.074e-6 0.0355
/gps/hist/point 3.068e-6 0.0573
/gps/hist/point 3.060e-6 0.0839
/gps/hist/point 3.054e-6 0.1081
/gps/hist/point 3.052e-6 0.1274
/gps/hist/point 3.048e-6 0.1444
/gps/hist/point 3.044e-6 0.1710
/gps/hist/point 3.038e-6 0.1927
/gps/hist/point 3.032e-6 0.2218
/gps/hist/point 3.028e-6 0.2508
/gps/hist/point 3.024e-6 0.2677
/gps/hist/point 3.020e-6 0.2968
/gps/hist/point 3.016e-6 0.3185
/gps/hist/point 3.012e-6 0.3500
/gps/hist/point 3.008e-6 0.3790
/gps/hist/point 3.004e-6 0.4056
/gps/hist/point 3.000e-6 0.4323
/gps/hist/point 2.998e-6 0.4565
/gps/hist/point 2.995e-6 0.4903
/gps/hist/point 2.991e-6 0.5218
/gps/hist/point 2.989e-6 0.5532
/gps/hist/point 2.985e-6 0.5798
/gps/hist/point 2.983e-6 0.6065
/gps/hist/point 2.980e-6 0.6282
/gps/hist/point 2.968e-6 0.7637
/gps/hist/point 2.964e-6 0.7927
/gps/hist/point 2.961e-6 0.8218
/gps/hist/point 2.957e-6 0.8483
/gps/hist/point 2.954e-6 0.8774
/gps/hist/point 2.951e-6 0.9065
/gps/hist/point 2.947e-6 0.9355
/gps/hist/point 2.943e-6 0.9597
/gps/hist/point 2.939e-6 0.9790
/gps/hist/point 2.932e-6 0.9887
/gps/hist/point 2.926e-6 0.9983
/gps/hist/point 2.917e-6 1.0000
/gps/hist/point 2.909e-6 0.9960
/gps/hist/point 2.901e-6 0.9887
/gps/hist/point 2.894e-6 0.9790
/gps/hist/point 2.890e-6 0.9645
/gps/hist/point 2.883e-6 0.9500
/gps/hist/point 2.875e-6 0.9355
/gps/hist/point 2.868e-6 0.9161
/gps/hist/point 2.861e-6 0.8968
/gps/hist/point 2.853e-6 0.8774
/gps/hist/point 2.850e-6 0.8605
/gps/hist/point 2.842e-6 0.8387
/gps/hist/point 2.835e-6 0.8145
/gps/hist/point 2.830e-6 0.7903
/gps/hist/point 2.826e-6 0.7734
/gps/hist/point 2.821e-6 0.7565
/gps/hist/point 2.816e-6 0.7371
/gps/hist/point 2.810e-6 0.7177
/gps/hist/point 2.805e-6 0.6935
/gps/hist/point 2.971e-6 0.7177
/gps/hist/point 2.973e-6 0.6984
/gps/hist/point 2.976e-6 0.6790
/gps/hist/point 2.977e-6 0.6548
/gps/hist/point 2.969e-6 0.7419
/gps/hist/point 2.800e-6 0.6766
/gps/hist/point 2.795e-6 0.6597
/gps/hist/point 2.789e-6 0.6452
/gps/hist/point 2.784e-6 0.6258
/gps/hist/point 2.779e-6 0.6089
/gps/hist/point 2.774e-6 0.5919
/gps/hist/point 2.768e-6 0.5774
/gps/hist/point 2.762e-6 0.5629
/gps/hist/point 2.757e-6 0.5508
/gps/hist/point 2.750e-6 0.5315
/gps/hist/point 2.743e-6 0.5145
/gps/hist/point 2.735e-6 0.5000
/gps/hist/point 2.728e-6 0.4855
/gps/hist/point 2.721e-6 0.4710
/gps/hist/point 2.715e-6 0.4565
/gps/hist/point 2.708e-6 0.4468
/gps/hist/point 2.702e-6 0.4371
/gps/hist/point 2.695e-6 0.4274
/gps/hist/point 2.689e-6 0.4177
/gps/hist/point 2.681e-6 0.4081
/gps/hist/point 2.674e-6 0.3960
/gps/hist/point 2.666e-6 0.3790
/gps/hist/point 2.670e-6 0.3887
/gps/hist/point 2.662e-6 0.3669
/gps/hist/point 2.657e-6 0.3548
/gps/hist/point 2.651e-6 0.3403
/gps/hist/point 2.646e-6 0.3258
/gps/hist/point 2.638e-6 0.3016
/gps/hist/point 2.643e-6 0.3113
/gps/hist/point 2.632e-6 0.2823
/gps/hist/point 2.626e-6 0.2629
/gps/hist/point 2.620e-6 0.2484
/gps/hist/point 2.615e-6 0.2339
/gps/hist/point 2.609e-6 0.2242
/gps/hist/point 2.604e-6 0.2097
/gps/hist/point 2.598e-6 0.2000
/gps/hist/point 2.592e-6 0.1855
/gps/hist/point 2.586e-6 0.1758
/gps/hist/point 2.580e-6 0.1661
/gps/hist/point 2.575e-6 0.1540
/gps/hist/point 2.569e-6 0.1468
/gps/hist/point 2.561e-6 0.1323
/gps/hist/point 2.554e-6 0.1226
/gps/hist/point 2.545e-6 0.1129
/gps/hist/point 2.538e-6 0.1032
/gps/hist/point 2.530e-6 0.0960
/gps/hist/point 2.524e-6 0.0887
/gps/hist/point 2.517e-6 0.0839
/gps/hist/point 2.511e-6 0.0766
/gps/hist/point 2.506e-6 0.0742
/gps/hist/point 2.499e-6 0.0669
/gps/hist/point 2.492e-6 0.0597
/gps/hist/point 2.486e-6 0.0573
/gps/hist/point 2.482e-6 0.0548
/gps/hist/point 2.480E-6 0.0536
/gps/hist/point 2.455E-6 0.0402
/gps/hist/point 2.431E-6 0.0302
/gps/hist/point 2.407E-6 0.0226
/gps/hist/point 2.384E-6 0.0170
/gps/hist/point 2.362E-6 0.0127
/gps/hist/point 2.339E-6 0.0096
/gps/hist/point 2.317E-6 0.0072
/gps/hist/point 2.296E-6 0.0054
/gps/hist/point 2.275E-6 0.0040
/gps/hist/point 2.254E-6 0.0030
/gps/hist/point 2.234E-6 0.0023
/gps/hist/point 2.214E-6 0.0017
/gps/hist/point 2.194E-6 0.0013
/gps/hist/point 2.175E-6 0.0010
/gps/hist/point 2.156E-6 0.0007
/gps/hist/point 2.138E-6 0.0005
/gps/hist/point 2.119E-6 0.0004
/gps/hist/point 2.101E-6 0.0003
/gps/hist/point 2.066E-6 0.0002
/gps/hist/point 1.907E-6 0.0000
/gps/hist/inter Lin
/LYSim/Update
/random/setSeeds {5} {6}
/gps/number {4}
/run/beamOn 100
"""

CONDOR_DIRECTORY = ''

CONDOR_JDL_TEMPLATE = """
universe = vanilla
Executable = {0}
should_transfer_files = NO
Requirements = TARGET.FileSystemDomain == "privnet"
Output = {1}/sce_$(cluster)_$(process).stdout
Error  = {1}/sce_$(cluster)_$(process).stderr
Log    = {1}/sce_$(cluster)_$(process).condor
Arguments = {2} {3}
Queue
"""

X_LIST = [
    0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0,
    14.0
]
Y_LIST = [0.0]


def random_N_digit(n):
  """
  Generating a random number with n digits long (in decimal)
  """
  range_start = 10**(n - 1)
  range_end = (10**n) - 1
  return randint(range_start, range_end)


def compute_thickness(x, y):
  """
  Calculating the material thickness for a expected x-y input point
  """
  distance_from_center = math.sqrt(x**2 + y**2)

  big_rad = 0.5 * (DIMPLE_RADIUS**2 / DIMPLE_INDENT + DIMPLE_INDENT)
  if distance_from_center <= DIMPLE_RADIUS:
    if DIMPLE_TYPE == 0:  # Normal
      return 2.98 - ((big_rad**2 - distance_from_center**2)**0.5 -
                     (big_rad - DIMPLE_INDENT))
    elif DIMPLE_TYPE == 2:  # Parabolic
      return 2.98 - abs(1.35 * distance_from_center**2 - DIMPLE_INDENT)
    elif DIMPLE_TYPE == 1:
      if abs(y) <= abs(x):
        return 2.98 - (DIMPLE_INDENT - (DIMPLE_INDENT / DIMPLE_RADIUS) *
                       (abs(x)))
      else:
        return 2.98 - (DIMPLE_INDENT - (DIMPLE_INDENT / DIMPLE_RADIUS) *
                       (abs(y)))
    else:
      return 2.98
  else:
    return 2.98


def compute_n_photons(thickness):
  """
  Calculating the number of expected photons based on expected material
  thickness.
  """
  return int(math.floor((6677 / 0.3) * thickness))


#Random Numbers to start with (1: 9 digit, 2: 8 digit) make sure these are in photontest too
random.seed(datetime.now())
for x in X_LIST:
  for y in Y_LIST:
    for n in range(1, 1001):

      active_x = x if SOURCE == 1 else random.uniform(x - 0.15, x + 0.15)
      active_y = y if SOURCE == 1 else random.uniform(y - 0.15, y + 0.15)
      thickness = compute_thickness(x, y)
      n_photons = compute_n_photons(thickness)

      macro_content = PHOTON_TESTBEAM_MACRO_TEMPLATE.format(
          BASE_DIR,
          active_x,
          active_y,
          thickness / 2,
          n_photons,
          random_N_digit(9),
          random_N_digit(8),
      )

      macro_filename = DATA_DIR + '/macro/' + \
                       'photon_test_x{0:.1f}_y{1:.1f}_{2}'.format(
          x, y, n).replace('.', 'p') + '.mac'

      save_filename  = DATA_DIR + '/output/' + \
                       'photon_test_x{0:.1f}_y{1:.1f}_{2}'.format(
          x, y, n).replace('.', 'p')

      jdl_content = CONDOR_JDL_TEMPLATE.format(
          BASE_DIR + '/condor/condor-LYsim.sh',
          DATA_DIR + '/log/',
          macro_filename,
          save_filename,
      )

      jdl_filename = DATA_DIR + '/condor/' + \
                    'hgcal_tilesim_x{0:.1f}_y{1:.1f}_{2}'.format(
          x, y, n).replace('.', 'p') + '.jdl'

      ## Writing macro files
      os.makedirs(os.path.dirname(macro_filename), exist_ok=True)
      with open(macro_filename, 'w') as file:
        file.write(macro_content)

      ## Writing jdl files
      os.makedirs(os.path.dirname(jdl_filename), exist_ok=True)
      with open(jdl_filename, 'w') as file:
        file.write(jdl_content)

      ## Making directory for output files
      os.makedirs(os.path.dirname(save_filename), exist_ok=True)

      print(jdl_filename)
"""
          if n == 1000:
            command = ["condor_q ahorst"]
            p = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
            text = p.stdout.readlines()
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
                hadd = [
                    "hadd Part[%d][%s][%s].root Analysis*.root" %
                    (w, Arr2[x], Arr3[y])
                ]
                p = subprocess.Popen(hadd, shell=True)
                p.wait()
                if q == 300:
                  movefile = ["mv Analysis*.root HitData/RootsForTestPoint"]
                  p = subprocess.Popen(movefile, shell=True)
                  p.wait()
                else:
                  removefiles = ["rm Analysis*.root"]
                  p = subprocess.Popen(removefiles, shell=True)
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
          p = subprocess.Popen(hadd, shell=True)
          p.wait()
          f = ROOT.TFile("Total[%s][%s].root" % (Arr2[x], Arr3[y]))
          t = f.Get("Ndetect")
          meant = t.GetMean()
          meant = meant
          rms = t.GetRMS()
          ent = t.GetEntries()
          err = rms / math.sqrt(ent)
          with open('FinalShallow.csv', 'a') as file:
            file.write("\n%f,%f,%f,%f" % (Arr2[x], Arr3[y], meant, err))
          rmparts = ["rm Part*.root"]
          p = subprocess.Popen(rmparts, shell=True)
          p.wait()
"""