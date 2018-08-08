#include "Getter.h"
#include <iostream>

namespace Centrality {


float Getter::GetCentrality(float value) const 
{
    const int ibin = borders_.FindBin(value);    
    if ( ibin == 0 || ibin > borders_.GetNbins() )
        return -1;
    
//     std::cout << value << " " << ranges_.at(ibin-1) << "  " << ranges_.at(ibin) << std::endl; 
    
//     const float centrality = 0.5 * (ranges_.at(ibin-1) + ranges_.at(ibin) );
    const float centrality = 0.5 * ( ranges_.at(ibin-1) + ranges_.at(ibin) );

    return centrality;
}


}
