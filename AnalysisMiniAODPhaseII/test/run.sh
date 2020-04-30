#!/bin/bash

source /cvmfs/cms.cern.ch/cmsset_default.sh

cd /gpfs/users/piedra/project/work/CMSSW_9_1_1_patch1/src

printf "\n [run.sh] We are in the CMSSW release %s\n\n" $PWD

eval `scramv1 runtime -sh`

cd LeptonEfficiencies/AnalysisMiniAODPhaseII/test

printf "\n [run.sh] We are in the working directory %s\n" $PWD

mkdir -p rootfiles

cmsRun MuonAnalyzer_cfg.py inputDataset='DisplacedMuons_PU200' maxEvents=-1
