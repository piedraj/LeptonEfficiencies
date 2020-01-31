import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as opts

process = cms.Process("RecoMuon")

options = opts.VarParsing('analysis')

options.register ('inputDataset',
                  'DisplacedMuons_PU200',
                  opts.VarParsing.multiplicity.singleton,
                  opts.VarParsing.varType.string,
                  'Input dataset')

options.register ('location',
                  'gridui',
                  opts.VarParsing.multiplicity.singleton,
                  opts.VarParsing.varType.string,
                  'Can be lxplus or gridui')

samplesPath = '/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/'

if 'gridui' in options.location : samplesPath = '/gpfs/projects/cms/data/PhaseIITDRSpring17MiniAOD/'

# Defaults
options.maxEvents = -1

options.parseArguments()

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.maxEvents))

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('Demo')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.cerr.INFO = cms.untracked.PSet(limit = cms.untracked.int32(-1))


if 'DisplacedMuons_PU200' in options.inputDataset :
    print '\n Will read /DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/PhaseIITDRSpring17MiniAOD-PU200BX8_91X_upgrade2023_realistic_v3-v6/MINIAODSIM\n'
    options.inputFiles = \
    'file:' + samplesPath + 'DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/14211B25-C077-E711-90D0-0025905D1D78.root', \
    'file:' + samplesPath + 'DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/1E5E5389-B477-E711-8518-0025905D1C56.root', \
    'file:' + samplesPath + 'DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/5823B0EF-EB78-E711-A5B1-1866DAEA6E20.root', \
    'file:' + samplesPath + 'DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/6671BA0A-9378-E711-8976-0025904C540C.root', \
    'file:' + samplesPath + 'DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/68954ACD-E778-E711-BFE8-0025905521D2.root', \
    'file:' + samplesPath + 'DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/70BEFF66-CD77-E711-B41D-B083FECFF2BE.root', \
    'file:' + samplesPath + 'DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/7ADEB521-0578-E711-B0B4-842B2B1810D3.root', \
    'file:' + samplesPath + 'DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/82597027-E577-E711-AC49-141877411D83.root', \
    'file:' + samplesPath + 'DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/825E4ACC-BB77-E711-91D3-0025904C66EA.root', \
    'file:' + samplesPath + 'DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/A0AA1AF5-9279-E711-9B35-782BCB5300A1.root', \
    'file:' + samplesPath + 'DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/BC4957D1-C577-E711-8473-0025905D1CB6.root', \
    'file:' + samplesPath + 'DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/D6BF7ECF-1E78-E711-A99D-B083FED429D5.root', \
    'file:' + samplesPath + 'DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/DAE448AC-A977-E711-8A39-0025904C66A2.root', \
    'file:' + samplesPath + 'DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/F0154A8B-F077-E711-95DC-141877410E71.root', \
    'file:' + samplesPath + 'DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/F08EEFEF-DB77-E711-9912-A4BADB1C5E26.root'


if 'DisplacedMuons_noPU' in options.inputDataset :
    print '\n Will read /DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/PhaseIITDRSpring17MiniAOD-noPUBX8_91X_upgrade2023_realistic_v3-v3/MINIAODSIM\n'
    options.inputFiles = \
        'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/noPUBX8_91X_upgrade2023_realistic_v3-v3/50000/FCEFF295-A27C-E711-92A6-0CC47AD98CEA.root'


if 'DisplacedSUSY_CTau-1_PU200' in options.inputDataset :
    print '\n Will read /DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/PhaseIITDRSpring17MiniAOD-PU200_91X_upgrade2023_realistic_v3-v2/MINIAODSIM\n'
    options.inputFiles = \
        'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/110000/A2DEC450-707E-E711-BD0E-00259048AE50.root',\
        'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/004CD079-877B-E711-AA24-00269E95B1BC.root',\
        'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/0628DE6A-517B-E711-AF5B-002590E7DE26.root',\
        'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/0C089C80-517B-E711-A8A2-008CFAF2222E.root',\
        'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/0CDD60CA-277B-E711-B0A9-6C3BE5B59150.root',\
        'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/0EA9129C-217B-E711-9675-0CC47A5FA3B9.root',\
        'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/12C4CB84-E87A-E711-988E-0242AC1C0501.root',\
        'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/34650372-877B-E711-B277-A0369FC5B844.root',\
        'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/3E2B439C-E97A-E711-B783-00266CFFC80C.root',\
        'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/46DFDA8C-457B-E711-BD37-008CFA197480.root',\
        'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/4A8F61D3-427A-E711-BA3D-008CFA14FA8C.root',\
        'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/562EBE89-567B-E711-BC50-0025907B4EEE.root',\
        'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/5AFDDB67-5F7B-E711-B319-3417EBE5062D.root'


