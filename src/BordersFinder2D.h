/** @file   BordersFinder2D.h
    @class  Centrality::BordersFinder2D
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
    @brief  Class to determine centrality for 2D case. Inherited from Centrality::BordersFinder.
*/

#ifndef CENTRALITY_BORDERSFINDER2D_H
#define CENTRALITY_BORDERSFINDER2D_H

#include "BordersFinder.h"

#include "vector"
#include "array"

#include "TH2.h"
#include "TF1.h"

namespace Centrality {

constexpr int N = 3;
typedef std::array<double, N> FitParams;

struct NormalInfo {
  double p0;
  double p1;
  double x0;
  double y0;

  inline double operator () (double x) const {
    return p0 + p1 * x;
  }

  inline double xd(double distance) const {
    return x0 + TMath::Cos(TMath::ATan(p1))*distance;
  }

  inline double yd(double distance) const {
    return y0 + TMath::Sin(TMath::Abs(TMath::ATan(p1)))*distance;
  }
};

class BordersFinder2D : public BordersFinder {

 public:

  BordersFinder2D() = default;

  void SetHisto2D(TH2 *histo2d) { h2_ = histo2d; }

  void Init();
  TH1D *Convert();
  void Fit2D(const TString &func);
  NormalInfo FindNorm(const FitParams &par, double x0);
  double FindIntegral(const NormalInfo &n1, const NormalInfo &n2);
  void SaveBorders2D(const std::string &filename, const std::string &getter_name);

 private:

  TH2 *h2_{};
  TF1 *fit_{nullptr};

  TString fitname_{""};

  double xmax_{1.};
  double ymax_{1.};

//     ClassDef(BordersFinder2D, 1);

};
}

#endif //CENTRALITY_BORDERSFINDER2D_H
