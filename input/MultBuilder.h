//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Mar  7 17:50:17 2019 by ROOT version 6.12/06
// from TTree nt_Au_Au/Au + Au (x-sect = 30.0 mb) str gmc-AuAu-snn30.0-md0.4-nd-1.0-rc1-smax99.0
// found on file: glauber.Au.Au.30mb.1M.root
//////////////////////////////////////////////////////////

#ifndef MultBuilder_h
#define MultBuilder_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class MultBuilder {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         Npart;
   Float_t         Ncoll;

   // List of branches
   TBranch        *b_Npart;   //!
   TBranch        *b_Ncoll;   //!

   MultBuilder(TTree *tree=0);
   virtual ~MultBuilder();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(float mu, int k, float f);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef MultBuilder_cxx
MultBuilder::MultBuilder(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (/*tree == 0*/ false) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("glauber.Au.Au.30mb.1M.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("glauber.Au.Au.30mb.1M.root");
      }
      f->GetObject("nt_Au_Au",tree);

   }
   Init(tree);
}

MultBuilder::~MultBuilder()
{
   if (!fChain) return;
   //delete fChain->GetCurrentFile();
}

Int_t MultBuilder::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MultBuilder::LoadTree(Long64_t entry)
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

void MultBuilder::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Npart", &Npart, &b_Npart);
   fChain->SetBranchAddress("Ncoll", &Ncoll, &b_Ncoll);
}

Bool_t MultBuilder::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MultBuilder::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MultBuilder::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MultBuilder_cxx
