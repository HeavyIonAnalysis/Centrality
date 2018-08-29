//
// Created by Viktor Klochkov on 08.08.18.
//

#ifndef CENTRALITY_BORDERSFINDERHELPER_H
#define CENTRALITY_BORDERSFINDERHELPER_H

#include "TH1.h"
#include "TH2.h"
#include "TF1.h"

#include "Getter.h"

namespace Centrality {
    
class BordersFinderHelper {

public:
    
    BordersFinderHelper(){}
    void QA(const Getter& getter, const TH1F &histo) const;
    void PlotHisto(const Getter& getter, TH1F& histo) const;
    void PlotHisto2D(const Getter& getter, TH2F& histo, TF1& func) const;

    
private:
    
};

}

#endif //CENTRALITY_BORDERSFINDERHELPER_H
