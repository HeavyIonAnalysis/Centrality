/* Copyright (C) 2022, Universität Tübingen
SPDX-License-Identifier: GPL-3.0-only
Authors: Viktor Klochkov */

#include "CentralityFiller.hpp"

#include "TFile.h"
#include <memory>

namespace AnalysisTree {

CentralityFiller::CentralityFiller(const std::string& file, const std::string& getter) {
  auto centr_file = std::unique_ptr<TFile>(TFile::Open(file.c_str(), "read"));

  if ((!centr_file) || (centr_file->IsZombie())) {
    throw std::runtime_error("No file or file is zombie");
  }
  getter_ = centr_file->Get<Centrality::Getter>(getter.c_str());
  if (getter_ == nullptr) {
    throw std::runtime_error("Centrality Getter is nullptr");
  }
}

void CentralityFiller::Init() {
  auto man = TaskManager::GetInstance();
  auto chain = man->GetChain();
  input_ = chain->GetBranchObject(input_name_);
  input_event_header_ = chain->GetBranchObject(input_event_header_name_);

  is_event_header_ = input_.GetBranchType() == DetType::kEventHeader;

  if(is_event_header_)
    in_field_ = input_.GetField(input_field_name_);
  in_branches_.emplace(input_name_);

  auto conf = input_event_header_.GetConfig().Clone(output_name_, AnalysisTree::DetType::kEventHeader);
  conf.AddField<float>(output_field_name_, "centrality, %");
  output_ = Branch(conf);
  out_field_ = output_.GetField(output_field_name_);

  output_.SetMutable();
  output_.Freeze();

  input_.Freeze();
  input_event_header_.Freeze();
  
  if(track_cuts_)
    track_cuts_ -> Init(*(man->GetConfig()));

  man->AddBranch(&output_);
}

void CentralityFiller::Exec() {
  if (is_event_header_) {
    FillFromEventHeader();
  } else {
    FillFromChannels();
  }
}

void CentralityFiller::FillFromEventHeader() {

  const auto value = input_[0][in_field_];
  output_[0].CopyContent(input_[0]);

  const auto centrality = getter_->GetCentrality(value);
  output_[0].SetValue(out_field_, centrality);
}

void CentralityFiller::FillFromChannels() {
  output_[0].CopyContent(input_event_header_[0]);
  const auto n = input_.size();
  int m{0};
  if(!track_cuts_)
    m = n;
  else{
    for(size_t i = 0; i < n; ++i) {
      const auto& track = input_[i];
      if(track_cuts_->Apply(track))
        m++;
    }
  }
  const auto centrality = getter_->GetCentrality(m);
  output_[0].SetValue(out_field_, centrality);
}

}// namespace AnalysisTree