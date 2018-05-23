# Everything begins here

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240

    bash -l

    export SCRAM_ARCH=slc6_amd64_gcc630
    cmsrel CMSSW_9_1_1_patch1
    cd CMSSW_9_1_1_patch1/src
    cmsenv

    git clone https://github.com/piedraj/LeptonEfficiencies

# Produce histograms

    cd CMSSW_9_1_1_patch1/src
    scram b -j 8
    cd test
    cmsRun MuonAnalyzer_cfg.py

# Read histograms and draw distributions

    cd test
    root -l -b -q doEfficiencies.C+

