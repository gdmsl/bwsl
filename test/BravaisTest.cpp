//===-- HistAccumulatorTest.cpp --------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2016-2022 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// @file
/// @author     Tanul Gupta (tanulgupta123@gmail.com)
/// @brief      Tests for the Bravais Class
///
//===---------------------------------------------------------------------===//

// bwsl
#include <bwsl/Bravais.hpp>
#include <stdlib.h>

// catch
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace bwsl;
using namespace std;
using CApprox = Catch::Approx;

// Tests

TEST_CASE("Chain Lattice")
{
  auto bravais=ChainLattice;
  vector<Bravais::coords_t> coords{{0},{1},{-2}};
  vector<Bravais::realvec_t> realspace{{0.0},{1.0},{-2.0}};
  vector<Bravais::realvec_t> reciprocalspace{{0.0},{1.0},{-2.0}};

  vector<Bravais::coords_t> points{{2}};
  vector<Bravais::coords_t> neighbors{{3,1}};

  SECTION("Real space tests")
  {
    for (auto i=0; i < coords.size(); i++){
      auto eval_realspace = bravais.GetRealSpace(coords[i]);
      for (auto j=0; j < eval_realspace.size(); j++) {
        REQUIRE(realspace[i][j] == CApprox(eval_realspace[j]));
      }
    }
  }
  SECTION("Reciprocal space tests")
  {
    for (auto i=0; i < coords.size(); i++){
      auto eval_reciprocalspace = bravais.GetReciprocalSpace(coords[i]);
      for (auto j=0; j < eval_reciprocalspace.size(); j++) {
        REQUIRE(2*M_PI*reciprocalspace[i][j] == CApprox(eval_reciprocalspace[j]));
      }
    }
  }

  SECTION("GetNeighbor tests"){
    for (auto idx=0; idx < points.size(); idx++) // points represent neighbors
    {
      auto p = points[idx];
      auto n = bravais.GetNeighbor(p, idx);
      for (auto j=0; j < bravais.GetDim(); j++)
        REQUIRE(n[j] == neighbors[idx][j]);
    }
  }

}

TEST_CASE("Square Lattice")
{
  auto bravais=SquareLattice;
  vector<Bravais::coords_t> coords{{0,0},{1,0},{-1,2}};
  vector<Bravais::realvec_t> realspace{{0.0,0.0},{1.0,0.0},{-1.0,2.0}};
  vector<Bravais::realvec_t> reciprocalspace{{0.0,0.0},{1.0,0.0},{-1.0,2.0}};

  vector<Bravais::coords_t> points{{2,2}};
  vector<Bravais::coords_t> neighbors{{3,2},{1,2},{2,3},{2,1}};

  SECTION("Real space tests")
  {
    for (auto i=0; i < coords.size(); i++){
      auto eval_realspace = bravais.GetRealSpace(coords[i]);
      for (auto j=0; j < eval_realspace.size(); j++) {
        REQUIRE(realspace[i][j] == CApprox(eval_realspace[j]));
      }
    }
  }

  SECTION("Reciprocal space tests")
  {
    for (auto i=0; i < coords.size(); i++){
      auto eval_reciprocalspace = bravais.GetReciprocalSpace(coords[i]);
      for (auto j=0; j < eval_reciprocalspace.size(); j++) {
        REQUIRE(2*M_PI*reciprocalspace[i][j] == CApprox(eval_reciprocalspace[j]));
      }
    }
  }

  SECTION("GetNeighbor tests"){
    for (auto idx=0; idx < points.size(); idx++) // points represent neighbors
    {
      auto p = points[idx];
      auto n = bravais.GetNeighbor(p, idx);
      for (auto j=0; j < bravais.GetDim(); j++)
          REQUIRE(n[j] == neighbors[idx][j]);
    }
  }
}

TEST_CASE("Cubic Lattice")
{
  auto bravais=CubicLattice;
  vector<Bravais::coords_t> coords{{0,0,0},{1,0,1},{-1,2,3}};
  vector<Bravais::realvec_t> realspace{{0.0,0.0,0.0},{1.0,0.0,1.0},{-1.0,2.0,3.0}};
  vector<Bravais::realvec_t> reciprocalspace{{0.0,0.0,0.0},{1.0,0.0,1.0},{-1.0,2.0,3.0}};

  vector<Bravais::coords_t> points{{2,2,2}};
  vector<Bravais::coords_t> neighbors{{3,2,2},{1,2,2},{2,3,2},{2,1,2},{2,2,1},{2,2,3}};

  SECTION("Real space tests")
  {
    for (auto i=0; i < coords.size(); i++){
      auto eval_realspace = bravais.GetRealSpace(coords[i]);
      for (auto j=0; j < eval_realspace.size(); j++) {
        REQUIRE(realspace[i][j] == CApprox(eval_realspace[j]));
      }
    }
  }

  SECTION("Reciprocal space tests")
  {
    for (auto i=0; i < coords.size(); i++){
      auto eval_reciprocalspace = bravais.GetReciprocalSpace(coords[i]);
      for (auto j=0; j < eval_reciprocalspace.size(); j++) {
        REQUIRE(2*M_PI*reciprocalspace[i][j] == CApprox(eval_reciprocalspace[j]));
      }
    }
  }

  SECTION("GetNeighbor tests"){
    for (auto idx=0; idx < points.size(); idx++) // points represent neighbors
    {
      auto p = points[idx];
      auto n = bravais.GetNeighbor(p, idx);
      for (auto j=0; j < bravais.GetDim(); j++)
        REQUIRE(n[j] == neighbors[idx][j]);
    }
  }
}

TEST_CASE("Triangular Lattice")
{
  auto bravais=TriangularLattice;
  vector<Bravais::coords_t> coords{{0,0},{1,0},{1,1}};
  vector<Bravais::realvec_t> realspace{{0.0,0.0},{1.0,0.0},{1.5, sqrt(3.0)/2.0}};
  vector<Bravais::realvec_t> reciprocalspace{{0.0,0.0},{1.0,-1.0/sqrt(3.0)},{1.0, 1.0/sqrt(3.0)}};

  vector<Bravais::coords_t> points{{2,2}};
  vector<Bravais::coords_t> neighbors{{3,2},{1,2},{2,3},{2,1},{3,1},{1,3}};

  SECTION("Real space tests")
  {
    for (auto i=0; i < coords.size(); i++){
      auto eval_realspace = bravais.GetRealSpace(coords[i]);
      for (auto j=0; j < eval_realspace.size(); j++) {
        REQUIRE(realspace[i][j] == CApprox(eval_realspace[j]));
      }
    }
  }

  SECTION("Reciprocal space tests")
  {
    for (auto i=0; i < coords.size(); i++){
      auto eval_reciprocalspace = bravais.GetReciprocalSpace(coords[i]);
      for (auto j=0; j < eval_reciprocalspace.size(); j++) {
        REQUIRE(2*M_PI*reciprocalspace[i][j] == CApprox(eval_reciprocalspace[j]));
      }
    }
  }

  SECTION("GetNeighbor tests"){
    for (auto idx=0; idx < points.size(); idx++) // points represent neighbors
    {
      auto p = points[idx];
      auto n = bravais.GetNeighbor(p, idx);
      for (auto j=0; j < bravais.GetDim(); j++)
        REQUIRE(n[j] == neighbors[idx][j]);
    }
  }
}