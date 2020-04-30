#!/bin/bash

sbatch run.sh -o myoutput.txt -e myerror.err -q qos

squeue
