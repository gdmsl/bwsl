//===-- SquareLattice.hpp --------------------------------------*- C++ -*-===//
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
/// \brief      Definitions for the SquareLattice Class
///
//===---------------------------------------------------------------------===//
#ifndef BWSL_SQUARELATTICE_HPP
#define BWSL_SQUARELATTICE_HPP

// bwsl
#include <Lattice.hpp>
#include <MathUtils.hpp>

// std
#include <cmath>
#include <iostream>
#include <vector>

namespace bwsl {

///
/// \brief      A SquareLattice
/// \version    0.1
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       May, 2017
///
class SquareLattice : public Lattice
{
public:
  /// Default constructor
  SquareLattice() = delete;

  /// Construct a square lattice with given sizes
  SquareLattice(offsets_t const& size);

  /// Copy constructor
  SquareLattice(SquareLattice const& that) = default;

  /// Default destructor
  virtual ~SquareLattice() = default;

  /// Copy assignment operator
  SquareLattice& operator=(SquareLattice const& that) = default;

  /// Get the coordination number
  virtual size_t GetCoordination() const override;

  /// Get a vector of winding numbers, one for each direction, for
  /// a couple of sites
  virtual std::vector<int> GetWinding(size_t a, size_t b) const override;

protected:
  /// Create the vector of neighbors
  neighbors_t CreateNeighbors(offsets_t const& size) const override;

  /// Create the vector of distances
  distances_t GenerateDistances(offsets_t const& size) const override;

private:
}; // class SquareLattice

SquareLattice::SquareLattice(offsets_t const& size)
  : Lattice(size, CreateNeighbors(size), GenerateDistances(size))
{
}

Lattice::neighbors_t
SquareLattice::CreateNeighbors(offsets_t const& size) const
{
  auto nsites = bwsl::accumulate_product(size);
  auto dim = size.size();

  auto neighbors = neighbors_t(nsites, offsets_t(2ul * dim, 0ul));

  for (auto i = 0ul; i < nsites; i++) {
    auto coords = GetCoordinates(i, size);
    for (auto j = 0ul; j < dim * 2; j++) {
      auto neighbor = coords;
      neighbor[j / 2] += static_cast<long>(j % 2) * 2 - 1;
      EnforceBoundaries(neighbor, size);
      neighbors[i][j] = GetOffset(neighbor, size);
    }
  }

  return neighbors;
}

Lattice::distances_t
SquareLattice::GenerateDistances(offsets_t const& size) const
{
  auto numsites = bwsl::accumulate_product(size);
  auto dim = size.size();
  auto distances =
    distances_t(numsites * (numsites + 1) / 2, distance_t(dim, 0.0));
  for (auto i = 0ul; i < numsites; i++) {
    for (auto j = i; j < numsites; j++) {
      auto coordi = GetCoordinates(i, size);
      auto coordj = GetCoordinates(j, size);
      for (auto k = 0ul; k < dim; k++) {
        auto dist = coordi[k] - coordj[k];
        auto l = static_cast<long>(size[k]);
        if (dist > l/2) {
          dist -= l;
        } else if (dist < -l/2) {
          dist += l;
        }
        distances[bwsl::GetPairIndex(i, j, numsites)][k] = static_cast<double>(dist);
      }
    }
  }
  return distances;
}

size_t
SquareLattice::GetCoordination() const
{
  return dim_ * 2;
}

std::vector<int>
SquareLattice::GetWinding(size_t a, size_t b) const
{
  assert(AreNeighbors(a, b));

  auto winding = std::vector<int>(dim_, 0ul);

  for (auto i = 0ul; i < dim_; i++) {
    winding[i] = static_cast<int>(GetDistance(b, a, i));
  }

  return winding;
}

} // namespace bwsl

#endif // BWSL_SQUARELATTICE_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
