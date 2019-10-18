#!/bin/bash

python3 SubmitJobs.py  -x $(seq 0 0.5 14.5)  -d $(seq 1.5 0.1 2.0) -r $(seq 2.7 0.3 6) -W 1.4 -R 0.4 -S 0.3 -a 1.0 -w 0.985

python3 SubmitJobs.py -x $(seq 0 0.5 14.5 ) -d $(seq 1.5 0.1 2.0) -r $(seq 2.7 0.3 6.0)  -W 2.0 -R 0.1 -S 0.3 -a 1.0 -w 0.985

python3 SubmitJobs.py -x $(seq 0 0.5 14.5 ) -d $(seq 1.5 0.1 2.0) -r $(seq 3.6 0.3 6.0)  -W 3.0 -R 0.1 -S 0.3 -a 1.0 -w 0.985