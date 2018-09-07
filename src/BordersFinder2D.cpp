#include "BordersFinder2D.h"
#include "Getter.h"
#include "BordersFinderHelper.h"

#include <iostream>

#include "TProfile.h"
#include "TMath.h"
#include "TF1.h"
#include "TFile.h"

// ClassImp(Centrality::BordersFinder2D)

namespace Centrality {

void BordersFinder2D::Init()
{
    fitname_ = "pol2";

    for (int iBin=histo2d_.GetNbinsX(); iBin>=1; --iBin)
    {
        if ( histo2d_.Integral(iBin, iBin, 0, histo2d_.GetNbinsY()) >= 1. )
        {
            xmax_ = histo2d_.GetXaxis()->GetBinCenter(iBin);
            break;
        }
    }
    for (int iBin=histo2d_.GetNbinsY(); iBin>=1; --iBin)
    {
        if ( histo2d_.Integral(0, histo2d_.GetNbinsY(), iBin, iBin) >= 1. )
        {
            ymax_ = histo2d_.GetYaxis()->GetBinCenter(iBin);
            break;
        }
    }
    histo2d_.GetXaxis()->SetLimits(0, histo2d_.GetXaxis()->GetXmax()/xmax_);
    histo2d_.GetYaxis()->SetLimits(0, histo2d_.GetYaxis()->GetXmax()/ymax_);
}
    

std::unique_ptr<TH1F> BordersFinder2D::Convert()
{
    Init();
    
    std::unique_ptr<TH1F> histo1d{new TH1F("histo1d", "", histo2d_.GetNbinsX(), histo2d_.GetXaxis()->GetXmin(), histo2d_.GetXaxis()->GetXmax())};
    
    Fit2D(fitname_);
    std::vector <double> par;
    for (int ipar=0; ipar<fit_->GetNpar(); ++ipar)
        par.push_back( fit_->GetParameter(ipar) );
        
    for (int iBin=1; iBin<=histo2d_.GetNbinsX(); ++iBin)
    {
//         if ( histo2d_.Integral(iBin, iBin, 0, histo2d_.GetNbinsY()) <= 1. ) continue;
                
        const float x1 = iBin==1 ? -0.2 : histo2d_.GetXaxis()->GetBinCenter(iBin-1);
        const float x2 = iBin==histo2d_.GetNbinsX() ? 1.2 : histo2d_.GetXaxis()->GetBinCenter(iBin);

        const auto kb1 = FindNorm( par, x1);
        const auto kb2 = FindNorm( par, x2);
        
        const float integral = FindIntegral(kb1, kb2);
//         std::cout << integral << std::endl;

        histo1d->SetBinContent(iBin, integral);
    }
    return std::move(histo1d);
}

float BordersFinder2D::FindIntegral( const std::array <float,2> &norm1, const std::array <float,2> &norm2 )
{
    float sum{0.};
    
    for (int iBinX=1; iBinX<=histo2d_.GetNbinsX(); ++iBinX)
    {
        for (int iBinY=1; iBinY<=histo2d_.GetNbinsY(); ++iBinY)
        {
            const float entries = histo2d_.GetBinContent(iBinX, iBinY);
            if (entries == 0) continue;
            
            const float x = histo2d_.GetXaxis()->GetBinCenter(iBinX);
            const float y = histo2d_.GetYaxis()->GetBinCenter(iBinY);

            const float ynorm1 = norm1[0] + norm1[1] * x;
            const float ynorm2 = norm2[0] + norm2[1] * x;
            
            if (y<ynorm1 && y>ynorm2)
                sum += entries;
        }
    }
    return sum;
}


void BordersFinder2D::SaveBorders2D(std::string filename)
{
    Getter getter;
    
    if (this->GetBorders().size() < 2) return;
    
    TFile *f = TFile::Open(filename.data(), "recreate");
    
    getter.SetRanges(this->GetRanges());
    getter.IsSpectator(this->GetIsSpectator());
    getter.SetMax(xmax_, ymax_);
    
    std::vector <double> par;
    for (int ipar=0; ipar<fit_->GetNpar(); ++ipar)
        par.push_back( fit_->GetParameter(ipar) );

    for ( const auto& iborder : this->GetBorders() )
    {
        const auto kb = FindNorm( par, iborder);
        getter.AddBorder2D(kb);
    }

    BordersFinderHelper h;
    h.QA(getter, this->GetHisto());
    h.PlotHisto(getter, this->GetHisto());
    h.PlotHisto2D(getter, histo2d_, *fit_);
    
    getter.Write("centr_getter2D");
    
    f->Close();
}

    
void BordersFinder2D::Fit2D(const TString func)
{
    std::unique_ptr<TProfile> prof{ histo2d_.ProfileX() };
    fit_ = new TF1("fit", func, histo2d_.GetXaxis()->GetXmin(), histo2d_.GetXaxis()->GetXmax() );    
    prof->Fit(fit_, "Q");
}
    
    
/**
 *
 * @param par parameters of polinomial function
 * @param x argument
 * @return a0 and a1 parameters y = a0 + a1 * x
 */
std::array <float,2> BordersFinder2D::FindNorm (const std::vector <double> par, float x)
{
    std::array <float,2> kb;
    const float dx = (histo2d_.GetXaxis()->GetXmax() - histo2d_.GetXaxis()->GetXmin()) / 10000. ;

    /* left */
    const float y1 = polN(par, x - dx);
    /* right */
    const float y2 = polN(par, x + dx);
        
    // cx*x + cy*y + c == 0

    /* 1/df */
    const float cx = 1/(y2 - y1);
    /* 1/2dx */
    const float cy = 0.5/dx;
        
    const float c = -cx*x - cy*polN(par, x);
    
    kb[1] = -cx/cy;
    kb[0] = -c/cy;    
    return kb;
}    
    

}