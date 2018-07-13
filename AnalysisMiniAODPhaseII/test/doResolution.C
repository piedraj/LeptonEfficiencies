//------------------------------------------------------------------------------
//
// Compare displaced muons resolution for samples with no PU and 200 PU
//
// root -l -b -q doResolution.C+
//
//------------------------------------------------------------------------------
#include "TCanvas.h"
#include "TFile.h"
#include "TFrame.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TInterpreter.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "TString.h"
#include "TSystem.h"


// Data members
//------------------------------------------------------------------------------
enum          {noPU, PU200};

const Int_t   nbinspt = 6;

const Float_t ptbins[nbinspt+1] = {10, 20, 35, 50, 100, 200, 500};

Color_t       ptcolor[nbinspt] = {kRed-10, kRed-9, kRed-7, kRed-4, kRed, kRed+1};

Bool_t        doSavePdf = true;
Bool_t        doSavePng = true;

TFile*        file_PU200 = NULL;
TFile*        file_noPU  = NULL;

Bool_t        draw_sta   = false;
Bool_t        draw_trk   = false;
Bool_t        draw_glb   = false;
Bool_t        draw_tight = true;
Bool_t        draw_soft  = true;

TString       directory = "displaced-muons";

TMultiGraph*  mg_mean  = NULL;
TMultiGraph*  mg_width = NULL;


// Member functions
//------------------------------------------------------------------------------
void          DrawResolution(TString  muonType,
			     TString  xtitle,
			     Int_t    PU,
			     Color_t  color);

void          SetLegend     (TLegend* tl);

TGraphErrors* SetGraph      (Int_t    npoints,
			     Color_t  color,
			     Style_t  style);


//------------------------------------------------------------------------------
//
// doResolution
//
//------------------------------------------------------------------------------
void doResolution()
{
  gInterpreter->ExecuteMacro("PaperStyle.C");

  if (doSavePdf) gSystem->mkdir(directory, kTRUE);

  TH1::SetDefaultSumw2();


  // Input files
  //----------------------------------------------------------------------------
  file_PU200 = TFile::Open("rootfiles/DisplacedSUSY_PU200.root");
  file_noPU  = TFile::Open("rootfiles/DisplacedSUSY_noPU.root");


  // Do the work
  //----------------------------------------------------------------------------
  mg_mean  = new TMultiGraph();
  mg_width = new TMultiGraph();

  if (draw_sta)   DrawResolution("Sta",   "standalone muons", noPU, kBlack);
  if (draw_trk)   DrawResolution("Trk",   "tracker muons",    noPU, kRed+1);
  if (draw_glb)   DrawResolution("Glb",   "global muons",     noPU, kBlue);
  if (draw_tight) DrawResolution("Tight", "tight muons",      noPU, kGreen+2);
  if (draw_soft)  DrawResolution("Soft",  "soft muons",       noPU, kOrange+7);

  if (draw_sta)   DrawResolution("Sta",   "standalone muons", PU200, kBlack);
  if (draw_trk)   DrawResolution("Trk",   "tracker muons",    PU200, kRed+1);
  if (draw_glb)   DrawResolution("Glb",   "global muons",     PU200, kBlue);
  if (draw_tight) DrawResolution("Tight", "tight muons",      PU200, kGreen+2);
  if (draw_soft)  DrawResolution("Soft",  "soft muons",       PU200, kOrange+7);

  
  // Draw mean
  //----------------------------------------------------------------------------
  TCanvas* c1 = new TCanvas("mean", "mean");

  mg_mean->Draw("apz");

  mg_mean->GetXaxis()->SetTitle("gen p_{T} [GeV]");
  mg_mean->GetYaxis()->SetTitle("#Deltaq/p_{T} / (q/p_{T}) mean");

  mg_mean->GetXaxis()->SetTitleOffset(1.6);
  mg_mean->GetYaxis()->SetTitleOffset(1.8);

  if (doSavePdf) c1->SaveAs(directory + "/resolution_mean.pdf");
  if (doSavePng) c1->SaveAs(directory + "/resolution_mean.png");


  // Draw width
  //----------------------------------------------------------------------------
  TCanvas* c2 = new TCanvas("width", "width");

  mg_width->Draw("apz");

  mg_width->GetXaxis()->SetTitle("gen p_{T} [GeV]");
  mg_width->GetYaxis()->SetTitle("#Deltaq/p_{T} / (q/p_{T}) RMS");

  mg_width->GetXaxis()->SetTitleOffset(1.6);
  mg_width->GetYaxis()->SetTitleOffset(2.3);

  if (doSavePdf) c2->SaveAs(directory + "/resolution_RMS.pdf");
  if (doSavePng) c2->SaveAs(directory + "/resolution_RMS.png");
}


