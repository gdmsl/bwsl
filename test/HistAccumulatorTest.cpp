//===-- HistAccumulatorTest.cpp --------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2016-2018 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// \file
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       July, 2018
/// \brief      Tests for the HistAccumulator Class
///
//===---------------------------------------------------------------------===//
// bwsl
#include <bwsl/HistAccumulator.hpp>

// std
#include <algorithm>
#include <vector>
#include <random>
#include <cmath>

// catch
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace bwsl;

auto factorial(int n) -> int
{
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

auto poisson(int n, int mu) -> double {
  return exp(-mu) * std::pow(mu, n) / factorial(n);
}

// TDD example
TEST_CASE("histograms can be constructed", "")
{
  auto n = 10UL;
  auto h = HistAccumulator(n);

  REQUIRE(h.GetNbins() == n);

  SECTION("newly created histograms have all components zero")
  {
    for (auto i = 0UL; i < n; i++) {
      REQUIRE(h.GetResult(i) == 0.0);
    }
  }
}

TEST_CASE("sampling discrete distribution")
{
  std::mt19937 gen(19890501UL);
  auto weights = std::vector<double>{{40, 10, 10, 40}};
  auto s = std::accumulate(weights.begin(), weights.end(), 0.0);
  std::discrete_distribution<> d(weights.begin(), weights.end());
  auto count = 10000000UL;

  auto h = HistAccumulator(weights.size());

  for (auto i = 0UL; i < count; i++) {
    auto val = d(gen);
    h.Add(val);
  }

  for (auto i = 0UL; i < weights.size(); i++) {
    auto p = weights[i] / s;
    REQUIRE(h.GetResult(i) == Approx(p).epsilon(0.01));
  }
}


// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
