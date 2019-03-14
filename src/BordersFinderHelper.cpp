#include "BordersFinderHelper.h"

#include <iostream>

#include "TRandom.h"
#include "TCanvas.h"
#include "TLine.h"

namespace Centrality {

void BordersFinderHelper::QA(const Getter &getter, const TH1 *histo) const {
  auto ranges = getter.GetRanges();
  std::sort(ranges.begin(), ranges.end());

  TH1D hCentr("hCentr", "", ranges.size() - 1, &(ranges.front()));

  std::unique_ptr<TRandom> r{new TRandom};

  for (Int_t iBin = 0; iBin < histo->GetNbinsX(); ++iBin) {
    double Mult = histo->GetBinCenter(iBin + 1);

    for (Int_t j = 0; j < histo->GetBinContent(iBin + 1); ++j) {
      hCentr.Fill(getter.GetCentrality(Mult + (r->Rndm() - 0.5)));
    }
  }
  hCentr.Write(name_ + "_qa");
  if (ispdf_) {
    std::unique_ptr<TCanvas> c{new TCanvas("c", "", 1200, 800)};
    hCentr.Draw();
    c->SaveAs(name_ + "_qa" + ".pdf");
  }
}

void BordersFinderHelper::PlotHisto(const Getter &getter, TH1 *histo) const {
  std::unique_ptr<TCanvas> c{new TCanvas("c", "", 1200, 800)};
  histo->Draw();

  auto borders = getter.GetBorders();
  TLine *line;

  for (int i = 0; i < borders.GetNbins(); ++i) {
    const double border = borders.GetBinLowEdge(i + 1);
    const double height = histo->GetBinContent(histo->FindBin(border));

    line = new TLine(border, 0, border, height);
    line->Draw("same");
  }
  c->SetLogy();
  c->Write(name_ + "_histo_1d");

  if (ispdf_)
    c->SaveAs(name_ + "_histo_1d" + ".pdf");

  delete line;
}

void BordersFinderHelper::PlotHisto2D(const Getter &getter, TH2 *histo, TF1 &func) const {
  std::unique_ptr<TCanvas> c{new TCanvas("c", "", 1200, 800)};
  histo->Draw("colz");
  func.Draw("same");

  const auto &borders = getter.GetBorders2D();

  double x1 = 0.;
  double x2 = 1.;

  TLine line;
  for (const auto &border : borders) {
    double y1 = border.p0 + border.p1 * x1;
    double y2 = border.p0 + border.p1 * x2;
    line.DrawLine(x1, y1, x2, y2);
  }

  c->Write(name_ + "_histo_2d");

  if (ispdf_)
    c->SaveAs(name_ + "_histo_2d" + ".pdf");

}

}
