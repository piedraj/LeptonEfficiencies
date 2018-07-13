#include "MuonAnalyzer.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/MessageService/interface/MessageServicePresence.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TH1I.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TMath.h"

using namespace edm;
using namespace reco;
using namespace std;


ExampleMuonAnalyzer::ExampleMuonAnalyzer(const ParameterSet& pset)
{
  beamSpotToken  = consumes<reco::BeamSpot>(pset.getParameter<InputTag>("beamSpot"));
  muonToken      = consumes<pat::MuonCollection>(pset.getParameter<InputTag>("MuonCollection"));
  packedGenToken = consumes<edm::View<pat::PackedGenParticle>>(pset.getParameter<InputTag>("packed"));
  prunedGenToken = consumes<edm::View<reco::GenParticle>>(pset.getParameter<InputTag>("pruned"));
  vtxToken       = consumes<reco::VertexCollection>(pset.getParameter<InputTag>("vertices"));
}


ExampleMuonAnalyzer::~ExampleMuonAnalyzer() {}


void ExampleMuonAnalyzer::beginJob()
{
  cout << "\n [ExampleMuonAnalyzer::beginJob]\n" << endl;

  edm::Service<TFileService> fileService;

  hGenMuons_vx         = fileService->make<TH1F>("GenMuons_vx",         "", 750, 0, 750);
  hGenMuons_vy         = fileService->make<TH1F>("GenMuons_vy",         "", 750, 0, 750);
  hGenMuons_vz         = fileService->make<TH1F>("GenMuons_vz",         "", 750, 0, 750);
  hGenMuons_vr         = fileService->make<TH1F>("GenMuons_vr",         "", 750, 0, 750);
  hStaMuons_vr         = fileService->make<TH1F>("StaMuons_vr",         "", 750, 0, 750);
  hTrkMuons_vr         = fileService->make<TH1F>("TrkMuons_vr",         "", 750, 0, 750);
  hGlbMuons_vr         = fileService->make<TH1F>("GlbMuons_vr",         "", 750, 0, 750);
  hTightMuons_vr       = fileService->make<TH1F>("TightMuons_vr",       "", 750, 0, 750);
  hSoftMuons_vr        = fileService->make<TH1F>("SoftMuons_vr",        "", 750, 0, 750);
  hStaMuons_noGen_vr   = fileService->make<TH1F>("StaMuons_noGen_vr",   "", 750, 0, 750);
  hTrkMuons_noGen_vr   = fileService->make<TH1F>("TrkMuons_noGen_vr",   "", 750, 0, 750);
  hGlbMuons_noGen_vr   = fileService->make<TH1F>("GlbMuons_noGen_vr",   "", 750, 0, 750);
  hTightMuons_noGen_vr = fileService->make<TH1F>("TightMuons_noGen_vr", "", 750, 0, 750);
  hSoftMuons_noGen_vr  = fileService->make<TH1F>("SoftMuons_noGen_vr",  "", 750, 0, 750);

  hGenMuons_eta         = fileService->make<TH1F>("GenMuons_eta",         "", 100, -2.5, 2.5);
  hStaMuons_eta         = fileService->make<TH1F>("StaMuons_eta",         "", 100, -2.5, 2.5);
  hTrkMuons_eta         = fileService->make<TH1F>("TrkMuons_eta",         "", 100, -2.5, 2.5);
  hGlbMuons_eta         = fileService->make<TH1F>("GlbMuons_eta",         "", 100, -2.5, 2.5);
  hTightMuons_eta       = fileService->make<TH1F>("TightMuons_eta",       "", 100, -2.5, 2.5);
  hSoftMuons_eta        = fileService->make<TH1F>("SoftMuons_eta",        "", 100, -2.5, 2.5);
  hStaMuons_noGen_eta   = fileService->make<TH1F>("StaMuons_noGen_eta",   "", 100, -2.5, 2.5);
  hTrkMuons_noGen_eta   = fileService->make<TH1F>("TrkMuons_noGen_eta",   "", 100, -2.5, 2.5);
  hGlbMuons_noGen_eta   = fileService->make<TH1F>("GlbMuons_noGen_eta",   "", 100, -2.5, 2.5);
  hTightMuons_noGen_eta = fileService->make<TH1F>("TightMuons_noGen_eta", "", 100, -2.5, 2.5);
  hSoftMuons_noGen_eta  = fileService->make<TH1F>("SoftMuons_noGen_eta",  "", 100, -2.5, 2.5);

  hGenMuons_phi         = fileService->make<TH1F>("GenMuons_phi",         "", 100, -3.2, 3.2);
  hStaMuons_phi         = fileService->make<TH1F>("StaMuons_phi",         "", 100, -3.2, 3.2);
  hTrkMuons_phi         = fileService->make<TH1F>("TrkMuons_phi",         "", 100, -3.2, 3.2);
  hGlbMuons_phi         = fileService->make<TH1F>("GlbMuons_phi",         "", 100, -3.2, 3.2);
  hTightMuons_phi       = fileService->make<TH1F>("TightMuons_phi",       "", 100, -3.2, 3.2);
  hSoftMuons_phi        = fileService->make<TH1F>("SoftMuons_phi",        "", 100, -3.2, 3.2);
  hStaMuons_noGen_phi   = fileService->make<TH1F>("StaMuons_noGen_phi",   "", 100, -3.2, 3.2);
  hTrkMuons_noGen_phi   = fileService->make<TH1F>("TrkMuons_noGen_phi",   "", 100, -3.2, 3.2);
  hGlbMuons_noGen_phi   = fileService->make<TH1F>("GlbMuons_noGen_phi",   "", 100, -3.2, 3.2);
  hTightMuons_noGen_phi = fileService->make<TH1F>("TightMuons_noGen_phi", "", 100, -3.2, 3.2);
  hSoftMuons_noGen_phi  = fileService->make<TH1F>("SoftMuons_noGen_phi",  "", 100, -3.2, 3.2);

  hGenMuons_pt         = fileService->make<TH1F>("GenMuons_pt",         "", 500, 0, 500);
  hStaMuons_pt         = fileService->make<TH1F>("StaMuons_pt",         "", 500, 0, 500);
  hTrkMuons_pt         = fileService->make<TH1F>("TrkMuons_pt",         "", 500, 0, 500);
  hGlbMuons_pt         = fileService->make<TH1F>("GlbMuons_pt",         "", 500, 0, 500);
  hTightMuons_pt       = fileService->make<TH1F>("TightMuons_pt",       "", 500, 0, 500);
  hSoftMuons_pt        = fileService->make<TH1F>("SoftMuons_pt",        "", 500, 0, 500);
  hStaMuons_noGen_pt   = fileService->make<TH1F>("StaMuons_noGen_pt",   "", 500, 0, 500);
  hTrkMuons_noGen_pt   = fileService->make<TH1F>("TrkMuons_noGen_pt",   "", 500, 0, 500);
  hGlbMuons_noGen_pt   = fileService->make<TH1F>("GlbMuons_noGen_pt",   "", 500, 0, 500);
  hTightMuons_noGen_pt = fileService->make<TH1F>("TightMuons_noGen_pt", "", 500, 0, 500);
  hSoftMuons_noGen_pt  = fileService->make<TH1F>("SoftMuons_noGen_pt",  "", 500, 0, 500);

  hStaMuons_dR   = fileService->make<TH1F>("StaMuons_dR",   "", 100, 0, 4);
  hTrkMuons_dR   = fileService->make<TH1F>("TrkMuons_dR",   "", 100, 0, 4);
  hGlbMuons_dR   = fileService->make<TH1F>("GlbMuons_dR",   "", 100, 0, 4);
  hTightMuons_dR = fileService->make<TH1F>("TightMuons_dR", "", 100, 0, 4);
  hSoftMuons_dR  = fileService->make<TH1F>("SoftMuons_dR",  "", 100, 0, 4);

  for (Int_t i=0; i<nbinspt; i++) {
    hStaMuons_res[i]   = fileService->make<TH1F>(Form("StaMuons_res_%d",   i), "#Deltaq/p_{T} / q/p_{T}", 120, -0.15, 0.15);
    hTrkMuons_res[i]   = fileService->make<TH1F>(Form("TrkMuons_res_%d",   i), "#Deltaq/p_{T} / q/p_{T}", 120, -0.15, 0.15);
    hGlbMuons_res[i]   = fileService->make<TH1F>(Form("GlbMuons_res_%d",   i), "#Deltaq/p_{T} / q/p_{T}", 120, -0.15, 0.15);
    hTightMuons_res[i] = fileService->make<TH1F>(Form("TightMuons_res_%d", i), "#Deltaq/p_{T} / q/p_{T}", 120, -0.15, 0.15);
    hSoftMuons_res[i]  = fileService->make<TH1F>(Form("SoftMuons_res_%d",  i), "#Deltaq/p_{T} / q/p_{T}", 120, -0.15, 0.15);
  }

  hMuPFChargeIso  = fileService->make<TH1F>("MuPFChargeIso",  "#DeltaR=0.4 PFChargeIso",  200, 0, 1);
  hMuPFNeutralIso = fileService->make<TH1F>("MuPFNeutralIso", "#DeltaR=0.4 PFNeutralIso", 200, 0, 1);
  hMuPFPhotonIso  = fileService->make<TH1F>("MuPFPhotonIso",  "#DeltaR=0.4 PFPhotonIso",  200, 0, 1);
  hMuPFPUIso      = fileService->make<TH1F>("MuPFPUIso",      "#DeltaR=0.4 PFPUIso",      200, 0, 1);
  hMuPFIso        = fileService->make<TH1F>("MuPFIso",        "#DeltaR=0.4 SumPtIso",     200, 0, 1); 
}


