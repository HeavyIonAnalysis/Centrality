//
// Created by Viktor Klochkov on 24.01.18.
//

#include <iostream>
#include "DataFiller.h"

namespace Interface {

DataFiller::DataFiller(std::string filelist, std::string incalib, std::string treename) :
    in_tree_(this->MakeChain(filelist, treename))
{
  event_ = new DataTreeEvent();
  in_tree_->SetBranchAddress("fDTEvent", &event_);
}

std::unique_ptr<TChain> DataFiller::MakeChain(std::string filename, std::string treename) {
  std::unique_ptr<TChain> chain(new TChain(treename.data()));
  std::ifstream in;
  in.open(filename);
  std::string line;
  std::cout << "Adding files to chain:" << std::endl;
  while ((in >> line).good()) {
    if (!line.empty()) {
      chain->AddFile(line.data());
      std::cout << line << std::endl;
    }
  }
  std::cout << "Number of entries = " << chain->GetEntries() << std::endl;
  return chain;
}

// std::unique_ptr<TChain> DataFiller::MakeChain(std::string filename, std::string treename) {
//   std::unique_ptr<TChain> chain(new TChain(treename.data()));
// 
//   std::cout << "Adding files to chain:" << std::endl;
//   chain->Add(filename.data());
//   
//   event_ = new DataTreeEvent();
//   chain->SetBranchAddress("fDTEvent", &event_);
//   
//   std::cout << "Number of entries = " << chain->GetEntries() << std::endl;
// 
// return chain;
// }







}
