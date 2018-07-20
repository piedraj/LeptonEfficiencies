//------------------------------------------------------------------------------
//
//  This macro looks for new bins that have as similar areas as possible. As
//  the distributions of interest (vxy and vz) are falling from left to right,
//  the bins are defined from the right, where the statistics is smaller.
//
//------------------------------------------------------------------------------
const Int_t nbins = 10;

void equalBinContent(TString hname = "muonAnalysis/GenMuons_vxy",
		     TString fname = "MyMuonPlots.root")
{
  printf("\n Finding %d bins with variable sizes for %s in %s\n",
	 nbins, hname.Data(), fname.Data());

  TFile* file = new TFile(fname);

  TH1F* h1 = (TH1F*)file->Get(hname);

  TCanvas* c1 = new TCanvas("c1", "c1");

  h1->Draw();


  // Do the work
  //----------------------------------------------------------------------------
  Float_t integral = h1->Integral(-1, -1);

  Float_t binContent = integral / nbins;

  Float_t partialContent = 0;

  Int_t usedBins = 0;

  printf("\n integral = %.1f; nbins = %d; target = %.1f\n\n",
	 integral, nbins, binContent);
  
  Int_t i_begin = h1->GetNbinsX()+1;
  Int_t i_end   = 0;

  Double_t xbins[nbins+1];

  for (Int_t i=h1->GetNbinsX()+1; i>=0; i--)
    {
      if (partialContent < binContent)
	{
	  partialContent += h1->GetBinContent(i);
	  
	  i_end = i;
	}
      else
	{
	  partialContent = 0;

	  ++usedBins;

	  integral -= h1->Integral(i_end, i_begin);

	  binContent = integral / (nbins - usedBins);

	  xbins[nbins-usedBins] = h1->GetBinLowEdge(i+1);

	  printf(" bin edge = %f; integral = %.1f; target = %.1f\n",
		 h1->GetBinLowEdge(i+1), h1->Integral(i_end, i_begin), binContent);

	  i_begin = i;
	}
    }


  // Test the work
  //----------------------------------------------------------------------------
  xbins[0]     = h1->GetBinLowEdge(1);
  xbins[nbins] = h1->GetBinLowEdge(h1->GetNbinsX()+1);

  printf("\n The new bin edges are:\n\n");

  for (Int_t i=0; i<=nbins; i++) printf(" %f\n", xbins[i]);

  printf("\n");

  TH1F* h1_rebin = (TH1F*)h1->Rebin(nbins, "h1_rebin", xbins);

  TCanvas* c2 = new TCanvas("c2", "c2");

  h1_rebin->Draw();
}
