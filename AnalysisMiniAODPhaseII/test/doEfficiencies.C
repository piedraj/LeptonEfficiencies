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

Bool_t      doSetRanges = false;
Bool_t      doSavePdf   = false;
Bool_t      doSavePng   = true;

TFile*      file_PU200 = NULL;
TFile*      file_noPU  = NULL;

Bool_t      draw_sta   = false;
Bool_t      draw_trk   = false;
Bool_t      draw_glb   = false;
Bool_t      draw_tight = true;
Bool_t      draw_soft  = true;


// Member functions
//------------------------------------------------------------------------------
TGraphAsymmErrors* MakeEfficiency(TString type,
				  TString variable,
				  Int_t   PU,
				  Color_t color,
				  Int_t   rebin);

TGraphAsymmErrors* MakeFakes     (TString type,
				  TString variable,
				  Int_t   PU,
				  Color_t color,
				  Int_t   rebin);

void               DrawEfficiency(TString variable,
				  Int_t   rebin = -1);

void               DrawFakes     (TString variable,
				  Int_t   rebin = -1);

void               DrawResolution(TString muonType);

void               DrawTH2       (TString variable,
				  TString muonType);

void               Compare       (TString variable,
				  TString muonType = "ANY",
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

  TH1::SetDefaultSumw2();


  // Input files
  //----------------------------------------------------------------------------
  file_PU200 = TFile::Open("rootfiles/DisplacedSUSY_PU200.root");
  file_noPU  = TFile::Open("rootfiles/DisplacedSUSY_noPU.root");


  // Do the work
  //----------------------------------------------------------------------------
  DrawEfficiency("vr", 2);

  return;

  DrawEfficiency("eta");
  DrawEfficiency("pt", 10);

  //  DrawFakes("vr");
  //  DrawFakes("eta");
  //  DrawFakes("pt", 10);

  if (draw_sta)   DrawResolution("Sta");
  if (draw_trk)   DrawResolution("Trk");
  if (draw_glb)   DrawResolution("Glb");
  if (draw_tight) DrawResolution("Tight");
  if (draw_soft)  DrawResolution("Soft");
  
  if (draw_sta)   Compare("dR", "Sta");
  if (draw_trk)   Compare("dR", "Trk");
  if (draw_glb)   Compare("dR", "Glb");
  if (draw_tight) Compare("dR", "Tight");
  if (draw_soft)  Compare("dR", "Soft");
  
  if (draw_sta)   Compare("pt", "Sta");
  if (draw_trk)   Compare("pt", "Trk");
  if (draw_glb)   Compare("pt", "Glb");
  if (draw_tight) Compare("pt", "Tight");
  if (draw_soft)  Compare("pt", "Soft");
  
  Compare("vr", "Gen", 25);
  
  Compare("MuPFIso");
  Compare("MuPFChargeIso");
  Compare("MuPFNeutralIso");
  Compare("MuPFPhotonIso");
  Compare("MuPFPUIso");
}


//------------------------------------------------------------------------------
//
// Make efficiency
//
//------------------------------------------------------------------------------
TGraphAsymmErrors* MakeEfficiency(TString type,
				  TString variable,
				  Int_t   PU,
				  Color_t color,
				  Int_t   rebin)
{
  TFile* file = (PU == noPU) ? file_noPU : file_PU200;

  Style_t style = (PU == noPU) ? kOpenCircle : kFullCircle;

  TString num_name = "muonAnalysis/" + type + "Muons_" + variable;
  TString den_name = "muonAnalysis/GenMuons_" + variable;

  TH1F* hnum = (TH1F*)(file->Get(num_name))->Clone("hnum");
  TH1F* hden = (TH1F*)(file->Get(den_name))->Clone("hden");

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


  // Print
  //----------------------------------------------------------------------------
  std::ofstream efficiency_tcl;

  TString pu_string = (PU == noPU) ? "noPU" : "PU200";
  
  efficiency_tcl.open(Form("muon%sId_vs_%s_%s.tcl",
			   type.Data(),
			   variable.Data(),
			   pu_string.Data()),
		      std::ofstream::out);

  efficiency_tcl << Form("# %s muons efficiency vs. %s with PU = %d\n", type.Data(), variable.Data(), (PU == 0) ? 0 : 200); 

  efficiency_tcl << "set EfficiencyFormula {\n";

  for (int i=0; i<tgae->GetN(); i++)
    {
      efficiency_tcl << Form("    (vr > %.0f && vr < %.0f) * %.3f +\n",
			     tgae->GetX()[i] - tgae->GetEXlow()[i],
			     tgae->GetX()[i] + tgae->GetEXhigh()[i],
			     tgae->GetY()[i]);
    }

  efficiency_tcl << "}\n";
  
  efficiency_tcl.close();


  return tgae;
}


