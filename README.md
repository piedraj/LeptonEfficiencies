# Everything begins here

    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240

    bash -l

    export SCRAM_ARCH=slc6_amd64_gcc530
    cmsrel CMSSW_9_1_1_patch1
    cd CMSSW_9_1_1_patch1/src
    cmsenv

    git clone https://github.com/piedraj/LeptonEfficiencies


# Produce histograms

    cd CMSSW_9_1_1_patch1/src
    scram b -j 8

    cd LeptonEfficiencies/AnalysisMiniAODPhaseII/test
    mkdir rootfiles

    cmsRun MuonAnalyzer_cfg.py inputDataset='DisplacedMuons_PU200' maxEvents=-1
    mv MyMuonPlots.root rootfiles/DisplacedMuons_PU200.root

    cmsRun MuonAnalyzer_cfg.py inputDataset='DisplacedMuons_noPU' maxEvents=-1
    mv MyMuonPlots.root rootfiles/DisplacedMuons_noPU.root

    cmsRun MuonAnalyzer_cfg.py inputDataset='DisplacedSUSY_PU200' maxEvents=-1
    mv MyMuonPlots.root rootfiles/DisplacedSUSY_PU200.root

    cmsRun MuonAnalyzer_cfg.py inputDataset='DisplacedSUSY_noPU' maxEvents=-1
    mv MyMuonPlots.root rootfiles/DisplacedSUSY_noPU.root


# Read histograms and draw distributions

    cd LeptonEfficiencies/AnalysisMiniAODPhaseII/test
    root -l -b -q doEfficiencies.C+

