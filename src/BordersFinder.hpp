/** @file   BordersFinder.h
    @class  Centrality::BordersFinder
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
    @brief  Class to determine centrality
*/

#ifndef CENTRALITY_BORDERSFINDER_H
#define CENTRALITY_BORDERSFINDER_H

#include "TH1.h"

namespace Centrality {

class BordersFinder {

 public:
  BordersFinder() = default;

  void FindBorders();
  void SaveBorders(const std::string& filename, const std::string& getter_name);

  void SetHisto(const TH1F& h) { histo_ = h; }
  TH1F& GetHisto() { return histo_; }// not const to use Draw etc

  void SetRanges(const std::vector<double>& ranges) { ranges_ = ranges; }
  void SetRanges(int n, double min, double max) {
    ranges_.clear();
    for (int i = 0; i <= n; ++i) {
      ranges_.push_back(min + i * (max - min) / n);
    }
  }
  /**
  * To be used only in tests
  */
  void SetBorders(const std::vector<double>& borders) { borders_ = borders; }

  void SetLimits(double xLo, double xHi) {
    xLo_ = xLo;
    xHi_ = xHi;
    applyLimits_ = true;
  }

  void IsSpectator(bool is = true) { isSpectator_ = is; }

  [[nodiscard]] const std::vector<double>& GetRanges() const { return ranges_; }
  [[nodiscard]] const std::vector<double>& GetBorders() const { return borders_; }
  [[nodiscard]] bool GetIsSpectator() const { return isSpectator_; }

 private:
  TH1F histo_;

  std::vector<double> ranges_{};
  std::vector<double> borders_{};

  bool isSpectator_{false};

  bool applyLimits_{false};
  double xLo_{-1};
  double xHi_{-1};
};
}// namespace Centrality

#endif//CENTRALITY_BORDERSFINDER_H
