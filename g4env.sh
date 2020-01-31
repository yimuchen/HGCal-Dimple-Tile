#!/bin/bash -f
export USERBASE=$(pwd)
source /cvmfs/sft.cern.ch/lcg/external/gcc/4.6.3/x86_64-slc6/setup.sh
export PATH=/cvmfs/sft.cern.ch/lcg/external/CMake/2.8.6/x86_64-slc6-gcc46-opt/bin:$PATH
export Geant4_DIR=/data/users/eno/geant4.9.6.p04-install/
## Must be done like this... wierd...
cd     ${Geant4_DIR}/share/Geant4-9.6.4/geant4make/
source geant4make.sh
## Must be done like this... wierd...
cd     /cvmfs/sft.cern.ch/lcg/external/ROOT/5.34.00/x86_64-slc6-gcc46-opt/root/
source ./bin/thisroot.sh
export LIBRARY_PATH=$LIBRARY_PATH:$PWD/lib

#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/lib
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/cvmfs/cms.cern.ch/slc6_amd64_gcc600/external/pcre/8.37-cms2/lib/
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/cvmfs/cms.cern.ch/slc6_amd64_gcc600/external/xz/5.2.2-cms2/lib
cd ${USERBASE}
