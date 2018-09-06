//------------------------------------------------------------------------------
//
// Compare displaced muons histograms for samples with no PU and 200 PU
//
// root -l -b -q doEfficiencies.C+
//
//------------------------------------------------------------------------------
#include "TCanvas.h"
#include "TFile.h"
#include "TFrame.h"
#include "TGraphAsymmErrors.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TInterpreter.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "TString.h"
#include "TSystem.h"

#include <fstream>


// Data members
//------------------------------------------------------------------------------
enum        {noPU, PU200};

Bool_t      doSavePdf = true;
Bool_t      doSavePng = true;
Bool_t      doSaveTcl = false;

TFile*      file1 = NULL;
TFile*      file2 = NULL;

Bool_t      draw_sta   = false;
Bool_t      draw_trk   = false;
Bool_t      draw_glb   = false;
Bool_t      draw_tight = true;
Bool_t      draw_soft  = true;
Bool_t      draw_fakes = false;

TString     directory = "displaced-muons";


// Member functions
//------------------------------------------------------------------------------
TGraphAsymmErrors* MakeEfficiency(TString  effType,
				  TString  muonType,
				  TString  variable,
				  Int_t    PU,
				  Color_t  color,
				  Int_t    rebin);

void               DrawEfficiency(TString  effType,
				  TString  variable,
				  TString  xtitle,
				  Int_t    rebin = -1);

void               Compare       (TString  variable,
				  TString  muonType,
				  TString  xtitle,
				  Float_t  xmax = -999);

TH1F*              AddOverflow   (TH1F*    h);

void               SetLegend     (TLegend* tl,
				  Size_t   tsize);


//------------------------------------------------------------------------------
//
// doEfficiencies
//
//------------------------------------------------------------------------------
void doEfficiencies()
{
  gInterpreter->ExecuteMacro("PaperStyle.C");

  if (doSavePdf) gSystem->mkdir(directory,          kTRUE);
  if (doSavePng) gSystem->mkdir(directory,          kTRUE);
  if (doSaveTcl) gSystem->mkdir(directory + "/tcl", kTRUE);

  TH1::SetDefaultSumw2();


  // Input files
  //----------------------------------------------------------------------------
  file1 = TFile::Open("rootfiles/DisplacedSUSY_CTau-1_PU200.root");
//file1 = TFile::Open("rootfiles/DisplacedSUSY_CTau-10_noPU.root");
  file2 = TFile::Open("rootfiles/DisplacedSUSY_CTau-1_noPU.root");


  // Do the work
  //----------------------------------------------------------------------------
  DrawEfficiency("efficiency", "vxy", "gen production distance in xy [cm]");
  DrawEfficiency("efficiency", "vz",  "gen production distance in z [cm]");
  DrawEfficiency("efficiency", "vr",  "gen production distance in xyz [cm]");
  DrawEfficiency("efficiency", "eta", "gen #eta");
  DrawEfficiency("efficiency", "pt",  "gen p_{T} [GeV]");

  if (draw_fakes)
    {
      DrawEfficiency("fakes", "vxy", "gen production distance in xy [cm]");
      DrawEfficiency("fakes", "vz",  "gen production distance in z [cm]");
      DrawEfficiency("fakes", "vr",  "gen production distance in xyz [cm]");
      DrawEfficiency("fakes", "eta", "gen #eta");
      DrawEfficiency("fakes", "pt",  "gen p_{T} [GeV]");
  }

  if (draw_sta)   Compare("dR", "Sta",   "#DeltaR(gen, standalone)");
  if (draw_trk)   Compare("dR", "Trk",   "#DeltaR(gen, tracker)");
  if (draw_glb)   Compare("dR", "Glb",   "#DeltaR(gen, global)");
  if (draw_tight) Compare("dR", "Tight", "#DeltaR(gen, tight)");
  if (draw_soft)  Compare("dR", "Soft",  "#DeltaR(gen, soft)");

  Compare("MuPFIso",        "NA", "muon PF isolation");
  Compare("MuPFChargeIso",  "NA", "muon charged PF isolation");
  Compare("MuPFNeutralIso", "NA", "muon neutral PF isolation");
  Compare("MuPFPhotonIso",  "NA", "muon photon PF isolation");
  Compare("MuPFPUIso",      "NA", "muon PU PF isolation");
}


