/** @file   Fitter.h
 *    @class  Glauber::Fitter
 *    @author Viktor Klochkov (klochkov44@gmail.com)
 *    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
 *    @brief  Class to fit histo with Glauber based function
 */

#ifndef GlauberFitter_H
#define GlauberFitter_H 1

#include "TH1F.h"
#include "TNamed.h"
#include "TString.h"
#include "TTree.h"

#include <vector>
// #include "TMinuit.h"

namespace Glauber {
class Fitter {

 public:
  /**   Default constructor   **/
  Fitter() = default;
  ;
  explicit Fitter(std::unique_ptr<TTree> tree);
  /**   Destructor   **/
  virtual ~Fitter() = default;
  ;

  void Init(int nEntries);
  void SetGlauberFitHisto(double f, double mu, double k, Int_t n = 10000, Bool_t Norm2Data = true);
  void NormalizeGlauberFit();
  //  void DrawHistos(Bool_t isSim = true, Bool_t isData = true, Bool_t isGlauber = false, Bool_t isNBD = false) {};

  double FitGlauber(double* par, double f0, Int_t k0, Int_t k1, Int_t nEvents);
  void FindMuGoldenSection(double* mu,
                           double* chi2,
                           double mu_min,
                           double mu_max,
                           double f,
                           double k,
                           Int_t nEvents = 10000,
                           Int_t nIter = 5);

  double GetChi2() const;

  double NBD(double n, double mu, double k) const;
  void SetNBDhist(double mu, double k);
  double Nancestors(double f) const;
  double NancestorsMax(double f) const;

  std::unique_ptr<TH1F> GetModelHisto(const double range[2], const TString& name, const double par[3], Int_t nEvents);

  //
  //         Setters
  //
  void SetInputHisto(const TH1F& h) { fDataHisto = h; }
  void SetFitMinBin(Int_t min) { fFitMinBin = min; }
  void SetFitMaxBin(Int_t min) { fFitMaxBin = min; }
  void SetNormMinBin(Int_t min) { fNormMinBin = min; }
  void SetBinSize(Int_t size) { fBinSize = size; }
  void SetOutDirName(const TString& name) { fOutDirName = name; }
  void SetMode(const TString& mode) { fMode = mode; }

  //
  //         Getters
  //
  TH1F GetGlauberFitHisto() const { return fGlauberFitHisto; }
  TH1F GetDataHisto() const { return fDataHisto; }
  TH1F GetNBDHisto() const { return fNbdHisto; }
  TH1F GetNpartHisto() const { return fNpartHisto; }
  TH1F GetNcollHisto() const { return fNcollHisto; }
  TH1F GetBestFiHisto() const { return fBestFitHisto; }

 private:
  /**   Data members  **/
  TH1F fNpartHisto;
  TH1F fNcollHisto;
  TH1F fDataHisto;
  TH1F fNbdHisto;
  TH1F fGlauberFitHisto;
  TH1F fBestFitHisto;

  /* MC data */
  std::unique_ptr<TTree> fSimTree{nullptr};

  double fNpart{-1.};
  double fNcoll{-1.};

  double fMaxValue{-1.};

  Int_t fNbins{-1};
  Int_t fBinSize{-1};

  Int_t fFitMinBin{-1};
  Int_t fFitMaxBin{-1};

  Int_t fNormMinBin{-1};

  TString fMode{"Default"};

  TString fOutDirName{""};
  ClassDef(Fitter, 2);
};
}// namespace Glauber

#endif
