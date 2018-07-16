//------------------------------------------------------------------------------
//
// Compare displaced muons resolution for samples with no PU and 200 PU
//
// root -l -b -q doResolution.C+
//
//------------------------------------------------------------------------------
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TFrame.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TInterpreter.h"
#include "TLatex.h"
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

Bool_t        draw_sta   = false;
Bool_t        draw_trk   = false;
Bool_t        draw_glb   = false;
Bool_t        draw_tight = true;
Bool_t        draw_soft  = true;

TString       directory = "displaced-muons";

TFile*        file_PU200 = NULL;
TFile*        file_noPU  = NULL;

TLegend*      resolution_legend = NULL;

TMultiGraph*  mg_mean  = NULL;
TMultiGraph*  mg_width = NULL;


// Member functions
//------------------------------------------------------------------------------
void          DrawResolution(TString     muonType,
			     TString     xtitle,
			     Int_t       PU,
			     Color_t     color);

void          SetLegend     (TLegend*    tl,
			     Size_t      tsize);

TGraphErrors* SetGraph      (Int_t       npoints,
			     Color_t     color,
			     Style_t     style);

void          DrawLatex     (Font_t      tfont,
			     Float_t     x,
			     Float_t     y,
			     Float_t     tsize,
			     Short_t     align,
			     const char* text,
			     Bool_t      setndc = true);

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

  resolution_legend = new TLegend(0.81, 0.5, 0.95, 0.91);

  SetLegend(resolution_legend, 0.025);

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

  c1->SetLeftMargin (0.165);
  c1->SetRightMargin(0.205);

  c1->SetGridx();
  c1->SetGridy();

  mg_mean->Draw("apz");

  mg_mean->SetMinimum(-0.002);
  mg_mean->SetMaximum(+0.002);

  mg_mean->GetXaxis()->SetTitle("gen p_{T} [GeV]");
  mg_mean->GetYaxis()->SetTitle("");

  DrawLatex(42, 0.5, 0.95, 0.04, 21, "#Deltaq/p_{T} / (q/p_{T}) fit mean");

  mg_mean->GetXaxis()->SetTitleOffset(1.6);
  mg_mean->GetYaxis()->SetTitleOffset(1.8);

  resolution_legend->Draw("same");

  if (doSavePdf) c1->SaveAs(directory + "/resolution_mean.pdf");
  if (doSavePng) c1->SaveAs(directory + "/resolution_mean.png");


  // Draw width
  //----------------------------------------------------------------------------
  TCanvas* c2 = new TCanvas("width", "width");

  c2->SetLeftMargin (0.165);
  c2->SetRightMargin(0.205);

  c2->SetGridx();
  c2->SetGridy();

  mg_width->Draw("apz");

  mg_width->GetXaxis()->SetTitle("gen p_{T} [GeV]");
  mg_width->GetYaxis()->SetTitle("");

  DrawLatex(42, 0.5, 0.95, 0.04, 21, "#Deltaq/p_{T} / (q/p_{T}) fit width");

  mg_width->GetXaxis()->SetTitleOffset(1.6);
  mg_width->GetYaxis()->SetTitleOffset(2.3);

  resolution_legend->Draw("same");

  if (doSavePdf) c2->SaveAs(directory + "/resolution_width.pdf");
  if (doSavePng) c2->SaveAs(directory + "/resolution_width.png");
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
  TFile* file = (PU == noPU) ? file_noPU : file_PU200;
  
  TString pu_string = (PU == noPU) ? "noPU" : "PU200";

  TString pu_label = (PU == noPU) ? "no PU" : "200 PU";

  Style_t pu_style = (PU == noPU) ? kOpenCircle : kFullCircle;

  TCanvas* canvas = new TCanvas(xtitle + " resolution (" + pu_label + ")",
				xtitle + " resolution (" + pu_label + ")");
  
  // One histogram per pt bin
  TH1F* h_resolution[nbinspt];

  // Graphs that will store the mean and width of the resolution
  TGraphErrors* gr_mean  = SetGraph(nbinspt, color, pu_style);
  TGraphErrors* gr_width = SetGraph(nbinspt, color, pu_style);

  // Legend
  TLegend* legend = new TLegend(0.61, 0.6, 0.82, 0.89);

  SetLegend(legend, 0.03);

  // Vertical maximum
  Float_t ymax = 0;

  printf("\n");

  // Loop
  for (Int_t i=0; i<nbinspt; i++) {

    h_resolution[i] = (TH1F*)file->Get(Form("muonAnalysis/%sMuons_res_%d", muonType.Data(), i));

    // Fit
    TF1 *gfit = new TF1("gfit", "gaus", -0.035, 0.035);

    gfit->SetParameters(1,0,1);

    h_resolution[i]->Fit("gfit", "nqr");

    printf(" %sMuons_res_%d (%s) Chisquare/NDF = %5.2f;  Prob = %5.2f%s\n",
	   muonType.Data(), i, pu_label.Data(), gfit->GetChisquare()/gfit->GetNDF(), 1e2*gfit->GetProb(), "%");

    Float_t mean_value  = gfit->GetParameter(1);  // h_resolution[i]->GetMean());
    Float_t width_value = gfit->GetParameter(2);  // h_resolution[i]->GetRMS());

    Float_t mean_error  = gfit->GetParError(1);   // h_resolution[i]->GetMeanError());
    Float_t width_error = gfit->GetParError(2);   // h_resolution[i]->GetRMSError());

    // Get the mean and the width
    gr_mean ->SetPoint(i, 0.5*(ptbins[i]+ptbins[i+1]), mean_value);
    gr_width->SetPoint(i, 0.5*(ptbins[i]+ptbins[i+1]), width_value);

    gr_mean ->SetPointError(i, 0.5*(ptbins[i+1]-ptbins[i]), mean_error);
    gr_width->SetPointError(i, 0.5*(ptbins[i+1]-ptbins[i]), width_error);

    if (h_resolution[i]->GetMaximum() > ymax) ymax = h_resolution[i]->GetMaximum();

    h_resolution[i]->SetLineColor(ptcolor[i]);
    h_resolution[i]->SetLineWidth(2);

    TString option = (i == 0) ? "" : "same";

    h_resolution[i]->Draw(option);

    //    gfit->Draw("same");

    legend->AddEntry(h_resolution[i], Form(" %.0f < p_{T} < %.0f GeV", ptbins[i], ptbins[i+1]), "l");
  }

  printf("\n");

  // Save the mean and the width
  mg_mean ->Add(gr_mean);
  mg_width->Add(gr_width);

  // Legend
  Bool_t pickMe = false;

  if (muonType.EqualTo("Sta")   && draw_sta)   resolution_legend->AddEntry(gr_mean, "(" + pu_label + ") sta",   "lp");
  if (muonType.EqualTo("Trk")   && draw_trk)   resolution_legend->AddEntry(gr_mean, "(" + pu_label + ") trk",   "lp");
  if (muonType.EqualTo("Glb")   && draw_glb)   resolution_legend->AddEntry(gr_mean, "(" + pu_label + ") glb",   "lp");
  if (muonType.EqualTo("Tight") && draw_tight) resolution_legend->AddEntry(gr_mean, "(" + pu_label + ") tight", "lp");
  if (muonType.EqualTo("Soft")  && draw_soft)  resolution_legend->AddEntry(gr_mean, "(" + pu_label + ") soft",  "lp");

  // Cosmetics
  h_resolution[0]->SetMaximum(1.1 * ymax);
  h_resolution[0]->SetTitle("");
  h_resolution[0]->SetXTitle(xtitle + " #Deltaq/p_{T} / (q/p_{T})");
  h_resolution[0]->SetYTitle("entries / bin");
  h_resolution[0]->GetXaxis()->SetTitleOffset(1.5);
  h_resolution[0]->GetYaxis()->SetTitleOffset(2.0);

  DrawLatex(42, 0.940, 0.945, 0.04, 31, pu_label);

  legend->Draw();

  canvas->GetFrame()->DrawClone();

  // Save
  if (doSavePdf) canvas->SaveAs(directory + "/resolution_" + muonType + "_" + pu_string + ".pdf");
  if (doSavePng) canvas->SaveAs(directory + "/resolution_" + muonType + "_" + pu_string + ".png");
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
  gr->SetLineWidth  (    1);
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
