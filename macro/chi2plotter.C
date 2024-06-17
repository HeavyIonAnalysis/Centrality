#define chi2plotter_cxx
#include "chi2plotter.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <string>
#include <iostream>

void run()
{
        string filename;
        std::cout << "Enter the file name" << std::endl;
        std::cin >> filename;

        TFile *f = TFile::Open(filename.c_str());
        TTree *tree = (TTree*) f -> Get("test_tree");
        chi2plotter analize(tree);
        analize.Loop();
}


void chi2plotter::Loop()
{
gStyle -> SetOptStat(0000);
//   In a ROOT session, you can do:
//      root> .L chi2plotter.C
//      root> chi2plotter t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

	float Min_Chi2 = 999999.; // a very large number
	float opt_f;
	float opt_k;
	float opt_mu;

	TH2F *histo_chi2 = new TH2F("histo_chi2", "#chi^{2} vs f and k", 101, -0.01, 1.01, 31, -0.5, 30.5);
	histo_chi2 -> GetXaxis() -> SetTitle("f"); 
	histo_chi2 -> GetYaxis() -> SetTitle("k");
	histo_chi2 -> GetXaxis() -> CenterTitle();
	histo_chi2 -> GetYaxis() -> CenterTitle();

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
	 std::cout << "nentries = " << nentries << "\n";
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

	std::cout << "entry # " << jentry << "\t" << "f = " << f << ", k = " << k << ", mu = " << mu << ", chi2 = " << chi2 << std::endl;

	if(chi2 < Min_Chi2)
		{
		Min_Chi2 = chi2;
		opt_f = f;
		opt_k = k;
		opt_mu = mu;
		}
	histo_chi2 -> Fill(f, k, chi2);
	
   }
	std::cout << "f = " << opt_f << ", k = " << opt_k << ", mu = " << opt_mu << ", chi2 = " << Min_Chi2 << std::endl;

	TCanvas *c1 = new TCanvas("c1", "c1", 1200, 800);
	c1 -> cd();
	histo_chi2 -> Draw("colz");

/*	TFile histosave("histogram.root", "recreate");
	histo_chi2 -> Write();
	histosave.Close();*/

}
