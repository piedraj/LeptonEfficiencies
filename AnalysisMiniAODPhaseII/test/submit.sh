#!/bin/bash

# Following https://indico.ifca.es/event/1353/contributions/6553/attachments/1068/1420/Tier2.pdf

sbatch run.sh -o myoutput.txt -e myerror.err -q qos

squeue

