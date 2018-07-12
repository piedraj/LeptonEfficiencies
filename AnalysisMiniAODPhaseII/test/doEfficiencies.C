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

const Int_t nbinspt = 6;

Color_t     ptcolor[nbinspt] = {kRed-10, kRed-9, kRed-7, kRed-4, kRed, kRed+1};

Bool_t      doSavePdf = false;
Bool_t      doSavePng = true;
Bool_t      doSaveTcl = true;

TFile*      file_PU200 = NULL;
TFile*      file_noPU  = NULL;

Bool_t      draw_sta   = false;
Bool_t      draw_trk   = false;
Bool_t      draw_glb   = false;
Bool_t      draw_tight = true;
Bool_t      draw_soft  = true;


// Member functions
//------------------------------------------------------------------------------
TGraphAsymmErrors* MakeEfficiency(TString effType,
				  TString muonType,
				  TString variable,
				  Int_t   PU,
				  Color_t color,
				  Int_t   rebin);

void               DrawEfficiency(TString effType,
				  TString variable,
				  TString xtitle,
				  Int_t   rebin = -1);

void               DrawResolution(TString muonType,
				  TString xtitle);

void               Compare       (TString variable,
				  TString muonType,
				  TString xtitle,
				  Float_t xmax     = -999);

TH1F*              AddOverflow   (TH1F*   h);


//------------------------------------------------------------------------------
//
// doEfficiencies
//
//------------------------------------------------------------------------------
void doEfficiencies()
{
  gInterpreter->ExecuteMacro("PaperStyle.C");

  if (doSavePdf) gSystem->mkdir("pdf", kTRUE);
  if (doSavePng) gSystem->mkdir("png", kTRUE);
  if (doSaveTcl) gSystem->mkdir("tcl", kTRUE);

  TH1::SetDefaultSumw2();


  // Input files
  //----------------------------------------------------------------------------
  file_PU200 = TFile::Open("rootfiles/DisplacedSUSY_PU200.root");
  file_noPU  = TFile::Open("rootfiles/DisplacedSUSY_noPU.root");


  // Do the work
  //----------------------------------------------------------------------------
  DrawEfficiency("efficiency", "vr",  "gen production radius [cm]", 2);
  DrawEfficiency("efficiency", "eta", "gen #eta");
  DrawEfficiency("efficiency", "pt",  "gen p_{T} [GeV]", 10);

  DrawEfficiency("fakes", "vr",  "gen production radius [cm]", 2);
  DrawEfficiency("fakes", "eta", "gen #eta");
  DrawEfficiency("fakes", "pt",  "gen p_{T} [GeV]", 10);

  if (draw_sta)   DrawResolution("Sta",   "standalone muons");
  if (draw_trk)   DrawResolution("Trk",   "tracker muons");
  if (draw_glb)   DrawResolution("Glb",   "global muons");
  if (draw_tight) DrawResolution("Tight", "tight muons");
  if (draw_soft)  DrawResolution("Soft",  "soft muons");
  
  if (draw_sta)   Compare("dR", "Sta",   "#DeltaR(gen, standalone)");
  if (draw_trk)   Compare("dR", "Trk",   "#DeltaR(gen, tracker)");
  if (draw_glb)   Compare("dR", "Glb",   "#DeltaR(gen, global)");
  if (draw_tight) Compare("dR", "Tight", "#DeltaR(gen, tight)");
  if (draw_soft)  Compare("dR", "Soft",  "#DeltaR(gen, soft)");
  
  if (draw_sta)   Compare("pt", "Sta",   "standalone-matched gen p_{T} [GeV]");
  if (draw_trk)   Compare("pt", "Trk",   "tracker-matched gen p_{T} [GeV]");
  if (draw_glb)   Compare("pt", "Glb",   "global-matched gen p_{T} [GeV]");
  if (draw_tight) Compare("pt", "Tight", "tight-matched gen p_{T} [GeV]");
  if (draw_soft)  Compare("pt", "Soft",  "soft-matched gen p_{T} [GeV]");
  
  Compare("vr", "Gen", "gen production radius [cm]", 25);

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
  TFile* file = (PU == noPU) ? file_noPU : file_PU200;

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
  if (doSaveTcl)
    {
      std::ofstream efficiency_tcl;

      TString pu_string = (PU == noPU) ? "noPU" : "PU200";
  
      efficiency_tcl.open(Form("tcl/muon%sId_vs_%s_%s_%s.tcl",
			       muonType.Data(),
			       variable.Data(),
			       pu_string.Data(),
			       effType.Data()),
			  std::ofstream::out);

      efficiency_tcl << Form("# %s muons %s vs. %s with PU = %d\n", muonType.Data(), effType.Data(), variable.Data(), (PU == 0) ? 0 : 200); 

      efficiency_tcl << "set EfficiencyFormula {\n";
  
      for (int i=0; i<tgae->GetN(); i++)
	{
	  efficiency_tcl << Form("    (%s > %.0f && %s < %.0f) * %.3f",
				 variable.Data(),
				 tgae->GetX()[i] - tgae->GetEXlow()[i],
				 variable.Data(),
				 tgae->GetX()[i] + tgae->GetEXhigh()[i],
				 tgae->GetY()[i]);

	  if (i < tgae->GetN() - 1) efficiency_tcl << " +";

	  efficiency_tcl << "\n";
	}

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
				effType + " vs. " + xtitle,
				600,
				600);

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

  legend->SetBorderSize(    0);
  legend->SetFillColor (    0);
  legend->SetTextAlign (   12);
  legend->SetTextFont  (   42);
  legend->SetTextSize  (0.025);

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

  if (doSavePdf) canvas->SaveAs("pdf/" + effType + "_" + variable + ".pdf");
  if (doSavePng) canvas->SaveAs("png/" + effType + "_" + variable + ".png");
}


