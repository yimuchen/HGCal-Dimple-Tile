# HGCal Tile

GEANT4-based simulation of CE-H HGCal optical tile.  Working Copy from FNAL Stay.
Tested/Edited by Alan Horst, Yi-Mu Chen

## ADDITIONS

1. Added new dimple geometries
2. Added generalparticlesource support
3. Wrote python scripts for handling jobs (hadd script not included)
4. Functions for source position/angle

## compiling

```bash
git clone https://github.com/yimuchen/HGCal-Dimple-Tile.git
cd hgcal_tile
./g4env.sh
mkdir build
cd build
cmake -DWITH_GEANT4_UIVIS=ON -DGeant4_DIR=$G4LIB ../TILE
cmake --build ../TILE
```

## running

```bash
./LYSIM photontest.mac [-novis] [output_filename] [root_filename}
```

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

*WARNING* it's currently set to run 2000 jobs at 31 points, it takes a long time.
Use screen or create fewer jobs per point

The results (hits/err) of each point and it's location will be printed in
FinalShallow.csv (add a line that says "end" at the end of the csv after runs are
complete)

Then to graph, use DatatoRoot.py, which will create TGraphErrors plots for each
data set
