//===-- AccumulatorTest.cpp ------------------------------------*- C++ -*-===//
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
/// \date       July, 2017
/// \brief      Tests for the Accumulator Class
///
//===---------------------------------------------------------------------===//
// camarosgf
#include <bwsl/Accumulator.hpp>

// std
#include <random>

// catch
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace bwsl;
using namespace std;

TEST_CASE("unweighted averages", "")
{
  auto prova = Accumulator();
  auto count = 2398ul;
  auto testval = 9.89;

  SECTION("adding equal measurements")
  {
    for (auto i = 0ul; i < count; i++) {
      prova.add(testval);
    }
    REQUIRE(prova.GetCount() == count);
    REQUIRE(prova.GetResult() == testval);
  }
  SECTION("adding the result does not changes it")
  {
    for (auto i = 0ul; i < count; i++) {
      prova.add(rand());
    }
    auto res = prova.GetResult();
    prova.add(res);
    REQUIRE(prova.GetCount() == count+1ul);
    REQUIRE(prova.GetResult() == res);
  }
}

TEST_CASE("distributions")
{
  auto count = 10000000ul;
  auto prova = Accumulator();
  auto mean = 5.0;
  auto var = 2.0;
  std::random_device rd{};
  std::mt19937 gen{rd()};
  std::normal_distribution<> d{mean,var};

  SECTION("reproducing mean of a gaussian distribution")
  {

    for (auto i = 0ul; i < count; i++) {
      prova.add(d(gen));
    }

    REQUIRE(prova.GetResult() == Approx(mean).epsilon(1e-3));
  }
}

TEST_CASE("weighted averages", "")
{
  auto prova = Accumulator();
  auto provaunw = Accumulator();
  auto count = 2398ul;
  auto testval = 9.89;
  auto mean = 5.0;
  auto var = 2.0;
  std::random_device rd{};
  std::mt19937 gen{rd()};
  std::normal_distribution<> d{mean,var};

  SECTION("adding equal-weight measurements")
  {
    for (auto i = 0ul; i < count; i++) {
      auto val = d(gen);
      prova.add(val, testval);
      provaunw.add(val);
    }
    REQUIRE(prova.GetCount() == Approx(provaunw.GetCount()).epsilon(1e-3));
    REQUIRE(prova.GetResult() == Approx(provaunw.GetResult()).epsilon(1e-3));
  }
}

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
