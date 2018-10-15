# HGcal Tile
GEANT4-based simulation of CE-H HGCal optical tile

Working Copy from FNAL Stay

Tested/Edited by Alan Horst

## ADDITIONS ##

1. Added new dimple geometries
2. Added generalparticlesource support
3. Wrote python scripts for handling jobs (hadd script not included)
4. Functions for source position/angle

## compiling

  git clone https://github.com/ahorstt/HGCal-Dimple-Tile.git

  cd hgcal_tile

  ./g4env.csh

  mkdir build

  cd build 

  cmake -DWITH_GEANT4_UIVIS=ON -DGeant4_DIR=$G4LIB ../TILE

  make


## running
  ./LYSIM photontest.mac [-novis] [output_filename] [root_filename}

    (if running without -novis
         /run/beamOn 5


## run the job on condor.

edit the condor-*.* so that they point to your areas, not mine

submit with

condor_submit condor_jobs.jdl

see if it is running with

condor_q -submitter your-user-name  

look at Analysis.root 


  OR
  
     python SubmitJobs.py  
  
  --- WARNING it's currently set to run 2000 jobs at 31 points, it takes a long time. Use screen or create fewer jobs per point
  
  The results (hits/err) of each point and it's location will be printed in FinalShallow.csv
