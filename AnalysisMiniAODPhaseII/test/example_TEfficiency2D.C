//------------------------------------------------------------------------------
//
//  Discussion in the ROOT forum
//
//  https://root-forum.cern.ch/t/th2-tefficiency-example/30578
//
//------------------------------------------------------------------------------
#include "TCanvas.h"
#include "TEfficiency.h"
#include "TF2.h"
#include "TH2.h"
#include "TMath.h"
#include "TRandom.h"


void example_TEfficiency2D(bool th1SetDefaultSumw2 = false,
			   bool th2SetDefaultSumw2 = false)
{
  if (th1SetDefaultSumw2 || th2SetDefaultSumw2) printf("\n");

  if (th1SetDefaultSumw2) {TH1::SetDefaultSumw2(); printf(" Executing TH1::SetDefaultSumw2();\n");}
  if (th2SetDefaultSumw2) {TH2::SetDefaultSumw2(); printf(" Executing TH2::SetDefaultSumw2();\n");}

  if (th1SetDefaultSumw2 || th2SetDefaultSumw2) printf("\n");

  double xbins[] = {0, 1, 3, 7, 15};
  double ybins[] = {0, 5, 20};

  auto hpass = new TH2D("pass", "passed events", 4, xbins, 2, ybins);
  auto htot  = new TH2D("tot",  "total events",  4, xbins, 2, ybins);

  TF2 f2("f2", "xygaus", 0, 100, 0, 100);

  f2.SetParameters(1, 0, 5, 0, 4);

  double x;
  double y;

  for(int i=0; i<100; ++i)
  {
    f2.GetRandom2(x,y); 

    auto t = gRandom->Rndm();

    if (t < .9) hpass->Fill(x,y);

    htot->Fill(x,y); 
  }

  
  // Get the same efficiencies in a TH1
  //----------------------------------------------------------------------------
  auto h1_pass = new TH1D("h1_pass", "1D passed events", 8, 0, 8);
  auto h1_tot  = new TH1D("h1_tot",  "1D total  events", 8, 0, 8);
  auto h1_eff  = new TH1D("h1_eff",  "1D efficiency",    8, 0, 8);

  int k=0;
  
  for (int i=1; i<=hpass->GetNbinsX(); i++) {
    for (int j=1; j<=hpass->GetNbinsY(); j++) {

      k++;

      h1_pass->SetBinContent(k, hpass->GetBinContent(i,j));
      h1_pass->SetBinError  (k, hpass->GetBinError  (i,j));

      h1_tot->SetBinContent(k, htot->GetBinContent(i,j));
      h1_tot->SetBinError  (k, htot->GetBinError  (i,j));
    }
  }

  h1_eff->Divide(h1_pass, h1_tot, 1, 1, "B");


  // Draw pass and total
  //----------------------------------------------------------------------------
  auto c1 = new TCanvas();

  c1->Divide(1,2);

  c1->cd(1);

  hpass->Draw("colz");

  c1->cd(2);

  htot->Draw("colz");


  // Get the 2D efficiency and draw it
  //----------------------------------------------------------------------------
  TEfficiency* pEff = new TEfficiency(*hpass,*htot);

  pEff->SetStatisticOption(TEfficiency::kBBayesian);

  pEff->SetConfidenceLevel(0.68);

  auto c2 = new TCanvas();

  pEff->Draw("ey");

  c2->Update();  // Needed to get the painted histogram

  auto heff = pEff->GetPaintedHistogram();

  heff->SetMinimum(0.0);

  c2->Update();

  c2->Draw();


  // Compare the results
  //----------------------------------------------------------------------------
  int verbose = 1;

  if (verbose > 0) {

    k = 0;

    printf("\n");

    for (int i=1; i<=hpass->GetNbinsX(); i++) {
      for (int j=1; j<=hpass->GetNbinsY(); j++) {

	k++;

	if (verbose > 1) {
	  printf(" bin %d,%d | pass TH2 %3.0f(%2.0f)   TH1 %3.0f(%2.0f) | total TH2 %3.0f(%2.0f)   TH1 %3.0f(%2.0f) | eff TH2 %.3f +- %.3f   TH1 %.3f +- %.3f\n",
		 i, j,
		 hpass->GetBinContent(i,j), hpass->GetBinError(i,j), h1_pass->GetBinContent(k), h1_pass->GetBinError(k),
		 htot ->GetBinContent(i,j), htot ->GetBinError(i,j), h1_tot ->GetBinContent(k), h1_tot ->GetBinError(k),
		 heff ->GetBinContent(i,j), heff ->GetBinError(i,j), h1_eff ->GetBinContent(k), h1_eff ->GetBinError(k));
	} else {
	  printf(" bin %d,%d | TEff %.3f - %.3f + %.3f   TH2 from TEff %.3f +- %.3f   (sanity check sqrt(%.3f) is %.3f)   TH1 %.3f +- %.3f\n",
                 i, j,
                 pEff->GetEfficiency(heff->GetBin(i,j)),
                 pEff->GetEfficiencyErrorLow(heff->GetBin(i,j)),
                 pEff->GetEfficiencyErrorUp(heff->GetBin(i,j)),
                 heff->GetBinContent(i,j), heff->GetBinError(i,j),
		 heff->GetBinContent(i,j),
                 TMath::Sqrt(heff->GetBinContent(i,j)),
		 h1_eff->GetBinContent(k),
		 h1_eff->GetBinError(k)
		 );
	}
      }
    }

    printf("\n");
  }
}
