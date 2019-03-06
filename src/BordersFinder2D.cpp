#include "BordersFinder2D.h"
#include "Getter.h"
#include "BordersFinderHelper.h"

#include <iostream>

#include "TProfile.h"
#include "TMath.h"
#include "TF1.h"
#include "TFile.h"

// ClassImp(Centrality::BordersFinder2D)

namespace Centrality {

void BordersFinder2D::Init() {
  fitname_ = "pol2";

  for (int iBin = h2_->GetNbinsX(); iBin >= 1; --iBin) {
    if (h2_->Integral(iBin, iBin, 0, h2_->GetNbinsY()) >= 1.) {
      xmax_ = h2_->GetXaxis()->GetBinCenter(iBin);
      break;
    }
  }
  for (int iBin = h2_->GetNbinsY(); iBin >= 1; --iBin) {
    if (h2_->Integral(0, h2_->GetNbinsX(), iBin, iBin) >= 1.) {
      ymax_ = h2_->GetYaxis()->GetBinCenter(iBin);
      break;
    }
  }
  h2_->GetXaxis()->SetLimits(0, h2_->GetXaxis()->GetXmax() / xmax_);
  h2_->GetYaxis()->SetLimits(0, h2_->GetYaxis()->GetXmax() / ymax_);
}

/**
 * Converts 2D histo to 1D. Transformation is done moving along 
 * fit to 2D distribution and counting number of entries perpendicular to it.
 * @return pointer to 1D histo
 */
TH1D *BordersFinder2D::Convert() {
  Init();

  auto histo1d =
      new TH1D("histo1d", "", h2_->GetNbinsX(), h2_->GetXaxis()->GetXmin(), h2_->GetXaxis()->GetXmax());

  Fit2D(fitname_);
  std::vector<double> par;
  for (int ipar = 0; ipar < fit_->GetNpar(); ++ipar)
    par.push_back(fit_->GetParameter(ipar));

  for (int iBin = 1; iBin <= h2_->GetNbinsX(); ++iBin) {
//         if ( h2_.Integral(iBin, iBin, 0, h2_.GetNbinsY()) <= 1. ) continue;

    const double x1 = iBin == 1 ? -0.2 : h2_->GetXaxis()->GetBinCenter(iBin - 1);
    const double x2 = h2_->GetXaxis()->GetBinCenter(iBin);

    const auto norm1 = FindNorm(par, x1);
    const auto norm2 = FindNorm(par, x2);

    auto integral = FindIntegral(norm1, norm2);
//         std::cout << integral << std::endl;

    histo1d->SetBinContent(iBin, integral);
  }
  return histo1d;
}

/**
 * Find number of entries between lines norm1 and norm2
 * @param norm1 first line parametrization y=a+bx
 * @param norm2 second line parametrization
 * @return number of entries (integral)
 */
double BordersFinder2D::FindIntegral(const FLinearPar &norm1, const FLinearPar &norm2) {
  double sum{0.};

  for (int iBinX = 1; iBinX <= h2_->GetNbinsX(); ++iBinX) {
    for (int iBinY = 1; iBinY <= h2_->GetNbinsY(); ++iBinY) {
      const double entries = h2_->GetBinContent(iBinX, iBinY);
      if (entries == 0) continue;

      sum += entries*Intersection(iBinX, iBinY, norm1, norm2);
    }
  }
  return sum;
}

void BordersFinder2D::SaveBorders2D(const std::string &filename, const std::string &getter_name) {
  Getter getter;

  if (this->GetBorders().size() < 2) return;

  std::unique_ptr<TFile> f{TFile::Open(filename.data(), "update")};

  getter.SetRanges(this->GetRanges());
  getter.IsSpectator(this->GetIsSpectator());
  getter.SetMax(xmax_, ymax_);

  std::vector<double> par;
  for (int ipar = 0; ipar < fit_->GetNpar(); ++ipar)
    par.push_back(fit_->GetParameter(ipar));

  for (const auto &iborder : this->GetBorders()) {
    const auto kb = FindNorm(par, iborder);
    getter.AddBorder2D(kb);
  }
  getter.Write(getter_name.c_str());

//     f->mkdir( ("dir_" + getter_name).c_str());
//     f->cd( ("dir_" + getter_name).c_str() );

  BordersFinderHelper h;
  h.SetName(getter_name);
  h.SetIsPdf(true);
  h.PlotHisto(getter, this->GetHisto());
  h.PlotHisto2D(getter, h2_, *fit_);

  f->Close();
}

void BordersFinder2D::Fit2D(const TString &func) {
  std::unique_ptr<TProfile> prof{h2_->ProfileX()};
  fit_ = new TF1("fit", func, h2_->GetXaxis()->GetXmin(), h2_->GetXaxis()->GetXmax());
  prof->Fit(fit_, "Q");
}

/**
 *
 * @param par parameters of polinomial function
 * @param x argument
 * @return a0 and a1 parameters y = a0 + a1 * x
 */
std::array<float, 2> BordersFinder2D::FindNorm(const std::vector<double> par, float x) {
  std::array<float, 2> ret;
  const float dx = (h2_->GetXaxis()->GetXmax() - h2_->GetXaxis()->GetXmin()) / 10000.;

  /* left */
  const float y1 = polN(par, x - dx);
  /* right */
  const float y2 = polN(par, x + dx);

  // cx*x + cy*y + c == 0

  /* 1/df */
  const float cx = 1 / (y2 - y1);
  /* 1/2dx */
  const float cy = 0.5 / dx;

  const float c = -cx * x - cy * polN(par, x);

  ret[0] = -c / cy;
  ret[1] = -cx / cy;
  return ret;
}

}
