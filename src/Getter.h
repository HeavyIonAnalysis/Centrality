//
// Created by Viktor Klochkov on 08.08.18.
//

#ifndef CENTRALITY_GETTER_H
#define CENTRALITY_GETTER_H

#include "vector"

#include "TObject.h"
#include "TAxis.h"
#include "TRandom.h"

namespace Centrality {
    
class Getter : public TObject {

public:
    
    Getter(){}
    
    float GetCentrality(float value) const;
    
    void SetBorders(const std::vector<double> &borders)
    {
        borders_ = TAxis( borders.size()-1, &(borders[0]) );
    }
    
    const TAxis& GetBorders() const { return  borders_; };
    const std::vector<float>& GetRanges() const { return  ranges_; };

    
    void SetRanges(const std::vector<float> &ranges) { ranges_ = ranges; }
    void IsSpectator(bool is=true) { isspectator_ = is; }
    
private:
    
    TAxis borders_;
    std::vector<float> ranges_;

    bool isspectator_{false};

    ClassDef(Getter, 2);

    
};
}

#endif //CENTRALITY_GETTER_H
