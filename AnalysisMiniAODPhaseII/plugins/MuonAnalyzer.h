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


const Int_t nbins_pt  =  6;
const Int_t nbins_vxy = 10;
const Int_t nbins_vz  = 10;

const Float_t pt_bins[nbins_pt+1] = {10, 20, 35, 50, 100, 200, 500};

const Float_t vxy_bins[nbins_vxy+1] = {0.0000, 0.0088, 0.0200, 0.0334, 0.0498, 0.0704, 0.0968, 0.1332, 0.1892, 0.2988, 3.0000};

const Float_t vz_bins[nbins_vz+1] = {0.0000, 0.5080, 1.0364, 1.5974, 2.1970, 2.8732, 3.6246, 4.4544, 5.5000, 7.0216, 20.0000};

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

  TH1F* h_dxy;
  TH1F* h_dz;
  TH1F* h_vxy;
  TH1F* h_vz;

  TH2F* hGenMuons_vxy_vz;
  TH2F* hStaMuons_vxy_vz;
  TH2F* hTrkMuons_vxy_vz;
  TH2F* hGlbMuons_vxy_vz;
  TH2F* hTightMuons_vxy_vz;
  TH2F* hSoftMuons_vxy_vz;
  TH2F* hStaMuons_noGen_vxy_vz;
  TH2F* hTrkMuons_noGen_vxy_vz;
  TH2F* hGlbMuons_noGen_vxy_vz;
  TH2F* hTightMuons_noGen_vxy_vz;
  TH2F* hSoftMuons_noGen_vxy_vz;

  TH1F* hGenMuons_vxy;
  TH1F* hStaMuons_vxy;
  TH1F* hTrkMuons_vxy;
  TH1F* hGlbMuons_vxy;
  TH1F* hTightMuons_vxy;
  TH1F* hSoftMuons_vxy;
  TH1F* hStaMuons_noGen_vxy;
  TH1F* hTrkMuons_noGen_vxy; 
  TH1F* hGlbMuons_noGen_vxy; 
  TH1F* hTightMuons_noGen_vxy; 
  TH1F* hSoftMuons_noGen_vxy; 

  TH1F* hGenMuons_vz;
  TH1F* hStaMuons_vz;
  TH1F* hTrkMuons_vz;
  TH1F* hGlbMuons_vz;
  TH1F* hTightMuons_vz;
  TH1F* hSoftMuons_vz;
  TH1F* hStaMuons_noGen_vz;
  TH1F* hTrkMuons_noGen_vz; 
  TH1F* hGlbMuons_noGen_vz; 
  TH1F* hTightMuons_noGen_vz; 
  TH1F* hSoftMuons_noGen_vz; 

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

  TH1F* hStaMuons_res  [nbins_pt];
  TH1F* hTrkMuons_res  [nbins_pt];
  TH1F* hGlbMuons_res  [nbins_pt];
  TH1F* hTightMuons_res[nbins_pt];
  TH1F* hSoftMuons_res [nbins_pt];

  TH1F* hMuPFChargeIso; 
  TH1F* hMuPFNeutralIso;
  TH1F* hMuPFPhotonIso; 
  TH1F* hMuPFPUIso;   
  TH1F* hMuPFIso;
};
#endif
