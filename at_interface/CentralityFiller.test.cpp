/* Copyright (C) 2022, Universität Tübingen
SPDX-License-Identifier: GPL-3.0-only
Authors: Viktor Klochkov */

#ifndef CENTRALITY_FILLER_TEST_HPP_
#define CENTRALITY_FILLER_TEST_HPP_

#include <gtest/gtest.h>

#include "AnalysisTree/ToyMC.hpp"
#include "CentralityFiller.hpp"

#include "BordersFinder.hpp"

namespace {

using namespace AnalysisTree;
using namespace Centrality;

void CreateCentralityFile() {
  BordersFinder bf;

  bf.SetRanges(4, 0, 100);
  bf.SetBorders({0, 90, 100, 110, 200});
  bf.IsSpectator(false);

  bf.SaveBorders("getter_test_at_interface.root", "centr_getter_1d");
}

TEST(CentralityFiller, Basics) {

  const int n_events = 1000;
  const std::string filelist = "fl_toy_mc_centrality_filler.txt";

  RunToyMC(n_events, filelist);

  auto* man = TaskManager::GetInstance();
  man->SetOutputName("centrality_filler_test.root", "aTree");
  man->SetWriteMode(eBranchWriteMode::kCopyTree);

  CreateCentralityFile();

  auto* task = new CentralityFiller("getter_test_at_interface.root", "centr_getter_1d");
  task->SetInputEventHeader("SimEventHeader");
  task->SetInput("RecTracks", "ones");
  task->SetOutput("AnaEventHeader", "centrality_tracks");

  man->AddTask(task);

  man->Init({filelist}, {"tTree"});
  man->Run(-1);
  man->Finish();
}

}// namespace

#endif