//------------------------------------------------------------------------------
//
// Make fakes
//
//------------------------------------------------------------------------------
TGraphAsymmErrors* MakeFakes(TString type,
			     TString variable,
			     Int_t   PU,
			     Color_t color,
			     Int_t   rebin)
{
  TFile* file = (PU == noPU) ? file_noPU : file_PU200;

  Style_t style = (PU == noPU) ? kOpenCircle : kFullCircle;

  TString num_name = "muonAnalysis/" + type + "Muons_noGen_" + variable;
  TString den_name = "muonAnalysis/GenMuons_" + variable;

  TH1F* hnum = (TH1F*)(file->Get(num_name))->Clone("hnum");
  TH1F* hden = (TH1F*)(file->Get(den_name))->Clone("hden");

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

  return tgae;
}


//------------------------------------------------------------------------------
//
// Draw efficiency
//
//------------------------------------------------------------------------------
void DrawEfficiency(TString variable,
		    Int_t   rebin)
{
  TGraphAsymmErrors* sta_efficiency   = MakeEfficiency("Sta",   variable, PU200, kBlack,    rebin);
  TGraphAsymmErrors* trk_efficiency   = MakeEfficiency("Trk",   variable, PU200, kRed+1,    rebin);
  TGraphAsymmErrors* glb_efficiency   = MakeEfficiency("Glb",   variable, PU200, kBlue,     rebin);
  TGraphAsymmErrors* tight_efficiency = MakeEfficiency("Tight", variable, PU200, kGreen+2,  rebin);
  TGraphAsymmErrors* soft_efficiency  = MakeEfficiency("Soft",  variable, PU200, kOrange+7, rebin);

  TGraphAsymmErrors* sta_efficiency_noPU   = MakeEfficiency("Sta",   variable, noPU, kBlack,    rebin);
  TGraphAsymmErrors* trk_efficiency_noPU   = MakeEfficiency("Trk",   variable, noPU, kRed+1,    rebin);
  TGraphAsymmErrors* glb_efficiency_noPU   = MakeEfficiency("Glb",   variable, noPU, kBlue,     rebin);
  TGraphAsymmErrors* tight_efficiency_noPU = MakeEfficiency("Tight", variable, noPU, kGreen+2,  rebin);
  TGraphAsymmErrors* soft_efficiency_noPU  = MakeEfficiency("Soft",  variable, noPU, kOrange+7, rebin);

  TCanvas* canvas = new TCanvas("efficiency " + variable, "efficiency " + variable, 600, 600);

  canvas->SetLeftMargin (0.13);
  canvas->SetRightMargin(0.24);

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

  // Ranges
  if (doSetRanges)
    {
      mg->SetMinimum(-0.05);
      mg->SetMaximum( 1.05);
    }

  // Cosmetics
  canvas->SetGridx();
  canvas->SetGridy();

  // Labels
  mg->SetTitle("");

  if (variable == "vr")  mg->GetXaxis()->SetTitle("production radius [cm]");
  if (variable == "pt")  mg->GetXaxis()->SetTitle("p_{T} [GeV]");
  if (variable == "eta") mg->GetXaxis()->SetTitle("#eta");
 
  mg->GetYaxis()->SetTitle("reconstruction efficiency");
  mg->GetXaxis()->SetTitleOffset(1.5);
  mg->GetYaxis()->SetTitleOffset(1.5);

  // Legend
  TLegend* legend = new TLegend(0.775, 0.5, 0.95, 0.91);

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

  if (doSavePdf) canvas->SaveAs("pdf/efficiency_" + variable + ".pdf");
  if (doSavePng) canvas->SaveAs("png/efficiency_" + variable + ".png");
}


