//===-- PairsTest.cpp ------------------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2017-2022 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// @file
/// @author     Guido Masella (guido.masella@gmail.com)
/// @brief      Tests for Pairs functions
///
//===---------------------------------------------------------------------===//
// bwsl
#include <bwsl/Pairs.hpp>

// std
#include <iostream>

// catch
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace bwsl::pairs;

// TDD example
TEST_CASE("Pairs with self pair", "[math]")
{
  SECTION("GetPairIndex is inverted with GetPair")
  {
    const auto n = 489;
    for (auto s1 = 0ul; s1 < n; s1++) {
      for (auto s2 = s1; s2 < n; s2++) {
        auto k = GetPairIndex(s1, s2, n);
        auto indices = GetPair(k, n);
        REQUIRE(s1 == indices.first);
        REQUIRE(s2 == indices.second);
      }
    }
  }
}

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