if 'DisplacedSUSY_CTau-1_noPU' in options.inputDataset :
    print '\n Will read /DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/PhaseIITDRSpring17MiniAOD-noPU_91X_upgrade2023_realistic_v3-v2/MINIAODSIM\n'
    options.inputFiles = \
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/50000/AA5A0677-FD7D-E711-AA69-48FD8EE73A8D.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/1EF1CCD3-787A-E711-9BE0-1866DAEA7A40.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/282CCB76-C47A-E711-969B-0242AC110009.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/2E1DDD0D-C07A-E711-BB26-7845C4FC3C98.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/3C0EA981-457A-E711-B93B-A0369F3102F6.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/42B5A080-C47A-E711-8581-008CFAF5592A.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/48FEC467-C47A-E711-9225-003048C8F3A2.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/76AFD275-C47A-E711-866A-A0369F836334.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/B8E1D1B7-647A-E711-AFBD-549F35AD8BE3.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/C66A2A15-687A-E711-9958-003048C91B0E.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/EA815E8F-C47A-E711-899F-B499BAAC0068.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/F0705882-C47A-E711-9B1C-001E67E713FE.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/F6DB155E-C47A-E711-BD8E-008CFA11118C.root'


if 'DisplacedSUSY_CTau-10_noPU' in options.inputDataset :
    print '\n Will read /DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/PhaseIITDRSpring17MiniAOD-noPU_91X_upgrade2023_realistic_v3-v2/MINIAODSIM\n'
    options.inputFiles = \
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/E09D9CD9-2F7A-E711-A40D-E0071B7A8590.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/D89A325B-C37A-E711-A911-0CC47AD98F6A.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/D889BAC6-C27A-E711-B44C-0025901AC0FA.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/CAB87706-C37A-E711-A22B-0025904C7FBE.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/B8B7DA5A-387A-E711-947B-001E67E71BC8.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/B044890A-C37A-E711-AE0C-A0369FC5EEB0.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/B00F9461-C37A-E711-9739-BCAEC509720C.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/A670DFDA-2A7A-E711-A098-0025905C42A4.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/A4F1D964-407A-E711-A69E-A0369FD8FDB0.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/9E0B3D01-C37A-E711-A920-002481CFE1EC.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/9441B700-C37A-E711-90ED-141877638F39.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/8860050E-C37A-E711-8866-0242AC110008.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/848E2ECF-C27A-E711-94DA-3417EBE64B9D.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/8067B768-CB7A-E711-88C4-002590E7D7C2.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/80648906-C37A-E711-8071-002590D0B09C.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/6E946200-C37A-E711-B3B6-009C02AAB554.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/4CA28D07-C37A-E711-A4DB-008CFAF5592A.root',\
        'file:/afs/cern.ch/work/p/piedra/public/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-10_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/304C1A01-C37A-E711-AC11-484D7E8DF092.root'


