//------------------------------------------------------------------------------
//
// Produce 2D efficiencies
//
// root -l -b -q 'doEfficiencies2D.C+("Tight", "CTau-1000_PU200")'
//
//------------------------------------------------------------------------------
#include "TCanvas.h"
#include "TFile.h"
#include "TFrame.h"
#include "TH2F.h"
#include "TInterpreter.h"
#include "TLatex.h"
#include "TPaletteAxis.h"
#include "TString.h"
#include "TSystem.h"

#include <fstream>


// Data members
//------------------------------------------------------------------------------
Float_t        bigLabelSize = 0.04;

Bool_t         printEfficiencies = false;

TString        directory = "displaced-muons";


// Member functions
//------------------------------------------------------------------------------
void AxisFonts    (TAxis*      axis,
		   TString     title);

void TH2FAxisFonts(TH2F*       h,
		   TString     coordinate,
		   TString     title);

void DrawTLatex   (Font_t      tfont,
		   Double_t    x,
		   Double_t    y,
		   Double_t    tsize,
		   Short_t     align,
		   const char* text,
		   Bool_t      setndc = true);


//------------------------------------------------------------------------------
//
// doEfficiencies2D
//
//------------------------------------------------------------------------------
void doEfficiencies2D(TString muontype = "Soft",
		      TString filename = "CTau-1_PU200")
{
  gInterpreter->ExecuteMacro("PaperStyle.C");

  gSystem->mkdir(directory + "/tcl", kTRUE);
  
  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();

  TFile* inputfile = TFile::Open("rootfiles/DisplacedSUSY_" + filename + ".root");

  TH2F* hnum = (TH2F*)inputfile->Get("muonAnalysis/" + muontype + "Muons_vxy_vz");
  TH2F* hden = (TH2F*)inputfile->Get("muonAnalysis/GenMuons_vxy_vz");

  hnum->Sumw2();  // TH2::SetDefaultSumw2() doesn't affect already existing histograms

  TString hname = "efficiency_vxy_vz_muon" + muontype + "Id_" + filename;

  TH2F* h = (TH2F*)hnum->Clone(hname);

  h->Divide(hnum, hden, 1, 1, "B");

  h->SetDirectory(0);
  h->SetTitle("");


  // Draw
  //----------------------------------------------------------------------------
  TCanvas* canvas = new TCanvas(hname, hname);

  canvas->SetLogx();
  canvas->SetLogy();

  canvas->SetLeftMargin (0.9 * canvas->GetLeftMargin());
  canvas->SetRightMargin(3.5 * canvas->GetRightMargin());
  canvas->SetTopMargin  (1.2 * canvas->GetTopMargin());

  TH2FAxisFonts(h, "x", "vxy [cm]");
  TH2FAxisFonts(h, "y", "vz [cm]");

  h->Draw("colz");
  
  DrawTLatex(42, 0.940, 0.976, bigLabelSize, 33, filename + " " + muontype + " muons efficiency");

  TAxis* xaxis = h->GetXaxis();
  TAxis* yaxis = h->GetYaxis();


  // Save tcl
  //----------------------------------------------------------------------------
  std::ofstream efficiency_tcl;

  efficiency_tcl.open(Form("%s/tcl/%s_error.tcl", directory.Data(), hname.Data()), std::ofstream::out);

  efficiency_tcl << Form("# %s %s muons efficiency for d0 [mm] and dz [mm]\n\n", filename.Data(), muontype.Data()); 

  efficiency_tcl << "set EfficiencyFormula {\n";

  for (Int_t i=1; i<=h->GetNbinsX(); i++) {
    for (Int_t j=1; j<=h->GetNbinsX(); j++) {

      efficiency_tcl << Form(" (abs(d0) > %6.3f && abs(d0) <= %8.3f) * (abs(dz) > %7.3f && abs(dz) < %9.3f) * %.3f   (error = %.3f)\n",
			     10*xaxis->GetBinLowEdge(i), 10*xaxis->GetBinLowEdge(i+1),
			     10*yaxis->GetBinLowEdge(j), 10*yaxis->GetBinLowEdge(j+1),
			     h->GetBinContent(i,j), h->GetBinError(i,j));
    }
  }

  efficiency_tcl << "}\n";
  
  efficiency_tcl.close();


  // Print efficiencies
  //----------------------------------------------------------------------------
  if (printEfficiencies) {

    Double_t hmin = h->GetMinimum();
    Double_t hmax = h->GetMaximum();

    for (Int_t i=1; i<=h->GetNbinsX(); i++) {
      for (Int_t j=1; j<=h->GetNbinsY(); j++) {

	Double_t value = h->GetBinContent(i,j);
      
	Double_t ypos = yaxis->GetBinCenter(j);
	Double_t xpos = xaxis->GetBinCenter(i);
      
	TLatex* latex = new TLatex(xpos, ypos, Form("%.2f", value));

	latex->SetTextAlign(   22);
	latex->SetTextFont (   42);
	latex->SetTextSize (0.027);

	if (value < hmin + 0.3*(hmax - hmin)) latex->SetTextColor(kWhite);
	
	latex->Draw();
      }
    }
  }


  // Set the palette font
  //----------------------------------------------------------------------------
  canvas->Update();

  TPaletteAxis* palette = (TPaletteAxis*)h->GetListOfFunctions()->FindObject("palette");

  palette->SetLabelFont(42);


  // Save the plot
  //----------------------------------------------------------------------------
  canvas->Update();
  
  canvas->Modified();

  canvas->GetFrame()->DrawClone();

  canvas->SaveAs(directory + "/" + hname + ".pdf");
  canvas->SaveAs(directory + "/" + hname + ".png");
}


//------------------------------------------------------------------------------
// AxisFonts
//------------------------------------------------------------------------------
void AxisFonts(TAxis*  axis,
	       TString title)
{
  axis->SetLabelFont  (   42);
  axis->SetLabelOffset(0.015);
  axis->SetNdivisions (  505);
  axis->SetTitleFont  (   42);
  axis->SetTitleOffset(  1.5);
  axis->SetLabelSize  (bigLabelSize);
  axis->SetTitleSize  (bigLabelSize);

  axis->SetTitle(title);
}


//------------------------------------------------------------------------------
// TH2FAxisFonts
//------------------------------------------------------------------------------
void TH2FAxisFonts(TH2F*   h,
		   TString coordinate,
		   TString title)
{
  TAxis* axis = NULL;

  if (coordinate.Contains("x")) axis = h->GetXaxis();
  if (coordinate.Contains("y")) axis = h->GetYaxis();

  AxisFonts(axis, title);
}


//------------------------------------------------------------------------------
// DrawTLatex
//------------------------------------------------------------------------------
void DrawTLatex(Font_t      tfont,
		Double_t    x,
		Double_t    y,
		Double_t    tsize,
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
