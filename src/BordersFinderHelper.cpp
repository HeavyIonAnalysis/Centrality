#include "BordersFinderHelper.h"

#include <iostream>

#include "TRandom.h"

namespace Centrality {

void BordersFinderHelper::QA(const Getter& getter, const TH1F& histo)
{
    auto ranges = getter.GetRanges();
    std::sort (ranges.begin(), ranges.end());
           
    TH1D hCentr("hCentr", "", ranges.size()-1, &(ranges.front()) );
    
    TRandom *r = new TRandom;
        
    for (Int_t iBin = 0; iBin<histo.GetNbinsX(); ++iBin )
    {
        const Float_t Mult = histo.GetBinCenter(iBin+1) + (r->Rndm()-0.5);
        if (Mult <= 0) continue;
        
//         std::cout << Mult << "  " << getter.GetCentrality(Mult) << std::endl;
        
        for (Int_t j = 0; j<histo.GetBinContent(iBin+1); ++j )
        {
            hCentr.Fill( getter.GetCentrality(Mult) );
        }
    }
    hCentr.Write("qa_histo");
}

void BordersFinderHelper::PlotHisto(const Getter& getter, const TH1F& histo)
{
    
}


    

}
