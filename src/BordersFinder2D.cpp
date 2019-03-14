#include "BordersFinder2D.h"
#include "Getter.h"
#include "BordersFinderHelper.h"

#include <iostream>
#include <TCutG.h>

#include "TProfile.h"
#include "TMath.h"
#include "TF1.h"
#include "TFile.h"

// ClassImp(Centrality::BordersFinder2D)

namespace Centrality {


template <int N>
inline double pol(const double *xp, const double *pp) {
  double x = xp[0];
  double result = 0.;
  double pow_x = 1.;
  for (int i = 0; i < N; ++i) {
    result += pp[i]*pow_x;
    pow_x *= x;
  }
  return result;
}

template <int N>
inline double d_pol(const double *xp, const double *pp) {
  double x = xp[0];
  double result = 0.;
  double pow_x = 1.;

  for (int i = 1; i < N; ++i) {
    result += i*pp[i]*pow_x;
    pow_x *= x;
  }

  return result;
}

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
  FitParams par{};
  std::copy(fit_->GetParameters(), fit_->GetParameters() + N, par.begin());

  for (int iBin = 1; iBin <= h2_->GetNbinsX(); ++iBin) {
//         if ( h2_.Integral(iBin, iBin, 0, h2_.GetNbinsY()) <= 1. ) continue;

    const double x1 = iBin == 1 ? -0.2 : h2_->GetXaxis()->GetBinCenter(iBin - 1);
    const double x2 = h2_->GetXaxis()->GetBinCenter(iBin);

    const auto norm1 = FindNorm(par, x1);
    const auto norm2 = FindNorm(par, x2);

    auto integral = FindIntegral(norm1, norm2);

    histo1d->SetBinContent(iBin, integral);
  }
  return histo1d;
}

/**
 * Find number of entries between lines norm1 and norm2
 * @param n1 first line parametrization y=a+bx
 * @param n2 second line parametrization
 * @return number of entries (integral)
 */
double BordersFinder2D::FindIntegral(const NormalInfo &n1, const NormalInfo &n2) {

  std::vector<double> xv{}, yv{};

  double s = 100.1;

  if (TMath::Abs(n1.p1 - n2.p1) < 1e-3) {
    xv = {n1.xd(-s), n1.xd(s), n2.xd(s), n2.xd(-s)};
    yv = {n1.yd(-s), n1.yd(s), n2.yd(s), n2.yd(-s)};
  } else {
    double xi = (n1.p0 - n2.p0)/(n2.p1 - n1.p1);
    double yi = n1(xi);

    double h = TMath::Sqrt(
        TMath::Power((n1.y0 + n2.y0)*0.5 - yi, 2.) +
        TMath::Power((n1.x0 + n2.x0)*0.5 - yi, 2.));


    if (h < s) {
      auto sign = ( yi < (n1.y0 + n2.y0)/2 )? 1 : -1;
      xv = {n1.xd(sign*s), xi, n2.xd(sign*s)};
      yv = {n1.yd(sign*s), yi, n2.yd(sign*s)};
    } else {
      xv = {n1.xd(-s), n1.xd(s), n2.xd(s), n2.xd(-s)};
      yv = {n1.yd(-s), n1.yd(s), n2.yd(s), n2.yd(-s)};
    }
  }

  TCutG area("temp", static_cast<int>(xv.size()), xv.data(), yv.data());
  double sum = area.IntegralHist(h2_);
  return sum;
}

void BordersFinder2D::SaveBorders2D(const std::string &filename, const std::string &getter_name) {
  Getter getter;

  if (this->GetBorders().size() < 2) return;

  std::unique_ptr<TFile> f{TFile::Open(filename.data(), "update")};

  getter.SetRanges(this->GetRanges());
  getter.IsSpectator(this->GetIsSpectator());
  getter.SetMax(xmax_, ymax_);

  FitParams par{};
  std::copy(fit_->GetParameters(), fit_->GetParameters() + N, par.begin());

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
  fit_ = new TF1("fit", pol<N>, 0.0, h2_->GetXaxis()->GetXmax(), N); // FIXME fit ranges
  prof->Fit(fit_, "Q");
}

/**
 *
 * @param par parameters of polynomial function
 * @param x0 argument
 * @return a0 and a1 parameters y = a0 + a1 * x
 */
NormalInfo BordersFinder2D::FindNorm(const FitParams &par, double x0) {
  double xp[1] = {x0};
  auto f = pol<N>(xp, par.data());
  auto df = d_pol<N>(xp, par.data());

  double df_threshold = 1e-2;
  if (TMath::Abs(df) < df_threshold) {
    Warning(__func__, "x0 = %f, |df| == %f < %f", x0, df, df_threshold);
  }

  auto p1 = -1./df;
  auto p0 = f - p1*x0;

  return {p0, p1, x0, f};
}

}
