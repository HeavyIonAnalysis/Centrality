//
// Created by Viktor Klochkov on 24.01.18.
//

#ifndef CENTRALITY_DETECTOR_H
#define CENTRALITY_DETECTOR_H

namespace Centrality {
enum class DetectorType {
  Track,
  Channel
};

class Detector {
 public:
  Detector() {}

  Detector(DetectorType type, int nchannels) :
      nchannels_(nchannels),
      type_(type)
  {
  }

  float GetDetectorSignal(int subevent)
  {
      float signal = 0.;
      for (iChannel : weights_, iCut : cutsmap_)
      {
          if (iCut.at(subevent)) 
              signal += iChannel;
      }
      return signal;
  }
  
 private:
  
  int detid_ = 0;   
  int nchannels_ = 0;

  DetectorType type_;
  std::vector<float> weights_;
  std::vector<std::vector<bool>> cutsmap_;

};
}

#endif //CENTRALITY_DETECTOR_H
