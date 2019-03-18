// Builds multiplicity distribution from events generated via Glauber model

#define MultBuilder_cxx
#include "MultBuilder.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <string>
#include <iostream>
#include <TRandom3.h>
using std::cout;
using std::cin;
using std::endl;

void run()
{
        string filename;
        float mu;
        int k;
        float f;
        cout << "Enter the file name" << std::endl;
        cin >> filename;
        cout << "Enter mu, k and f (with spaces between)" << endl;
        cin >> mu >> k >> f;
        
        
        TFile *file = TFile::Open(filename.c_str());
        TTree *tree = (TTree*) file -> Get("nt_Au_Au");
        MultBuilder analize(tree);
        analize.Loop(mu, k, f);
}

int Na(int Npart, int Ncoll, float f)
{
    return f*Npart + (1-f)*Ncoll;
}

float NBD(float mu, int k, int n)
{
    return 1.*TMath::Factorial(n+k-1)/TMath::Factorial(n)/TMath::Factorial(k-1)*TMath::Power(mu/k, (double)n)/TMath::Power(mu/k+1., (double)n + (double)k);
}

int NBD_n(float *cumP, float x)
{
    int n=0;
    while(x>cumP[n])
        n++;
    
    return n;
}
    

void MultBuilder::Loop(float mu, int k, float f)
{
//   In a ROOT session, you can do:
//      root> .L MultBuilder.C
//      root> MultBuilder t
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
   
   TRandom3 *rand = new TRandom3();
   TH1F *histo = new TH1F("hMreco", "Mreco", 2000, 0, 1000);
   
   int nmax = 0;
   do
       nmax++;
   while(NBD(mu, k, nmax)>1e-10);
   
   float P;
   float *cumP = new float[nmax];
   
   cumP[0] = NBD(mu, k, 0);
   for(int n=1; n<nmax; n++)
   {
       P = NBD(mu, k, n);
       cumP[n] = cumP[n-1] + P;
   }
   

   Long64_t nentries = fChain->GetEntriesFast();
   int Nancestors;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      
      if(jentry%100000 == 0)
          cout << jentry << endl;
      
      Nancestors = Na(Npart, Ncoll, f);
      int multiplicity = 0;
      for(int i=0; i<Nancestors; i++)
          multiplicity += NBD_n(cumP, rand -> Uniform(1));
      histo -> Fill(multiplicity);

   }
   
   histo -> Draw();
   
   TFile histosave("test_input_00.root", "recreate");
   histo -> Write();
   histosave.Close();
   
   
}
