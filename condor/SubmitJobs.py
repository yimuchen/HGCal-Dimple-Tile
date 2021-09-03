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
parser.add_argument('--coverref',
                    '-c',
                    type=float,
                    nargs='+',
                    default=[1.52],
                    help='Whether or not to add the epoxy cover')
parser.add_argument('--dimplerad',
                    '-r',
                    type=float,
                    nargs='+',
                    default=[6.0],
                    help='List of dimple radius to try out')
parser.add_argument('--dimpleind',
                    '-d',
                    type=float,
                    nargs='+',
                    default=[0.9],
                    help='List of dimple indents to try out')
parser.add_argument('--dimpletype',
                    '-T',
                    type=int,
                    nargs='+',
                    default=[0],
                    help='Type of dimple to test out')
parser.add_argument('--sipmrefalpha',
                    '-F',
                    type=float,
                    nargs='+',
                    default=[0.05],
                    help='Active area roughness angle')
parser.add_argument('--sipmrefmult',
                    '-M',
                    type=float,
                    nargs='+',
                    default=[1.0],
                    help='Active area reflectivity multiplier')
parser.add_argument('--sipmsref',
                    '-s',
                    type=float,
                    nargs='+',
                    default=[0.8],
                    help='SiPM stand reflectivity')
parser.add_argument('--sipmwidth',
                    '-W',
                    type=float,
                    nargs='+',
                    default=[2.0],
                    help='SiPM width parameter')
parser.add_argument('--sipmrim',
                    '-R',
                    type=float,
                    nargs='+',
                    default=[0.1],
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
parser.add_argument('--tilealpha',
                    '-A',
                    type=float,
                    nargs='+',
                    default=[0.01],
                    help='Bulk Tile microfacet alpha value')
parser.add_argument('--dimplealpha',
                    '-D',
                    type=float,
                    nargs='+',
                    default=[0.1],
                    help='Dimple surface Tile microfacet alpha value')
parser.add_argument('--pcbreflect',
                    '-p',
                    type=float,
                    nargs='+',
                    default=[0.8],
                    help='List of pcb reflectivities to test')
parser.add_argument('--pcbradius',
                    '-b',
                    type=float,
                    nargs='+',
                    default=[2.5],
                    help='List of pcb exposed radii to test')
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
universe              = vanilla
Executable            = {0}/condor-LYSquareTrigger_CMSSW.sh
should_transfer_files = NO
Requirements          = TARGET.FileSystemDomain == "privnet"
request_memory        = 1 GB
Output                = {1}.stdout
Error                 = {1}.stderr
Log                   = {1}.condor
Arguments             = {2}
Queue
"""

#print(args.addcover)

for x, y, L, r, d, T, a, w, W, p, b, R, S, A, D, M, s, c, F in [
    (x, y, L, r, d, T, a, w, W, p, b, R, S, A, D, M, s, c, F) for x in args.beamx
    for y in args.beamy for L in args.tilewidth for r in args.dimplerad
    for d in args.dimpleind for T in args.dimpletype for a in args.absmult
    for w in args.wrapreflect for W in args.sipmwidth for p in args.pcbreflect
    for b in args.pcbradius for R in args.sipmrim for S in args.sipmstand
    for A in args.tilealpha for D in args.dimplealpha for M in args.sipmrefmult
    for s in args.sipmsref for c in args.coverref for F in args.sipmrefalpha
]:

  def make_str(prefix):
    args_string = '_'.join([
        'x{0:.1f}'.format(x), 'y{0:.1f}'.format(y), 'L{0:.1f}'.format(L),
        'r{0:.1f}'.format(r), 'd{0:.1f}'.format(d), 'T{0:d}'.format(T),
        'a{0:.1f}'.format(a * 100), 'm{0:.1f}'.format(
            w * 100), 'A{0:0.3f}'.format(A), 'D{0:0.3f}'.format(D),
        'W{0:.1f}'.format(W), 'P{:.1f}'.format(
            p * 100), 'Pr{:.1f}'.format(b), 'R{0:.1f}'.format(R),
        'S{0:.2f}'.format(S), 'M{:.2f}'.format(M), 's{:.1f}'.format(
            s * 100), 'c{:.2f}'.format(c), 'F{:0.2f}'.format(F)
    ])
    return prefix + args.prefix + '_' + args_string.replace('.', 'p')

  save_filename = os.path.abspath(DATA_DIR + '/root/' + '/' +
                                  make_str('hgcal_tilesim') + '.root')

  condor_args = ' '.join([
      '-x {}'.format(x), '-y {}'.format(y), '-L {}'.format(L), '-w 1.5',
      '-r {}'.format(r), '-d {}'.format(d), '-T {}'.format(T), '-a {}'.format(a),
      '-m {}'.format(w), '-W {}'.format(W), '-A {}'.format(A), '-D {}'.format(D),
      '-p {}'.format(p), '-b {}'.format(b), '-R {}'.format(R), '-S {}'.format(S),
      '-N {}'.format(args.NEvents), '-s {}'.format(s), '-M {}'.format(M),
      '-c {}'.format(c), '-F {}'.format(F), '-o {}'.format(
          os.path.abspath(save_filename)),
  ])

  log_filename = os.path.abspath(DATA_DIR + '/log/' + '/' +
                                 make_str('log_tilesim'))
  jdl_filename = os.path.abspath(DATA_DIR + '/condor/' + '/' +
                                 make_str('hgcal_tilesim') + '.jdl')
  jdl_content = CONDOR_JDL_TEMPLATE.format(BASE_DIR, log_filename, condor_args)

  ## Writing jdl files
  os.makedirs(os.path.dirname(jdl_filename), exist_ok=True)
  with open(jdl_filename, 'w') as file:
    file.write(jdl_content)

  os.makedirs(DATA_DIR + '/log/', exist_ok=True)

  ## Making directory for output files
  os.makedirs(os.path.dirname(save_filename), exist_ok=True)

  print(jdl_filename)
