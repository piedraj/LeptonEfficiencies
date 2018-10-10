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


const int    nbins_pt =   6;
const double pt_min   =  10;
const double pt_max   = 500;
const double pt_bins[nbins_pt+1] = {pt_min, 60, 90, 130, 170, 250, pt_max};

const int    nbins_vxy =  15;
const double vxy_max   = 300;
const double vxy_bins[nbins_vxy+1] = {0.0, 0.0072, 0.0160, 0.0260, 0.0384, 0.0526, 0.0702, 0.0916, 0.1194, 0.1576, 0.2168, 0.3292, 1.0, 2.0, 3.0, vxy_max};

const int    nbins_vz =   15;
const double vz_max   = 1000;
const double vz_bins[nbins_vz+1] = {0.0, 0.4292, 0.8582, 1.3158, 1.7888, 2.3100, 2.8726, 3.4866, 4.1736, 4.9496, 5.9360, 7.3894, 10.0, 15.0, 20.0, vz_max};

const int    nbins_vr =   15;
const double vr_max   = 1000;
const double vr_bins[nbins_vr+1] = {0.0, 0.4706, 0.8794, 1.3364, 1.8032, 2.3190, 2.8786, 3.4920, 4.1806, 4.9548, 5.9422, 7.3908, 10.0, 15.0, 20.0, vr_max};

const double deltaR_max = 0.3;


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
  TH1F* h_vr;
  TH1F* h_pt;

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

  TH2F* hStaMuons_pt_resolution  [nbins_pt];
  TH2F* hTrkMuons_pt_resolution  [nbins_pt];
  TH2F* hGlbMuons_pt_resolution  [nbins_pt];
  TH2F* hTightMuons_pt_resolution[nbins_pt];
  TH2F* hSoftMuons_pt_resolution [nbins_pt];

  TH1F* hMuPFChargeIso; 
  TH1F* hMuPFNeutralIso;
  TH1F* hMuPFPhotonIso; 
  TH1F* hMuPFPUIso;   
  TH1F* hMuPFIso;
};
#endif
