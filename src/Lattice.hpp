//===-- Lattice.hpp --------------------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2017 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// \file
/// \version    0.3
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       May, 2017
/// \brief      Definitions for the Lattice Class
///
//===---------------------------------------------------------------------===//
#ifndef BWSL_LATTICE_HPP
#define BWSL_LATTICE_HPP

// std
#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

namespace bwsl {

///
/// \brief      Representation of a Lattice
/// \version    0.3
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       May, 2017
///
class Lattice
{
public:
  /// Unsigned type for sizes and indices
  using size_t = std::size_t;

  /// Coordinates of a point on a lattice
  using coords_t = std::vector<long>;

  /// Vector of offsets
  using offsets_t = std::vector<size_t>;

  /// Vector of neighbors
  using neighbors_t = std::vector<offsets_t>;

  /// Default constructor
  Lattice() = delete;

  /// Construct a lattice with given size
  Lattice(const offsets_t& size, const neighbors_t& neighbors);

  /// Copy constructor
  Lattice(const Lattice& that) = default;

  /// Default destructor
  virtual ~Lattice() = default;

  /// Copy assignment operator
  Lattice& operator=(const Lattice& that) = default;

  /// Get the dimensionality of the lattice
  size_t GetDim() const { return dim_; };

  /// Get the size of the lattice
  const offsets_t& GetSize() const { return size_; }

  /// Get the number of sites in the lattice
  size_t GetNumSites() const;

  /// Get nearest neighbors of site i
  const offsets_t& GetNeighbors(size_t i) const { return neighbors_[i]; }

  /// Convert coordinates to an offset
  size_t CoordinatesToOffset(const coords_t& coords) const;
  size_t CoordinatesToOffset(const coords_t& coords, const offsets_t& size) const;

  /// Convert an offset to coordinates
  coords_t OffsetToCoordinates(size_t offset) const;
  coords_t OffsetToCoordinates(size_t offset, const offsets_t& size) const;

  /// Distance betweeen two sites of the lattice
  virtual double GetDistance(size_t a, size_t b) const;
  virtual double GetDistance(const coords_t& a, const coords_t& b) const = 0;

  /// Distance betweeen two sites of the lattice
  virtual double GetDistanceSquared(size_t a, size_t b) const;
  virtual double GetDistanceSquared(const coords_t& a,
                                    const coords_t& b) const = 0;

  /// Factory function
  static Lattice* CreateLattice(const std::string& name, const offsets_t& size);

  /// Change a vector so that he will matche the boundaries conditions
  void EnforceBoundaries(coords_t& coords, const offsets_t& size) const;

  /// Change a vector so that he will matche the boundaries conditions
  void EnforceBoundaries(coords_t& coords) const;

  /// Check if two sites are neighbors
  bool AreNeighbors(size_t a, size_t b) const;

protected:
  /// Create the vector of neighbors
  virtual neighbors_t CreateNeighbors(const offsets_t& size) const = 0;

  /// Dimensionality of the lattice
  size_t dim_{};

  /// Size of the lattice
  const offsets_t size_{};

private:
  /// vector of nearest neighbors
  const neighbors_t neighbors_{};
}; // class Lattice

Lattice::Lattice(const offsets_t& size, const neighbors_t& neighbors)
  : dim_(size.size())
  , size_(size)
  , neighbors_(neighbors)
{
}

double Lattice::GetDistance(size_t a, size_t b) const
{
  return GetDistance(OffsetToCoordinates(a), OffsetToCoordinates(b));
}

double Lattice::GetDistanceSquared(size_t a, size_t b) const
{
  return GetDistanceSquared(OffsetToCoordinates(a), OffsetToCoordinates(b));
}

size_t Lattice::CoordinatesToOffset(const coords_t& coords) const
{
  assert(coords.size() == dim_ && "Dimensions not matching");

  auto offset = 0ul;
  auto prod = 1ul;

  for (auto i = 0ul; i < dim_ - 1; i++) {
    prod *= size_[i];
  }
  for (auto i = 0ul; i < dim_; i++) {
    auto x = coords[i];
    auto size = size_[i];
    while (x < 0) {
      x += size;
    }

    auto xul = static_cast<size_t>(x);
    while (xul >= size) {
      xul -= size;
    }
    offset += prod * xul;
    prod /= size;
  }

  return offset;
}

size_t Lattice::CoordinatesToOffset(const coords_t& coords, const offsets_t& size) const
{
  auto dim = size.size();
  assert(coords.size() == dim && "Dimensions not matching");

  auto offset = 0ul;
  auto prod = 1ul;

  for (auto i = 0ul; i < dim - 1; i++) {
    prod *= size[i];
  }
  for (auto i = 0ul; i < dim; i++) {
    auto x = coords[i];
    auto s = size[i];
    while (x < 0) {
      x += s;
    }

    auto xul = static_cast<size_t>(x);
    while (xul >= s) {
      xul -= s;
    }
    offset += prod * xul;
    prod /= s;
  }

  return offset;
}


Lattice::coords_t Lattice::OffsetToCoordinates(size_t offset) const
{
  auto prod = 1ul;

  auto d = coords_t(dim_, 0);

  for (auto i = 0ul; i < dim_ - 1; i++) {
    prod *= size_[i];
  }
  for (auto i = 0ul; i < dim_; i++) {
    d[i] = static_cast<long>(offset / prod);
    offset = offset % prod;
    prod /= size_[i];
  }

  return d;
}

Lattice::coords_t Lattice::OffsetToCoordinates(size_t offset, const offsets_t& size) const
{
  auto dim = size.size();
  auto prod = 1ul;

  auto d = coords_t(dim, 0);

  for (auto i = 0ul; i < dim - 1; i++) {
    prod *= size[i];
  }
  for (auto i = 0ul; i < dim; i++) {
    d[i] = static_cast<long>(offset / prod);
    offset = offset % prod;
    prod /= size[i];
  }

  return d;
}


void Lattice::EnforceBoundaries(coords_t& coords, const offsets_t& size) const
{
  auto d = size.size();
  for (auto i = 0ul; i < d; i++) {
    auto s = size[i];
    while (coords[i] < 0)
      coords[i] += s;
    while (coords[i] >= static_cast<long>(s))
      coords[i] -= s;
  }
}

void Lattice::EnforceBoundaries(coords_t& coords) const
{
  EnforceBoundaries(coords, size_);
}

size_t Lattice::GetNumSites() const
{
  return std::accumulate(
    size_.begin(), size_.end(), 1ul, std::multiplies<size_t>());
}

bool Lattice::AreNeighbors(size_t a, size_t b) const
{
  assert(a < neighbors_.size());

  auto result = std::find(neighbors_[a].begin(), neighbors_[a].end(), b);

  return result != neighbors_[a].end();
}

} // namespace bwsl

#endif // BWSL_LATTICE_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
