#include "Fitter.hpp"

#include "TF1.h"
#include "TFile.h"
#include "TMath.h"
#include "TRandom.h"
#include "TTree.h"

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
  if(nEntries < 0) {
    nEntries = fSimTree->GetEntries();
  }
  if(nEntries > fSimTree->GetEntries()) {
    nEntries = fSimTree->GetEntries();
    std::cout << "Warning - Fitter::Init() nEntries > fSimTree->GetEntries()" << std::endl;
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

  const auto min = fDataHisto.GetXaxis()->GetXmin();
  const auto max = fDataHisto.GetXaxis()->GetXmax();

  fMaxValue = min + (max - min) * fNbins / fDataHisto.GetNbinsX();

  std::cout << "fNbins = " << fNbins << std::endl;
  std::cout << "fMaxValue = " << fMaxValue << std::endl;
}

double Glauber::Fitter::Nancestors(double f) const {
  if (fMode == "Default") {
    return f * fNpart + (1 - f) * fNcoll;
  } else if (fMode == "PSD") {
    return f - fNpart;
  } else if (fMode == "Npart") {
    return TMath::Power(fNpart, f);
  } else if (fMode == "Ncoll") {
    return TMath::Power(fNcoll, f);
  }

  return -1.;
}

double Glauber::Fitter::NancestorsMax(double f) const {
  const int NpartMax = fNpartHisto.GetXaxis()->GetXmax();// some magic
  const int NcollMax = fNcollHisto.GetXaxis()->GetXmax();//TODO

  if (fMode == "Default") {
    return f * NpartMax + (1 - f) * NcollMax;
  } else if (fMode == "PSD") {
    return NpartMax;
  } else if (fMode == "Npart") {
    return TMath::Power(NpartMax, f);
  } else if (fMode == "Ncoll") {
    return TMath::Power(NcollMax, f);
  }

  return -1.;
}

/*
 * take Glauber MC data from fSimTree
 * Populate fGlauberFitHisto with NBD x Na
 */
void Glauber::Fitter::SetGlauberFitHisto(double f, double mu, double k, int n, Bool_t Norm2Data) {
  fGlauberFitHisto = TH1F("glaub", "", fNbins * 1.3, 0, 1.3 * fMaxValue);
  fGlauberFitHisto.SetName(Form("glaub_%4.2f_%d", f, int(k)));

  SetNBDhist(mu, k);

  std::unique_ptr<TH1F> htemp{(TH1F*) fNbdHisto.Clone("htemp")};// WTF??? Not working without pointer
  for (int i = 0; i < n; i++) {
    fSimTree->GetEntry(i);
    const int Na = int(std::round(Nancestors(f)));

    double nHits{0.};
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

  const double ScaleFactor = (double) fDataHistoInt / fGlauberFitHistoInt;

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
void Glauber::Fitter::FindMuGoldenSection(double* mu,
                                          double* chi2,
                                          double mu_min,
                                          double mu_max,
                                          double f,
                                          double k,
                                          int nEvents,
                                          int nIter) {
  const double phi{(1. + TMath::Sqrt(5)) / 2.};

  /* left */
  double mu_1 = mu_max - (mu_max - mu_min) / phi;

  /* right */
  double mu_2 = mu_min + (mu_max - mu_min) / phi;

  SetGlauberFitHisto(f, mu_1, k, nEvents);
  double chi2_mu1 = GetChi2();

  SetGlauberFitHisto(f, mu_2, k, nEvents);
  double chi2_mu2 = GetChi2();

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
double Glauber::Fitter::FitGlauber(double* par, double f0, Int_t k0, Int_t k1, Int_t nEvents) {
  if(nEvents < 0 || nEvents > fSimTree->GetEntries()) {
    nEvents = fSimTree->GetEntries();
  }

  double f_fit{-1};
  double mu_fit{-1};
  double k_fit{-1};
  double Chi2Min{1e10};

  const TString filename = Form("%s/fit_%4.2f_%d_%d_%d.root", fOutDirName.Data(), f0, k0, k1, fFitMinBin);

  TFile* file{TFile::Open(filename, "recreate")};
  TTree* tree{new TTree("test_tree", "tree")};

  TH1F h1("h1", "", fNbins, 0, fMaxValue);

  double f, mu, k, chi2, sigma2;

  tree->Branch("f", &f, "f/D");
  tree->Branch("mu", &mu, "mu/D");
  tree->Branch("k", &k, "k/D");
  tree->Branch("chi2", &chi2, "chi2/D");
  tree->Branch("sigma2", &sigma2, "sigma2/D");

  f = f0;
  for (int j = k0; j <= k1; j++) {
    mu = fMaxValue / NancestorsMax(f);
    k = j;
    const double mu_min = 0.7 * mu;
    const double mu_max = 1.0 * mu;

    FindMuGoldenSection(&mu, &chi2, mu_min, mu_max, f, k, nEvents, 10);
    sigma2 = (mu / k + 1) * mu;
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
  file->cd();
  tree->Write();
  h1.Write();
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
double Glauber::Fitter::GetChi2() const {
  double chi2{0.0};

  const int lowchibin = fFitMinBin;
  const int highchibin = fFitMaxBin < fNbins ? fFitMaxBin : fNbins;

  for (int i = lowchibin; i <= highchibin; ++i) {
    if (fDataHisto.GetBinContent(i) < 1.0) continue;
    const double error2 = TMath::Power(fDataHisto.GetBinError(i), 2) + TMath::Power(fGlauberFitHisto.GetBinError(i), 2);
    const double diff2 = TMath::Power((fGlauberFitHisto.GetBinContent(i) - fDataHisto.GetBinContent(i)), 2) / error2;

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
void Glauber::Fitter::SetNBDhist(double mu, double k) {
  // Interface for TH1F.
  const int nBins = (mu + 1.) * 3 < 10 ? 10 : (mu + 1.) * 3;

  fNbdHisto = TH1F("fNbdHisto", "", nBins, 0, nBins);
  fNbdHisto.SetName(Form("nbd_%f_%f", mu, k));

  for (int i = 0; i < nBins; ++i) {
    const double val = NBD(i, mu, k);
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
double Glauber::Fitter::NBD(double n, double mu, double k) const {
  // Compute NBD.
  double F;
  double f;

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
std::unique_ptr<TH2F> Glauber::Fitter::GetModelHisto(const TString& name,
                                                     const double par[3],
                                                     int nEvents) {
  const double f = par[0];
  const double mu = par[1];
  const double k = par[2];

  float modelpar{-999.};
  fSimTree->SetBranchAddress(name, &modelpar);

  SetNBDhist(mu, k);
  std::unique_ptr<TH2F> hModel(new TH2F("hModel", "", 100, fSimTree->GetMinimum(name), fSimTree->GetMaximum(name), fBestFitHisto.GetNbinsX(), fBestFitHisto.GetXaxis()->GetXmin(), fBestFitHisto.GetXaxis()->GetXmax()));

#pragma omp parallel for
  for (int i = 0; i < nEvents; i++) {
    fSimTree->GetEntry(i);
    const int Na = int(std::round(Nancestors(f)));
    double nHits{0.};
    for (int j = 0; j < Na; ++j) {
      nHits += (int) fNbdHisto.GetRandom();
    }

    hModel->Fill(modelpar, nHits);
  }

  hModel->GetXaxis()->SetTitle(name);
  hModel->GetYaxis()->SetTitle("Multiplicity");

  return hModel;
}
