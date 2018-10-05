//------------------------------------------------------------------------------
//
// Compare displaced muons resolution for samples with no PU and 200 PU
//
// root -l -b -q 'doResolution.C+(-1)'
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
enum           {noPU, PU200};

const Int_t    nbins_pt  =  6;
const Int_t    nbins_vxy = 14;

const Double_t pt_bins[nbins_pt+1] = {10, 60, 90, 130, 170, 250, 500};

const Double_t vxy_bins[nbins_vxy+1] = {0.0000, 0.0072, 0.0160, 0.0260, 0.0384, 0.0526, 0.0702, 0.0916, 0.1194, 0.1576, 0.2168, 0.3292, 1.0, 2.0, 3.0000};  // 1.0 and 2.0 added by hand

Color_t        ptcolor[nbins_pt] = {kRed-10, kRed-9, kRed-7, kRed-4, kRed, kRed+1};

Bool_t         doSavePdf = true;
Bool_t         doSavePng = true;

Bool_t         draw_sta   = false;
Bool_t         draw_trk   = false;
Bool_t         draw_glb   = true;
Bool_t         draw_tight = true;
Bool_t         draw_soft  = true;
Bool_t         draw_fits  = false;

TString        directory = "displaced-muons";

TFile*         file1 = NULL;
TFile*         file2 = NULL;

TLegend*       resolution_legend = NULL;

TMultiGraph*   mg_mean  = NULL;
TMultiGraph*   mg_width = NULL;

Int_t          vxy_bin;


// Member functions
//------------------------------------------------------------------------------
void          DrawResolution(TString       muonType,
			     TString       xtitle,
			     Int_t         PU,
			     Color_t       color);

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

void          PrintContent  (TGraphErrors* gr,
			     TString       variable,
			     TString       title);


//------------------------------------------------------------------------------
//
// doResolution
//
//------------------------------------------------------------------------------
void doResolution(Int_t vxy = -1)
{
  if (vxy >= nbins_vxy) return;

  vxy_bin = vxy;

  gInterpreter->ExecuteMacro("PaperStyle.C");

  if (doSavePdf) gSystem->mkdir(directory, kTRUE);

  TH1::SetDefaultSumw2();


  // Input files
  //----------------------------------------------------------------------------
  file1 = TFile::Open("rootfiles/DisplacedSUSY_CTau-1_PU200.root");
  file2 = TFile::Open("rootfiles/DisplacedSUSY_CTau-1_noPU.root");


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

  mg_mean->SetMinimum(-0.005);
  mg_mean->SetMaximum(+0.005);

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

  mg_width->SetMinimum(0.000);
  mg_width->SetMaximum(0.025);

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
  TFile* file = (PU == noPU) ? file2 : file1;
  
  TString pu_string = (PU == noPU) ? "noPU" : "PU200";

  TString pu_label = (PU == noPU) ? "no PU" : "200 PU";

  Style_t pu_style = (PU == noPU) ? kOpenCircle : kFullCircle;

  TCanvas* canvas = new TCanvas(xtitle + " resolution (" + pu_label + ")",
				xtitle + " resolution (" + pu_label + ")");
  
  // One histogram per pt bin
  TH1F* h_resolution[nbins_pt];

  // Graphs that will store the mean and width of the resolution
  TGraphErrors* gr_mean  = SetGraph(nbins_pt, color, pu_style);
  TGraphErrors* gr_width = SetGraph(nbins_pt, color, pu_style);

  // Legend
  TLegend* legend = new TLegend(0.61, 0.6, 0.82, 0.89);

  SetLegend(legend, 0.03);

  // Vertical maximum
  Float_t ymax = 0;

  printf("\n");

  // Loop
  for (Int_t i=0; i<nbins_pt; i++) {

    TH2F* h2 = (TH2F*)file->Get(Form("muonAnalysis/%sMuons_pt_resolution_pt%d", muonType.Data(), i));

    // TH1D* ProjectionY(const char* name = "_py", Int_t firstxbin = 0, Int_t lastxbin = -1, Option_t* option = "")

    TString vxy_suffix = (vxy_bin < 0) ? "all" : Form("%d", vxy_bin);

    TString hname = Form("%s_%s_vxy_%s", h2->GetName(), pu_string.Data(), vxy_suffix.Data());

    h_resolution[i] = (vxy_bin < 0) ? (TH1F*)h2->ProjectionY(hname) : (TH1F*)h2->ProjectionY(hname, vxy_bin, vxy_bin+1);

    // Fit
    TF1 *gfit = new TF1("gfit", "gaus", -0.035, 0.035);

    gfit->SetParameters(1,0,1);

    h_resolution[i]->Fit("gfit", "nqr");
    
    if (draw_fits) printf(" %sMuons_res_%d (%s) Chisquare/NDF = %5.2f;  Prob = %5.2f%s\n",
			  muonType.Data(), i, pu_label.Data(), gfit->GetChisquare()/gfit->GetNDF(), 1e2*gfit->GetProb(), "%");

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

    h_resolution[i]->SetLineColor(ptcolor[i]);
    h_resolution[i]->SetLineWidth(2);

    TString option = (i == 0) ? "hist" : "hist,same";

    h_resolution[i]->Draw(option);

    if (draw_fits) gfit->Draw("same");

    legend->AddEntry(h_resolution[i], Form(" %.0f < p_{T} < %.0f GeV", pt_bins[i], pt_bins[i+1]), "l");
  }

  if (draw_fits) printf("\n");


  // Save the mean and the width
  mg_mean ->Add(gr_mean);
  mg_width->Add(gr_width);

  // Print the mean and the width
  PrintContent(gr_mean,  "mean",  muonType + "Muons_" + pu_string);
  PrintContent(gr_width, "width", muonType + "Muons_" + pu_string);


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


//------------------------------------------------------------------------------
// PrintContent
//------------------------------------------------------------------------------
void PrintContent(TGraphErrors* gr,
		  TString       variable,
		  TString       title)
{
  TString print_vxy_bin = "";

  if (vxy_bin >= 0) print_vxy_bin = Form(" (%.4f < dxy < %.4f cm)", vxy_bins[vxy_bin], vxy_bins[vxy_bin+1]);

  printf(" Resolution %s for %s%s\n\n", variable.Data(), title.Data(), print_vxy_bin.Data());

  for (Int_t i=0; i<gr->GetN(); i++) {

    printf(" [ %3.0f < ptgen < %3.0f]  %s = %9.6f +- %8.6f\n",
	   gr->GetX()[i] - gr->GetErrorX(i),
	   gr->GetX()[i] + gr->GetErrorX(i),
	   variable.Data(),
	   gr->GetY()[i], gr->GetErrorY(i));
  }

  printf("\n");
}
