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
#include <iostream>

using namespace std;

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

  /// Distance betweeen two sites of the lattice
  virtual double GetDistance(const coords_t& a,
                             const coords_t& b) const override;

  /// Distance betweeen two sites of the lattice
  virtual double GetDistanceSquared(const coords_t& a,
                                    const coords_t& b) const override;

protected:
  /// Create the vector of neighbors
  virtual neighbors_t CreateNeighbors(const offsets_t& size) const override;

private:
}; // class SquareLattice

SquareLattice::SquareLattice(const offsets_t& size)
  : Lattice(size, CreateNeighbors(size))
{
}

double
SquareLattice::GetDistance(const coords_t& a, const coords_t& b) const
{
  return std::sqrt(GetDistanceSquared(a, b));
}

double
SquareLattice::GetDistanceSquared(const coords_t& a, const coords_t& b) const
{
  auto dist = 0l;
  for (auto i = 0ul; i < dim_; i++) {
    auto tmp = std::abs(a[i]-b[i]);
    auto l2 = static_cast<long>(size_[i]/2);
    while (tmp > l2) {
      tmp -= l2;
    }
    dist += bwsl::square(tmp);
  }
  return static_cast<double>(dist);
}

Lattice::neighbors_t
SquareLattice::CreateNeighbors(const offsets_t& size) const
{
  auto nsites =
    bwsl::accumulate_product(size);
  auto dim = size.size();

  auto neighbors = neighbors_t(nsites, offsets_t(2ul * dim, 0ul));

  for (auto i = 0ul; i < nsites; i++) {
    auto coords = OffsetToCoordinates(i, size);
    for (auto j = 0ul; j < dim * 2; j++) {
      auto neighbor = coords;
      neighbor[j / 2] += static_cast<long>(j % 2) * 2 - 1;
      EnforceBoundaries(neighbor, size);
      neighbors[i][j] = CoordinatesToOffset(neighbor, size);
    }
  }

  return neighbors;
}

} // namespace bwsl

#endif // BWSL_SQUARELATTICE_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
