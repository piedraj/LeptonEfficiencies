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


// Data members
//------------------------------------------------------------------------------
enum        {noPU, PU200};

const Int_t nbinspt = 3;

Color_t     ptcolor[nbinspt] = {kRed+1, kBlue, kBlack};

Bool_t      doRebin     = true;
Bool_t      doSetRanges = true;
Bool_t      doSavePdf   = true;
Bool_t      doSavePng   = true;

TFile*      file_PU200 = NULL;
TFile*      file_noPU  = NULL;


// Member functions
//------------------------------------------------------------------------------
TGraphAsymmErrors* MakeEfficiency(TString variable,
				  Int_t   PU,
				  Color_t color);

void               DrawEfficiency();

void               DrawResolution(TString muonType);

void               DrawTH2       (TString variable);

void               Compare       (TString variable,
				  TString muonType);

void               PaintOverflow (TH1*    h,
				  TString option,
				  TString xtitle = "");


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
  file_PU200 = TFile::Open("rootfiles/MyMuonPlots_PU200.root");
  file_noPU  = TFile::Open("rootfiles/MyMuonPlots_noPU.root");


  // Do the work
  //----------------------------------------------------------------------------
  DrawEfficiency();

  DrawResolution("Sta");
  DrawResolution("Trk");
  DrawResolution("Glb");

  DrawTH2("eta");
  DrawTH2("phi");

  Compare("dR", "Sta");
  Compare("dR", "Trk");
  Compare("dR", "Glb");

  Compare("pt", "Sta");
  Compare("pt", "Trk");
  Compare("pt", "Glb");
}


