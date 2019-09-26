#!/usr/bin/env python3
import os
import datetime
import argparse

parser = argparse.ArgumentParser(
    description='Options for generating condor scripts')
parser.add_argument('--beamx',
                    '-x',
                    type=float,
                    nargs='+',
                    required=True,
                    help='List of x values of beam center')
parser.add_argument('--beamy',
                    '-y',
                    type=float,
                    nargs='+',
                    default=[0],
                    help='List of y values of beam center')
parser.add_argument('--dimplerad',
                    '-r',
                    type=float,
                    nargs='+',
                    default=[3],
                    help='List of dimple radius to try out')
parser.add_argument('--dimpleind',
                    '-d',
                    type=float,
                    nargs='+',
                    default=[1.5],
                    help='List of dimple indents to try out')
parser.add_argument('--absmult',
                    '-a',
                    type=float,
                    nargs='+',
                    default=[1.],
                    help='List of tile absorption length multiple')
parser.add_argument('--wrapreflect',
                    '-w',
                    type=float,
                    nargs='+',
                    default=[1.],
                    help='List of wrap reflectivity')
parser.add_argument('--NEvents',
                    '-N',
                    type=int,
                    default=100,
                    help='Number of events to run')
parser.add_argument('--prefix',
                    type=str,
                    default='',
                    help='Additional string to place in filename')

args = parser.parse_args()

BASE_DIR = os.path.abspath(os.environ['CMSSW_BASE'] + '/src/' +
                           '/HGCalTileSim/condor/')
DATA_DIR = os.path.abspath(BASE_DIR + '/results/')

CONDOR_JDL_TEMPLATE = """
universe = vanilla
Executable = {0}/condor-LYSquareTrigger_CMSSW.sh
should_transfer_files = NO
Requirements = TARGET.FileSystemDomain == "privnet"
Output = {1}/sce_{2}_{3}_{4}_{5}_{6}_{7}_$(cluster)_$(process).stdout
Error  = {1}/sce_{2}_{3}_{4}_{5}_{6}_{7}_$(cluster)_$(process).stderr
Log    = {1}/sce_{2}_{3}_{4}_{5}_{6}_{7}_$(cluster)_$(process).condor
Arguments = -x {2} -y {3} -w 1.5 -r {4} -d {5} -a {6} -m {7} -o {8} -N {9}
Queue
"""

for x, y, r, d, a, w in [(x, y, r, d, a, w) for x in args.beamx
                         for y in args.beamy for r in args.dimplerad
                         for d in args.dimpleind for a in args.absmult
                         for w in args.wrapreflect]:

  def make_str(prefix):
    return prefix+args.prefix+ \
         '_x{0:.1f}_y{1:.1f}_r{2:.1f}_d{3:.1f}_a{4:.1f}_w{5:.1f}'.format(
        x, y, r, d, a*100, w*100).replace('.', 'p')

  save_filename = os.path.abspath(DATA_DIR + '/root/' + '/' +
                                  make_str('hgcal_tilesim') + '.root')

  jdl_content = CONDOR_JDL_TEMPLATE.format(
      BASE_DIR,
      os.path.abspath(DATA_DIR + '/log/'),
      x,
      y,
      r,
      d,
      a,
      w,
      os.path.abspath(save_filename),
      args.NEvents,
  )

  jdl_filename = os.path.abspath(DATA_DIR + '/condor/' + '/' +
                                 make_str('hgcal_tilesim') + '.jdl')

  ## Writing jdl files
  os.makedirs(os.path.dirname(jdl_filename), exist_ok=True)
  with open(jdl_filename, 'w') as file:
    file.write(jdl_content)

  os.makedirs(DATA_DIR + '/log/', exist_ok=True)

  ## Making directory for output files
  os.makedirs(os.path.dirname(save_filename), exist_ok=True)

  print(jdl_filename)
