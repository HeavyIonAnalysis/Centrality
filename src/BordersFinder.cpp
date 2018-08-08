#include "BordersFinder.h"
#include "Getter.h"

#include <iostream>

#include "TFile.h"

namespace Centrality {

void BordersFinder::FindBorders()
{
    if (ranges_.size() < 2) return;
    if (norm_ == -1) norm_ = histo_.GetEntries();
    
    uint iSlice{0};
    long int entriesCurrent{0};
    
    for (uint iBin=1; iBin<=histo_.GetNbinsX() && iSlice<ranges_.size() ; ++iBin)
    {
        const long int entriesNeeeded = ranges_.at(iSlice)/100. * norm_;
        entriesCurrent += histo_.GetBinContent(iBin);
        
//         std::cout << entriesNeeeded << " " << entriesCurrent << std::endl;
        
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
    
    std::cout << getter.GetCentrality(100) << std::endl;
    
}



}
