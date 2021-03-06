//------------------------------------------------------------------------------
//
// Produce and compare displaced muons resolution
//
// root -l -b -q 'doResolution.C+("CTau-1_PU200","CTau-100_PU200")'
//
//------------------------------------------------------------------------------
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TFrame.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TInterpreter.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "TString.h"
#include "TSystem.h"


// Data members
//------------------------------------------------------------------------------
const Int_t    nbins_pt = 6;

const Double_t pt_bins[nbins_pt+1] = {10, 60, 90, 130, 170, 250, 500};

Color_t        ptcolor[nbins_pt] = {kRed-10, kRed-9, kRed-7, kRed-4, kRed, kRed+1};

Bool_t         doSavePdf = true;
Bool_t         doSavePng = true;

Bool_t         draw_sta   = false;
Bool_t         draw_trk   = true;
Bool_t         draw_glb   = true;
Bool_t         draw_tight = false;
Bool_t         draw_soft  = false;

TString        directory = "displaced-muons";

TFile*         file1 = NULL;
TFile*         file2 = NULL;

TLegend*       resolution_legend = NULL;

TMultiGraph*   mg_mean  = NULL;
TMultiGraph*   mg_width = NULL;


// Main arguments
//------------------------------------------------------------------------------
TString        file1name;
TString        file2name;


// Member functions
//------------------------------------------------------------------------------
void          DrawMultiGraph(TMultiGraph* mg,
			     TString      title);

void          DrawResolution(TString      muonType,
			     TString      xtitle,
			     TString      filename,
			     Color_t      color);

void          SetLegend     (TLegend*     tl,
			     Size_t       tsize);

TGraphErrors* SetGraph      (Int_t        npoints,
			     Color_t      color,
			     Style_t      style);

void          DrawLatex     (Font_t       tfont,
			     Float_t      x,
			     Float_t      y,
			     Float_t      tsize,
			     Short_t      align,
			     const char*  text,
			     Bool_t       setndc = true);


//------------------------------------------------------------------------------
//
// doResolution
//
//------------------------------------------------------------------------------
void doResolution(TString name1 = "CTau-1_PU200",
		  TString name2 = "CTau-1_noPU")
{
  file1name = name1;
  file2name = name2;

  printf("\n");
  printf(" file1name = %s\n", file1name.Data());
  printf(" file2name = %s\n", file2name.Data());
  printf("\n");

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
  mg_mean  = new TMultiGraph();
  mg_width = new TMultiGraph();

  resolution_legend = new TLegend(0.74, 0.5, 0.95, 0.91);

  SetLegend(resolution_legend, 0.025);

  if (draw_sta)   DrawResolution("Sta",   "standalone muons", file1name, kBlack);
  if (draw_trk)   DrawResolution("Trk",   "tracker muons",    file1name, kRed+1);
  if (draw_glb)   DrawResolution("Glb",   "global muons",     file1name, kBlue);
  if (draw_tight) DrawResolution("Tight", "tight muons",      file1name, kGreen+2);
  if (draw_soft)  DrawResolution("Soft",  "soft muons",       file1name, kOrange+7);

  if (draw_sta)   DrawResolution("Sta",   "standalone muons", file2name, kBlack);
  if (draw_trk)   DrawResolution("Trk",   "tracker muons",    file2name, kRed+1);
  if (draw_glb)   DrawResolution("Glb",   "global muons",     file2name, kBlue);
  if (draw_tight) DrawResolution("Tight", "tight muons",      file2name, kGreen+2);
  if (draw_soft)  DrawResolution("Soft",  "soft muons",       file2name, kOrange+7);

  DrawMultiGraph(mg_mean,  "mean");
  DrawMultiGraph(mg_width, "width");
}


//------------------------------------------------------------------------------
// Draw MultiGraph
//------------------------------------------------------------------------------
void DrawMultiGraph(TMultiGraph* mg,
		    TString      title)
{
  TCanvas* canvas = new TCanvas(title, title, 650, 600);

  canvas->SetLeftMargin (0.14);
  canvas->SetRightMargin(0.28);
  canvas->SetGridx();
  canvas->SetGridy();

  mg->Draw("apz");

  mg->GetXaxis()->SetTitle("gen p_{T} [GeV]");
  mg->GetYaxis()->SetTitle("");

  DrawLatex(42, 0.43, 0.95, 0.04, 21, Form("#Deltaq/p_{T} / (q/p_{T}) fit %s", title.Data()));

  mg->GetXaxis()->SetTitleOffset(1.6);
  mg->GetYaxis()->SetTitleOffset(2.1);

  resolution_legend->Draw("same");

  if (doSavePdf) canvas->SaveAs(directory + "/resolution_" + title + ".pdf");
  if (doSavePng) canvas->SaveAs(directory + "/resolution_" + title + ".png");
}


