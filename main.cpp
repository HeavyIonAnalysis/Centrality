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
  
  if (argc < 3)
  {
    std::cout << "Not enough arguments! Please use:" << std::endl;
    std::cout << "   ./main filename histoname is2d" << std::endl;
    return -1;
  }
  
  std::unique_ptr <TFile> fIn {TFile::Open(argv[1], "read")};
  std::string outfilename = "test.root";
  if ( strcmp( argv[3], "false") == 0 )
  {
    std::unique_ptr <TH1F> histo {(TH1F*) (fIn->Get(argv[2]))};
    Centrality::BordersFinder bf;
    bf.SetHisto(*histo);
    bf.SetRanges( 20,0,100 );   // number of bins, min, max value
    //   bf.SetRanges( {0,10,30,60,100} );  // centrality bins borders with array
    bf.IsSpectator(true);  // true if impact parameter b correlated with estimator (spectators eneggy), 
    // false - anticorrelated (multiplicity of produced particles) 
    
    bf.FindBorders();
    bf.SaveBorders(outfilename, "centr_getter_1d");
  }
  else if ( strcmp( argv[3], "true") == 0 )
  {
    std::unique_ptr <TH2F> histo2d {(TH2F*) (fIn->Get(argv[2]))};
    Centrality::BordersFinder2D bf;
    bf.SetHisto2D( std::move(*histo2d) );
    std::unique_ptr<TH1F> h1d = bf.Convert();
    
    bf.SetHisto(*h1d);
    bf.SetRanges( 20,0,100 );   // number of bins, min, max value
    //   bf.SetRanges( {0,10,30,60,100} );  // centrality bins borders with array
    bf.IsSpectator(false);  // true if impact parameter b correlated with estimator (spectators eneggy), 
    // false - anticorrelated (multiplicity of produced particles) 
    
    bf.FindBorders();
    bf.SaveBorders2D(outfilename, "centr_getter_2d");
  }
  
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "elapsed time: " << elapsed_seconds.count() << " s\n";
  return 0;
}
