//
// Created by Viktor Klochkov on 08.08.18.
//

#ifndef CENTRALITY_GETTER_H
#define CENTRALITY_GETTER_H

#include "vector"

#include "TAxis.h"
#include "TRandom.h"

namespace Centrality {
    
class Getter {

public:
    
    Getter(){}
    
    float GetCentrality(float value);
    
    void SetBorders(const std::vector<double> &borders)
    {
        borders_ = TAxis( borders.size()-1, &(borders[0]) );
    }
    
    void SetRanges(const std::vector<float> &ranges) { ranges_ = ranges; }

    
private:
    
    TAxis borders_;
    std::vector<float> ranges_;

    TRandom random_;
    
};
}

#endif //CENTRALITY_GETTER_H
