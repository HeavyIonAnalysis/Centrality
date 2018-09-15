#include "Getter.h"
#include <iostream>

ClassImp(Centrality::Getter)

namespace Centrality {


float Getter::GetCentrality(float value) const 
{
    if (!isinitialized_) 
    {
        std::cout << "Centrality::Getter is not initialized!" << std::endl;
        exit(-1);
    }
    const int ibin = borders_.FindBin(value);    
    if ( ibin == 0 || ibin > borders_.GetNbins() )
        return -1;
    
//     std::cout << value << " " << ranges_.at(ibin-1) << "  " << ranges_.at(ibin) << std::endl; 
    
    const float centrality = 0.5 * ( ranges_.at(ibin-1) + ranges_.at(ibin) );

    return centrality;
}

float Getter::GetCentrality(float xvalue, float yvalue) const 
{
    if (!isinitialized2D_) 
    {
        std::cout << "Centrality::Getter is not initialized!" << std::endl;
        exit(-1);
    }
    
    xvalue /= xmax_;
    yvalue /= ymax_;
    
    for (uint iborder=0; iborder<borders2d_.size()-1; ++iborder )
    {
        const float y1 = borders2d_.at(iborder)[0] + borders2d_.at(iborder)[1]*xvalue ;
        const float y2 = borders2d_.at(iborder+1)[0] + borders2d_.at(iborder+1)[1]*xvalue ;
        
        if ( yvalue < y1 && yvalue > y2)
            return 0.5 * ( ranges_.at(iborder-1) + ranges_.at(iborder) );
    }
    
    return -1.;
}

}
