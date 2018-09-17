/** @file   BordersFinderHelper.h
    @class  Centrality::BordersFinderHelper
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
    @brief  Class for centrality QA and plots producing
*/

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

    void SetName(const TString name) { name_ = name; }
    void SetIsPdf(bool is = true) { ispdf_ = is; }
private:
    
    TString name_{"test"};
    bool ispdf_{false};
};

}

#endif //CENTRALITY_BORDERSFINDERHELPER_H
