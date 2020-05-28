//===-- MoveStatsTest.cpp --------------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2016-2019 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// @file
/// @author     Guido Masella (guido.masella@gmail.com)
/// @brief      Tests for the MoveStats Class
///
//===---------------------------------------------------------------------===//
// bwsl
#include <bwsl/MonteCarloUtils.hpp>

// std
#include <vector>

// catch
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace bwsl::montecarlo;

// TDD example
TEST_CASE("vectors can be sized and resized", "[vector]")
{

  auto movestats = MoveStats("TestMove");

  SECTION("")
  {
    for (auto i = 0UL; i < 600UL; i++) {
      movestats.Propose();
      movestats.Accept(0.6);
    }
    for (auto i = 0UL; i < 400UL; i++) {
      movestats.Propose();
      movestats.Reject(0.6);
    }

    REQUIRE(movestats.GetAcceptedRatio() == Approx(0.6));
    REQUIRE(movestats.GetAverageProbability() == Approx(0.6));
    REQUIRE(movestats.GetRejectedRatio() == Approx(0.4));
    REQUIRE(movestats.GetImpossibleRatio() == Approx(0.0));
  }
}

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