//------------------------------------------------------------------------------
//
// Make efficiency
//
//------------------------------------------------------------------------------
TGraphAsymmErrors* MakeEfficiency(TString effType,
				  TString muonType,
				  TString variable,
				  Int_t   PU,
				  Color_t color,
				  Int_t   rebin)
{
  TFile* file = (PU == noPU) ? file2 : file1;

  Style_t style = (PU == noPU) ? kOpenCircle : kFullCircle;

  TString num_name = "muonAnalysis/" + muonType + "Muons_";

  if (effType.Contains("fakes")) num_name = num_name + "noGen_";

  TString den_name = "muonAnalysis/GenMuons_";

  TH1F* hnum = (TH1F*)(file->Get(num_name + variable))->Clone("hnum");
  TH1F* hden = (TH1F*)(file->Get(den_name + variable))->Clone("hden");

  if (rebin > -1)
    {
      hnum->Rebin(rebin);
      hden->Rebin(rebin);
    }

  TGraphAsymmErrors* tgae = new TGraphAsymmErrors(hnum, hden);

  tgae->SetLineColor  (color);
  tgae->SetLineWidth  (    1);
  tgae->SetMarkerColor(color);
  tgae->SetMarkerStyle(style);


  //----------------------------------------------------------------------------
  //
  // Print
  //
  Bool_t pickMe = false;

  TString delphes_variable = (variable.EqualTo("vr")) ? "sqrt(v0*v0 + vz*vz)" : variable;

  if (muonType.EqualTo("Sta")   && draw_sta)   pickMe = true;
  if (muonType.EqualTo("Trk")   && draw_trk)   pickMe = true;
  if (muonType.EqualTo("Glb")   && draw_glb)   pickMe = true;
  if (muonType.EqualTo("Tight") && draw_tight) pickMe = true;
  if (muonType.EqualTo("Soft")  && draw_soft)  pickMe = true;

  if (doSaveTcl && pickMe && effType.EqualTo("efficiency"))
    {
      std::ofstream efficiency_tcl;

      TString pu_string = (PU == noPU) ? "noPU" : "PU200";
  
      efficiency_tcl.open(Form(directory + "/tcl/muon%sId_vs_%s_%s_%s.tcl",
			       muonType.Data(),
			       variable.Data(),
			       pu_string.Data(),
			       effType.Data()),
			  std::ofstream::out);

      efficiency_tcl << Form("# %s muons %s vs. %s with PU = %d\n", muonType.Data(), effType.Data(), delphes_variable.Data(), (PU == 0) ? 0 : 200); 

      efficiency_tcl << "set EfficiencyFormula {\n";

      Int_t npoints = tgae->GetN();
  
      for (Int_t i=0; i<npoints; i++)
	{	
	  efficiency_tcl << Form("    (%s > %.2f && %s < %.2f) * %.3f +\n",
				 delphes_variable.Data(),
				 tgae->GetX()[i] - tgae->GetEXlow()[i],
				 delphes_variable.Data(),
				 tgae->GetX()[i] + tgae->GetEXhigh()[i],
				 tgae->GetY()[i]);
	}

      efficiency_tcl << Form("    (%s > %.2f) * 0.000\n",
			     delphes_variable.Data(),
			     tgae->GetX()[npoints-1] + tgae->GetEXhigh()[npoints-1]);

      efficiency_tcl << "}\n";
  
      efficiency_tcl.close();
    }
  //
  // Print
  //
  //----------------------------------------------------------------------------


  return tgae;
}


