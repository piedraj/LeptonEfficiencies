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

    cmsRun MuonAnalyzer_cfg.py  inputDataset='DisplacedMuons_PU200'           maxEvents=-1
    cmsRun MuonAnalyzer_cfg.py  inputDataset='DisplacedMuons_noPU'            maxEvents=-1
    cmsRun MuonAnalyzer_cfg.py  inputDataset='DisplacedSUSY_CTau-1_noPU'      maxEvents=-1
    cmsRun MuonAnalyzer_cfg.py  inputDataset='DisplacedSUSY_CTau-10_noPU'     maxEvents=-1
    cmsRun MuonAnalyzer_cfg.py  inputDataset='DisplacedSUSY_CTau-1_PU200'     maxEvents=-1
    cmsRun MuonAnalyzer_cfg.py  inputDataset='DisplacedSUSY_CTau-100_PU200'   maxEvents=-1
    cmsRun MuonAnalyzer_cfg.py  inputDataset='DisplacedSUSY_CTau-1000_PU200'  maxEvents=-1


# Prepare variable bin sizes for vxy and vz

    cd LeptonEfficiencies/AnalysisMiniAODPhaseII/test

    root -l -b -q 'equalBinContent.C(6,"pt")'
    root -l -b -q 'equalBinContent.C(12,"vxy")'
    root -l -b -q 'equalBinContent.C(12,"vz")'
    root -l -b -q 'equalBinContent.C(12,"vr")'


# Read histograms and draw distributions

    cd LeptonEfficiencies/AnalysisMiniAODPhaseII/test

    root -l -b -q 'doEfficiencies.C+("CTau-1_PU200", "CTau-100_PU200")'

    root -l -b -q 'doResolution.C+("CTau-1_PU200", "CTau-100_PU200")'

    root -l -b -q 'doResolutionDxy.C+("Soft",  "soft muons",  noPU)'
    root -l -b -q 'doResolutionDxy.C+("Soft",  "soft muons",  PU200)'
    root -l -b -q 'doResolutionDxy.C+("Tight", "tight muons", noPU)'
    root -l -b -q 'doResolutionDxy.C+("Tight", "tight muons", PU200)'

    root -l -b -q 'doEfficiencies2D.C+("Soft",  "PU200", "soft muons efficiency (200 PU)")'
    root -l -b -q 'doEfficiencies2D.C+("Soft",  "noPU",  "soft muons efficiency (no PU)")'
    root -l -b -q 'doEfficiencies2D.C+("Tight", "PU200", "tight muons efficiency (200 PU)")'
    root -l -b -q 'doEfficiencies2D.C+("Tight", "noPU",  "tight muons efficiency (no PU)")'

