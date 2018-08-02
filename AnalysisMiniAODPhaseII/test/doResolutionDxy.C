//------------------------------------------------------------------------------
//
// Compare displaced muons resolution for different dxy values
//
// root -l -b -q 'doResolutionDxy.C+("Soft", "soft muons", PU200)'
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
enum          {noPU, PU200};

const Int_t   nbins_pt  =  6;
const Int_t   nbins_vxy = 10;

const Float_t pt_bins[nbins_pt+1] = {10, 60, 90, 130, 170, 250, 500};

const Float_t vxy_bins[nbins_vxy+1] = {0.0000, 0.0088, 0.0200, 0.0334, 0.0498, 0.0704, 0.0968, 0.1332, 0.1892, 0.2988, 3.0000};

Color_t       pt_colors[nbins_pt] = {kRed-10, kRed-9, kRed-7, kRed-4, kRed, kRed+1};

Color_t       vxy_colors[nbins_vxy] = {kRed-10, kRed-9, kRed-8, kRed-7, kRed-6, kRed-5, kRed-4, kRed-3, kRed-2, kRed-1};

Bool_t        doSavePdf = true;
Bool_t        doSavePng = true;

TString       directory = "displaced-muons-dxy";

TFile*        file = NULL;

TLegend*      resolution_legend = NULL;

TMultiGraph*  mg_mean  = NULL;
TMultiGraph*  mg_width = NULL;

TString       pu_name;
TString       pu_label;
TString       muonType_name;
TString       muonType_label;


// Member functions
//------------------------------------------------------------------------------
void          DrawResolution(Int_t         vxy_bin);

void          SetLegend     (TLegend*      tl,
			     Size_t        tsize);

TGraphErrors* SetGraph      (Int_t         npoints,
			     Color_t       color,
			     Style_t       style);

void          DrawLatex     (Font_t        tfont,
			     Float_t       x,
			     Float_t       y,
			     Float_t       tsize,
			     Short_t       align,
			     const char*   text,
			     Bool_t        setndc = true);


//------------------------------------------------------------------------------
//
// doResolutionDxy
//
//------------------------------------------------------------------------------
void doResolutionDxy(TString name  = "Soft",
		     TString label = "soft muons",
		     Int_t   PU    = PU200)
{
  gInterpreter->ExecuteMacro("PaperStyle.C");

  if (doSavePdf || doSavePng) gSystem->mkdir(directory, kTRUE);

  muonType_name  = name;
  muonType_label = label;

  if (PU == noPU)
    {
      pu_name  = "noPU";
      pu_label = "no PU";
    }
  else
    {
      pu_name  = "PU200";
      pu_label = "200 PU";
    }

  TH1::SetDefaultSumw2();

  file = TFile::Open("rootfiles/DisplacedSUSY_" + pu_name + ".root");


  // Do the work
  //----------------------------------------------------------------------------
  mg_mean  = new TMultiGraph();
  mg_width = new TMultiGraph();

  resolution_legend = new TLegend(0.81, 0.5, 0.95, 0.91);

  SetLegend(resolution_legend, 0.025);

  for (Int_t i=0; i<nbins_vxy; i++) DrawResolution(i);


  // Draw mean
  //----------------------------------------------------------------------------
  TCanvas* c1 = new TCanvas("mean", "mean");

  c1->SetLeftMargin (0.165);
  c1->SetRightMargin(0.205);

  c1->SetGridx();
  c1->SetGridy();

  mg_mean->Draw("apz");

  mg_mean->SetMinimum(-0.005);
  mg_mean->SetMaximum(+0.005);

  mg_mean->GetXaxis()->SetTitle(muonType_label + " (" + pu_label + ") gen p_{T} [GeV]");
  mg_mean->GetYaxis()->SetTitle("");

  DrawLatex(42, 0.5, 0.95, 0.04, 21, "#Deltaq/p_{T} / (q/p_{T}) fit mean");

  mg_mean->GetXaxis()->SetTitleOffset(1.6);
  mg_mean->GetYaxis()->SetTitleOffset(1.8);

  resolution_legend->Draw("same");

  if (doSavePdf) c1->SaveAs(directory + "/resolution_vs_dxy_mean_" + muonType_name + "Muons_" + pu_name + ".pdf");
  if (doSavePng) c1->SaveAs(directory + "/resolution_vs_dxy_mean_" + muonType_name + "Muons_" + pu_name + ".png");


  // Draw width
  //----------------------------------------------------------------------------
  TCanvas* c2 = new TCanvas("width", "width");

  c2->SetLeftMargin (0.165);
  c2->SetRightMargin(0.205);

  c2->SetGridx();
  c2->SetGridy();

  mg_width->Draw("apz");

  mg_width->SetMinimum(0.000);
  mg_width->SetMaximum(0.025);

  mg_width->GetXaxis()->SetTitle(muonType_label + " (" + pu_label + ") gen p_{T} [GeV]");
  mg_width->GetYaxis()->SetTitle("");

  DrawLatex(42, 0.5, 0.95, 0.04, 21, "#Deltaq/p_{T} / (q/p_{T}) fit width");

  mg_width->GetXaxis()->SetTitleOffset(1.6);
  mg_width->GetYaxis()->SetTitleOffset(2.3);

  resolution_legend->Draw("same");

  if (doSavePdf) c2->SaveAs(directory + "/resolution_vs_dxy_width_" + muonType_name + "Muons_" + pu_name + ".pdf");
  if (doSavePng) c2->SaveAs(directory + "/resolution_vs_dxy_width_" + muonType_name + "Muons_" + pu_name + ".png");
}


