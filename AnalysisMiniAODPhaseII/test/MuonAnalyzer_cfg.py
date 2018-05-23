import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as opts

process = cms.Process("RecoMuon")

options = opts.VarParsing('analysis')

options.register ('inputDataset',
                  'PU200',
                  opts.VarParsing.multiplicity.singleton,
                  opts.VarParsing.varType.string,
                  'Input dataset')

options.parseArguments()

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('Demo')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.cerr.INFO = cms.untracked.PSet(limit = cms.untracked.int32(-1))


if 'PU200' in options.inputDataset :
    print '\n Will read /DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/PhaseIITDRSpring17MiniAOD-PU200BX8_91X_upgrade2023_realistic_v3-v6/MINIAODSIM\n'
    options.inputFiles = \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/14211B25-C077-E711-90D0-0025905D1D78.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/1E5E5389-B477-E711-8518-0025905D1C56.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/5823B0EF-EB78-E711-A5B1-1866DAEA6E20.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/6671BA0A-9378-E711-8976-0025904C540C.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/68954ACD-E778-E711-BFE8-0025905521D2.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/70BEFF66-CD77-E711-B41D-B083FECFF2BE.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/7ADEB521-0578-E711-B0B4-842B2B1810D3.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/82597027-E577-E711-AC49-141877411D83.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/825E4ACC-BB77-E711-91D3-0025904C66EA.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/A0AA1AF5-9279-E711-9B35-782BCB5300A1.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/BC4957D1-C577-E711-8473-0025905D1CB6.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/D6BF7ECF-1E78-E711-A99D-B083FED429D5.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/DAE448AC-A977-E711-8A39-0025904C66A2.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/F0154A8B-F077-E711-95DC-141877410E71.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/F08EEFEF-DB77-E711-9912-A4BADB1C5E26.root'

if 'noPU' in options.inputDataset :
    print '\n Will read /DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/PhaseIITDRSpring17MiniAOD-noPUBX8_91X_upgrade2023_realistic_v3-v3/MINIAODSIM\n'
    options.inputFiles = 'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/noPUBX8_91X_upgrade2023_realistic_v3-v3/50000/FCEFF295-A27C-E711-92A6-0CC47AD98CEA.root'


process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(options.inputFiles))


#process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(3000))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.TFileService=cms.Service('TFileService',
                                 fileName=cms.string('MyMuonPlots.root')
                                 )

process.muonAnalysis = cms.EDAnalyzer("ExampleMuonAnalyzer",
                                      MuonCollection = cms.InputTag('slimmedMuons'),
                                      packed = cms.InputTag("packedGenParticles"),
                                      pruned = cms.InputTag("prunedGenParticles"),
                                      vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                      beamSpot = cms.InputTag("offlineBeamSpot")
                                      )

process.p = cms.Path(process.muonAnalysis)
