/** @file   BordersFinder2D.h
    @class  Centrality::BordersFinder2D
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
    @brief  Class to determine centrality for 2D case. Inherited from Centrality::BordersFinder.
*/

#ifndef CENTRALITY_BORDERSFINDER2D_H
#define CENTRALITY_BORDERSFINDER2D_H

#include "BordersFinder.hpp"

#include "array"
#include "vector"

#include "TF1.h"
#include "TH2.h"

namespace Centrality {

class BordersFinder2D : public BordersFinder {

 public:
  BordersFinder2D() = default;

  void SetHisto2D(TH2F&& histo2d) { histo2d_ = histo2d; }
  TH2F&& GetHisto2D() { return std::move(histo2d_); }

  void Init();
  std::unique_ptr<TH1F> Convert();
  void Fit2D(const TString& func);
  std::array<double, 2> FindNorm(const std::vector<double>& par, double x);
  double FindIntegral(const std::array<double, 2>& norm1, const std::array<double, 2>& norm2);
  void SaveBorders2D(const std::string& filename, const std::string& getter_name);

  /**
   *
   * @param par parameters of polinom
   * @param x argument
   * @param N order
   * @return
   */
  static double polN(const std::vector<double>& par, double x) {
    double res{0.};
    double xn{1.};
    for (const auto ipar : par) {
      res += ipar * xn;
      xn *= x;
    }
    return res;
  }

 private:
  TH2F histo2d_;
  TF1* fit_{nullptr};

  TString fitname_{""};

  double xmax_{1.};
  double ymax_{1.};

  //     ClassDef(BordersFinder2D, 1);
};
}// namespace Centrality

#endif//CENTRALITY_BORDERSFINDER2D_H
