#include "Converter1D.h"
#include <iostream>

#include "TProfile.h"
#include "TMath.h"


// ClassImp(Centrality::Converter1D)

namespace Centrality {

void Converter1D::Convert()
{
    std::cout << histo2d_.GetNbinsX() << std::endl;
    std::cout << histo2d_.GetNbinsY() << std::endl;
}
    
    
std::unique_ptr<TF1> Converter1D::Fit2D()
{
    std::unique_ptr<TProfile> prof{ histo2d_.ProfileX() };
    std::unique_ptr<TF1> fitf = new TF1("fit", "pol2", histo2d_.GetXaxis()->GetXmin(), histo2d_.GetXaxis()->GetXmax() );
    
    prof->Fit(fitf);
    return fitf;
}
    
    
/**
 *
 * @param par parameters of polinomial function
 * @param x argument
 * @param kb return value
 * @param N order
 */
void Converter1D::FindNorm (float par[], float x, float kb[], int N)
{
    const float dx = (histo2d_.GetXaxis()->GetXmax() - histo2d_.GetXaxis()->GetXmin()) / 10000. ;

    /* left */
    const float y1 = polN(par, x - dx, N);
    /* right */
    const float y2 = polN(par, x + dx, N);
    
    // cx*x + cy*y + c == 0

    /* 1/df */
    const float cx = 1/(y2 - y1);
    /* 1/2dx */
    const float cy = 0.5/dx;
        
    const float c = -cx*x - cy*polN(par, x, N);
    
    kb[0] = -cx/cy;
    kb[1] = -c/cy;
}    
    

}
