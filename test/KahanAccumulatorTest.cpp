//===-- KahanAccumulatorTest.cpp -------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2016-2022 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// @file
/// @author     Guido Masella (guido.masella@gmail.com)
/// @brief      Tests for the KahanAccumulator Class
///
//===---------------------------------------------------------------------===//
// camarosgf
#include <bwsl/accumulators/KahanAccumulator.hpp>

// std
#include <vector>
#include <iostream>

// catch
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace bwsl::accumulators;
using Catch::Approx;

auto
epsilon() -> double {
  auto eps = 1.0;
  while (1.0 + eps != 1.0) {
    eps = eps / 2.0;
  }
  return eps;
}

// TDD example
TEST_CASE("produce correct sum", "[sum]")
{
  auto k = KahanAccumulator();

  SECTION("Testing on a vector of integers")
  {
    auto v = std::vector<long>{{ 1, 2, 3, 4, 5, 6, 7, 8 }};
    auto s = 0L;

    for (auto x : v) {
      k.Add(static_cast<double>(x));
      s += x;
    }
    REQUIRE(k.Sum() == Approx(s));
  }
}

TEST_CASE("The sum is correct")
{
  auto k = KahanAccumulator();

  SECTION("Rosetta code epsilon test")
  {
    auto eps = epsilon();
    auto directsum = (1.0 + eps) - eps;

    REQUIRE(directsum != 1.0);

    k.Add(1.0);
    k.Add(eps);
    k.Add(-eps);

    REQUIRE(k.Sum() == 1.0);
  }
}


// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
