/** @file   Getter.h
    @class  Centrality::Getter
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
    @brief  Class to calculate centrality class
*/

#ifndef CENTRALITY_GETTER_H
#define CENTRALITY_GETTER_H

#include "array"
#include "vector"
#include <cassert>

#include "TAxis.h"
#include "TObject.h"
#include "TRandom.h"

namespace Centrality{

class Getter : public TObject{

 public:
  Getter() = default;

  double GetCentrality(double value) const;
  double GetCentrality(double xvalue, double yvalue) const;

  void SetBorders(const std::vector<double>& borders) {
    borders_ = TAxis(borders.size() - 1, &(borders[0]));
    isinitialized_ = true;
  }

  const TAxis& GetBorders() const { return borders_; };
  const std::vector<double>& GetRanges() const { return ranges_; };

  void SetRanges(const std::vector<double>& ranges) { ranges_ = ranges; }
  void IsSpectator(bool is = true) { isspectator_ = is; }

  void AddBorder2D(const std::array<double, 2>& border2D) {
    borders2d_.push_back(border2D);
    if(!isinitialized2D_) { isinitialized2D_ = true; }
  }

  const std::vector<std::array<double, 2>>& GetBorders2D() const { return borders2d_; };

  void SetMax(double x, double y) {
    xmax_ = x;
    ymax_ = y;
  }

  static Getter* Create1DGetter(std::vector<double> borders);

 private:
  TAxis borders_;
  std::vector<std::array<double, 2>> borders2d_{};
  std::vector<double> ranges_{};

  double xmax_{1.};
  double ymax_{1.};

  bool isspectator_{false};
  bool isinitialized_{false};
  bool isinitialized2D_{false};

 ClassDef(Getter, 2);
};
}// namespace Centrality

#endif//CENTRALITY_GETTER_H
