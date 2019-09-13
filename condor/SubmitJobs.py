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
                    required=True,
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

args = parser.parse_args()

BASE_DIR = os.path.abspath(os.environ['CMSSW_BASE'] + '/src/'
                           '/HGCalTileSim/condor/')
DATA_DIR = os.path.abspath(BASE_DIR + '/results/')

CONDOR_JDL_TEMPLATE = """
universe = vanilla
Executable = {0}/condor-LYSquareTrigger_CMSSW.sh
should_transfer_files = NO
Requirements = TARGET.FileSystemDomain == "privnet"
Output = {1}/sce_{2}_{3}_{4}_{5}_$(cluster)_$(process).stdout
Error  = {1}/sce_{2}_{3}_{4}_{5}_$(cluster)_$(process).stderr
Log    = {1}/sce_{2}_{3}_{4}_{5}_$(cluster)_$(process).condor
Arguments = -x {2} -y {3} -w 1.5 -N 1000 -r {4} -d {5} -o {6}
Queue
"""

timestr = datetime.datetime.now().strftime("%Y%m%d_%H%M")
for x, y, r, d in [(x, y, r, d) for x in args.beamx for y in args.beamy
                   for r in args.dimplerad for d in args.dimpleind]:

  save_filename = os.path.abspath(
      DATA_DIR + '/root/' +
      timestr + '/hgcal_tilesim_x{0:.1f}_y{1:.1f}_r{2:.1f}_d{3:.1f}'.format(
          x, y, r, d).replace('.', 'p') + '.root')

  jdl_content = CONDOR_JDL_TEMPLATE.format(
      BASE_DIR,
      os.path.abspath(DATA_DIR + '/log/'),
      x,
      y,
      r,
      d,
      os.path.abspath(save_filename),
  )

  jdl_filename = os.path.abspath(
      DATA_DIR + '/condor/' +
      timestr + '/hgcal_tilesim_x{0:.1f}_y{1:.1f}_r{2:.1f}_d{3:.1f}'.format(
          x, y, r, d).replace('.', 'p') + '.jdl')

  ## Writing jdl files
  os.makedirs(os.path.dirname(jdl_filename), exist_ok=True)
  with open(jdl_filename, 'w') as file:
    file.write(jdl_content)

  os.makedirs(DATA_DIR + '/log/', exist_ok=True)

  ## Making directory for output files
  os.makedirs(os.path.dirname(save_filename), exist_ok=True)

  print(jdl_filename)
