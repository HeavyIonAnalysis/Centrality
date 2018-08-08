//
// Created by Viktor Klochkov on 24.01.18.
//

#ifndef CENTRALITY_EVENT_H
#define CENTRALITY_EVENT_H

#include "Detector.h"

namespace Centrality {

class Event {
 public:
  Event() {}

 private:
  
  int runid_ = 0;   
  int eventid_ = 0;

  std::vector<Detector> detectors_;

};
}

#endif //CENTRALITY_EVENT_H
