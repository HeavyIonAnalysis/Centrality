#include "BordersFinderHelper.h"

#include <iostream>

#include "TRandom.h"
#include "TCanvas.h"
#include "TLine.h"

namespace Centrality {

void BordersFinderHelper::QA(const Getter& getter, const TH1F& histo) const
{
    auto ranges = getter.GetRanges();
    std::sort (ranges.begin(), ranges.end());
           
    TH1D hCentr("hCentr", "", ranges.size()-1, &(ranges.front()) );
    
    std::unique_ptr <TRandom> r {new TRandom};
        
    for (Int_t iBin = 0; iBin<histo.GetNbinsX(); ++iBin )
    {
        const Float_t Mult = histo.GetBinCenter(iBin+1);
        
        for (Int_t j = 0; j<histo.GetBinContent(iBin+1); ++j )
        {
            hCentr.Fill( getter.GetCentrality(Mult + (r->Rndm()-0.5)) );
        }
    }
    hCentr.Write("qa_histo");
}

void BordersFinderHelper::PlotHisto(const Getter& getter, TH1F& histo) const
{
    std::unique_ptr <TCanvas> c {new TCanvas("c", "", 1200, 800)};
    histo.Draw();
    
    auto borders = getter.GetBorders();
    TLine *line;

    for (uint i=0; i<borders.GetNbins(); ++i)
    {
        const float border = borders.GetBinLowEdge(i+1);
        const int height = histo.GetBinContent( histo.FindBin(border) );
        
        line = new TLine(border, 0, border ,height); 
        line->Draw("same");
    }
    c->SetLogy();
    c->Write("histo");
    delete line;
}


    

}
