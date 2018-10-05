//------------------------------------------------------------------------------
//
// Compare histograms for two samples
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
Bool_t  doSavePdf = true;
Bool_t  doSavePng = true;

TFile*  file1 = NULL;
TFile*  file2 = NULL;

Bool_t  draw_sta   = false;
Bool_t  draw_trk   = false;
Bool_t  draw_glb   = true;
Bool_t  draw_tight = true;
Bool_t  draw_soft  = false;

TString directory = "displaced-muons";

TString file1name;
TString file2name;


// Member functions
//------------------------------------------------------------------------------
TGraphAsymmErrors* MakeEfficiency(TString  effType,
				  TString  muonType,
				  TString  variable,
				  TFile*   file,
				  Color_t  color,
				  Style_t  style);

void               DrawEfficiency(TString  effType,
				  TString  variable,
				  TString  xtitle);

void               Compare       (TString  variable,
				  TString  muonType,
				  TString  xtitle,
				  Float_t  xmax = -999);

TH1F*              AddOverflow   (TH1F*    hist);

void               SetLegend     (TLegend* tl,
				  Size_t   tsize);


//------------------------------------------------------------------------------
//
// doEfficiencies
//
//------------------------------------------------------------------------------
void doEfficiencies(TString name1 = "CTau-1_PU200",
		    TString name2 = "CTau-100_PU200")
{
  file1name = name1;
  file2name = name2;

  gInterpreter->ExecuteMacro("PaperStyle.C");

  if (doSavePdf) gSystem->mkdir(directory, kTRUE);
  if (doSavePng) gSystem->mkdir(directory, kTRUE);

  TH1::SetDefaultSumw2();


  // Input files
  //----------------------------------------------------------------------------
  file1 = TFile::Open("rootfiles/DisplacedSUSY_" + file1name + ".root");
  file2 = TFile::Open("rootfiles/DisplacedSUSY_" + file2name + ".root");


  // Do the work
  //----------------------------------------------------------------------------
  DrawEfficiency("efficiency", "vxy", "gen production distance in xy [cm]");
  DrawEfficiency("efficiency", "vz",  "gen production distance in z [cm]");
  DrawEfficiency("efficiency", "vr",  "gen production distance in xyz [cm]");
  DrawEfficiency("efficiency", "eta", "gen #eta");
  DrawEfficiency("efficiency", "pt",  "gen p_{T} [GeV]");

  //  DrawEfficiency("fakes", "vxy", "gen production distance in xy [cm]");
  //  DrawEfficiency("fakes", "vz",  "gen production distance in z [cm]");
  //  DrawEfficiency("fakes", "vr",  "gen production distance in xyz [cm]");
  //  DrawEfficiency("fakes", "eta", "gen #eta");
  //  DrawEfficiency("fakes", "pt",  "gen p_{T} [GeV]");

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
				  TFile*  file,
				  Color_t color,
				  Style_t style)
{
  TString num_name = "muonAnalysis/" + muonType + "Muons_";

  if (effType.Contains("fakes")) num_name = num_name + "noGen_";

  TString den_name = "muonAnalysis/GenMuons_";

  TH1F* hnum = (TH1F*)(file->Get(num_name + variable))->Clone("hnum");
  TH1F* hden = (TH1F*)(file->Get(den_name + variable))->Clone("hden");

  TGraphAsymmErrors* tgae = new TGraphAsymmErrors(hnum, hden);

  tgae->SetLineColor  (color);
  tgae->SetLineWidth  (    1);
  tgae->SetMarkerColor(color);
  tgae->SetMarkerStyle(style);

  return tgae;
}