//------------------------------------------------------------------------------
//
// Make efficiency
//
//------------------------------------------------------------------------------
TGraphAsymmErrors* MakeEfficiency(TString variable,
				  Int_t   PU,
				  Color_t color)
{
  TFile* file = (PU == noPU) ? file_noPU : file_PU200;

  Style_t style = (PU == noPU) ? kOpenCircle : kFullCircle;

  TString num_name = "muonAnalysis/" + variable + "Muons_vr";
  TString den_name = "muonAnalysis/GenMuons_vr";

  TH1F* hnum = (TH1F*)(file->Get(num_name))->Clone("hnum");
  TH1F* hden = (TH1F*)(file->Get(den_name))->Clone("hden");

  if (doRebin) hnum->Rebin(5);
  if (doRebin) hden->Rebin(5);

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
void DrawEfficiency()
{
  TGraphAsymmErrors* sta_efficiency = MakeEfficiency("Sta", PU200, kBlack);
  TGraphAsymmErrors* trk_efficiency = MakeEfficiency("Trk", PU200, kRed+1);
  TGraphAsymmErrors* glb_efficiency = MakeEfficiency("Glb", PU200, kBlue);

  TGraphAsymmErrors* sta_efficiency_noPU = MakeEfficiency("Sta", noPU, kBlack);
  TGraphAsymmErrors* trk_efficiency_noPU = MakeEfficiency("Trk", noPU, kRed+1);
  TGraphAsymmErrors* glb_efficiency_noPU = MakeEfficiency("Glb", noPU, kBlue);

  TCanvas* canvas = new TCanvas("efficiency", "efficiency", 600, 600);

  canvas->SetLeftMargin (0.13);
  canvas->SetRightMargin(0.24);

  TMultiGraph* mg = new TMultiGraph();

  mg->Add(sta_efficiency);
  mg->Add(sta_efficiency_noPU);
  mg->Add(trk_efficiency);
  mg->Add(trk_efficiency_noPU);
  mg->Add(glb_efficiency);
  mg->Add(glb_efficiency_noPU);

  mg->Draw("apz");

  // Ranges
  if (doSetRanges)
    {
      mg->GetXaxis()->SetRangeUser(0, 49);

      mg->SetMinimum(-0.05);
      mg->SetMaximum( 1.05);
    }

  // Cosmetics
  canvas->SetGridx();
  canvas->SetGridy();

  // Labels
  mg->SetTitle("");
  mg->GetXaxis()->SetTitle("production radius [cm]");
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

  legend->AddEntry(sta_efficiency_noPU, "(no PU) sta",  "lp");
  legend->AddEntry(trk_efficiency_noPU, "(no PU) trk",  "lp");
  legend->AddEntry(glb_efficiency_noPU, "(no PU) glb",  "lp");
  legend->AddEntry(sta_efficiency,      "(200 PU) sta", "lp");
  legend->AddEntry(trk_efficiency,      "(200 PU) trk", "lp");
  legend->AddEntry(glb_efficiency,      "(200 PU) glb", "lp");

  legend->Draw();

  canvas->Modified();
  canvas->Update();

  if (doSavePdf) canvas->SaveAs("pdf/efficiency.pdf");
  if (doSavePng) canvas->SaveAs("png/efficiency.png");
}


//------------------------------------------------------------------------------
//
// Draw resolution
//
//------------------------------------------------------------------------------
void DrawResolution(TString muonType)
{
  TCanvas* c2 = new TCanvas("resolution " + muonType, "resolution " + muonType);

  TH1F* hStaMuons_res[nbinspt];

  Float_t ymax = 0;

  for (Int_t i=0; i<nbinspt; i++) {

    hStaMuons_res[i] = (TH1F*)file_PU200->Get(Form("muonAnalysis/%sMuons_res_%d", muonType.Data(), i));

    if (hStaMuons_res[i]->GetMaximum() > ymax) ymax = hStaMuons_res[i]->GetMaximum();

    hStaMuons_res[i]->SetLineColor(ptcolor[i]);
    hStaMuons_res[i]->SetLineWidth(3);

    TString option = (i == 0) ? "" : "same";

    hStaMuons_res[i]->Draw(option);
  }

  hStaMuons_res[0]->SetTitle("");
  hStaMuons_res[0]->SetXTitle(muonType + " #Deltaq/p_{T} / (q/p_{T})");
  hStaMuons_res[0]->SetYTitle("entries / bin");
  hStaMuons_res[0]->SetMaximum(1.1 * ymax);
  hStaMuons_res[0]->GetXaxis()->SetTitleOffset(1.5);
  hStaMuons_res[0]->GetYaxis()->SetTitleOffset(2.0);

  // Legend
  TLegend* legend = new TLegend(0.59, 0.72, 0.85, 0.88);

  legend->SetBorderSize(    0);
  legend->SetFillColor (    0);
  legend->SetTextAlign (   12);
  legend->SetTextFont  (   42);
  legend->SetTextSize  (0.035);

  legend->AddEntry(hStaMuons_res[0], "10 < p_{T} < 20 GeV", "l");
  legend->AddEntry(hStaMuons_res[1], "20 < p_{T} < 35 GeV", "l");
  legend->AddEntry(hStaMuons_res[2], "35 < p_{T} < 50 GeV", "l");

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
void DrawTH2(TString variable)
{
  TH2F* h2 = (TH2F*)file_PU200->Get("muonAnalysis/GenStaMuons_" + variable);

  TCanvas* canvas = new TCanvas(variable, variable);

  h2->SetTitle("");
  h2->GetXaxis()->SetTitle("gen #" + variable);
  h2->GetYaxis()->SetTitle("sta #" + variable);
  h2->GetXaxis()->SetTitleOffset(1.5);
  h2->GetYaxis()->SetTitleOffset(2.0);

  h2->Draw("colz");

  if (doSavePdf) canvas->SaveAs("pdf/" + variable + ".pdf");
  if (doSavePng) canvas->SaveAs("png/" + variable + ".png");
}


//------------------------------------------------------------------------------
//
// Compare
//
//------------------------------------------------------------------------------
void Compare(TString variable,
	     TString muonType)
{
  TH1F* h_noPU  = (TH1F*)(file_noPU ->Get("muonAnalysis/" + muonType + "Muons_" + variable))->Clone("h_" + muonType + "_noPU_"  + variable);
  TH1F* h_PU200 = (TH1F*)(file_PU200->Get("muonAnalysis/" + muonType + "Muons_" + variable))->Clone("h_" + muonType + "_PU200_" + variable);

  h_noPU ->Rebin(2);
  h_PU200->Rebin(2);

  h_noPU ->Scale(1. / h_noPU ->Integral(-1, -1));
  h_PU200->Scale(1. / h_PU200->Integral(-1, -1));

  h_noPU ->SetLineColor(kBlack);
  h_PU200->SetLineColor(kRed+1);

  h_noPU ->SetLineWidth(2);
  h_PU200->SetLineWidth(2);


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas("compare " + muonType + " " + variable,
				"compare " + muonType + " " + variable);

  if (variable.Contains("dR")) canvas->SetLogy();

  Int_t nx = h_noPU->GetNbinsX() + 1;

  Float_t ymax_noPU  = (h_noPU ->GetMaximum() > h_noPU ->GetBinContent(nx)) ? h_noPU ->GetMaximum() : h_noPU ->GetBinContent(nx);
  Float_t ymax_PU200 = (h_PU200->GetMaximum() > h_PU200->GetBinContent(nx)) ? h_PU200->GetMaximum() : h_PU200->GetBinContent(nx);

  if (ymax_noPU > ymax_PU200)
    {
      PaintOverflow(h_noPU,  "hist", muonType + " " + variable);
      PaintOverflow(h_PU200, "hist,same");
    }
  else
    {
      PaintOverflow(h_PU200, "hist", muonType + " " + variable);
      PaintOverflow(h_noPU,  "hist,same");
    }

  canvas->GetFrame()->DrawClone();

  if (doSavePdf) canvas->SaveAs("pdf/compare_" + muonType + "_" + variable + ".pdf");
  if (doSavePng) canvas->SaveAs("png/compare_" + muonType + "_" + variable + ".png");
}


//------------------------------------------------------------------------------
// Paint overflow
//------------------------------------------------------------------------------
void PaintOverflow(TH1*    h,
		   TString option,
		   TString xtitle)
{
  TString  name = h->GetName();
  Int_t    nx   = h->GetNbinsX()+1;
  Double_t bw   = h->GetBinWidth(nx);
  Double_t x1   = h->GetBinLowEdge(1);
  Double_t x2   = h->GetBinLowEdge(nx) + bw;
  
  // Book a temporary histogram having an extra bin for overflows
  TH1F* htmp = new TH1F(name + "_overflow", "", nx, x1, x2);

  // Set line color and width
  htmp->SetLineColor(h->GetLineColor());
  htmp->SetLineWidth(h->GetLineWidth());

  // Fill the new histogram including the extra bin for overflows
  for (Int_t i=1; i<=nx; i++) {
    htmp->Fill(htmp->GetBinCenter(i), h->GetBinContent(i));
  }

  // Fill the underflow
  htmp->Fill(x1-1, h->GetBinContent(0));

  // Restore the number of entries
  htmp->SetEntries(h->GetEntries());

  // Draw the temporary histogram
  htmp->Draw(option);

  // Draw x-title
  if (!option.Contains("same"))
    {
      htmp->GetXaxis()->SetTitle      (xtitle);
      htmp->GetXaxis()->SetTitleOffset(   1.5);
    }
}
