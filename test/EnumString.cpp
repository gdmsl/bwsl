//===-- EnumStringTest.cpp -------------------------------------*- C++ -*-===//
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
/// @brief      Tests for the EnumString Macro
///
//===---------------------------------------------------------------------===//

// bwsl
#include <bwsl/EnumString.hpp>

// std
#include <vector>

BWSL_DEFINE_ENUM_WITH_STRING_CONVERSIONS(OS_type, (Linux)(Apple)(Windows))

// catch
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

// TDD example
TEST_CASE("conversion works", "[vector]")
{
  OS_type t = Windows;

  SECTION("converting to a string")
  {
    REQUIRE(ToString(t) == "Windows");
  }
  SECTION("converting from a string")
  {
    OS_type t2{};
    FromString(t2, "Windows");
    REQUIRE(t2 == t);
  }
}

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
