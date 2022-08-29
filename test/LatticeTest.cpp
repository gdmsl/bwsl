//===-- LatticeTest.cpp ----------------------------------------*- C++ -*-===//
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
/// @brief      Tests for the Lattice Class
///
//===---------------------------------------------------------------------===//
// bwsl
#include <bwsl/Lattice.hpp>

// std
#include <cmath>
#include <random>
#include <vector>

// catch
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace bwsl;
using CApprox = Catch::Approx;

// TDD example
TEST_CASE("Square Lattice", "[lattice]")
{
  auto structure = Lattice(SquareLattice, std::vector<size_t>{ 3ul, 3ul });

  SECTION("neighbors are correct")
  {
    REQUIRE(structure.AreNeighbors(4, 1));
    REQUIRE(structure.AreNeighbors(1, 4));
    REQUIRE(structure.AreNeighbors(0, 1));
    REQUIRE(structure.AreNeighbors(0, 3));
    REQUIRE(structure.AreNeighbors(0, 6));
    REQUIRE(structure.AreNeighbors(0, 2));
    REQUIRE(structure.AreNeighbors(8, 5));
    REQUIRE(structure.AreNeighbors(8, 7));
    REQUIRE(structure.AreNeighbors(8, 2));
    REQUIRE(structure.AreNeighbors(8, 6));
  }

  SECTION("distances are correct")
  {
    REQUIRE(structure.GetDistance(0, 1) == CApprox(1.0));
    REQUIRE(structure.GetDistance(0, 2) == CApprox(1.0));
    REQUIRE(structure.GetDistance(0, 4) == CApprox(sqrt(2)));
    REQUIRE(structure.GetDistance(0, 8) == CApprox(sqrt(2)));
    REQUIRE(structure.GetDistance(4, 7) == CApprox(1.0));
  }

  SECTION("jumps are correct")
  {
    REQUIRE(structure.GetJump(0, 1)[0] == 0);
    REQUIRE(structure.GetJump(0, 1)[1] == 1);
  }
}

TEST_CASE("Rectangular Lattice")
{
  auto structure = Lattice(SquareLattice, { 3UL, 4UL });

  SECTION("neighbors are correct")
  {
    REQUIRE(structure.AreNeighbors(0, 1));
    REQUIRE(structure.AreNeighbors(0, 4));
    REQUIRE(structure.AreNeighbors(0, 8));
    REQUIRE(structure.AreNeighbors(0, 3));
    REQUIRE(structure.AreNeighbors(7, 4));
    REQUIRE(structure.AreNeighbors(7, 3));
    REQUIRE(structure.AreNeighbors(7, 11));
    REQUIRE(structure.AreNeighbors(7, 6));
    REQUIRE(structure.AreNeighbors(5, 1));
    REQUIRE(structure.AreNeighbors(5, 6));
    REQUIRE(structure.AreNeighbors(5, 4));
    REQUIRE(structure.AreNeighbors(5, 9));
  }

  SECTION("distances are correct")
  {
    REQUIRE(structure.GetDistance(0, 1) == CApprox(1.0));
    REQUIRE(structure.GetDistance(0, 2) == CApprox(2.0));
    REQUIRE(structure.GetDistance(0, 5) == CApprox(sqrt(2.0)));
    REQUIRE(structure.GetDistance(0, 11) == CApprox(sqrt(2.0)));
    REQUIRE(structure.GetDistance(0, 8) == CApprox(1.0));
    REQUIRE(structure.GetDistance(6, 7) == CApprox(1.0));
    REQUIRE(structure.GetDistance(6, 5) == CApprox(1.0));
    REQUIRE(structure.GetDistance(6, 1) == CApprox(sqrt(2.0)));
    REQUIRE(structure.GetDistance(6, 9) == CApprox(sqrt(2.0)));
    REQUIRE(structure.GetDistance(6, 4) == CApprox(2.0));
  }
}

TEST_CASE("Big Square Lattice")
{
  auto structure = Lattice(SquareLattice, std::vector<size_t>{ 16ul, 16ul });
  auto nsites = structure.GetNumSites();
  auto size = structure.GetSize();

  SECTION("distances are correct")
  {
    for (auto i = 0ul; i < nsites; i++) {
      auto coordi = structure.GetCoordinates(i);
      for (auto j = i; j < nsites; j++) {
        if (i == j)
          continue;
        auto coordj = structure.GetCoordinates(j);
        auto distx = coordj[0] - coordi[0];
        auto disty = coordj[1] - coordi[1];
        if (distx > static_cast<long>(size[0] / 2)) {
          distx -= static_cast<long>(size[0]);
        } else if (distx < -static_cast<long>(size[0] / 2)) {
          distx += static_cast<long>(size[0]);
        }
        if (disty > static_cast<long>(size[1] / 2)) {
          disty -= static_cast<long>(size[1]);
        } else if (disty < -static_cast<long>(size[1] / 2)) {
          disty += static_cast<long>(size[1]);
        }
        auto dist =
          sqrt(static_cast<double>(bwsl::square(distx) + bwsl::square(disty)));
        REQUIRE(structure.GetDistance(i, j) == CApprox(dist));
        REQUIRE(structure.GetDistance(j, i) == CApprox(dist));
      }
    }
  }
}

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
