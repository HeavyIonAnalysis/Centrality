#include "Fitter.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TMath.h"
#include "TRandom.h"

ClassImp(Glauber::Fitter)

// -----   Default constructor   -------------------------------------------
Glauber::Fitter::Fitter(std::unique_ptr<TTree> tree) {
  fSimTree = std::move(tree);
  std::cout << fSimTree->GetEntries() << std::endl;

  if (!fSimTree) {
    std::cout << "SetSimHistos: *** Error - " << std::endl;
    exit(EXIT_FAILURE);
  }

  fSimTree->SetBranchAddress("Npart", &fNpart);
  fSimTree->SetBranchAddress("Ncoll", &fNcoll);
}

void Glauber::Fitter::Init(int nEntries) {
  if (nEntries < 0 || nEntries > fSimTree->GetEntries()) {
    std::cout << "Init: *** ERROR - number of entries < 0 or less that number of entries in input tree" << std::endl;
    std::cout << "Init: *** number of entries in input tree = " << fSimTree->GetEntries() << std::endl;
    exit(EXIT_FAILURE);
  }

  const int NpartMax = int(fSimTree->GetMaximum("Npart"));
  const int NcollMax = int(fSimTree->GetMaximum("Ncoll"));

  fNpartHisto = TH1F("fNpartHisto", "Npart", NpartMax / fBinSize, 0, NpartMax);
  fNcollHisto = TH1F("fNcollHisto", "Ncoll", NcollMax / fBinSize, 0, NcollMax);

  for (int i = 0; i < nEntries; i++) {
    fSimTree->GetEntry(i);
    fNcollHisto.Fill(fNcoll);
    fNpartHisto.Fill(fNpart);
  }
  std::cout << fSimTree->GetEntries() << std::endl;

  fNbins = fDataHisto.GetNbinsX();

  while (fDataHisto.GetBinContent(fNbins - 1) == 0)
    fNbins--;

  fNbins++;

  const float min = fDataHisto.GetXaxis()->GetXmin();
  const float max = fDataHisto.GetXaxis()->GetXmax();

  fMaxValue = min + (max - min) * fNbins / fDataHisto.GetNbinsX();

  std::cout << "fNbins = " << fNbins << std::endl;
  std::cout << "fMaxValue = " << fMaxValue << std::endl;

}

float Glauber::Fitter::Nancestors(float f) const {
  if (fMode == "Default") return f * fNpart + (1 - f) * fNcoll;
  else if (fMode == "PSD") return f - fNpart;
  else if (fMode == "Npart") return TMath::Power(fNpart, f);
  else if (fMode == "Ncoll") return TMath::Power(fNcoll, f);

  return -1.;
}

float Glauber::Fitter::NancestorsMax(float f) const {
  const int NpartMax = fNpartHisto.GetXaxis()->GetXmax();  // some magic
  const int NcollMax = fNcollHisto.GetXaxis()->GetXmax(); //TODO

  if (fMode == "Default") return f * NpartMax + (1 - f) * NcollMax;
  else if (fMode == "PSD") return NpartMax;
  else if (fMode == "Npart") return TMath::Power(NpartMax, f);
  else if (fMode == "Ncoll") return TMath::Power(NcollMax, f);

  return -1.;
}

/*
 * take Glauber MC data from fSimTree
 * Populate fGlauberFitHisto with NBD x Na
 */
void Glauber::Fitter::SetGlauberFitHisto(float f, float mu, float k, int n, Bool_t Norm2Data) {
  fGlauberFitHisto = TH1F("glaub", "", fNbins * 1.3, 0, 1.3 * fMaxValue);
  fGlauberFitHisto.SetName(Form("glaub_%4.2f_%6.4f_%4.2f_%d", f, mu, k, n));

  SetNBDhist(mu, k);

  std::unique_ptr<TH1F> htemp{(TH1F *) fNbdHisto.Clone("htemp")}; // WTF??? Not working without pointer
  for (int i = 0; i < n; i++) {
    fSimTree->GetEntry(i);
    const int Na = int(Nancestors(f));

    float nHits{0.};
    for (int j = 0; j < Na; j++) {
      nHits += int(htemp->GetRandom());
    }
    fGlauberFitHisto.Fill(nHits);
  }

  if (Norm2Data)
    NormalizeGlauberFit();
}

