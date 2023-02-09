#include "Getter.hpp"

#include <iostream>

ClassImp(Centrality::Getter)

    namespace Centrality {

  double Getter::GetCentrality(double value) const {
    if (!isinitialized_) {
      std::cout << "Centrality::Getter is not initialized!" << std::endl;
      exit(-1);
    }
    const int ibin = borders_.FindBin(value);
    if (ibin == 0 || ibin > borders_.GetNbins()) {
      return -1;
    }

    auto lo = borders_.GetBinLowEdge(ibin);
    auto hi = borders_.GetBinUpEdge(ibin);
    auto left_weight = (hi - value) / (hi - lo);
    auto right_weight = (value - lo) / (hi - lo);

    const double centrality = left_weight * ranges_.at(ibin - 1) + right_weight * ranges_.at(ibin);

    return centrality;
  }

  double Getter::GetCentrality(double xvalue, double yvalue) const {
    if (!isinitialized2D_) {
      std::cout << "Centrality::Getter is not initialized!" << std::endl;
      exit(-1);
    }

    xvalue /= xmax_;
    yvalue /= ymax_;

    for (uint iborder = 0; iborder < borders2d_.size() - 1; ++iborder) {
      const double y1 = borders2d_.at(iborder)[0] + borders2d_.at(iborder)[1] * xvalue;
      const double y2 = borders2d_.at(iborder + 1)[0] + borders2d_.at(iborder + 1)[1] * xvalue;

      if (yvalue < y1 && yvalue > y2) {
        return 0.5 * (ranges_.at(iborder - 1) + ranges_.at(iborder));
      }
    }

    return -1.;
  }

  Getter* Getter::Create1DGetter(std::vector<double> borders) {
    typedef decltype(ranges_)::value_type doubleing_t;

    size_t n_borders = borders.size();
    assert(n_borders > 2);

    doubleing_t range_max = 100.f;
    doubleing_t range_min = 0.f;
    doubleing_t range_step = (range_max - range_min) / (n_borders - 1);

    decltype(ranges_) ranges(n_borders);
    for (uint i = 0; i < n_borders; ++i) {
      auto rr = range_min + range_step * i;
      ranges[i] = rr;

      std::cout << rr << "%: " << borders[i] << std::endl;
    }

    TAxis ax_borders(static_cast<Int_t>(n_borders - 1), &(borders[0]));

    auto getter = new Getter;
    getter->ranges_ = std::move(ranges);
    getter->borders_ = ax_borders;
    getter->isinitialized_ = true;

    return getter;
  }
}