//------------------------------------------------------------------------------
//
// Draw fakes
//
//------------------------------------------------------------------------------
void DrawFakes(TString variable,
	       Int_t   rebin)
{
  TGraphAsymmErrors* sta_efficiency   = MakeFakes("Sta",   variable, PU200, kBlack,    rebin);
  TGraphAsymmErrors* trk_efficiency   = MakeFakes("Trk",   variable, PU200, kRed+1,    rebin);
  TGraphAsymmErrors* glb_efficiency   = MakeFakes("Glb",   variable, PU200, kBlue,     rebin);
  TGraphAsymmErrors* tight_efficiency = MakeFakes("Tight", variable, PU200, kGreen+2,  rebin);
  TGraphAsymmErrors* soft_efficiency  = MakeFakes("Soft",  variable, PU200, kOrange+7, rebin);

  TGraphAsymmErrors* sta_efficiency_noPU   = MakeFakes("Sta",   variable, noPU, kBlack,    rebin);
  TGraphAsymmErrors* trk_efficiency_noPU   = MakeFakes("Trk",   variable, noPU, kRed+1,    rebin);
  TGraphAsymmErrors* glb_efficiency_noPU   = MakeFakes("Glb",   variable, noPU, kBlue,     rebin);
  TGraphAsymmErrors* tight_efficiency_noPU = MakeFakes("Tight", variable, noPU, kGreen+2,  rebin);
  TGraphAsymmErrors* soft_efficiency_noPU  = MakeFakes("Soft",  variable, noPU, kOrange+7, rebin);

  TCanvas* canvas = new TCanvas("fakes " + variable, "fakes " + variable, 600, 600);

  canvas->SetLeftMargin (0.13);
  canvas->SetRightMargin(0.24);

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

  // Ranges
  if (doSetRanges)
    {
      mg->SetMinimum(-0.05);
      mg->SetMaximum( 1.05);
    }

  // Cosmetics
  canvas->SetGridx();
  canvas->SetGridy();

  // Labels
  mg->SetTitle("");

  if (variable == "vr")  mg->GetXaxis()->SetTitle("production radius [cm]");
  if (variable == "pt")  mg->GetXaxis()->SetTitle("p_{T} [GeV]");
  if (variable == "eta") mg->GetXaxis()->SetTitle("#eta");

  mg->GetYaxis()->SetTitle("reconstruction fake rate");
  mg->GetXaxis()->SetTitleOffset(1.5);
  mg->GetYaxis()->SetTitleOffset(1.5);

  // Legend
  TLegend* legend = new TLegend(0.775, 0.5, 0.95, 0.91);

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

  if (doSavePdf) canvas->SaveAs("pdf/fakes_" + variable + ".pdf");
  if (doSavePng) canvas->SaveAs("png/fakes_" + variable + ".png");
}


//------------------------------------------------------------------------------
//
// Draw resolution
//
//------------------------------------------------------------------------------
void DrawResolution(TString muonType)
{
  TCanvas* c2 = new TCanvas("resolution " + muonType, "resolution " + muonType);

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
  h_resolution[0]->SetXTitle(muonType + " #Deltaq/p_{T} / (q/p_{T})");
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
// Draw TH2
//
//------------------------------------------------------------------------------
void DrawTH2(TString variable,
	     TString muonType)
{
  TH2F* h2 = (TH2F*)file_noPU->Get("muonAnalysis/Gen" + muonType + "Muons_" + variable);

  TCanvas* canvas = new TCanvas("Gen vs. " + muonType + " " + variable,
				"Gen vs. " + muonType + " " + variable);

  h2->SetTitle("");
  h2->GetXaxis()->SetTitle("gen #" + variable);
  h2->GetYaxis()->SetTitle("(no PU) " + muonType + " #" + variable);
  h2->GetXaxis()->SetTitleOffset(1.5);
  h2->GetYaxis()->SetTitleOffset(2.0);

  h2->Draw("colz");

  if (doSavePdf) canvas->SaveAs("pdf/Gen_vs_" + muonType + "_" + variable + ".pdf");
  if (doSavePng) canvas->SaveAs("png/Gen_vs_" + muonType + "_" + variable + ".png");
}


//------------------------------------------------------------------------------
//
// Compare
//
//------------------------------------------------------------------------------
void Compare(TString variable,
	     TString muonType,
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
  TString the_title = (muonType.Contains("ANY")) ? variable : muonType + " " + variable;
  TString the_label = (muonType.Contains("ANY")) ? variable : muonType + "_" + variable;

  TCanvas* canvas = new TCanvas("compare " + the_title,
				"compare " + the_title);

  if (variable.Contains("dR"))   canvas->SetLogy();
  if (variable.Contains("MuPF")) canvas->SetLogy();

  TH1F* h_noPU_overflow  = AddOverflow(h_noPU);
  TH1F* h_PU200_overflow = AddOverflow(h_PU200);
  
  if (h_noPU_overflow->GetMaximum() > h_PU200_overflow->GetMaximum())
    {
      h_noPU_overflow ->Draw("hist");
      h_PU200_overflow->Draw("hist,same");
      h_noPU_overflow ->GetXaxis()->SetTitle(the_title);
      
      if (xmax > -999) h_noPU_overflow->GetXaxis()->SetRangeUser(-1, xmax);
    }
  else
    {
      h_PU200_overflow->Draw("hist");
      h_noPU_overflow ->Draw("hist,same");
      h_PU200_overflow->GetXaxis()->SetTitle(the_title);
      
      if (xmax > -999) h_PU200_overflow->GetXaxis()->SetRangeUser(-1, xmax);
    }
  
  canvas->GetFrame()->DrawClone();

  if (doSavePdf) canvas->SaveAs("pdf/compare_" + the_label + ".pdf");
  if (doSavePng) canvas->SaveAs("png/compare_" + the_label + ".png");
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

  // Fill the new hisogram including the extra bin for overflows
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
