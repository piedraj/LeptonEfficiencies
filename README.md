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

    mkdir rootfiles

    cmsRun MuonAnalyzer_cfg.py print inputDataset='PU200'
    mv MyMuonPlots.root rootfiles/MyMuonPlots_PU200.root

    cmsRun MuonAnalyzer_cfg.py print inputDataset='noPU'
    mv MyMuonPlots.root rootfiles/MyMuonPlots_noPU.root

# Read histograms and draw distributions

    cd test
    root -l -b -q doEfficiencies.C+

