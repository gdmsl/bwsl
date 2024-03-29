//===-- RationalNumTest.cpp ------------------------------------*- C++ -*-===//
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
/// @brief      Tests for the RationalNum Class
///
//===---------------------------------------------------------------------===//
// bwsl
#include <bwsl/MathUtils.hpp>
#include <bwsl/RationalNum.hpp>

// std
#include <vector>

// catch
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace bwsl;

// TDD example
TEST_CASE("Relational operators", "")
{
  auto a = RationalLong(5L, 7L);
  auto b = RationalLong(10L, 11L);

  REQUIRE(a < b);
  REQUIRE(b > a);
  REQUIRE(a <= a);
  REQUIRE(b <= b);
  REQUIRE(a <= b);
  REQUIRE(b >= a);
}

TEST_CASE("Arithmetic operators", "")
{
  auto a = RationalLong(5L, 9L);
  auto b = RationalLong(13L, 3L);

  REQUIRE(a + b == RationalLong(44L, 9L));
  REQUIRE(a - b == RationalLong(-34L, 9L));
  REQUIRE(b - a == RationalLong(34L, 9L));
  REQUIRE(a * b == b * a);
  REQUIRE(a * b == RationalLong(65L, 27L));
  REQUIRE(a / b == RationalLong(5L, 39L));
  REQUIRE(b / a == RationalLong(39L, 5L));

  REQUIRE(a + 10L == RationalLong(95L, 9L));
  REQUIRE(10L + a == a + 10L);
  REQUIRE(10L * a == RationalLong(50L, 9L));
  REQUIRE(11L / a == RationalLong(99L, 5L));
  REQUIRE(a / 11L == RationalLong(5L, 99L));
}

TEST_CASE("Parsing")
{
  auto str = std::string{ "3//2" };
  auto x = RationalLong{};

  FromString(str, x);
  REQUIRE(x == RationalLong{ 3L, 2L });
}

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
