#ifndef MuonAnalyzer_H
#define MuonAnalyzer_H

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"


namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

class TH1I;
class TH1F;
class TH2F;


const Int_t   nbinspt = 6;

const Float_t ptbins[nbinspt+1] = {10, 20, 35, 50, 100, 200, 500};

const Float_t max_deltaR = 0.3;


class ExampleMuonAnalyzer: public edm::EDAnalyzer {
 public:
  // Constructor
  ExampleMuonAnalyzer(const edm::ParameterSet& pset);

  // Destructor
  virtual ~ExampleMuonAnalyzer();

  // Operations
  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);

  virtual void beginJob();
  virtual void endJob();
 protected:

 private:
  edm::EDGetTokenT<reco::BeamSpot>                    beamSpotToken;
  edm::EDGetTokenT<pat::MuonCollection>               muonToken;
  edm::EDGetTokenT<edm::View<pat::PackedGenParticle>> packedGenToken;
  edm::EDGetTokenT<edm::View<reco::GenParticle>>      prunedGenToken;
  edm::EDGetTokenT<reco::VertexCollection>            vtxToken;

  TH1F* hGenMuons_dxy;
  TH1F* hGenMuons_dz;
  TH1F* hGenMuons_vx;
  TH1F* hGenMuons_vy;
  TH1F* hGenMuons_vz;

  TH1F* hGenMuons_vr;
  TH1F* hStaMuons_vr;
  TH1F* hTrkMuons_vr;
  TH1F* hGlbMuons_vr;
  TH1F* hTightMuons_vr;
  TH1F* hSoftMuons_vr;
  TH1F* hStaMuons_noGen_vr;
  TH1F* hTrkMuons_noGen_vr; 
  TH1F* hGlbMuons_noGen_vr; 
  TH1F* hTightMuons_noGen_vr; 
  TH1F* hSoftMuons_noGen_vr; 

  TH1F* hGenMuons_eta;
  TH1F* hStaMuons_eta;
  TH1F* hTrkMuons_eta;
  TH1F* hGlbMuons_eta;
  TH1F* hTightMuons_eta;
  TH1F* hSoftMuons_eta;
  TH1F* hStaMuons_noGen_eta;
  TH1F* hTrkMuons_noGen_eta;
  TH1F* hGlbMuons_noGen_eta;
  TH1F* hTightMuons_noGen_eta;
  TH1F* hSoftMuons_noGen_eta;

  TH1F* hGenMuons_phi;
  TH1F* hStaMuons_phi;
  TH1F* hTrkMuons_phi;
  TH1F* hGlbMuons_phi;
  TH1F* hTightMuons_phi;                                                                                                           
  TH1F* hSoftMuons_phi;
  TH1F* hStaMuons_noGen_phi;    
  TH1F* hTrkMuons_noGen_phi;
  TH1F* hGlbMuons_noGen_phi;                                                                                                        
  TH1F* hTightMuons_noGen_phi;
  TH1F* hSoftMuons_noGen_phi;

  TH1F* hGenMuons_pt;
  TH1F* hStaMuons_pt;
  TH1F* hTrkMuons_pt;
  TH1F* hGlbMuons_pt;
  TH1F* hTightMuons_pt;                                                                                                             
  TH1F* hSoftMuons_pt;
  TH1F* hStaMuons_noGen_pt;                                                                                            
  TH1F* hTrkMuons_noGen_pt;                                                                                                         
  TH1F* hGlbMuons_noGen_pt;                                                                                                         
  TH1F* hTightMuons_noGen_pt;                                                                                                          
  TH1F* hSoftMuons_noGen_pt;

  TH1F* hStaMuons_dR;
  TH1F* hTrkMuons_dR;
  TH1F* hGlbMuons_dR;
  TH1F* hTightMuons_dR;                                                                                                            
  TH1F* hSoftMuons_dR;

  TH1F* hStaMuons_res  [nbinspt];
  TH1F* hTrkMuons_res  [nbinspt];
  TH1F* hGlbMuons_res  [nbinspt];
  TH1F* hTightMuons_res[nbinspt];
  TH1F* hSoftMuons_res [nbinspt];

  TH1F* hMuPFChargeIso; 
  TH1F* hMuPFNeutralIso;
  TH1F* hMuPFPhotonIso; 
  TH1F* hMuPFPUIso;   
  TH1F* hMuPFIso;
};
#endif
