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


# Prepare variable bin sizes for vxy and vz

    cd LeptonEfficiencies/AnalysisMiniAODPhaseII/test

    root -l -b -q 'equalBinContent.C("vxy")'
    root -l -b -q 'equalBinContent.C("vz")'


# Read histograms and draw distributions

    cd LeptonEfficiencies/AnalysisMiniAODPhaseII/test

    root -l -b -q doEfficiencies.C+
    root -l -b -q doResolution.C+

    root -l -b -q 'doResolutionDxy.C+("Soft",  "soft muons",  noPU)'
    root -l -b -q 'doResolutionDxy.C+("Soft",  "soft muons",  PU200)'
    root -l -b -q 'doResolutionDxy.C+("Tight", "tight muons", noPU)'
    root -l -b -q 'doResolutionDxy.C+("Tight", "tight muons", PU200)'

    root -l -b -q 'doEfficiencies2D.C+("Soft",  "PU200", "soft muons efficiency (200 PU)")'
    root -l -b -q 'doEfficiencies2D.C+("Soft",  "noPU",  "soft muons efficiency (no PU)")'
    root -l -b -q 'doEfficiencies2D.C+("Tight", "PU200", "tight muons efficiency (200 PU)")'
    root -l -b -q 'doEfficiencies2D.C+("Tight", "noPU",  "tight muons efficiency (no PU)")'

