/* Copyright (C) 2022, Universität Tübingen
SPDX-License-Identifier: GPL-3.0-only
Authors: Viktor Klochkov */

#ifndef CENTRALITY_GETTER_TEST_HPP_
#define CENTRALITY_GETTER_TEST_HPP_

#include <gtest/gtest.h>

#include "Getter.hpp"

namespace {

using namespace Centrality;

TEST(Getter, Basics) {

  Getter getter;
  getter.IsSpectator(true);

  getter.SetBorders({0, 1, 2, 3, 4});
  getter.SetRanges({0, 25, 50, 75, 100});

  auto axis = getter.GetBorders();

  EXPECT_EQ(axis.GetXmin(), 0);
  EXPECT_EQ(axis.GetXmax(), 4);
  EXPECT_EQ(axis.GetNbins(), 4);

  EXPECT_EQ(getter.GetCentrality(0.5), 12.5);
  EXPECT_EQ(getter.GetCentrality(1.5), 37.5);

}

}
#endif