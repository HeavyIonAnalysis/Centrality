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

class BordersFinder2D : public BordersFinder {

 public:
  typedef std::array<float, 2> FLinearPar;

  BordersFinder2D() = default;

  void SetHisto2D(TH2 *histo2d) { h2_ = histo2d; }

  void Init();
  TH1D *Convert();
  void Fit2D(const TString &func);
  std::array<float, 2> FindNorm(const std::vector<double> par, float x);
  double FindIntegral(const FLinearPar &norm1, const FLinearPar &norm2);
  void SaveBorders2D(const std::string &filename, const std::string &getter_name);

  /**
   *
   * @param par parameters of polinom
   * @param x argument
   * @param N order
   * @return
   */
  float polN(std::vector<double> par, float x) {
    float res{0.};
    float xn{1.};
    for (const auto ipar : par) {
      res += ipar * xn;
      xn *= x;
    }
    return res;
  }

 protected:

  /**
   * Calculates the ratio of intersection volume to (ix, iy) bin volume
   * @param ix
   * @param iy
   * @param parN1
   * @param parN2
   * @return
   */
  inline double Intersection(int ix, int iy, const FLinearPar &parN1, const FLinearPar &parN2) {
    double x1 = h2_->GetXaxis()->GetBinLowEdge(ix);
    double x2 = h2_->GetXaxis()->GetBinUpEdge(ix);
    double y1 = h2_->GetYaxis()->GetBinLowEdge(iy);
    double y2 = h2_->GetYaxis()->GetBinUpEdge(iy);
    double binArea = (y2 - y1)*(x2 - x1);

    auto nHi = [&parN1](double x) { return parN1[0] + x*parN1[1]; };
    auto nLo = [&parN2](double x) { return parN2[0] + x*parN2[1]; };

    auto dY1 = TMath::Min(y2, nHi(x1)) - TMath::Max(y1, nLo(x1));
    auto dY2 = TMath::Min(y2, nHi(x2)) - TMath::Max(y1, nLo(x2));

    if (dY1 < 0 || dY2 < 0) {
      return 0;
    }

    auto intersectionArea = 0.5*(dY1 + dY2)*(x2 - x1);
    auto intersectionRatio = intersectionArea/binArea;

    if (intersectionRatio > 1.) {
      throw std::logic_error("intersectionArea/binArea > 1");
    }

    return intersectionRatio;
  }

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
