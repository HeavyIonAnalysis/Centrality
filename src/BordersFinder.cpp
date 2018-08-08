#include "BordersFinder.h"
#include "Getter.h"
#include "BordersFinderHelper.h"

#include <iostream>

#include "TFile.h"

namespace Centrality {

void BordersFinder::FindBorders()
{
    if (ranges_.size() < 2) return;
    if (norm_ == -1) norm_ = histo_.GetEntries();
    if (!isspectator_) std::reverse(std::begin(ranges_), std::end(ranges_));
    
    uint iSlice{0};
    long int entriesCurrent{0};
    
    for (uint iBin=1; iBin<=histo_.GetNbinsX() && iSlice<ranges_.size() ; ++iBin)
    {
        const float step = isspectator_ ? ranges_.at(iSlice) : 100. - ranges_.at(iSlice);
        const long int entriesNeeeded = step/100. * norm_;
        entriesCurrent += histo_.GetBinContent(iBin);
        
        if (entriesCurrent >= entriesNeeeded)
        {
            const float ratio = histo_.GetBinContent(iBin)>0 ? (entriesCurrent - entriesNeeeded) / histo_.GetBinContent(iBin) : 0;
            const float border = histo_.GetBinLowEdge(iBin) + histo_.GetBinWidth(iBin) * ratio;
            
            borders_.push_back(border);
            
            std::cout << "!!!!!" << border << std::endl;;
            iSlice++;
        }
    }
}

void BordersFinder::SaveBorders(std::string filename)
{
    Getter getter;
    
    if (borders_.size() < 2) return;
    
    TFile *f = TFile::Open(filename.data(), "recreate");
    
    getter.SetBorders(borders_);
    getter.SetRanges(ranges_);
    getter.IsSpectator(isspectator_);

    BordersFinderHelper h;
    h.QA(getter, histo_);
    
    getter.Write("getter");
    
    f->Close();
    
    
}



}
