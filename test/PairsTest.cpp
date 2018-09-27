//===-- PairsTest.cpp ------------------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2017 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// \file
/// \version    0.1
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       May, 2017
/// \brief      Tests for Pairs functions
///
//===---------------------------------------------------------------------===//
// bwsl
#include <bwsl/Pairs.hpp>

// std
#include <iostream>

// catch
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace bwsl::pairs;

// TDD example
TEST_CASE("Pairs with self pair", "[math]")
{
  SECTION("GetPairIndex is inverted with GetPair")
  {
    const auto n = 3489;
    for (auto s1 = 0ul; s1 < n; s1++) {
      for (auto s2 = s1; s2 < n; s2++) {
        auto k = GetPairIndex(s1,s2,n, false);
        auto indices = GetPair(k, n, false);
        REQUIRE(s1 == indices.first);
        REQUIRE(s2 == indices.second);
      }
    }
  }
}

TEST_CASE("Pairs withoud self pair", "[math]")
{
  SECTION("GetPairIndex is inverted with GetPair")
  {
    const auto n = 3489;
    for (auto s1 = 0ul; s1 < n; s1++) {
      for (auto s2 = s1+1ul; s2 < n; s2++) {
        auto k = GetPairIndex(s1,s2,n, true);
        auto indices = GetPair(k, n, true);
        REQUIRE(s1 == indices.first);
        REQUIRE(s2 == indices.second);
      }
    }
  }
}

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
