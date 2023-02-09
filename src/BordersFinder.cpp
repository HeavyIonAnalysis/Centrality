#include "BordersFinder.hpp"
#include "BordersFinderHelper.hpp"
#include "Getter.hpp"

#include <TGraph.h>
#include <iostream>

#include "TFile.h"

namespace Centrality {

void BordersFinder::FindBorders() {
  using namespace std;

  bool is_ranges_predefined;
  if (ranges_.size() > 1) {
    is_ranges_predefined = true;
  } else if (ranges_.size() == 0) {
    is_ranges_predefined = false;
  } else {
    throw std::runtime_error("BordersFinder::ranges_.size() == 1 is not a legal value");
  }

  if (norm_ == -1) norm_ = histo_.Integral(0, histo_.GetNbinsX());
  if (!isSpectator_) std::reverse(std::begin(ranges_), std::end(ranges_));

  auto axis = histo_.GetXaxis();

  double xLo = (applyLimits_) ? xLo_ : axis->GetXmin();
  double xHi = (applyLimits_) ? xHi_ : axis->GetXmax();

  int n = axis->GetNbins();

  double* histIntegral = histo_.GetIntegral();
  double* x = new double[n + 1];
  for (int i = 0; i <= n; ++i) {
    x[i] = axis->GetBinUpEdge(i);
  }

  TGraph intVsXGraph(n + 1, x, histIntegral);
  intVsXGraph.SetBit(TGraph::kIsSortedX);

  TGraph xVsIntGraph(n + 1, histIntegral, x);
  xVsIntGraph.SetBit(TGraph::kIsSortedX);

  double intLo = intVsXGraph.Eval(xLo);
  double intHi = intVsXGraph.Eval(xHi);
  double norm = intHi - intLo;

  auto cX = [=](double x) { return 100. / norm * (intVsXGraph.Eval(x) - intVsXGraph.Eval(xLo)); };
  auto xC = [=](double c) { return xVsIntGraph.Eval((c / 100.) * norm + intLo); };

  if (is_ranges_predefined) {
    for (auto cc : ranges_) {
      double xx = isSpectator_ ? xC(cc) : xC(100 - cc);
      cout << cc << "%"
           << ", border:" << xx << endl;
      borders_.push_back(xx);
    }
  } else {
    for (int i = 0; i <= n; i++) {
      borders_.push_back(x[i]);
      auto cc = isSpectator_ ? histIntegral[i] * 100 : (1 - histIntegral[i]) * 100;
      ranges_.push_back(cc);
      std::cout << cc << "%"
                << ", border:" << x[i] << "\n";
    }
  }
}

void BordersFinder::SaveBorders(const std::string& filename, const std::string& getter_name) {
  Getter getter;

  if (this->GetBorders().size() < 2) { return; }

  std::unique_ptr<TFile> f{TFile::Open(filename.data(), "recreate")};

  getter.SetBorders(this->GetBorders());
  getter.SetRanges(this->GetRanges());
  getter.IsSpectator(this->GetIsSpectator());

  getter.Write(getter_name.c_str());

  //     f->mkdir( ("dir_" + getter_name).c_str());
  //     f->cd( ("dir_" + getter_name).c_str() );

  BordersFinderHelper h;
  h.SetName(getter_name);
  h.SetIsPdf(true);
  h.QA(getter, this->GetHisto());
  h.PlotHisto(getter, this->GetHisto());

  f->Close();
}

}// namespace Centrality
