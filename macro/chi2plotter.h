//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Mar  7 16:13:14 2019 by ROOT version 6.12/06
// from TTree test_tree/tree
// found on file: merged.root
//////////////////////////////////////////////////////////

#ifndef chi2plotter_h
#define chi2plotter_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TH1.h"

class chi2plotter {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   TH1F            *histo;
   Double_t         f;
   Double_t         mu;
   Double_t         k;
   Double_t         chi2;
   Double_t         sigma;

   // List of branches
   TBranch        *b_histo;   //!
   TBranch        *b_f;   //!
   TBranch        *b_mu;   //!
   TBranch        *b_k;   //!
   TBranch        *b_chi2;   //!
   TBranch        *b_chi2err;
   TBranch        *b_sigma;   //!

   chi2plotter(TTree *tree=0);
   virtual ~chi2plotter();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef chi2plotter_cxx
chi2plotter::chi2plotter(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("merged.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../install/bin/fit_merged.root");
      }
      f->GetObject("test_tree",tree);

   }
   Init(tree);
}

chi2plotter::~chi2plotter()
{
   if (!fChain) return;
//   delete fChain->GetCurrentFile();
}

Int_t chi2plotter::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t chi2plotter::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void chi2plotter::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   histo = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("histo", &histo, &b_histo);
   fChain->SetBranchAddress("f", &f, &b_f);
   fChain->SetBranchAddress("mu", &mu, &b_mu);
   fChain->SetBranchAddress("k", &k, &b_k);
   fChain->SetBranchAddress("chi2", &chi2, &b_chi2);
   fChain->SetBranchAddress("sigma", &sigma, &b_sigma);
   Notify();
}

Bool_t chi2plotter::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void chi2plotter::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t chi2plotter::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef chi2plotter_cxx
