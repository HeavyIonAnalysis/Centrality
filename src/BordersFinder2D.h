/** @file   BordersFinder2D.h
    @class  Centrality::BordersFinder2D
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
    @brief  Class to determine centrality for 2D case. Inherited from Centrality::BordersFinder.
*/

#ifndef CENTRALITY_BORDERSFINDER2D_H
#define CENTRALITY_BORDERSFINDER2D_H

#include "BordersFinder.h"

#include "vector"
#include "array"

#include "TH2.h"
#include "TF1.h"


namespace Centrality {
    
class BordersFinder2D : public BordersFinder {

public:
    
    BordersFinder2D(){}
        
    void SetHisto2D(TH2F&& histo2d) { histo2d_ = histo2d; }
    TH2F&& GetHisto2D() { return std::move(histo2d_); }
    
    void Init();
    std::unique_ptr<TH1F> Convert();
    void Fit2D(const TString func);
    std::array <float,2> FindNorm (const std::vector <double> par, float x);
    float FindIntegral( const std::array <float,2> &norm1, const std::array <float,2> &norm2);
    void SaveBorders2D(const std::string &filename, const std::string &getter_name);
    
    /**
     *
     * @param par parameters of polinom
     * @param x argument
     * @param N order
     * @return
     */
    float polN (std::vector <double> par, float x)
    {
        float res{0.};
        float xn{1.};
        for (const auto ipar : par){
            res += ipar*xn;
            xn *= x;
        }
        return res;
    }

private:
    
    TH2F histo2d_;
    TF1* fit_{nullptr};
    
    TString fitname_{""};
    
    float xmax_{1.};
    float ymax_{1.};
    
//     ClassDef(BordersFinder2D, 1);
    
};
}

#endif //CENTRALITY_BORDERSFINDER2D_H