void Glauber::Fitter::NormalizeGlauberFit() {

  int fGlauberFitHistoInt{0};
  int fDataHistoInt{0};

  const int lowchibin = fFitMinBin;
  const int highchibin = fFitMaxBin < fNbins ? fFitMaxBin : fNbins;

  for (int i = lowchibin; i < highchibin; i++) {
    fGlauberFitHistoInt += fGlauberFitHisto.GetBinContent(i + 1);
    fDataHistoInt += fDataHisto.GetBinContent(i + 1);
  }

  const float ScaleFactor = (float) fDataHistoInt / fGlauberFitHistoInt;

  //     std::cout << "Scale = " << Scale << std::endl;
  fGlauberFitHisto.Scale(ScaleFactor);
}

/**
 * 
 * @param mu mean value of negative binominal distribution (we are looking for it)
 * @param chi2 return value (indicates good match)
 * @param mu_min lower search edge for mean value NBD
 * @param mu_max upper search edge for mean value NBD
 * @param f parameter of Na
 * @param k NBD parameter
 * @param nEvents
 * @param nIter
 */
void Glauber::Fitter::FindMuGoldenSection(float *mu,
                                          float *chi2,
                                          float mu_min,
                                          float mu_max,
                                          float f,
                                          float k,
                                          int nEvents,
                                          int nIter) {
  const float phi{(1 + TMath::Sqrt(5)) / 2};

  /* left */
  float mu_1 = mu_max - (mu_max - mu_min) / phi;

  /* right */
  float mu_2 = mu_min + (mu_max - mu_min) / phi;

  SetGlauberFitHisto(f, mu_1, k, nEvents);
  float chi2_mu1 = GetChi2();

  SetGlauberFitHisto(f, mu_2, k, nEvents);
  float chi2_mu2 = GetChi2();

  for (int j = 0; j < nIter; j++) {
    if (chi2_mu1 > chi2_mu2) {
      mu_min = mu_1;
      mu_1 = mu_2;
      mu_2 = mu_min + (mu_max - mu_min) / phi;
      chi2_mu1 = chi2_mu2;
      SetGlauberFitHisto(f, mu_2, k, nEvents);
      chi2_mu2 = GetChi2();
    } else {
      mu_max = mu_2;
      mu_2 = mu_1;
      mu_1 = mu_max - (mu_max - mu_min) / phi;
      chi2_mu2 = chi2_mu1;
      SetGlauberFitHisto(f, mu_1, k, nEvents);
      chi2_mu1 = GetChi2();
    }

    std::cout << "mu1 = " << mu_1 << " mu2 = " << mu_2 << " chi2_mu1 = " << chi2_mu1 << " chi2_mu2 = " << chi2_mu2
              << std::endl;
  }

  /* take min(mu) */
  *mu = (chi2_mu1 < chi2_mu2) ? mu_1 : mu_2;
  /* take min(chi2) */
  *chi2 = (chi2_mu1 < chi2_mu2) ? chi2_mu1 : chi2_mu2;
}

/**
 * Find the best match
 *
 * @param return value of best fit parameters
 * @param f0 parameter of Na, for which chi2 will be calculated
 * @param k0 lower search edge for NBD parameter
 * @param k1 upper search edge for NBD parameter
 * @param nEvents
 */
float Glauber::Fitter::FitGlauber(float *par, Float_t f0, Int_t k0, Int_t k1, Int_t nEvents) {
  float f_fit{-1};
  float mu_fit{-1};
  float k_fit{-1};
  float Chi2Min{1e10};

  const TString filename = Form("%s/fit_%4.2f_%d_%d_%d.root", fOutDirName.Data(), f0, k0, k1, fFitMinBin);

  //     std::unique_ptr<TFile> file {TFile::Open(filename, "recreate")};    
  //     std::unique_ptr<TTree> tree {new TTree("test_tree", "tree" )};

  TFile *file{TFile::Open(filename, "recreate")};
  TTree *tree{new TTree("test_tree", "tree")};

  TH1F h1("h1", "", fNbins, 0, fMaxValue);

  float f, mu, k, chi2, sigma;

  tree->Branch("histo", "TH1F", &h1);
  tree->Branch("f", &f, "f/F");
  tree->Branch("mu", &mu, "mu/F");
  tree->Branch("k", &k, "k/F");
  tree->Branch("chi2", &chi2, "chi2/F");
  tree->Branch("sigma", &sigma, "sigma/F");

  f = f0;
  for (int j = k0; j <= k1; j++) {
    mu = fMaxValue / NancestorsMax(f);
    k = j;
    const float mu_min = 0.7 * mu;
    const float mu_max = 1.0 * mu;

    FindMuGoldenSection(&mu, &chi2, mu_min, mu_max, f, k, nEvents, 10);
    sigma = (mu / k + 1) * mu;
    h1 = fGlauberFitHisto;

    tree->Fill();

    if (chi2 < Chi2Min) {
      f_fit = f;
      mu_fit = mu;
      k_fit = k;
      Chi2Min = chi2;
      fBestFitHisto = fGlauberFitHisto;
    }

  }
  tree->Write();
  file->Write();
  file->Close();

  par[0] = f_fit;
  par[1] = mu_fit;
  par[2] = k_fit;

  return Chi2Min;
}