//------------------------------------------------------------------------------
//
// Draw resolution
//
//------------------------------------------------------------------------------
void DrawResolution(TString muonType,
		    TString xtitle,
		    TString filename,
		    Color_t color)
{
  TFile* file = (filename == file1name) ? file1 : file2;
  
  TCanvas* canvas = new TCanvas(xtitle + " resolution (" + filename + ")",
				xtitle + " resolution (" + filename + ")");
  
  // One histogram per pt bin
  TH1F* h_resolution[nbins_pt];

  // Graphs that will store the mean and width of the resolution
  Style_t gr_style = (filename == file1name) ? kFullCircle : kOpenCircle;

  TGraphErrors* gr_mean  = SetGraph(nbins_pt, color, gr_style);
  TGraphErrors* gr_width = SetGraph(nbins_pt, color, gr_style);

  // Legend
  TLegend* legend = new TLegend(0.61, 0.6, 0.82, 0.89);

  SetLegend(legend, 0.03);

  // Vertical maximum
  Float_t ymax = 0;


  // Loop over pt bins
  //----------------------------------------------------------------------------
  for (Int_t i=0; i<nbins_pt; i++) {

    TH2F* h2 = (TH2F*)file->Get(Form("muonAnalysis/%sMuons_pt_resolution_pt%d", muonType.Data(), i));

    TString hname = Form("%s_%s_vxy", h2->GetName(), filename.Data());

    h_resolution[i] = (TH1F*)h2->ProjectionY(hname);

    TF1* gfit = new TF1("gfit", "gaus", -0.035, 0.035);

    gfit->SetParameters(1,0,1);

    h_resolution[i]->Fit("gfit", "nqr");
    
    Float_t mean_value  = gfit->GetParameter(1);
    Float_t width_value = gfit->GetParameter(2);

    Float_t mean_error  = gfit->GetParError(1);
    Float_t width_error = gfit->GetParError(2);

    gr_mean ->SetPoint(i, 0.5*(pt_bins[i]+pt_bins[i+1]), mean_value);
    gr_width->SetPoint(i, 0.5*(pt_bins[i]+pt_bins[i+1]), width_value);

    gr_mean ->SetPointError(i, 0.5*(pt_bins[i+1]-pt_bins[i]), mean_error);
    gr_width->SetPointError(i, 0.5*(pt_bins[i+1]-pt_bins[i]), width_error);

    if (h_resolution[i]->GetMaximum() > ymax) ymax = h_resolution[i]->GetMaximum();

    h_resolution[i]->SetLineColor(ptcolor[i]);
    h_resolution[i]->SetLineWidth(2);

    TString option = (i == 0) ? "hist" : "hist,same";

    h_resolution[i]->Draw(option);

    legend->AddEntry(h_resolution[i], Form(" %.0f < p_{T} < %.0f GeV", pt_bins[i], pt_bins[i+1]), "l");
  }


  // Save the mean and the width
  mg_mean ->Add(gr_mean);
  mg_width->Add(gr_width);

  // Legend
  if (muonType.EqualTo("Sta")   && draw_sta)   resolution_legend->AddEntry(gr_mean, "(" + filename + ") sta",   "lp");
  if (muonType.EqualTo("Trk")   && draw_trk)   resolution_legend->AddEntry(gr_mean, "(" + filename + ") trk",   "lp");
  if (muonType.EqualTo("Glb")   && draw_glb)   resolution_legend->AddEntry(gr_mean, "(" + filename + ") glb",   "lp");
  if (muonType.EqualTo("Tight") && draw_tight) resolution_legend->AddEntry(gr_mean, "(" + filename + ") tight", "lp");
  if (muonType.EqualTo("Soft")  && draw_soft)  resolution_legend->AddEntry(gr_mean, "(" + filename + ") soft",  "lp");

  // Cosmetics
  h_resolution[0]->SetMaximum(1.1 * ymax);
  h_resolution[0]->SetTitle("");
  h_resolution[0]->SetXTitle(xtitle + " #Deltaq/p_{T} / (q/p_{T})");
  h_resolution[0]->SetYTitle("entries / bin");
  h_resolution[0]->GetXaxis()->SetTitleOffset(1.5);
  h_resolution[0]->GetYaxis()->SetTitleOffset(2.0);

  DrawLatex(42, 0.940, 0.945, 0.04, 31, filename);

  legend->Draw();

  canvas->GetFrame()->DrawClone();

  // Save
  if (doSavePdf) canvas->SaveAs(directory + "/resolution_" + muonType + "_" + filename + ".pdf");
  if (doSavePng) canvas->SaveAs(directory + "/resolution_" + muonType + "_" + filename + ".png");
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


//------------------------------------------------------------------------------
// SetGraph
//------------------------------------------------------------------------------
TGraphErrors* SetGraph(Int_t   npoints,
		       Color_t color,
		       Style_t style)
{
  TGraphErrors* gr = new TGraphErrors(npoints);

  gr->SetLineColor  (color);
  gr->SetLineWidth  (1    );
  gr->SetMarkerColor(color);
  gr->SetMarkerStyle(style);

  return gr;
}


//------------------------------------------------------------------------------
// DrawLatex
//------------------------------------------------------------------------------
void DrawLatex(Font_t      tfont,
	       Float_t     x,
	       Float_t     y,
	       Float_t     tsize,
	       Short_t     align,
	       const char* text,
	       Bool_t      setndc)
{
  TLatex* tl = new TLatex(x, y, text);

  tl->SetNDC      (setndc);
  tl->SetTextAlign( align);
  tl->SetTextFont ( tfont);
  tl->SetTextSize ( tsize);

  tl->Draw("same");
}
