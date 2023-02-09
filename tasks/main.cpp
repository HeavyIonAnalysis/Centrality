#include "BordersFinder.hpp"
#include "BordersFinder2D.hpp"
#include "Getter.hpp"

#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>

#include "TFile.h"
#include "TH2.h"
#include "TTree.h"
#include <TROOT.h>
#include <TSystem.h>

int main(int argc, char** argv) {

  auto start = std::chrono::system_clock::now();
  ROOT::EnableImplicitMT(2);

  if (argc <= 4) {
    std::cout << "Not enough arguments! Please use:" << std::endl;
    std::cout << "   ./main filename histoname is_spectator is2d" << std::endl;
    return -1;
  }

  const char* filename{argv[1]};
  const char* histoname{argv[2]};
  const bool is_spectator = strcmp(argv[3], "true") == 0;
  const bool is_2d = strcmp(argv[4], "true") == 0;

  std::unique_ptr<TFile> fIn{TFile::Open(filename, "read")};
  std::string outfilename = "test.root";
  if (!is_2d) {
    std::unique_ptr<TH1F> histo{(TH1F*) (fIn->Get(histoname))};
    Centrality::BordersFinder bf;
    bf.SetHisto(*histo);
    bf.SetRanges(20, 0, 100);// number of bins, min, max value
//     bf.SetRanges( {0, 2, 4, 6, 8, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90, 100} );  // centrality bins borders with array
    // not to call SetRanges is also OK - then the ranging will be determined automatically according to input histo
    bf.IsSpectator(is_spectator);// true if impact parameter b correlated with estimator (spectators eneggy),
    // false - anticorrelated (multiplicity of produced particles)

    bf.FindBorders();
    bf.SaveBorders(outfilename, "centr_getter_1d");
  } else {
    std::unique_ptr<TH2F> histo2d{(TH2F*) (fIn->Get(argv[2]))};
    Centrality::BordersFinder2D bf;
    bf.SetHisto2D(std::move(*histo2d));
    std::unique_ptr<TH1F> h1d = bf.Convert();

    bf.SetHisto(*h1d);
    bf.SetRanges(20, 0, 100);// number of bins, min, max value
//     bf.SetRanges( {0,10,30,60,100} );  // centrality bins borders with array
    bf.IsSpectator(is_spectator);// true if impact parameter b correlated with estimator (spectators energy),
    // false - anticorrelated (multiplicity of produced particles)

    bf.FindBorders();
    bf.SaveBorders2D(outfilename, "centr_getter_2d");
  }

  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "elapsed time: " << elapsed_seconds.count() << " s\n";
  return 0;
}