//------------------------------------------------------------------------------
//
// Draw resolution
//
//------------------------------------------------------------------------------
void DrawResolution(TString muonType,
		    TString xtitle)
{
  TCanvas* c2 = new TCanvas(xtitle + " resolution", xtitle + " resolution");

  TH1F* h_resolution[nbinspt];

  Float_t ymax = 0;

  for (Int_t i=0; i<nbinspt; i++) {

    h_resolution[i] = (TH1F*)file_PU200->Get(Form("muonAnalysis/%sMuons_res_%d", muonType.Data(), i));

    if (h_resolution[i]->GetMaximum() > ymax) ymax = h_resolution[i]->GetMaximum();

    h_resolution[i]->SetLineColor(ptcolor[i]);
    h_resolution[i]->SetLineWidth(2);

    TString option = (i == 0) ? "" : "same";

    h_resolution[i]->Draw(option);
  }

  h_resolution[0]->SetTitle("");
  h_resolution[0]->SetXTitle(xtitle + " #Deltaq/p_{T} / (q/p_{T})");
  h_resolution[0]->SetYTitle("entries / bin");
  h_resolution[0]->SetMaximum(1.1 * ymax);
  h_resolution[0]->GetXaxis()->SetTitleOffset(1.5);
  h_resolution[0]->GetYaxis()->SetTitleOffset(2.0);

  // Legend
  TLegend* legend = new TLegend(0.61, 0.6, 0.82, 0.89);

  legend->SetBorderSize(   0);
  legend->SetFillColor (   0);
  legend->SetTextAlign (  12);
  legend->SetTextFont  (  42);
  legend->SetTextSize  (0.03);

  legend->AddEntry(h_resolution[0], " 10 < p_{T} < 20 GeV",   "l");
  legend->AddEntry(h_resolution[1], " 20 < p_{T} < 35 GeV",   "l");
  legend->AddEntry(h_resolution[2], " 35 < p_{T} < 50 GeV",   "l");
  legend->AddEntry(h_resolution[3], " 50 < p_{T} < 100 GeV",  "l");
  legend->AddEntry(h_resolution[4], " 100 < p_{T} < 200 GeV", "l");
  legend->AddEntry(h_resolution[5], " 200 < p_{T} < 500 GeV", "l");

  legend->Draw();

  c2->GetFrame()->DrawClone();

  if (doSavePdf) c2->SaveAs("pdf/resolution_" + muonType + ".pdf");
  if (doSavePng) c2->SaveAs("png/resolution_" + muonType + ".png");
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
      h_noPU  = (TH1F*)(file_noPU ->Get("muonAnalysis/" + variable))->Clone("h_noPU_"  + variable);
      h_PU200 = (TH1F*)(file_PU200->Get("muonAnalysis/" + variable))->Clone("h_PU200_" + variable);
    }
  else
    {
      h_noPU  = (TH1F*)(file_noPU ->Get("muonAnalysis/" + muonType + "Muons_" + variable))->Clone("h_" + muonType + "_noPU_"  + variable);
      h_PU200 = (TH1F*)(file_PU200->Get("muonAnalysis/" + muonType + "Muons_" + variable))->Clone("h_" + muonType + "_PU200_" + variable);
    }

  if (!muonType.Contains("Gen"))
    {
      h_noPU ->Rebin(2);
      h_PU200->Rebin(2);
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

  TCanvas* canvas = new TCanvas("compare " + xtitle, "compare " + xtitle);

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

  legend->SetBorderSize(   0);
  legend->SetFillColor (   0);
  legend->SetTextAlign (  12);
  legend->SetTextFont  (  42);
  legend->SetTextSize  (0.03);

  legend->AddEntry(h_PU200_overflow, " 200 PU", "l");
  legend->AddEntry(h_noPU_overflow,  " no PU",  "l");

  legend->Draw();

  // Save
  canvas->GetFrame()->DrawClone();

  if (doSavePdf) canvas->SaveAs("pdf/compare_" + label + ".pdf");
  if (doSavePng) canvas->SaveAs("png/compare_" + label + ".png");
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