//------------------------------------------------------------------------------
//
// Draw efficiency
//
//------------------------------------------------------------------------------
void DrawEfficiency(TString effType,
		    TString variable,
		    TString xtitle)
{
  TGraphAsymmErrors* sta_efficiency_1   = MakeEfficiency(effType, "Sta",   variable, file1, kBlack,    kFullCircle);
  TGraphAsymmErrors* trk_efficiency_1   = MakeEfficiency(effType, "Trk",   variable, file1, kRed+1,    kFullCircle);
  TGraphAsymmErrors* glb_efficiency_1   = MakeEfficiency(effType, "Glb",   variable, file1, kBlue,     kFullCircle);
  TGraphAsymmErrors* tight_efficiency_1 = MakeEfficiency(effType, "Tight", variable, file1, kGreen+2,  kFullCircle);
  TGraphAsymmErrors* soft_efficiency_1  = MakeEfficiency(effType, "Soft",  variable, file1, kOrange+7, kFullCircle);

  TGraphAsymmErrors* sta_efficiency_2   = MakeEfficiency(effType, "Sta",   variable, file2, kBlack,    kOpenCircle);
  TGraphAsymmErrors* trk_efficiency_2   = MakeEfficiency(effType, "Trk",   variable, file2, kRed+1,    kOpenCircle);
  TGraphAsymmErrors* glb_efficiency_2   = MakeEfficiency(effType, "Glb",   variable, file2, kBlue,     kOpenCircle);
  TGraphAsymmErrors* tight_efficiency_2 = MakeEfficiency(effType, "Tight", variable, file2, kGreen+2,  kOpenCircle);
  TGraphAsymmErrors* soft_efficiency_2  = MakeEfficiency(effType, "Soft",  variable, file2, kOrange+7, kOpenCircle);

  TCanvas* canvas = new TCanvas(effType + " vs. " + xtitle,
				effType + " vs. " + xtitle, 650, 600);

  canvas->SetLeftMargin (0.14);
  canvas->SetRightMargin(0.28);

  TMultiGraph* mg = new TMultiGraph();

  if (draw_sta) mg->Add(sta_efficiency_1);
  if (draw_sta) mg->Add(sta_efficiency_2);

  if (draw_trk) mg->Add(trk_efficiency_1);
  if (draw_trk) mg->Add(trk_efficiency_2);

  if (draw_glb) mg->Add(glb_efficiency_1);
  if (draw_glb) mg->Add(glb_efficiency_2);

  if (draw_tight) mg->Add(tight_efficiency_1);
  if (draw_tight) mg->Add(tight_efficiency_2);

  if (draw_soft) mg->Add(soft_efficiency_1);
  if (draw_soft) mg->Add(soft_efficiency_2);

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
  TLegend* legend = new TLegend(0.74, 0.5, 0.90, 0.91);

  SetLegend(legend, 0.025);

  if (draw_sta)   legend->AddEntry(sta_efficiency_1,   file1name + " sta",   "lp");
  if (draw_trk)   legend->AddEntry(trk_efficiency_1,   file1name + " trk",   "lp");
  if (draw_glb)   legend->AddEntry(glb_efficiency_1,   file1name + " glb",   "lp");
  if (draw_tight) legend->AddEntry(tight_efficiency_1, file1name + " tight", "lp");
  if (draw_soft)  legend->AddEntry(soft_efficiency_1,  file1name + " soft",  "lp");

  if (draw_sta)   legend->AddEntry(sta_efficiency_2,   file2name + " sta",   "lp");
  if (draw_trk)   legend->AddEntry(trk_efficiency_2,   file2name + " trk",   "lp");
  if (draw_glb)   legend->AddEntry(glb_efficiency_2,   file2name + " glb",   "lp");
  if (draw_tight) legend->AddEntry(tight_efficiency_2, file2name + " tight", "lp"); 
  if (draw_soft)  legend->AddEntry(soft_efficiency_2,  file2name + " soft",  "lp"); 

  legend->Draw();

  canvas->Modified();
  canvas->Update();

  if (doSavePdf) canvas->SaveAs(directory + "/" + file1name + "__vs__" + file2name + "__" + variable + "-" + effType + ".pdf");
  if (doSavePng) canvas->SaveAs(directory + "/" + file1name + "__vs__" + file2name + "__" + variable + "-" + effType + ".png");
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
  TH1F* hist1 = NULL;
  TH1F* hist2 = NULL;

  if (variable.Contains("MuPF"))
    {
      hist1 = (TH1F*)(file1->Get("muonAnalysis/" + variable))->Clone("hist1_" + variable);
      hist2 = (TH1F*)(file2->Get("muonAnalysis/" + variable))->Clone("hist2_" + variable);
    }
  else
    {
      hist1 = (TH1F*)(file1->Get("muonAnalysis/" + muonType + "Muons_" + variable))->Clone("hist1_" + muonType + "_" + variable);
      hist2 = (TH1F*)(file2->Get("muonAnalysis/" + muonType + "Muons_" + variable))->Clone("hist2_" + muonType + "_" + variable);
    }

  hist1->Scale(1. / hist1->Integral(-1, -1));
  hist2->Scale(1. / hist2->Integral(-1, -1));

  hist1->SetLineColor(kBlack);
  hist2->SetLineColor(kRed+1);

  hist1->SetLineWidth(2);
  hist2->SetLineWidth(2);


  // Draw
  //----------------------------------------------------------------------------
  TString label = (muonType.EqualTo("NA")) ? variable : muonType + "_" + variable;

  TCanvas* canvas = new TCanvas("compare " + xtitle,
				"compare " + xtitle);

  if (variable.Contains("dR"))   canvas->SetLogy();
  if (variable.Contains("MuPF")) canvas->SetLogy();

  TH1F* hist1_overflow = AddOverflow(hist1);
  TH1F* hist2_overflow = AddOverflow(hist2);
  
  if (hist1_overflow->GetMaximum() > hist2_overflow->GetMaximum())
    {
      hist1_overflow->Draw("hist");
      hist2_overflow->Draw("hist,same");
      hist1_overflow->GetXaxis()->SetTitle(xtitle);
      
      if (xmax > -999) hist1_overflow->GetXaxis()->SetRangeUser(-1, xmax);
    }
  else
    {
      hist2_overflow->Draw("hist");
      hist1_overflow->Draw("hist,same");
      hist2_overflow->GetXaxis()->SetTitle(xtitle);
      
      if (xmax > -999) hist2_overflow->GetXaxis()->SetRangeUser(-1, xmax);
    }

  // Legend
  TLegend* legend = new TLegend(0.64, 0.78, 0.80, 0.89);

  SetLegend(legend, 0.03);

  legend->AddEntry(hist1_overflow, " " + file1name, "l");
  legend->AddEntry(hist2_overflow, " " + file2name, "l");

  legend->Draw();

  // Save
  canvas->GetFrame()->DrawClone();

  if (doSavePdf) canvas->SaveAs(directory + "/" + file1name + "__vs__" + file2name + "__compare-" + label + ".pdf");
  if (doSavePng) canvas->SaveAs(directory + "/" + file1name + "__vs__" + file2name + "__compare-" + label + ".png");
}


