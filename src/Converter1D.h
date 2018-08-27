//
// Created by Viktor Klochkov on 08.08.18.
//

#ifndef CENTRALITY_CONVERTER1D_H
#define CENTRALITY_CONVERTER1D_H

#include "vector"

#include "TH2.h"
#include "TH1.h"


namespace Centrality {
    
class Converter1D {

public:
    
    Converter1D(){}
        
    void SetHisto2D(TH2F&& histo2d) { histo2d_ = histo2d; }
    TH2F&& GetHisto2D() { return std::move(histo2d_); }
    
    void Convert();
    std::unique_ptr<TF1> Fit2D();

    void FindNorm (float par[], float x, float kb[], int N);


    /**
     *
     * @param par parameters of polinom
     * @param x argument
     * @param N order
     * @return
     */
    float polN (float par[], float x, uint N)
    {
        float res = par[0];
        float xn = 1.;
        for (uint i=1; i<N;  ++i){
            xn *= x;
            res += par[i]*xn;
        }
        return res;
    }

private:
    
    TH2F histo2d_;
    TH1F histo1d_;
    
    
//     ClassDef(Converter1D, 1);
    
};
}

#endif //CENTRALITY_CONVERTER1D_H
