/* Copyright (C) 2022, Universität Tübingen
SPDX-License-Identifier: GPL-3.0-only
Authors: Viktor Klochkov */

#ifndef CENTRALITY_AT_INTERFACE_CENTRALITYFILLER_HPP_
#define CENTRALITY_AT_INTERFACE_CENTRALITYFILLER_HPP_

#include <utility>

#include "AnalysisTree/Task.hpp"
#include "AnalysisTree/TaskManager.hpp"
#include "Getter.hpp"

namespace AnalysisTree {

class CentralityFiller : public AnalysisTree::Task {
 public:
  CentralityFiller() = delete;
  CentralityFiller(const std::string& file, const std::string& getter);

  void Init() override;

  void Exec() override;

  void Finish() override {
    delete getter_;
  }
  
  void SetInputEventHeader(std::string branch) { input_event_header_name_ = branch; }

  void SetInput(std::string branch, std::string field = "") {
    input_name_ = std::move(branch);
    input_field_name_ = std::move(field);
  }

  void SetOutput(std::string branch, std::string field) {
    output_name_ = std::move(branch);
    output_field_name_ = std::move(field);
  }
  
  void SetTrackCuts(AnalysisTree::Cuts* track_cuts) { track_cuts_ = track_cuts; }

  ~CentralityFiller() override = default;

 protected:
  void FillFromEventHeader();
  void FillFromChannels();

  std::string input_event_header_name_;
  std::string input_name_;
  std::string input_field_name_;
  std::string output_name_;
  std::string output_field_name_;

  Centrality::Getter* getter_{nullptr};

  AnalysisTree::Branch input_event_header_;
  AnalysisTree::Branch input_;
  AnalysisTree::Branch output_;

  AnalysisTree::Field in_field_;
  AnalysisTree::Field out_field_;

  bool is_event_header_{true};

  AnalysisTree::Cuts* track_cuts_{nullptr};
};

}// namespace AnalysisTree

#endif//CENTRALITY_AT_INTERFACE_CENTRALITYFILLER_HPP_
