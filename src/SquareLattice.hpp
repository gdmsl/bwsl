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

  /// Get the projection of the distance between two sites
  virtual double GetDistance(size_t a, size_t b, size_t dir) const override;

  /// Get the coordination number
  virtual size_t GetCoordination() const override;

  /// Get a vector of winding numbers, one for each direction, for
  /// a couple of sites
  virtual std::vector<int> GetWinding(size_t a, size_t b) const override;

  /// Get a kappa
  virtual kappa_t GetK(size_t j) const override;

protected:
  /// Create the vector of neighbors
  neighbors_t CreateNeighbors(offsets_t const& size) const override;

private:
}; // class SquareLattice

SquareLattice::SquareLattice(offsets_t const& size)
  : Lattice(size, CreateNeighbors(size))
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

SquareLattice::kappa_t
SquareLattice::GetK(size_t j) const
{
  auto k = kappa_t(dim_, 0.0);
  auto r = GetCoordinates(j, size_);
  for (auto i = 0ul; i < dim_; i++) {
    auto l = static_cast<double>(size_[i]);
    auto l2 = static_cast<double>(size_[i] / 2);
    auto dist = static_cast<double>(r[i]);
    if (dist > l2)
      dist -= l;
    if (dist < -l2)
      dist += l;
    k[i] = 2.0 * M_PI * dist / l;
  }
  return k;
}

double
SquareLattice::GetDistance(size_t a, size_t b, size_t dir) const
{
  auto ca = GetCoordinates(a);
  auto cb = GetCoordinates(b);
  auto dist = ca[dir] - cb[dir];
  auto l = static_cast<long>(size_[dir]);
  if (dist > l/2) {
    dist -= l;
  }
  if (dist < -l/2) {
    dist += l;
  }
  return static_cast<double>(dist);
}

} // namespace bwsl

#endif // BWSL_SQUARELATTICE_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
