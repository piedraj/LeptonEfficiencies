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
#include "TH1F.h"
#include "TInterpreter.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "TString.h"
#include "TSystem.h"


// Data members
//------------------------------------------------------------------------------
enum        {noPU, PU200};

const Int_t nbinspt = 6;

Color_t     ptcolor[nbinspt] = {kRed-10, kRed-9, kRed-7, kRed-4, kRed, kRed+1};

Bool_t      doSavePdf = true;
Bool_t      doSavePng = true;

TFile*      file_PU200 = NULL;
TFile*      file_noPU  = NULL;

Bool_t      draw_sta   = false;
Bool_t      draw_trk   = false;
Bool_t      draw_glb   = false;
Bool_t      draw_tight = true;
Bool_t      draw_soft  = true;

TString     directory = "displaced-muons";


// Member functions
//------------------------------------------------------------------------------
void        DrawResolution(TString muonType,
			   TString xtitle,
			   Int_t   PU);


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
  if (draw_sta)   DrawResolution("Sta",   "standalone muons", noPU);
  if (draw_trk)   DrawResolution("Trk",   "tracker muons",    noPU);
  if (draw_glb)   DrawResolution("Glb",   "global muons",     noPU);
  if (draw_tight) DrawResolution("Tight", "tight muons",      noPU);
  if (draw_soft)  DrawResolution("Soft",  "soft muons",       noPU);

  if (draw_sta)   DrawResolution("Sta",   "standalone muons", PU200);
  if (draw_trk)   DrawResolution("Trk",   "tracker muons",    PU200);
  if (draw_glb)   DrawResolution("Glb",   "global muons",     PU200);
  if (draw_tight) DrawResolution("Tight", "tight muons",      PU200);
  if (draw_soft)  DrawResolution("Soft",  "soft muons",       PU200);
}


//------------------------------------------------------------------------------
//
// Draw resolution
//
//------------------------------------------------------------------------------
void DrawResolution(TString muonType,
		    TString xtitle,
		    Int_t   PU)
{
  TFile* file = (PU == noPU) ? file_noPU : file_PU200;

  TString pu_string = (PU == noPU) ? "noPU" : "PU200";

  TCanvas* canvas = new TCanvas(xtitle + " resolution (" + pu_string + ")",
				xtitle + " resolution (" + pu_string + ")");

  TH1F* h_resolution[nbinspt];

  Float_t ymax = 0;

  for (Int_t i=0; i<nbinspt; i++) {

    h_resolution[i] = (TH1F*)file->Get(Form("muonAnalysis/%sMuons_res_%d", muonType.Data(), i));

    if (h_resolution[i]->GetMaximum() > ymax) ymax = h_resolution[i]->GetMaximum();

    h_resolution[i]->SetLineColor(ptcolor[i]);
    h_resolution[i]->SetLineWidth(2);

    TString option = (i == 0) ? "" : "same";

    h_resolution[i]->Draw(option);
  }

  h_resolution[0]->SetMaximum(1.1 * ymax);

  h_resolution[0]->SetTitle(pu_string);

  h_resolution[0]->SetXTitle(xtitle + " #Deltaq/p_{T} / (q/p_{T})");
  h_resolution[0]->SetYTitle("entries / bin");
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

  canvas->GetFrame()->DrawClone();

  if (doSavePdf) canvas->SaveAs(directory + "/resolution_" + muonType + "_" + pu_string + ".pdf");
  if (doSavePng) canvas->SaveAs(directory + "/resolution_" + muonType + "_" + pu_string + ".png");
}
