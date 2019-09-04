#!/usr/bin/python3
import os
import datetime

USER_NAME = os.environ['USER']
BASE_DIR = os.path.abspath(os.path.dirname(os.path.abspath(__file__)) + '/../')
DATA_DIR = BASE_DIR + '/data/photontest/'

CONDOR_JDL_TEMPLATE = """
universe = vanilla
Executable = {0}/condor/condor-LYSquareTrigger.sh
should_transfer_files = NO
Requirements = TARGET.FileSystemDomain == "privnet"
Output = {1}/sce_{2}_{3}_$(cluster)_$(process).stdout
Error  = {1}/sce_{2}_{3}_$(cluster)_$(process).stderr
Log    = {1}/sce_{2}_{3}_$(cluster)_$(process).condor
Arguments = {2} {3} 1.5 1000000 {4}
Queue
"""

timestr = datetime.datetime.now().strftime("%Y%m%d_%H00")
#Random Numbers to start with (1: 9 digit, 2: 8 digit) make sure these are in photontest too
for x in range(15):
  for y in range(1):
    save_filename = DATA_DIR + '/output/' + timestr + '/' +  \
                   'hgcal_tilesim_x{0:.1f}_y{1:.1f}'.format(
                     x, y ).replace('.', 'p')

    jdl_content = CONDOR_JDL_TEMPLATE.format(
        BASE_DIR,
        DATA_DIR + '/log/',
        x,
        y,
        save_filename,
    )
    jdl_filename = DATA_DIR + '/condor/' + \
                  'hgcal_tilesim_x{0:.1f}_y{1:.1f}'.format(
        x, y,).replace('.', 'p') + '.jdl'

    ## Writing jdl files
    os.makedirs(os.path.dirname(jdl_filename), exist_ok=True)
    with open(jdl_filename, 'w') as file:
      file.write(jdl_content)

    ## Making directory for output files
    os.makedirs(os.path.dirname(save_filename), exist_ok=True)

    print(jdl_filename)