#include <iostream>
#include <chrono>

#include "Fitter.h"
#include "FitterHelper.h"

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TLegend.h"
#include "TH2.h"

using namespace Glauber;

int main(int argc, char *argv[])
{

  if (argc < 2)
  {
    std::cout << "Wrong number of parameters! Executable usage:" << std::endl;
    std::cout << "   ./glauber f0 k0" << std::endl;
    return -1;
  }
  const Float_t f0 = atof( argv[1]);
  const Int_t k0 = atoi( argv[2] );
  const Int_t k1 = atoi( argv[2] );
  
  // *****************************************
  // Modify this part according to your needs
  // *****************************************

  ///  |   mode    |   function for Na     |
  ///  |   Default |  Npart + (1-f)*Ncoll  |
  ///  |   PSD     |     f - Npart         |
  ///  |   Npart   |     Npart^f           |
  ///  |   Ncoll   |     Ncoll^f           |
  const TString mode = "Default";
  
  const TString glauber_filename = "../input/glauber_auau_sigma_30_100k.root";   // input files
  const TString glauber_treename = "nt_Au_Au";
  const TString in_filename = "../input/test_input.root";
  const TString histoname = "hMreco";

  const Int_t min_bin = 50;      // not fitting low multiplicity region due to trigger bias, etc
  const Int_t max_bin = 10000;   // very large number to fit the whole histo
  const Int_t nevents = 100000;

  const TString outdir = ".";
  // *****************************************
  // *****************************************

  std::unique_ptr<TFile> glauber_file{ TFile::Open(glauber_filename, "read") };
  std::unique_ptr<TTree> glauber_tree{ (TTree*) glauber_file->Get(glauber_treename) };

  std::unique_ptr<TFile> f{TFile::Open(in_filename)};    
  TH1F *hdata = (TH1F*)f->Get(histoname);
  
  Fitter fitter ( std::move(glauber_tree) );

  fitter.SetMode(mode);
  fitter.SetInputHisto(*hdata);
  fitter.SetBinSize(1);
  fitter.Init(nevents);
  
  fitter.SetFitMinBin(min_bin);
  fitter.SetFitMaxBin(max_bin);
  fitter.SetOutDirName(outdir);

  float par[3];
  
  auto start = std::chrono::system_clock::now();
  
  const float chi2 = fitter.FitGlauber(par, f0, k0, k1, nevents);
  
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "elapsed time: " << elapsed_seconds.count() << " s\n";

  std::cout << "f = " << par[0] << "    mu = " << par[1] << "    k = " << par[2] << "    chi2 = " << chi2 << std::endl; 
  
  Glauber::DrawHistos(fitter, true, true, true, true);

  const float range[2] = {300, 350.};
  std::unique_ptr<TH1F> hB(fitter.GetModelHisto (range, "B", par, 100000));
  hB->SaveAs( "b_test.root" );
  
  std::cout << "END!" << std::endl;

  return 0;
}
