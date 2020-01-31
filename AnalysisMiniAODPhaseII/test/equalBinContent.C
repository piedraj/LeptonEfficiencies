//------------------------------------------------------------------------------
//
//  This macro looks for new bins that have as similar areas as possible. As
//  the distributions of interest (vxy and vz) are falling from left to right,
//  the bins are defined from the right, where the statistics is smaller.
//
//------------------------------------------------------------------------------


void equalBinContent(Int_t   nbins = 6,
		     TString hname = "pt",
		     TString fname = "rootfiles/DisplacedSUSY_CTau-1_PU200.root")
{
  printf("\n Finding %d bins with variable sizes for the histogram %s in %s\n",
	 nbins, hname.Data(), fname.Data());

  TFile* file = new TFile(fname);

  TH1F* h1 = (TH1F*)file->Get("muonAnalysis/" + hname);

  printf("\n %s has %d bins and the bin width is %f\n",
	 hname.Data(), h1->GetNbinsX(), h1->GetBinWidth(0));

  TCanvas* c1 = new TCanvas("c1", "c1");

  h1->Draw();


  // Do the work
  //----------------------------------------------------------------------------
  Double_t integral = h1->Integral(-1, -1);  // Includes overflow

  Double_t targetBinContent = integral / nbins;

  Double_t partialContent = 0;

  Int_t usedBins = 0;

  printf("\n integral = %.1f; target bin content = %.1f\n\n",
	 integral, targetBinContent);
  
  Int_t i_begin = h1->GetNbinsX()+1;
  Int_t i_end   = 0;

  Double_t xbins[nbins+1];

  xbins[0]     = h1->GetBinLowEdge(1);
  xbins[nbins] = h1->GetBinLowEdge(h1->GetNbinsX()+1);

  printf(" bin edge %2d = %8.4f\n", nbins, xbins[nbins]);

  for (Int_t i=h1->GetNbinsX()+1; i>=0; i--)  // Includes overflow
    {
      Double_t binContent = h1->GetBinContent(i);
      
      if (binContent <= 0)
	{
	  i_end = i;
	}
      else if (partialContent < targetBinContent)
	{
	  partialContent += binContent;
	  
	  i_end = i;
	}
      else
	{
	  partialContent = 0;

	  ++usedBins;

	  integral -= h1->Integral(i_end, i_begin);

	  targetBinContent = integral / (nbins - usedBins);

	  xbins[nbins-usedBins] = h1->GetBinLowEdge(i+1);

	  printf(" bin edge %2d = %8.4f; integral = %.1f; target = %.1f\n",
		 nbins-usedBins, h1->GetBinLowEdge(i+1), h1->Integral(i_end, i_begin), targetBinContent);

	  i_begin = i;
	}
    }

  printf(" bin edge %2d = %8.4f\n", 0, xbins[0]);


  // Test the work
  //----------------------------------------------------------------------------
  printf("\n The new bin edges are:\n\n const Double_t %s_bins[nbins_%s+1] = {",
	 hname.Data(), hname.Data());

  for (Int_t i=0; i<=nbins; i++) {

    printf("%.4f", xbins[i]);

    (i < nbins) ? printf(", ") : printf("};\n");
  }

  printf("\n");

  TH1F* h1_rebin = (TH1F*)h1->Rebin(nbins, "h1_rebin", xbins);

  TCanvas* c2 = new TCanvas("c2", "c2");

  h1_rebin->Draw();

  // Cross-check the numbers
  //----------------------------------------------------------------------------
  printf(" Let's double check the numbers in the histogram with new bins\n\n");

  for (Int_t i=h1_rebin->GetNbinsX(); i>0; i--) {

    printf(" bin %2d; integral = %.1f\n", i, h1_rebin->GetBinContent(i));
  }

  printf("\n The new binning should be copied in LeptonEfficiencies/AnalysisMiniAODPhaseII/plugins/MuonAnalyzer.h\n");
  printf("\n Once the binning has been updated MuonAnalyzer should be recompiled and run again\n\n");
}
