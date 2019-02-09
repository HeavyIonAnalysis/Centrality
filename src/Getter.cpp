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

Getter *Getter::Create1DGetter(std::vector<double> borders) {
  typedef decltype(ranges_)::value_type Floating_t;

  size_t n_borders = borders.size();
  assert(n_borders > 2);

  Floating_t range_max = 100.f;
  Floating_t range_min = 0.f;
  Floating_t range_step = (range_max - range_min)/(n_borders - 1);

  decltype(ranges_) ranges(n_borders);
  for (int i = 0; i < n_borders; ++i) {
    auto rr = range_min + range_step*i;
    ranges[i] = rr;

    std::cout << rr << "%: " << borders[i] << std::endl;
  }

  TAxis ax_borders(static_cast<Int_t>(n_borders - 1), &(borders[0]));


  auto getter = new Getter;
  getter->ranges_ = std::move(ranges);
  getter->borders_ = ax_borders;
  getter->isinitialized_ = true;

  return getter;
}

}