//------------------------------------------------------------------------------
//
// Draw resolution
//
//------------------------------------------------------------------------------
void DrawResolution(Int_t vxy_bin)
{
  TString canvas_name = Form("resolution_vs_dxy_bin_%d_%sMuons_%s",
			     vxy_bin,
			     muonType_name.Data(),
			     pu_name.Data());

  TCanvas* canvas = new TCanvas(canvas_name, canvas_name);

  TH1F* h_resolution[nbins_pt];

  TGraphErrors* gr_mean  = SetGraph(nbins_pt, vxy_colors[vxy_bin], kFullCircle);
  TGraphErrors* gr_width = SetGraph(nbins_pt, vxy_colors[vxy_bin], kFullCircle);

  TLegend* legend = new TLegend(0.61, 0.6, 0.82, 0.89);

  SetLegend(legend, 0.03);

  Float_t ymax = 0;

  for (Int_t i=0; i<nbins_pt; i++) {

    TH2F* h2 = (TH2F*)file->Get(Form("muonAnalysis/%sMuons_pt_resolution_pt%d", muonType_name.Data(), i));

    TString vxy_suffix = (vxy_bin < 0) ? "all" : Form("%d", vxy_bin);

    TString hname = Form("%s_%s_vxy_%s", h2->GetName(), pu_name.Data(), vxy_suffix.Data());

    h_resolution[i] = (vxy_bin < 0) ? (TH1F*)h2->ProjectionY(hname) : (TH1F*)h2->ProjectionY(hname, vxy_bin, vxy_bin+1);

    TF1 *gfit = new TF1("gfit", "gaus", -0.035, 0.035);

    gfit->SetParameters(1,0,1);

    h_resolution[i]->Fit("gfit", "nqr");
    
    Float_t mean_value  = gfit->GetParameter(1);  // h_resolution[i]->GetMean());
    Float_t width_value = gfit->GetParameter(2);  // h_resolution[i]->GetRMS());

    Float_t mean_error  = gfit->GetParError(1);   // h_resolution[i]->GetMeanError());
    Float_t width_error = gfit->GetParError(2);   // h_resolution[i]->GetRMSError());

    // Get the mean and the width
    gr_mean ->SetPoint(i, 0.5*(pt_bins[i]+pt_bins[i+1]), mean_value);
    gr_width->SetPoint(i, 0.5*(pt_bins[i]+pt_bins[i+1]), width_value);

    gr_mean ->SetPointError(i, 0.5*(pt_bins[i+1]-pt_bins[i]), mean_error);
    gr_width->SetPointError(i, 0.5*(pt_bins[i+1]-pt_bins[i]), width_error);

    if (h_resolution[i]->GetMaximum() > ymax) ymax = h_resolution[i]->GetMaximum();

    h_resolution[i]->SetLineColor(pt_colors[i]);
    h_resolution[i]->SetLineWidth(2);

    TString option = (i == 0) ? "hist" : "hist,same";

    h_resolution[i]->Draw(option);

    legend->AddEntry(h_resolution[i], Form(" %.0f < p_{T} < %.0f GeV", pt_bins[i], pt_bins[i+1]), "l");
  }


  // Save the mean and the width
  mg_mean ->Add(gr_mean);
  mg_width->Add(gr_width);


  // Legend
  resolution_legend->AddEntry(gr_mean, Form("(%.3f,%.3f)", vxy_bins[vxy_bin], vxy_bins[vxy_bin+1]), "lp");


  // Cosmetics
  h_resolution[0]->SetMaximum(1.1 * ymax);
  h_resolution[0]->SetTitle("");
  h_resolution[0]->SetXTitle(muonType_label + " #Deltaq/p_{T} / (q/p_{T})");
  h_resolution[0]->SetYTitle("entries / bin");
  h_resolution[0]->GetXaxis()->SetTitleOffset(1.5);
  h_resolution[0]->GetYaxis()->SetTitleOffset(2.0);

  DrawLatex(42, 0.940, 0.945, 0.04, 31, pu_label);

  legend->Draw();

  canvas->GetFrame()->DrawClone();

  // Save
  if (doSavePdf) canvas->SaveAs(directory + "/" + canvas_name + ".pdf");
  if (doSavePng) canvas->SaveAs(directory + "/" + canvas_name + ".png");
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
