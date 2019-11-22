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
parser.add_argument('--tilewidth',
                    '-L',
                    type=float,
                    nargs='+',
                    default=[30],
                    help='Tile width')
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
parser.add_argument('--sipmwidth',
                    '-W',
                    type=float,
                    nargs='+',
                    default=[1.4],
                    help='SiPM width paramter')
parser.add_argument('--sipmrim',
                    '-R',
                    type=float,
                    nargs='+',
                    default=[0.4],
                    help='SiPM inactive rim width parameter')
parser.add_argument('--sipmstand',
                    '-S',
                    type=float,
                    nargs='+',
                    default=[0.3],
                    help='SiPM stand height')
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
                    default=[0.985],
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
Output = {1}/sce_$(cluster)_$(process).stdout
Error  = {1}/sce_$(cluster)_$(process).stderr
Log    = {1}/sce_$(cluster)_$(process).condor
Arguments = {2}
Queue
"""

for x, y, L, r, d, a, w, W, R, S in [(x, y, L, r, d, a, w, W, R, S)
                                     for x in args.beamx for y in args.beamy
                                     for L in args.tilewidth
                                     for r in args.dimplerad
                                     for d in args.dimpleind
                                     for a in args.absmult
                                     for w in args.wrapreflect
                                     for W in args.sipmwidth
                                     for R in args.sipmrim
                                     for S in args.sipmstand]:

  def make_str(prefix):
    args_string = '_'.join([
        'x{0:.1f}'.format(x), 'y{0:.1f}'.format(y), 'L{0:.1f}'.format(L),
        'r{0:.1f}'.format(r), 'd{0:.1f}'.format(d), 'a{0:.1f}'.format(
            a * 100), 'm{0:.1f}'.format(w * 100), 'W{0:.1f}'.format(W),
        'R{0:.1f}'.format(R), 'S{0:.2f}'.format(S),
    ])
    return prefix + args.prefix + '_' + args_string.replace('.', 'p')

  save_filename = os.path.abspath(DATA_DIR + '/root/' + '/' +
                                  make_str('hgcal_tilesim') + '.root')

  condor_args = ' '.join([
      '-x {}'.format(x), '-y {}'.format(y), '-L {}'.format(L), '-w 1.5',
      '-r {}'.format(r), '-d {}'.format(d), '-a {}'.format(a), '-m {}'.format(w),
      '-W {}'.format(W), '-R {}'.format(R), '-S {}'.format(S), '-N {}'.format(
          args.NEvents), '-o {}'.format(os.path.abspath(save_filename)),
  ])

  jdl_content = CONDOR_JDL_TEMPLATE.format(BASE_DIR,
                                           os.path.abspath(DATA_DIR + '/log/'),
                                           condor_args)

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
