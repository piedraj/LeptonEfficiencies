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


const Int_t   nbinspt = 3;

const Float_t ptbins[nbinspt+1] = {10, 20, 35, 50};

const Float_t max_deltaR = 0.5;
const Float_t max_vr     =  50;  // [cm]


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

  // TH1 histograms
  TH1F* hGenMuons_eta;
  TH1F* hGenMuons_pt;
  TH1F* hGenMuons_vx;
  TH1F* hGenMuons_vy;
  TH1F* hGenMuons_vz;
  TH1F* hGenMuons_vr;
  
  TH1F* hStaMuons_dR;
  TH1F* hStaMuons_pt;
  TH1F* hStaMuons_vr;
  TH1F* hStaMuons_res[nbinspt];

  TH1F* hTrkMuons_dR;
  TH1F* hTrkMuons_pt;
  TH1F* hTrkMuons_vr;
  TH1F* hTrkMuons_res[nbinspt];

  TH1F* hGlbMuons_dR;
  TH1F* hGlbMuons_pt;
  TH1F* hGlbMuons_vr;
  TH1F* hGlbMuons_res[nbinspt];

  // TH2 histograms
  TH2F* hGenStaMuons_eta;
  TH2F* hGenStaMuons_phi;
};
#endif
