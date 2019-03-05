#include "BordersFinder.h"
#include "Getter.h"
#include "BordersFinderHelper.h"

#include <iostream>
#include <TGraph.h>

#include "TFile.h"

namespace Centrality {

void BordersFinder::FindBorders()
{
    using namespace std;

    if (ranges_.size() < 2) return;
    if (norm_ == -1) norm_ = histo_.Integral(0, histo_.GetNbinsX());
    if (!isSpectator_) std::reverse(std::begin(ranges_), std::end(ranges_));

    auto axis = histo_.GetXaxis();

    double xLo = (applyLimits_)? xLo_ : axis->GetXmin();
    double xHi = (applyLimits_)? xHi_ : axis->GetXmax();


    int n = axis->GetNbins();

    double *histIntegral = histo_.GetIntegral();
    double x[n];
    for (int i = 0; i < n; ++i) {
        x[i] = axis->GetBinCenter(i+1);
    }

    TGraph intVsXGraph(n, x, histIntegral);
    intVsXGraph.SetBit(TGraph::kIsSortedX);

    TGraph xVsIntGraph(n, histIntegral, x);
    xVsIntGraph.SetBit(TGraph::kIsSortedX);

    double intLo = intVsXGraph.Eval(xLo);
    double intHi = intVsXGraph.Eval(xHi);
    double norm = intHi - intLo;

    auto cX = [=] (double x) { return 100./norm*(intVsXGraph.Eval(x) - intVsXGraph.Eval(xLo)); };
    auto xC = [=] (double c) { return xVsIntGraph.Eval((c/100.)*norm + intLo); };

    for (auto cc : ranges_) {
        double xx = isSpectator_? xC(cc) : xC(100 - cc);
        cout << cc << "%" << ", border:" << xx << endl;
        borders_.push_back(xx);
    }

/*
    uint iSlice{0};
    long int entriesCurrent{0};
    
    for (int iBin=1; iBin<=histo_.GetNbinsX() && iSlice<ranges_.size() ; ++iBin)
    {
        const float step = isSpectator_ ? ranges_.at(iSlice) : 100. - ranges_.at(iSlice);
        const long int entriesNeeeded = step/100. * norm_;
        entriesCurrent += histo_.GetBinContent(iBin);
        
        if (entriesCurrent >= entriesNeeeded)
        {
            const float ratio = histo_.GetBinContent(iBin)>0 ? (entriesCurrent - entriesNeeeded) / histo_.GetBinContent(iBin) : 0;
            const float border = histo_.GetBinLowEdge(iBin) + histo_.GetBinWidth(iBin) * ratio;
            
            borders_.push_back(border);
            
            std::cout << ranges_.at(iSlice) << "%, border: " << border << std::endl;;
            iSlice++;
        }
    }
*/
}

void BordersFinder::SaveBorders(const std::string &filename, const std::string &getter_name)
{
    Getter getter;
    
    if (this->GetBorders().size() < 2) return;
    
    std::unique_ptr<TFile> f{TFile::Open(filename.data(), "update")};
    
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

}
