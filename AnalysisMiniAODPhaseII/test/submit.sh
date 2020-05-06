#!/bin/bash

# Following https://indico.ifca.es/event/1353/contributions/6553/attachments/1068/1420/Tier2.pdf

# qos = quality of service
# gridui_sort   is for (up to)  3 hours jobs
# gridui_medium is for (up to) 24 hours jobs

sbatch -o logfile.log -e errofile.err --qos=gridui_sort --partition=cloudcms run.sh

squeue