//------------------------------------------------------------------------------
//
// Add overflow
//
//------------------------------------------------------------------------------
TH1F* AddOverflow(TH1F* hist)
{
  TString  name = hist->GetName();
  Int_t    nx   = hist->GetNbinsX()+1;
  Double_t bw   = hist->GetBinWidth(nx);
  Double_t x1   = hist->GetBinLowEdge(1);
  Double_t x2   = hist->GetBinLowEdge(nx) + bw;
  
  // Book a new histogram having an extra bin for overflows
  TH1F* htmp = new TH1F(name + "_overflow", "", nx, x1, x2);

  // Fill the new histogram including the extra bin for overflows
  for (Int_t i=1; i<=nx; i++) {
    htmp->Fill(htmp->GetBinCenter(i), hist->GetBinContent(i));
  }

  // Fill the underflow
  htmp->Fill(x1-1, hist->GetBinContent(0));

  // Restore the number of entries
  htmp->SetEntries(hist->GetEntries());

  // Cosmetics
  htmp->SetLineColor(hist->GetLineColor());
  htmp->SetLineWidth(hist->GetLineWidth());
  htmp->GetXaxis()->SetTitleOffset(1.5);

  return htmp;
}


//------------------------------------------------------------------------------
// Set legend
//------------------------------------------------------------------------------
void SetLegend(TLegend* tl,
	       Size_t   tsize)
{
  tl->SetBorderSize(    0);
  tl->SetFillColor (    0);
  tl->SetTextAlign (   12);
  tl->SetTextFont  (   42);
  tl->SetTextSize  (tsize);
}