void ExampleMuonAnalyzer::endJob() {}


void ExampleMuonAnalyzer::analyze(const Event& event, const EventSetup& eventSetup)
{
  // BeamSpot
  edm::Handle<reco::BeamSpot> beamSpot;
  event.getByToken(beamSpotToken, beamSpot);
  reco::BeamSpot bs;

  if (beamSpot.isValid()) bs = *beamSpot;    


  // Vertex collection
  edm::Handle<reco::VertexCollection> vertices;
  event.getByToken(vtxToken, vertices);


  // Look for the PrimaryVertex and use the BeamSpot if you can't find it. Code snippet taken from
  // https://github.com/cms-sw/cmssw/blob/master/DQMOffline/Muon/src/EfficiencyAnalyzer.cc#L186-L218
  //----------------------------------------------------------------------------
  reco::Vertex::Point posVtx;
  reco::Vertex::Error errVtx;

  UInt_t theIndexOfThePrimaryVertex = 999.;

  if (vertices.isValid()) {
    for (unsigned int ind=0; ind<vertices->size(); ++ind) {
      if ((*vertices)[ind].isValid() && !((*vertices)[ind].isFake())) {
	theIndexOfThePrimaryVertex = ind;
	break;
      }
    }
  }
  
  if (theIndexOfThePrimaryVertex < 100) {
    posVtx = ((*vertices)[theIndexOfThePrimaryVertex]).position();
    errVtx = ((*vertices)[theIndexOfThePrimaryVertex]).error();
  }
  else if (beamSpot.isValid()) {
    LogInfo("ExampleMuonAnalyzer") << "reco::PrimaryVertex not found, use BeamSpot position instead\n";
    
    posVtx      = bs.position();
    errVtx(0,0) = bs.BeamWidthX();
    errVtx(1,1) = bs.BeamWidthY();
    errVtx(2,2) = bs.sigmaZ();
  }
    
  const reco::Vertex thePrimaryVertex(posVtx,errVtx);


  // Pruned particles are the ones containing "important" stuff
  Handle<edm::View<reco::GenParticle> > pruned;
  event.getByToken(prunedGenToken, pruned);


  // Packed particles are all the status 1, so usable to remake jets
  // The navigation from status 1 to pruned is possible (the other direction should be made by hand)
  Handle<edm::View<pat::PackedGenParticle> > packed;
  event.getByToken(packedGenToken, packed);


  // Get the muon collection
  Handle<pat::MuonCollection> muons;
  event.getByToken(muonToken, muons);


  // Loop over pruned particles
  //----------------------------------------------------------------------------
  for (size_t i=0; i<pruned->size(); i++) {

    if (abs((*pruned)[i].pdgId()) != 13)    continue;
    if (!(*pruned)[i].isPromptFinalState()) continue;
    if (!(*pruned)[i].isLastCopy())         continue;

    Float_t charge = (*pruned)[i].charge();
    Float_t eta    = (*pruned)[i].eta();
    Float_t phi    = (*pruned)[i].phi();
    Float_t pt     = (*pruned)[i].pt();
    Float_t vx     = (*pruned)[i].vx();
    Float_t vy     = (*pruned)[i].vy();
    Float_t vz     = (*pruned)[i].vz();

    if (fabs(eta) > 2.4) continue;
    if (pt < ptbins[0])  continue;

    Float_t vr = sqrt(vx*vx + vy*vy + vz*vz);


    // Loop over reconstructed muons
    //--------------------------------------------------------------------------
    Float_t sta_min_deltaR   = 999;   
    Float_t trk_min_deltaR   = 999; 
    Float_t glb_min_deltaR   = 999; 
    Float_t tight_min_deltaR = 999;   
    Float_t soft_min_deltaR  = 999;   

    Float_t sta_res   = -999;
    Float_t trk_res   = -999;
    Float_t glb_res   = -999;
    Float_t tight_res = -999;
    Float_t soft_res  = -999;
    
    for (pat::MuonCollection::const_iterator muon=muons->begin(); muon!=muons->end(); ++muon) {

      Float_t chargeIso  = muon->pfIsolationR04().sumChargedHadronPt;
      Float_t neutralIso = muon->pfIsolationR04().sumNeutralHadronEt;
      Float_t photonIso  = muon->pfIsolationR04().sumPhotonEt;
      Float_t puIso      = muon->pfIsolationR04().sumPUPt;
      Float_t iso        = chargeIso + max(0., neutralIso + photonIso - 0.5*puIso);

      hMuPFChargeIso ->Fill(chargeIso  / muon->pt());
      hMuPFNeutralIso->Fill(neutralIso / muon->pt());
      hMuPFPhotonIso ->Fill(photonIso  / muon->pt());
      hMuPFPUIso     ->Fill(puIso      / muon->pt());
      hMuPFIso       ->Fill(iso        / muon->pt());


      // isStandAloneMuon
      //------------------------------------------------------------------------
      if (muon->isStandAloneMuon()) {
     
	Float_t muEta    = muon->standAloneMuon()->eta();
	Float_t muPhi    = muon->standAloneMuon()->phi();
	Float_t muPt     = muon->standAloneMuon()->pt();
	Float_t muCharge = muon->standAloneMuon()->charge();

	if (fabs(muEta) > 2.4) continue;
	if (muPt < ptbins[0])  continue;

	Float_t dPhi = muPhi - phi;
	Float_t dEta = muEta - eta;

	Float_t dR = sqrt(dPhi*dPhi + dEta*dEta); 

	if (dR < sta_min_deltaR) {
	  sta_min_deltaR = dR;
	  sta_res        = ((muCharge/muPt) - (charge/pt)) / (charge/pt);
	}
      }


      // isTrackerMuon
      //------------------------------------------------------------------------
      if (muon->isTrackerMuon()) {

	float muEta    = muon->innerTrack()->eta();
	float muPhi    = muon->innerTrack()->phi();
	float muPt     = muon->innerTrack()->pt();
	float muCharge = muon->innerTrack()->charge();

	if (fabs(muEta) > 2.4) continue;
	if (muPt < ptbins[0])  continue;
       
	float dPhi = muPhi - phi;
	float dEta = muEta - eta;

	float dR = sqrt(dPhi*dPhi + dEta*dEta); 
	
	if (dR < trk_min_deltaR) {
	  trk_min_deltaR = dR;
	  trk_res        = ((muCharge/muPt) - (charge/pt)) / (charge/pt);
	}
      }


      // isGlobalMuon
      //------------------------------------------------------------------------
      if (muon->isGlobalMuon() && muon->isStandAloneMuon()) {

	float muEta    = muon->globalTrack()->eta();
	float muPhi    = muon->globalTrack()->phi();
	float muPt     = muon->globalTrack()->pt();
	float muCharge = muon->globalTrack()->charge();

	if (fabs(muEta) > 2.4) continue;
	if (muPt < ptbins[0])  continue;
       
	float dPhi = muPhi - phi;
	float dEta = muEta - eta;

	float dR = sqrt(dPhi*dPhi + dEta*dEta); 
	
	if (dR < glb_min_deltaR) {
	  glb_min_deltaR = dR;
	  glb_res        = ((muCharge/muPt) - (charge/pt)) / (charge/pt);
	}
      }


      // isTightMuon
      //------------------------------------------------------------------------
      if (muon::isTightMuon(*muon, thePrimaryVertex)) {
	
	Float_t muEta    = muon->eta();
	Float_t muPhi    = muon->phi();
	Float_t muPt     = muon->pt();
	Float_t muCharge = muon->charge();

	if (fabs(muEta) > 2.4) continue;
	if (muPt < ptbins[0])  continue;
 
	Float_t dPhi = muPhi - phi;
	Float_t dEta = muEta - eta;

	Float_t dR = sqrt(dPhi*dPhi + dEta*dEta); 

	if (dR < tight_min_deltaR) {
	  tight_min_deltaR = dR;
	  tight_res        = ((muCharge/muPt) - (charge/pt)) / (charge/pt);
	}
      }


      // isSoftMuon
      //------------------------------------------------------------------------
      if (muon::isSoftMuon(*muon, thePrimaryVertex)) {
	
	Float_t muEta    = muon->eta();
	Float_t muPhi    = muon->phi();
	Float_t muPt     = muon->pt();
	Float_t muCharge = muon->charge();

	if (fabs(muEta) > 2.4) continue;
	if (muPt < ptbins[0])  continue;
 
	Float_t dPhi = muPhi - phi;
	Float_t dEta = muEta - eta;

	Float_t dR = sqrt(dPhi*dPhi + dEta*dEta); 

	if (dR < soft_min_deltaR) {
	  soft_min_deltaR = dR;
	  soft_res        = ((muCharge/muPt) - (charge/pt)) / (charge/pt);
	}
      }
    }  // for..muons


    if (vr > max_vr) continue;
  

    // Fill gen histograms
    //--------------------------------------------------------------------------
    hGenMuons_vx ->Fill(fabs(vx));
    hGenMuons_vy ->Fill(fabs(vy));
    hGenMuons_vz ->Fill(fabs(vz));
    hGenMuons_vr ->Fill(vr);
    hGenMuons_eta->Fill(eta);
    hGenMuons_phi->Fill(phi);
    hGenMuons_pt ->Fill(pt);


    // Fill sta histograms
    //--------------------------------------------------------------------------
    if (sta_min_deltaR < 999)
      {
	hStaMuons_dR->Fill(sta_min_deltaR);

	if (sta_min_deltaR < max_deltaR)
	  {
	    hStaMuons_vr ->Fill(vr);
	    hStaMuons_eta->Fill(eta);
	    hStaMuons_phi->Fill(phi);
	    hStaMuons_pt ->Fill(pt);

	    for (Int_t i=0; i<nbinspt; i++)
	      if (pt >= ptbins[i] && pt < ptbins[i+1]) hStaMuons_res[i]->Fill(sta_res);
	  }
	else
	  {
	    hStaMuons_noGen_vr ->Fill(vr);
	    hStaMuons_noGen_eta->Fill(eta);
	    hStaMuons_noGen_phi->Fill(phi);
	    hStaMuons_noGen_pt ->Fill(pt);
	  }
      }


    // Fill trk histograms
    //--------------------------------------------------------------------------
    if (trk_min_deltaR < 999)
      {
	hTrkMuons_dR->Fill(trk_min_deltaR);
	
	if (trk_min_deltaR < max_deltaR)
	  {
	    hTrkMuons_vr ->Fill(vr);
	    hTrkMuons_eta->Fill(eta);
	    hTrkMuons_phi->Fill(phi);
	    hTrkMuons_pt ->Fill(pt);
	
	    for (Int_t i=0; i<nbinspt; i++)
	      if (pt >= ptbins[i] && pt < ptbins[i+1]) hTrkMuons_res[i]->Fill(trk_res);
	  }
	else
	  {
	    hTrkMuons_noGen_vr ->Fill(vr);
	    hTrkMuons_noGen_eta->Fill(eta);
	    hTrkMuons_noGen_phi->Fill(phi);
	    hTrkMuons_noGen_pt ->Fill(pt);
	  }
      }


    // Fill glb histograms
    //--------------------------------------------------------------------------
    if (glb_min_deltaR < 999)
      {
	hGlbMuons_dR->Fill(glb_min_deltaR);

	if (glb_min_deltaR < max_deltaR)
	  {
	    hGlbMuons_vr ->Fill(vr);
	    hGlbMuons_eta->Fill(eta);	
	    hGlbMuons_phi->Fill(phi);
	    hGlbMuons_pt ->Fill(pt);

	    for (Int_t i=0; i<nbinspt; i++)
	      if (pt >= ptbins[i] && pt < ptbins[i+1]) hGlbMuons_res[i]->Fill(glb_res);
	  }
	else
	  {
	    hGlbMuons_noGen_vr ->Fill(vr);
	    hGlbMuons_noGen_eta->Fill(eta);	
	    hGlbMuons_noGen_phi->Fill(phi);
	    hGlbMuons_noGen_pt ->Fill(pt);
	  }
      }


    // Fill tight histograms
    //--------------------------------------------------------------------------
    if (tight_min_deltaR < 999)
      {
	hTightMuons_dR->Fill(tight_min_deltaR);
	
	if (tight_min_deltaR < max_deltaR)
	  {
	    hTightMuons_vr ->Fill(vr);
	    hTightMuons_eta->Fill(eta);	
	    hTightMuons_phi->Fill(phi);
	    hTightMuons_pt ->Fill(pt);

	    for (Int_t i=0; i<nbinspt; i++)
	      if (pt >= ptbins[i] && pt < ptbins[i+1]) hTightMuons_res[i]->Fill(tight_res);
	  }
	else
	  {
	    hTightMuons_noGen_vr ->Fill(vr);
	    hTightMuons_noGen_eta->Fill(eta);
	    hTightMuons_noGen_phi->Fill(phi);
	    hTightMuons_noGen_pt ->Fill(pt);
	  }
      }


    // Fill soft histograms
    //--------------------------------------------------------------------------
    if (soft_min_deltaR < 999)
      {
	hSoftMuons_dR->Fill(soft_min_deltaR);
	
	if (soft_min_deltaR < max_deltaR)
	  {
	    hSoftMuons_vr ->Fill(vr);
	    hSoftMuons_eta->Fill(eta);	
	    hSoftMuons_phi->Fill(phi);
	    hSoftMuons_pt ->Fill(pt);

	    for (Int_t i=0; i<nbinspt; i++)
	      if (pt >= ptbins[i] && pt < ptbins[i+1]) hSoftMuons_res[i]->Fill(soft_res);
	  }
	else
	  {
	    hSoftMuons_noGen_vr ->Fill(vr);
	    hSoftMuons_noGen_eta->Fill(eta);
	    hSoftMuons_noGen_phi->Fill(phi);
	    hSoftMuons_noGen_pt ->Fill(pt);
	  }
      }
  } // for..pruned
} 

DEFINE_FWK_MODULE(ExampleMuonAnalyzer);
