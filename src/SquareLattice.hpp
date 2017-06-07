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
#include <MathUtils.hpp>
#include <Lattice.hpp>

// std
#include <cmath>
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
  SquareLattice(const offsets_t& size);

  /// Copy constructor
  SquareLattice(const SquareLattice& that) = default;

  /// Default destructor
  virtual ~SquareLattice() = default;

  /// Copy assignment operator
  SquareLattice& operator=(const SquareLattice& that) = default;

  /// Get the coordination number
  virtual size_t GetCoordination() const override;

protected:
  /// Create the vector of neighbors
  neighbors_t CreateNeighbors(const offsets_t& size) const override;

  /// Create the vector of distances
  distances_t GenerateDistances(const offsets_t& size) const override;

private:
}; // class SquareLattice

SquareLattice::SquareLattice(const offsets_t& size)
  : Lattice(size, CreateNeighbors(size), GenerateDistances(size))
{
}

Lattice::neighbors_t
SquareLattice::CreateNeighbors(const offsets_t& size) const
{
  auto nsites =
    bwsl::accumulate_product(size);
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

/// Create the vector of distances
Lattice::distances_t SquareLattice::GenerateDistances(const offsets_t& size) const
{
  auto numsites = bwsl::accumulate_product(size);
  auto dim = size.size();
  auto distances = distances_t(numsites * (numsites+1) / 2, distance_t(dim, 0.0));
  for (auto i = 0ul; i < numsites; i++) {
    for (auto j = i; j < numsites; j++) {
      auto coordi = GetCoordinates(i, size);
      auto coordj = GetCoordinates(j, size);
      for (auto k = 0ul; k < dim; k++) {
        auto dist = coordi[k] - coordj[k];
        auto l2 = static_cast<long>(size[k]/2);
        if (dist > l2) dist -= l2;
        distances[PairIndex(i,j,numsites)][k] = static_cast<double>(dist);
      }
    }
  }
  return distances;
}

size_t SquareLattice::GetCoordination() const
{
  return dim_*2;
}

} // namespace bwsl

#endif // BWSL_SQUARELATTICE_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
