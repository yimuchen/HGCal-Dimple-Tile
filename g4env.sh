#!/bin/bash -f
export USERBASE=$(pwd)
export ARCH=x86_64-slc6-gcc46-opt
source /cvmfs/sft.cern.ch/lcg/external/gcc/4.6.3/x86_64-slc6/setup.sh
export PATH=$PATH:/cvmfs/sft.cern.ch/lcg/external/CMake/2.8.6/${ARCH}/bin
export PATH=$PATH:/cvmfs/sft.cern.ch/lcg/external/expat/2.0.1/${ARCH}/
#export BOOST_ROOT=/cvmfs/cms.cern.ch/slc6_amd64_gcc462/external/boost/1.47.0-cms
export Geant4_DIR=/data/users/eno/geant4.9.6.p04-install/
## Must be done like this... wierd...
cd     ${Geant4_DIR}/share/Geant4-9.6.4/geant4make/
source geant4make.sh
## Must be done like this... wierd...
cd      /cvmfs/sft.cern.ch/lcg/external/ROOT/5.34.00/${ARCH}/root/
source ./bin/thisroot.sh
LIBRARY_PATH=$LIBRARY_PATH:$PWD/lib

cd ${USERBASE}