if 'DisplacedSUSY_CTau-100_PU200' in options.inputDataset :
    print '\n Will read /DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-100_TuneCUETP8M1_14TeV-pythia8/PhaseIITDRSpring17MiniAOD-PU200_91X_upgrade2023_realistic_v3-v1/MINIAODSIM\n'
    options.inputFiles = \
        'file:/eos/user/c/calderon//PhaseII/DisplacedSUSY_ctau100/924D80D2-575F-E711-A83E-0242AC110003.root',\
        'file:/eos/user/c/calderon//PhaseII/DisplacedSUSY_ctau100/88630411-705F-E711-A6F6-D4AE52E7F60F.root',\
        'file:/eos/user/c/calderon//PhaseII/DisplacedSUSY_ctau100/82388918-105F-E711-9B7B-008CFA111200.root',\
        'file:/eos/user/c/calderon//PhaseII/DisplacedSUSY_ctau100/80E15E0E-C65E-E711-B6F3-549F35AE4FBC.root',\
        'file:/eos/user/c/calderon//PhaseII/DisplacedSUSY_ctau100/809BA468-DF5E-E711-90C9-A0369FD20CF0.root',\
        'file:/eos/user/c/calderon//PhaseII/DisplacedSUSY_ctau100/62969EE8-DD5E-E711-9039-10983627C3DB.root',\
        'file:/eos/user/c/calderon//PhaseII/DisplacedSUSY_ctau100/4E6C4A11-655F-E711-8B45-001E67398CAA.root',\
        'file:/eos/user/c/calderon//PhaseII/DisplacedSUSY_ctau100/2CB9CF5E-AE5E-E711-9C91-549F35AF44FD.root',\
        'file:/eos/user/c/calderon//PhaseII/DisplacedSUSY_ctau100/2C6A987C-E35F-E711-BCF8-008CFA197EB0.root',\
        'file:/eos/user/c/calderon//PhaseII/DisplacedSUSY_ctau100/1A1C7B18-C05F-E711-A43C-001E67E69E32.root'


if 'DisplacedSUSY_CTau-1000_PU200' in options.inputDataset :
    print '\n Will read /DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1000_TuneCUETP8M1_14TeV-pythia8/PhaseIITDRSpring17MiniAOD-PU200_91X_upgrade2023_realistic_v3-v1/MINIAODSIM\n'
    options.inputFiles = \
        'file:/eos/user/c/calderon/PhaseII/DisplacedSUSY_ctau1000/604587B6-5B5F-E711-8B59-0CC47A5FA211.root',\
        'file:/eos/user/c/calderon/PhaseII/DisplacedSUSY_ctau1000/502147A8-5C5E-E711-86E8-141877636851.root',\
        'file:/eos/user/c/calderon/PhaseII/DisplacedSUSY_ctau1000/4E10029E-685F-E711-921F-D48564591BF4.root',\
        'file:/eos/user/c/calderon/PhaseII/DisplacedSUSY_ctau1000/469E8CCC-615E-E711-B200-0026B94DBE17.root',\
        'file:/eos/user/c/calderon/PhaseII/DisplacedSUSY_ctau1000/421C6C34-A55E-E711-8CFF-549F35AD8BFD.root',\
        'file:/eos/user/c/calderon/PhaseII/DisplacedSUSY_ctau1000/34F4A2D5-4E5E-E711-B9CF-0CC47A5FC619.root',\
        'file:/eos/user/c/calderon/PhaseII/DisplacedSUSY_ctau1000/2E803035-E95F-E711-81A4-002590DE6E36.root',\
        'file:/eos/user/c/calderon/PhaseII/DisplacedSUSY_ctau1000/2AA71B0E-A45E-E711-BBAE-008CFAF2931E.root',\
        'file:/eos/user/c/calderon/PhaseII/DisplacedSUSY_ctau1000/20FDD87E-5060-E711-977D-7CD30AD08F0E.root',\
        'file:/eos/user/c/calderon/PhaseII/DisplacedSUSY_ctau1000/0A5CE5C3-B15F-E711-8CA5-0090FAA57F44.root'


# Output
outputFileName = 'rootfiles/' + options.inputDataset + '.root'

print ' The output will be written in', outputFileName, '\n'

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(options.inputFiles))

process.TFileService=cms.Service('TFileService',
                                 fileName=cms.string(outputFileName)
                                 )

process.muonAnalysis = cms.EDAnalyzer("ExampleMuonAnalyzer",
                                      MuonCollection = cms.InputTag('slimmedMuons'),
                                      packed = cms.InputTag("packedGenParticles"),
                                      pruned = cms.InputTag("prunedGenParticles"),
                                      vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                      beamSpot = cms.InputTag("offlineBeamSpot")
                                      )

process.p = cms.Path(process.muonAnalysis)
