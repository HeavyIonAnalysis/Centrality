#include "BordersFinderHelper.h"

#include <iostream>

#include "TRandom.h"
#include "TCanvas.h"
#include "TLine.h"

#include "TStyle.h"



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
    hCentr.Write(name_ + "_qa");
    if (ispdf_)
    {
        std::unique_ptr <TCanvas> c {new TCanvas("c", "", 1200, 800)};
        hCentr.Draw();
        c->SaveAs(name_ + "_qa" + ".pdf");
    }
}

void BordersFinderHelper::PlotHisto(const Getter& getter, TH1F& histo) const
{
    std::unique_ptr <TCanvas> c {new TCanvas("c", "", 1200, 800)};
    histo.Draw();
    
    auto borders = getter.GetBorders();
    TLine *line;

    for (int i=0; i<borders.GetNbins(); ++i)
    {
        const float border = borders.GetBinLowEdge(i+1);
        const int height = histo.GetBinContent( histo.FindBin(border) );
        
        line = new TLine(border, 0, border ,height); 
        line->Draw("same");
    }
    c->SetLogy();
    c->Write(name_ + "_histo_1d");

    if (ispdf_)
        c->SaveAs(name_ + "_histo_1d" + ".pdf");

    delete line;
}

void BordersFinderHelper::PlotHisto2D(const Getter& getter, TH2F& histo, TF1& func) const
{
    gStyle->SetOptStat(0000);
    
    std::unique_ptr <TCanvas> c {new TCanvas("c", "", 1000, 1000)};
    histo.GetXaxis()->SetRangeUser(0,1.1);
    histo.GetYaxis()->SetRangeUser(0,1.1);
    histo.Draw("colz");
    func.SetLineColor(kBlack);
    func.Draw("same");
    
    const auto& borders = getter.GetBorders2D();
    TLine *line{nullptr};

    for (uint i=0; i<borders.size()-1; ++i)
    {

      float x1{0.};
      float x2{1.};
      float x{0.5};
      
      for(int iter=0; iter<10; ++iter)
      {
        x=(x1+x2)/2;
        
        if ( (func.Eval(x1) - borders.at(i)[0] - borders.at(i)[1] * x1) * (func.Eval(x) - borders.at(i)[0] - borders.at(i)[1] * x) < 0 )
          x2=x;
        else
          x1=x;
      }
      x1 = x - 0.06*borders.at(i)[1];
      x2 = x + 0.06*borders.at(i)[1];
      if (x1 < 0) x1=0;
      
      float y1 = borders.at(i)[0] + borders.at(i)[1] * x1;
      float y2 = borders.at(i)[0] + borders.at(i)[1] * x2;
      
//       if (y1 < 0) y1=0;
      
      line = new TLine(x1, y1, x2 ,y2); 
      line->SetLineWidth(2);
      line->SetLineColor(kRed);
      line->Draw("same");
    }

    c->SetLogz(true);
    c->Write(name_ + "_histo_2d");
    
    if (ispdf_)
        c->SaveAs(name_ + "_histo_2d" + ".pdf");

    delete line;
}

    

}