//------------------------------------------------------------------------------
//
// Draw efficiency
//
//------------------------------------------------------------------------------
void DrawEfficiency(TString effType,
		    TString variable,
		    TString xtitle,
		    Int_t   rebin)
{
  TGraphAsymmErrors* sta_efficiency   = MakeEfficiency(effType, "Sta",   variable, PU200, kBlack,    rebin);
  TGraphAsymmErrors* trk_efficiency   = MakeEfficiency(effType, "Trk",   variable, PU200, kRed+1,    rebin);
  TGraphAsymmErrors* glb_efficiency   = MakeEfficiency(effType, "Glb",   variable, PU200, kBlue,     rebin);
  TGraphAsymmErrors* tight_efficiency = MakeEfficiency(effType, "Tight", variable, PU200, kGreen+2,  rebin);
  TGraphAsymmErrors* soft_efficiency  = MakeEfficiency(effType, "Soft",  variable, PU200, kOrange+7, rebin);

  TGraphAsymmErrors* sta_efficiency_noPU   = MakeEfficiency(effType, "Sta",   variable, noPU, kBlack,    rebin);
  TGraphAsymmErrors* trk_efficiency_noPU   = MakeEfficiency(effType, "Trk",   variable, noPU, kRed+1,    rebin);
  TGraphAsymmErrors* glb_efficiency_noPU   = MakeEfficiency(effType, "Glb",   variable, noPU, kBlue,     rebin);
  TGraphAsymmErrors* tight_efficiency_noPU = MakeEfficiency(effType, "Tight", variable, noPU, kGreen+2,  rebin);
  TGraphAsymmErrors* soft_efficiency_noPU  = MakeEfficiency(effType, "Soft",  variable, noPU, kOrange+7, rebin);

  TCanvas* canvas = new TCanvas(effType + " vs. " + xtitle,
				effType + " vs. " + xtitle);

  canvas->SetLeftMargin (0.14);
  canvas->SetRightMargin(0.23);

  TMultiGraph* mg = new TMultiGraph();

  if (draw_sta)   mg->Add(sta_efficiency);
  if (draw_sta)   mg->Add(sta_efficiency_noPU);
  if (draw_trk)   mg->Add(trk_efficiency);
  if (draw_trk)   mg->Add(trk_efficiency_noPU);
  if (draw_glb)   mg->Add(glb_efficiency);
  if (draw_glb)   mg->Add(glb_efficiency_noPU);
  if (draw_tight) mg->Add(tight_efficiency);
  if (draw_tight) mg->Add(tight_efficiency_noPU);
  if (draw_soft)  mg->Add(soft_efficiency);
  if (draw_soft)  mg->Add(soft_efficiency_noPU);

  mg->Draw("apz");

  mg->SetMinimum(-0.05);
  mg->SetMaximum( 1.05);

  // Cosmetics
  canvas->SetGridx();
  canvas->SetGridy();

  // Labels
  mg->SetTitle("");
  mg->GetXaxis()->SetTitle(xtitle);
  mg->GetYaxis()->SetTitle(effType);
  mg->GetXaxis()->SetTitleOffset(1.5);
  mg->GetYaxis()->SetTitleOffset(1.6);

  // Legend
  TLegend* legend = new TLegend(0.79, 0.5, 0.95, 0.91);

  SetLegend(legend, 0.025);

  if (draw_sta)   legend->AddEntry(sta_efficiency_noPU,   "(no PU) sta",    "lp");
  if (draw_trk)   legend->AddEntry(trk_efficiency_noPU,   "(no PU) trk",    "lp");
  if (draw_glb)   legend->AddEntry(glb_efficiency_noPU,   "(no PU) glb",    "lp");
  if (draw_tight) legend->AddEntry(tight_efficiency_noPU, "(no PU) tight",  "lp"); 
  if (draw_soft)  legend->AddEntry(soft_efficiency_noPU,  "(no PU) soft",   "lp"); 
  if (draw_sta)   legend->AddEntry(sta_efficiency,        "(200 PU) sta",   "lp");
  if (draw_trk)   legend->AddEntry(trk_efficiency,        "(200 PU) trk",   "lp");
  if (draw_glb)   legend->AddEntry(glb_efficiency,        "(200 PU) glb",   "lp");
  if (draw_tight) legend->AddEntry(tight_efficiency,      "(200 PU) tight", "lp");
  if (draw_soft)  legend->AddEntry(soft_efficiency,       "(200 PU) soft",  "lp");

  legend->Draw();

  canvas->Modified();
  canvas->Update();

  if (doSavePdf) canvas->SaveAs(directory + "/" + effType + "_" + variable + ".pdf");
  if (doSavePng) canvas->SaveAs(directory + "/" + effType + "_" + variable + ".png");
}


