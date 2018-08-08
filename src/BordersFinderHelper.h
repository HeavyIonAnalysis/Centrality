//
// Created by Viktor Klochkov on 08.08.18.
//

#ifndef CENTRALITY_BORDERSFINDERHELPER_H
#define CENTRALITY_BORDERSFINDERHELPER_H

#include "TH1.h"

#include "Getter.h"

namespace Centrality {
    
class BordersFinderHelper {

public:
    
    BordersFinderHelper(){}
    void QA(const Getter& getter, const TH1F &histo);
    void PlotHisto(const Getter& getter, const TH1F& histo);

    
private:
    
};

}

#endif //CENTRALITY_BORDERSFINDERHELPER_H
