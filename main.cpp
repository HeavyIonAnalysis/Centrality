#include "Getter.h"
#include "BordersFinder.h"
#include "Converter1D.h"

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
#include "TH2.h"


int main(int argc, char **argv) {
  auto start = std::chrono::system_clock::now();
  ROOT::EnableImplicitMT(2);

  std::unique_ptr <TFile> fIn {TFile::Open(argv[1], "read")};
  std::unique_ptr <TH1F> histo {(TH1F*) (fIn->Get(argv[2]))};

  std::unique_ptr <TH2F> histo2d {(TH2F*) (fIn->Get("hMEcorr"))};
  
  Centrality::Converter1D conv2d;
  conv2d.SetHisto2D( std::move(*histo2d) );
  conv2d.Convert();
  

  if (false)
  {
  
  Centrality::BordersFinder bf;
  bf.SetHisto( *histo);
  bf.SetRanges( 10,0,100 );   // number of bins, min, max value
//   bf.SetRanges( {0,10,30,60,100} );  // centrality bins borders with array
  bf.IsSpectator(true);  // true if impact parameter b correlated with estimator (spectators eneggy), false - anticorrelated (multiplicity of produced particles) 

  bf.FindBorders();
  
  std::string outfilename = "test.root";
  bf.SaveBorders(outfilename);
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "elapsed time: " << elapsed_seconds.count() << " s\n";
  return 0;
}