//------------------------------------------------------------------------------
//
// Draw resolution
//
//------------------------------------------------------------------------------
void DrawResolution(TString muonType,
		    TString xtitle,
		    Int_t   PU,
		    Color_t color)
{
  // Input file
  TFile* file = (PU == noPU) ? file_noPU : file_PU200;
  
  // PU string
  TString pu_string = (PU == noPU) ? "noPU" : "PU200";

  // Drawing canvas
  TCanvas* canvas = new TCanvas(xtitle + " resolution (" + pu_string + ")",
				xtitle + " resolution (" + pu_string + ")");
  
  // One histogram per pt bin
  TH1F* h_resolution[nbinspt];

  // PU marker style
  Style_t style = (PU == noPU) ? kOpenCircle : kFullCircle;

  // Graphs that will store the mean and width of the resolution
  TGraphErrors* gr_mean  = SetGraph(nbinspt, color, style);
  TGraphErrors* gr_width = SetGraph(nbinspt, color, style);

  // Legend
  TLegend* legend = new TLegend(0.61, 0.6, 0.82, 0.89);

  SetLegend(legend);

  // Vertical maximum
  Float_t ymax = 0;

  // Loop
  for (Int_t i=0; i<nbinspt; i++) {

    h_resolution[i] = (TH1F*)file->Get(Form("muonAnalysis/%sMuons_res_%d", muonType.Data(), i));

    // Get the mean and the width
    gr_mean ->SetPoint(i, 0.5*(ptbins[i]+ptbins[i+1]), h_resolution[i]->GetMean());
    gr_width->SetPoint(i, 0.5*(ptbins[i]+ptbins[i+1]), h_resolution[i]->GetRMS());

    gr_mean ->SetPointError(i, 0.5*(ptbins[i+1]-ptbins[i]), 1e-9);
    gr_width->SetPointError(i, 0.5*(ptbins[i+1]-ptbins[i]), 1e-9);

    if (h_resolution[i]->GetMaximum() > ymax) ymax = h_resolution[i]->GetMaximum();

    h_resolution[i]->SetLineColor(ptcolor[i]);
    h_resolution[i]->SetLineWidth(2);

    TString option = (i == 0) ? "" : "same";

    h_resolution[i]->Draw(option);

    legend->AddEntry(h_resolution[i], Form(" %.0f < p_{T} < %.0f GeV", ptbins[i], ptbins[i+1]), "l");
  }

  // Save the mean and the width
  mg_mean ->Add(gr_mean);
  mg_width->Add(gr_width);

  // Cosmetics
  h_resolution[0]->SetMaximum(1.1 * ymax);
  h_resolution[0]->SetTitle(pu_string);
  h_resolution[0]->SetXTitle(xtitle + " #Deltaq/p_{T} / (q/p_{T})");
  h_resolution[0]->SetYTitle("entries / bin");
  h_resolution[0]->GetXaxis()->SetTitleOffset(1.5);
  h_resolution[0]->GetYaxis()->SetTitleOffset(2.0);

  legend->Draw();

  canvas->GetFrame()->DrawClone();

  // Save
  if (doSavePdf) canvas->SaveAs(directory + "/resolution_" + muonType + "_" + pu_string + ".pdf");
  if (doSavePng) canvas->SaveAs(directory + "/resolution_" + muonType + "_" + pu_string + ".png");
}


//------------------------------------------------------------------------------
// SetLegend
//------------------------------------------------------------------------------
void SetLegend(TLegend* tl)
{
  tl->SetBorderSize(   0);
  tl->SetFillColor (   0);
  tl->SetTextAlign (  12);
  tl->SetTextFont  (  42);
  tl->SetTextSize  (0.03);
}


//------------------------------------------------------------------------------
// SetGraph
//------------------------------------------------------------------------------
TGraphErrors* SetGraph(Int_t   npoints,
		       Color_t color,
		       Style_t style)
{
  TGraphErrors* gr = new TGraphErrors(npoints);

  gr->SetLineColor  (color);
  gr->SetMarkerColor(color);
  gr->SetMarkerStyle(style);

  return gr;
}
