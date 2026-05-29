/* Copyright (C) 2022, Universität Tübingen
SPDX-License-Identifier: GPL-3.0-only
Authors: Viktor Klochkov */

#ifndef CENTRALITY_BORDERSFINDER_TEST_HPP_
#define CENTRALITY_BORDERSFINDER_TEST_HPP_

#include <gtest/gtest.h>

#include "BordersFinder.hpp"

namespace {

using namespace Centrality;

TH1F MakeHistoGaus() {
  TH1F h1("h1", "histo from a gaussian", 100, -3, 3);
  h1.FillRandom("gaus", 100000);
  return h1;
}

TEST(BordersFinder, Basics) {

  BordersFinder bf;

  auto h1 = MakeHistoGaus();
  bf.SetHisto(h1);
  bf.SetRanges(4, 0, 100);
  //   bf.SetRanges( {0,10,30,60,100} );
  bf.IsSpectator(true);

  bf.FindBorders();

  auto ranges = bf.GetRanges();
  auto borders = bf.GetBorders();

  EXPECT_FLOAT_EQ(ranges.at(0), 0);
  EXPECT_FLOAT_EQ(ranges.at(1), 25);
  EXPECT_FLOAT_EQ(ranges.at(2), 50);
  EXPECT_FLOAT_EQ(ranges.at(3), 75);
  EXPECT_FLOAT_EQ(ranges.at(4), 100);

  EXPECT_FLOAT_EQ(borders.at(0), -3);
  EXPECT_NEAR(borders.at(1), -0.6685, 0.05);
  EXPECT_NEAR(borders.at(2), 0.0, 0.05);
  EXPECT_NEAR(borders.at(3), 0.6685, 0.05);
  EXPECT_FLOAT_EQ(borders.at(4), 3);

  //  bf.SaveBorders(outfilename, "centr_getter_1d");
}

TEST(BordersFinder, NonSpectatorPartialRange) {
  BordersFinder bf;

  auto h1 = MakeHistoGaus();
  bf.SetHisto(h1);
  bf.SetRanges(4, 0, 20);
  bf.IsSpectator(false);

  bf.FindBorders();

  auto ranges = bf.GetRanges();
  auto borders = bf.GetBorders();

  // Non-spectator reverses ranges_ in FindBorders, so they are stored high→low
  EXPECT_FLOAT_EQ(ranges.at(0), 20);
  EXPECT_FLOAT_EQ(ranges.at(1), 15);
  EXPECT_FLOAT_EQ(ranges.at(2), 10);
  EXPECT_FLOAT_EQ(ranges.at(3), 5);
  EXPECT_FLOAT_EQ(ranges.at(4), 0);

  // Non-spectator: higher x = more central = lower %; borders increase with decreasing centrality
  EXPECT_LT(borders.at(0), borders.at(1));
  EXPECT_LT(borders.at(1), borders.at(2));
  EXPECT_LT(borders.at(2), borders.at(3));
  EXPECT_LT(borders.at(3), borders.at(4));
}

TEST(BordersFinder, SpectatorPartialRange) {
  BordersFinder bf;

  auto h1 = MakeHistoGaus();
  bf.SetHisto(h1);
  bf.SetRanges(2, 10, 20);
  bf.IsSpectator(true);

  bf.FindBorders();

  auto ranges = bf.GetRanges();
  auto borders = bf.GetBorders();

  EXPECT_FLOAT_EQ(ranges.at(0), 10);
  EXPECT_FLOAT_EQ(ranges.at(1), 15);
  EXPECT_FLOAT_EQ(ranges.at(2), 20);

  // Spectator: higher x = more peripheral = higher %; borders increase with centrality %
  EXPECT_LT(borders.at(0), borders.at(1));
  EXPECT_LT(borders.at(1), borders.at(2));
  // 10% border (most central in range) maps to median region; 20% (peripheral) maps further right
  EXPECT_NEAR(borders.at(1), 0.0, 0.1);
}

}// namespace
#endif