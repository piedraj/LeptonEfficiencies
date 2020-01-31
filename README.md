# Everything begins here

    ssh -Y gridui.ifca.es -o ServerAliveInterval=240
    ssh -Y lxplus.cern.ch -o ServerAliveInterval=240

    bash -l

Execute this line **only** in gridui.

    source /cvmfs/cms.cern.ch/cmsset_default.sh

Set a CMSSW release.

    export SCRAM_ARCH=slc7_amd64_gcc530
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

    root -l -b -q 'doEfficiencies.C+("CTau-1_noPU",    "CTau-1_PU200")'
    root -l -b -q 'doEfficiencies.C+("CTau-1_noPU",    "CTau-10_noPU")'
    root -l -b -q 'doEfficiencies.C+("CTau-100_PU200", "CTau-1000_PU200")'

    root -l -b -q 'doResolution.C+("CTau-1_noPU",    "CTau-1_PU200")'
    root -l -b -q 'doResolution.C+("CTau-1_noPU",    "CTau-10_noPU")'
    root -l -b -q 'doResolution.C+("CTau-100_PU200", "CTau-1000_PU200")'

    root -l -b -q 'doEfficiencies2D.C+("Soft", "CTau-1_noPU")'
    root -l -b -q 'doEfficiencies2D.C+("Soft", "CTau-10_noPU")'
    root -l -b -q 'doEfficiencies2D.C+("Soft", "CTau-1_PU200")'
    root -l -b -q 'doEfficiencies2D.C+("Soft", "CTau-100_PU200")'
    root -l -b -q 'doEfficiencies2D.C+("Soft", "CTau-1000_PU200")'

    root -l -b -q 'doEfficiencies2D.C+("Tight", "CTau-1_noPU")'
    root -l -b -q 'doEfficiencies2D.C+("Tight", "CTau-10_noPU")'
    root -l -b -q 'doEfficiencies2D.C+("Tight", "CTau-1_PU200")'
    root -l -b -q 'doEfficiencies2D.C+("Tight", "CTau-100_PU200")'
    root -l -b -q 'doEfficiencies2D.C+("Tight", "CTau-1000_PU200")'


# Combine samples

To gain statistics we perform an `hadd` of the **PU200** and **noPU** files.

    pushd rootfiles
    hadd DisplacedSUSY_PU200.root DisplacedSUSY_CTau-1_PU200.root DisplacedSUSY_CTau-100_PU200.root DisplacedSUSY_CTau-1000_PU200.root
    hadd DisplacedSUSY_noPU.root  DisplacedSUSY_CTau-1_noPU.root DisplacedSUSY_CTau-10_noPU.root
    popd

The distributions below are made with the `hadd` files.

    cd LeptonEfficiencies/AnalysisMiniAODPhaseII/test

    root -l -b -q 'doEfficiencies.C+("noPU", "PU200")'

    root -l -b -q 'doResolution.C+("noPU", "PU200")'

    root -l -b -q 'doEfficiencies2D.C+("Soft", "noPU")'
    root -l -b -q 'doEfficiencies2D.C+("Soft", "PU200")'

    root -l -b -q 'doEfficiencies2D.C+("Tight", "noPU")'
    root -l -b -q 'doEfficiencies2D.C+("Tight", "PU200")'

