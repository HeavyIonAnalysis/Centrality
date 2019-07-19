/** @file   Getter.h
    @class  Centrality::Getter
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
    @brief  Class to calculate centrality class
*/

#ifndef CENTRALITY_GETTER_H
#define CENTRALITY_GETTER_H

#include <assert.h>
#include "vector"
#include "array"

#include "TObject.h"
#include "TAxis.h"
#include "TRandom.h"

namespace Centrality {

class Getter : public TObject {

 public:

  Getter() = default;

  float GetCentrality(float value) const;
  float GetCentrality(float xvalue, float yvalue) const;

  void SetBorders(const std::vector<double> &borders) {
    borders_ = TAxis(borders.size() - 1, &(borders[0]));
    isinitialized_ = true;
  }

  const TAxis &GetBorders() const { return borders_; };
  const std::vector<float> &GetRanges() const { return ranges_; };

  void SetRanges(const std::vector<float> &ranges) { ranges_ = ranges; }
  void IsSpectator(bool is = true) { isspectator_ = is; }

  void AddBorder2D(const std::array<float, 2> &border2D) {
    borders2d_.push_back(border2D);
    if (!isinitialized2D_) isinitialized2D_ = true;
  }

  const std::vector<std::array<float, 2>> &GetBorders2D() const { return borders2d_; };

  void SetMax(float x, float y) {
    xmax_ = x;
    ymax_ = y;
  }

  static Getter *Create1DGetter(std::vector<double> borders);

 private:

  TAxis borders_;
  std::vector<std::array<float, 2>> borders2d_{};
  std::vector<float> ranges_{};

  float xmax_{1.};
  float ymax_{1.};

  bool isspectator_{false};
  bool isinitialized_{false};
  bool isinitialized2D_{false};

 ClassDef(Getter, 2);

};
}

#endif //CENTRALITY_GETTER_H
