#include <chrono>
#include <iostream>

#include "Fitter.hpp"
#include "FitterHelper.hpp"

#include "TFile.h"
#include "TH1.h"
#include "TLegend.h"

using namespace Glauber;

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cout << "Wrong number of parameters! Executable usage:" << std::endl;
    std::cout << "   ./glauber f0 k0" << std::endl;
    return -1;
  }
  const double_t f0 = atof(argv[1]);
  const Int_t k0 = atoi(argv[2]);
  const Int_t k1 = argc == 4 ? atoi(argv[3]) : k0;

  // *****************************************
  // Modify this part according to your needs
  // *****************************************

  ///  |   mode    |   function for Na     |
  ///  |   Default |  Npart + (1-f)*Ncoll  |
  ///  |   PSD     |     f - Npart         |
  ///  |   Npart   |     Npart^f           |
  ///  |   Ncoll   |     Ncoll^f           |
  const TString mode = "Default"; // leave Default if you need to fit a multiplicity distribution

  const TString glauber_filename = "../../input/glauber_auau_sigma_30_100k.root"; // input MC Glauber file
  const TString glauber_treename = "nt_Au_Au"; // name of the TNtuple in the MC Glauber file
  const TString in_filename = "../../input/multiplicity_qa.urqmd.12agev.root"; // name of the input file with multiplicity histogram to be fitted
  const TString histoname = "hMult"; // name of the multiplicity histogram to be fitted

  // fitting ranges:
  const Int_t min_bin = 50;    // not fitting low multiplicity region due to trigger bias, etc
  const Int_t max_bin = 10000; // very large number to fit the whole histo

  // number of events from input MC Glauber file used for building the fitting histogram
  // set -1 to take ALL available events
  const Int_t nevents = -1;

  const TString outdir = ".";
  // *****************************************

  if(mode == "Default" && !(f0>=0 && f0<=1)) {
    throw std::runtime_error("glauber::main(): In Default mode the f0 must be in [0; 1] range");
  }

  std::unique_ptr<TFile> glauber_file{TFile::Open(glauber_filename, "read")};
  std::unique_ptr<TTree> glauber_tree{(TTree*) glauber_file->Get(glauber_treename)};

  std::unique_ptr<TFile> f{TFile::Open(in_filename)};
  TH1F* hdata = (TH1F*) f->Get(histoname);

  Fitter fitter(std::move(glauber_tree));

  fitter.SetMode(mode);
  fitter.SetInputHisto(*hdata);
  fitter.SetBinSize(1);
  fitter.Init(nevents);

  fitter.SetFitMinBin(min_bin);
  fitter.SetFitMaxBin(max_bin);
  fitter.SetOutDirName(outdir);

  double par[3];

  auto start = std::chrono::system_clock::now();

  const double chi2 = fitter.FitGlauber(par, f0, k0, k1, nevents);

  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "elapsed time: " << elapsed_seconds.count() << " s\n";

  std::cout << "f = " << par[0] << "    mu = " << par[1] << "    k = " << par[2] << "    chi2 = " << chi2 << std::endl;

  Glauber::DrawHistos(fitter, true, true, true, true);

  std::unique_ptr<TH2F> hB(fitter.GetModelHisto("B", par, 100000));
  hB->SaveAs("mult_b.root");

  std::cout << "END!" << std::endl;

  return 0;
}
