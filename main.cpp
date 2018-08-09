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

  std::unique_ptr <TFile> fIn {TFile::Open(argv[1], "read")};
  std::unique_ptr <TH1F> histo {(TH1F*) (fIn->Get(argv[2]))};
    
  Centrality::BordersFinder bf;
  bf.SetHisto(*histo);
  bf.SetRanges( 10,0,100 );   // number of bins, min, max value
//   bf.SetRanges( {0,10,30,60,100} );  // centrality bins borders with array
  bf.IsSpectator(true);  // true if impact parameter b correlated with estimator (spectators eneggy), false - anticorrelated (multiplicity of produced particles) 

  bf.FindBorders();
  
  std::string outfilename = "test.root";
  bf.SaveBorders(outfilename);
  
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "elapsed time: " << elapsed_seconds.count() << " s\n";
  return 0;
}
