#include "Getter.h"
#include "BordersFinder.h"

#include <iostream>
#include <vector>
#include <random>
#include <array>
#include <iomanip>
#include <chrono>

#include <TSystem.h>
#include <TROOT.h>
#include "TTree.h"
#include "TFile.h"


int main(int argc, char **argv) {
  auto start = std::chrono::system_clock::now();
  ROOT::EnableImplicitMT(2);

  TFile *fIn = TFile::Open(argv[1], "read");
//   std::shared_ptr <TH1F> histo = (TH1F*) (fIn->Get(argv[2]));
  TH1F* histo = (TH1F*) (fIn->Get(argv[2]));
  
  Centrality::BordersFinder bf;
  bf.SetHisto(*histo);
  bf.SetRanges( 10,0,100 );
//   bf.SetRanges( {0,10,30,60,100} );

  bf.FindBorders();
  bf.SaveBorders("test.root");
  
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "elapsed time: " << elapsed_seconds.count() << " s\n";
  return 0;
}
