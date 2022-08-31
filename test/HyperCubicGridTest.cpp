//===-- HyperCubicGridTest.cpp ---------------------------------*- C++ -*-===//
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
/// @brief      Tests for the HyperCubicGrid Class
///
//===---------------------------------------------------------------------===//
// bwsl
#include <bwsl/HyperCubicGrid.hpp>

// std
#include <vector>

// catch
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace bwsl;
using namespace std;

TEST_CASE("Grid with closed boundaries addressed with indices and coordinates",
          "[index][coordinates]")
{
  auto h = HyperCubicGrid({ 3UL, 4UL }, HyperCubicGrid::boundaries_t::Closed);

  // The grid has the same number of points
  REQUIRE(h.GetNumSites() == 12UL);

  SECTION("Conversion between indices and coordinates")
  {
    for (auto i = 0UL; i < h.GetNumSites(); i++) {
      auto c = h.GetCoordinates(i);
      auto ic = h.GetIndex(c);

      REQUIRE(ic == i);

      auto cc = h.GetCoordinates(ic);
      for (auto j = 0UL; j < h.GetDim(); j++) {
        REQUIRE(cc[j] == c[j]);
      }
    }
  }

  SECTION("Mapping indices works")
  {
    REQUIRE(h.GetMappedSite(0UL, 1UL) == 1UL);
    REQUIRE(h.GetMappedSite(0UL, 2UL) == 2UL);
    REQUIRE(h.GetMappedSite(0UL, 3UL) == 3UL);
    REQUIRE(h.GetMappedSite(0UL, 4UL) == 4UL);
    REQUIRE(h.GetMappedSite(0UL, 5UL) == 5UL);
    REQUIRE(h.GetMappedSite(0UL, 6UL) == 6UL);
    REQUIRE(h.GetMappedSite(0UL, 7UL) == 7UL);
    REQUIRE(h.GetMappedSite(0UL, 8UL) == 8UL);
    REQUIRE(h.GetMappedSite(0UL, 9UL) == 9UL);
    REQUIRE(h.GetMappedSite(0UL, 10UL) == 10UL);
    REQUIRE(h.GetMappedSite(0UL, 11UL) == 11UL);

    REQUIRE(h.GetMappedSite(6UL, 7UL) == 1UL);
    REQUIRE(h.GetMappedSite(6UL, 2UL) == 8UL);
    REQUIRE(h.GetMappedSite(6UL, 1UL) == 11UL);

    REQUIRE(h.GetMappedSite(8UL, 8UL) == 0UL);
    REQUIRE(h.GetMappedSite(2UL, 2UL) == 0UL);
    REQUIRE(h.GetMappedSite(1UL, 1UL) == 0UL);
    REQUIRE(h.GetMappedSite(0UL, 0UL) == 0UL);
  }

  SECTION("Mapping and unmapping indices")
  {
    for (auto i = 0UL; i < h.GetNumSites(); i++) {
      for (auto j = 0UL; j < h.GetNumSites(); j++)
        REQUIRE(h.GetUnMappedSite(h.GetMappedSite(i, j), i) == j);
    }
  }

  SECTION("Checking for jumps")
  {
    auto idxpair =
      vector<vector<size_t>>{ { 1, 8 }, { 8, 1 }, { 0, 6 }, { 6, 0 } };
    auto jumps = vector<HyperCubicGrid::coords_t>{
      { -1, -1 }, { 1, 1 }, { 1, 2 }, { -1, 2 }
    };
    for (auto i = 0; i < idxpair.size(); i++) {
      auto site1 = idxpair[i][0];
      auto site2 = idxpair[i][1];
      REQUIRE(h.GetJump(site1, site2) == jumps[i]);
    }
  }
}

TEST_CASE("Grid with open boundaries addressed with indices and coordinates",
          "[index][coordinates]")
{
  auto h = HyperCubicGrid({ 3UL, 4UL }, HyperCubicGrid::boundaries_t::Open);

  // The grid has the expected number of points
  REQUIRE(h.GetNumSites() == 12UL);

  SECTION("Conversion between indices and coordinates")
  {
    for (auto i = 0UL; i < h.GetNumSites(); i++) {
      auto c = h.GetCoordinates(i);
      auto ic = h.GetIndex(c);

      REQUIRE(ic == i);

      auto cc = h.GetCoordinates(ic);
      for (auto j = 0UL; j < h.GetDim(); j++) {
        REQUIRE(cc[j] == c[j]);
      }
    }
  }

  SECTION("Checking for jumps")
  {
    auto idxpair =
      vector<vector<size_t>>{ { 1, 8 }, { 8, 1 }, { 0, 6 }, { 6, 0 } };
    auto jumps = vector<HyperCubicGrid::coords_t>{
      { 2, -1 }, { -2, 1 }, { 1, 2 }, { -1, -2 }
    };
    for (auto i = 0; i < idxpair.size(); i++) {
      auto site1 = idxpair[i][0];
      auto site2 = idxpair[i][1];
      REQUIRE(h.GetJump(site1, site2) == jumps[i]);
    }
  }
}

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
