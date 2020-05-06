#!/bin/bash
#SBATCH --mail-user=piedra@ifca.unican.es  # email address
#SBATCH --mail-type=ALL                    # Alerts sent when job begins, ends, or aborts
#SBATCH --nodes=1                          # Number of nodes required
#SBATCH --ntasks=1                         # Number of tasks required
#SBATCH --mem=2G                           # Total memory (RAM) required, per node
#SBATCH --time=1:00:00                     # Wall Clock time (dd-hh:mm:ss) [max of 14 days]
#SBATCH --output=myjob.output              # Output and error messages go to this file

# Additional information can be found in the Altamira Users Guide
# https://confluence.ifca.es/display/IC/Altamira+Users+Guide#AltamiraUsersGuide-Batchsystem

printf "\n [run.sh] Load PERL module\n"

module load PERL

printf "\n [run.sh] Setup CMS environment\n"

source /cvmfs/cms.cern.ch/cmsset_default.sh

cd /gpfs/users/piedra/project/work/CMSSW_9_1_1_patch1/src

printf "\n [run.sh] We are in the CMSSW release %s\n\n" $PWD

eval `scramv1 runtime -sh`

cd LeptonEfficiencies/AnalysisMiniAODPhaseII/test

printf "\n [run.sh] We are in the working directory %s\n\n" $PWD

mkdir -p rootfiles

cmsRun MuonAnalyzer_cfg.py inputDataset='DisplacedMuons_PU200' maxEvents=-1

