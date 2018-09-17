/** @file   Container.h
    @class  Centrality::Container
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
    @brief  Class to store centrality information
*/

#ifndef CENTRALITY_CONTAINER_H
#define CENTRALITY_CONTAINER_H

#include <map>

namespace Centrality {

class Container{

public:
    
    Container(){}
    
    void AddCentralityEstimator(uint num, float centrality)
    {
        centrality_.insert(std::make_pair(num, centrality));
    }
    
    float GetCentrality(uint num) const 
    { 
        auto find = centrality_.find(num);        
        return find != centrality_.end() ? find->second : -1;
    }
    
    void Clear() { centrality_.clear(); }
    
private:
    
    std::map <uint, float> centrality_;
    
//     ClassDef(Container, 1);
    
};
}

#endif //CENTRALITY_CONTAINER_H