/**
 * Compare fGlauberFitHisto with fDataHisto
 * @return chi2 value
 */
float Glauber::Fitter::GetChi2() const {
  float chi2{0.0};

  const int lowchibin = fFitMinBin;
  const int highchibin = fFitMaxBin < fNbins ? fFitMaxBin : fNbins;

  for (int i = lowchibin; i <= highchibin; ++i) {
    if (fDataHisto.GetBinContent(i) < 1.0) continue;
    const float error2 = TMath::Power(fDataHisto.GetBinError(i), 2) + TMath::Power(fGlauberFitHisto.GetBinError(i), 2);
    const float diff2 = TMath::Power((fGlauberFitHisto.GetBinContent(i) - fDataHisto.GetBinContent(i)), 2) / error2;

    chi2 += diff2;
  }

  chi2 = chi2 / (highchibin - lowchibin + 1);
  return chi2;
}

/**
 * Popuates histogram nbd_<mean>_<k> with values of NBD
 * @param mu
 * @param k
 */
void Glauber::Fitter::SetNBDhist(float mu, float k) {
  // Interface for TH1F.
  const int nBins = (mu + 1.) * 3 < 10 ? 10 : (mu + 1.) * 3;

  fNbdHisto = TH1F("fNbdHisto", "", nBins, 0, nBins);
  fNbdHisto.SetName(Form("nbd_%f_%f", mu, k));

  for (int i = 0; i < nBins; ++i) {
    const float val = NBD(i, mu, k);
    if (val > 1e-10) fNbdHisto.SetBinContent(i + 1, val);
    //         std::cout << "val " << val << std::endl;    
  }
}

/**
 * Negative Binomial Distribution (by definition)
 * @param n argument
 * @param mu mean
 * @param k argument
 * @return NBD for a given parameters
 */
float Glauber::Fitter::NBD(float n, float mu, float k) const {
  // Compute NBD.
  float F;
  float f;

  if (n + k > 100.0) {
    // log method for handling large numbers
    F = TMath::LnGamma(n + k) - TMath::LnGamma(n + 1.) - TMath::LnGamma(k);
    f = n * TMath::Log(mu / k) - (n + k) * TMath::Log(1.0 + mu / k);
    F += f;
    F = TMath::Exp(F);
  } else {
    F = TMath::Gamma(n + k) / (TMath::Gamma(n + 1.) * TMath::Gamma(k));
    f = n * TMath::Log(mu / k) - (n + k) * TMath::Log(1.0 + mu / k);
    f = TMath::Exp(f);
    F *= f;
  }

  return F;
}
/**
 * Creates histo with a given model parameter distribution
 * @param range observable range
 * @param name name of the MC-Glauber model parameter 
 * @param par array with fit parameters
 * @param Nevents
 * @return pointer to the histogram 
 */
std::unique_ptr<TH1F> Glauber::Fitter::GetModelHisto(const float range[2],
                                                     const TString &name,
                                                     const float par[3],
                                                     int nEvents) {
  const float f = par[0];
  const float mu = par[1];
  const float k = par[2];

  float modelpar{-999.};
  fSimTree->SetBranchAddress(name, &modelpar);

  SetNBDhist(mu, k);

  //     TRandom random;  
  //     random.SetSeed(mu*k);

  std::unique_ptr<TH1F> hModel(new TH1F("hModel", "name", 100, fSimTree->GetMinimum(name), fSimTree->GetMaximum(name)));

#pragma omp parallel for
  for (int i = 0; i < nEvents; i++) {
    fSimTree->GetEntry(i);
    const int Na = int(Nancestors(f));
    float nHits{0.};
    for (int j = 0; j < Na; ++j) {
      nHits += (int) fNbdHisto.GetRandom();
    }

    if (nHits > range[0] && nHits < range[1]) {
      hModel->Fill(modelpar);
    }
  }

  return std::move(hModel);

}



