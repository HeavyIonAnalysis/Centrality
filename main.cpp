#include "Getter.h"
#include "BordersFinder.h"
#include "BordersFinder2D.h"

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
  
  Centrality::BordersFinder2D conv2d;
  conv2d.SetHisto2D( std::move(*histo2d) );
  
  std::unique_ptr<TH1F> h1d = conv2d.Convert();

  conv2d.SetHisto(*h1d);
  conv2d.SetRanges( 10,0,100 );   // number of bins, min, max value
//   bf.SetRanges( {0,10,30,60,100} );  // centrality bins borders with array
  conv2d.IsSpectator(true);  // true if impact parameter b correlated with estimator (spectators eneggy), false - anticorrelated (multiplicity of produced particles) 

  conv2d.FindBorders();
  
  std::string outfilename = "test.root";
  conv2d.SaveBorders2D(outfilename);


//   Centrality::BordersFinder bf;
//   bf.SetHisto(*h1d);
//   bf.SetRanges( 10,0,100 );   // number of bins, min, max value
// //   bf.SetRanges( {0,10,30,60,100} );  // centrality bins borders with array
//   bf.IsSpectator(true);  // true if impact parameter b correlated with estimator (spectators eneggy), false - anticorrelated (multiplicity of produced particles) 
// 
//   bf.FindBorders();
//   
//   std::string outfilename = "test.root";
//   bf.SaveBorders(outfilename);
  
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "elapsed time: " << elapsed_seconds.count() << " s\n";
  return 0;
}