//------------------------------------------------------------------------------
//
// Compare
//
//------------------------------------------------------------------------------
void Compare(TString variable,
	     TString muonType,
	     TString xtitle,
	     Float_t xmax)
{
  TH1F* h_noPU  = NULL;
  TH1F* h_PU200 = NULL;

  if (variable.Contains("MuPF"))
    {
      h_PU200 = (TH1F*)(file1->Get("muonAnalysis/" + variable))->Clone("h_PU200_" + variable);
      h_noPU  = (TH1F*)(file2->Get("muonAnalysis/" + variable))->Clone("h_noPU_"  + variable);
    }
  else
    {
      h_PU200 = (TH1F*)(file1->Get("muonAnalysis/" + muonType + "Muons_" + variable))->Clone("h_" + muonType + "_PU200_" + variable);
      h_noPU  = (TH1F*)(file2->Get("muonAnalysis/" + muonType + "Muons_" + variable))->Clone("h_" + muonType + "_noPU_"  + variable);
    }

  h_noPU ->Scale(1. / h_noPU ->Integral(-1, -1));
  h_PU200->Scale(1. / h_PU200->Integral(-1, -1));

  h_noPU ->SetLineColor(kBlack);
  h_PU200->SetLineColor(kRed+1);

  h_noPU ->SetLineWidth(2);
  h_PU200->SetLineWidth(2);


  // Draw
  //----------------------------------------------------------------------------
  TString label = (muonType.EqualTo("NA")) ? variable : muonType + "_" + variable;

  TCanvas* canvas = new TCanvas("compare " + xtitle,
				"compare " + xtitle);

  if (variable.Contains("dR"))   canvas->SetLogy();
  if (variable.Contains("MuPF")) canvas->SetLogy();

  TH1F* h_noPU_overflow  = AddOverflow(h_noPU);
  TH1F* h_PU200_overflow = AddOverflow(h_PU200);
  
  if (h_noPU_overflow->GetMaximum() > h_PU200_overflow->GetMaximum())
    {
      h_noPU_overflow ->Draw("hist");
      h_PU200_overflow->Draw("hist,same");
      h_noPU_overflow ->GetXaxis()->SetTitle(xtitle);
      
      if (xmax > -999) h_noPU_overflow->GetXaxis()->SetRangeUser(-1, xmax);
    }
  else
    {
      h_PU200_overflow->Draw("hist");
      h_noPU_overflow ->Draw("hist,same");
      h_PU200_overflow->GetXaxis()->SetTitle(xtitle);
      
      if (xmax > -999) h_PU200_overflow->GetXaxis()->SetRangeUser(-1, xmax);
    }

  // Legend
  TLegend* legend = new TLegend(0.77, 0.78, 0.93, 0.89);

  SetLegend(legend, 0.03);

  legend->AddEntry(h_PU200_overflow, " 200 PU", "l");
  legend->AddEntry(h_noPU_overflow,  " no PU",  "l");

  legend->Draw();

  // Save
  canvas->GetFrame()->DrawClone();

  if (doSavePdf) canvas->SaveAs(directory + "/compare_" + label + ".pdf");
  if (doSavePng) canvas->SaveAs(directory + "/compare_" + label + ".png");
}


//------------------------------------------------------------------------------
//
// Add overflow
//
//------------------------------------------------------------------------------
TH1F* AddOverflow(TH1F* h)
{
  TString  name = h->GetName();
  Int_t    nx   = h->GetNbinsX()+1;
  Double_t bw   = h->GetBinWidth(nx);
  Double_t x1   = h->GetBinLowEdge(1);
  Double_t x2   = h->GetBinLowEdge(nx) + bw;
  
  // Book a new histogram having an extra bin for overflows
  TH1F* htmp = new TH1F(name + "_overflow", "", nx, x1, x2);

  // Fill the new histogram including the extra bin for overflows
  for (Int_t i=1; i<=nx; i++) {
    htmp->Fill(htmp->GetBinCenter(i), h->GetBinContent(i));
  }

  // Fill the underflow
  htmp->Fill(x1-1, h->GetBinContent(0));

  // Restore the number of entries
  htmp->SetEntries(h->GetEntries());

  // Cosmetics
  htmp->SetLineColor(h->GetLineColor());
  htmp->SetLineWidth(h->GetLineWidth());
  htmp->GetXaxis()->SetTitleOffset(1.5);

  return htmp;
}


//------------------------------------------------------------------------------
// SetLegend
//------------------------------------------------------------------------------
void SetLegend(TLegend* tl, Size_t tsize)
{
  tl->SetBorderSize(    0);
  tl->SetFillColor (    0);
  tl->SetTextAlign (   12);
  tl->SetTextFont  (   42);
  tl->SetTextSize  (tsize);
}
