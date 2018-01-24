//
// Created by Viktor Klochkov on 24.01.18.
//

#ifndef DATA_FILLER_H
#define DATA_FILLER_H

#include <vector>
#include <array>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TTreeReader.h"

#include "DataTreeEvent.h"

namespace Interface {
/**
 * @brief Centrality Framework interface for DataTree
 */
class DataFiller {
 public:
  DataFiller() = default;
  DataFiller(std::string filelist, std::string incalib, std::string treename);
  DataFiller(std::array<std::shared_ptr<TFile>, 4> files);
  ~DataFiller() = default;

 private:
  /**
   * Make TChain from file list
   * @param filename name of file containing paths to root files containing the input trees
   * @return Pointer to the TChain
   */
 virtual std::unique_ptr<TChain> MakeChain(std::string filename, std::string treename);

protected:
  std::unique_ptr<TTree> in_tree_;
  DataTreeEvent *event_;  
  bool write_tree_;

};
}
#endif //FLOW_QNTASK_H
