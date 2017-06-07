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
#include <cmath>
#include <vector>

// bwsl
#include <MathUtils.hpp>

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

  /// Type for distacne between a pair
  using distance_t = std::vector<double>;

  /// Type for distance vector
  using distances_t = std::vector<distance_t>;

  /// Default constructor
  Lattice() = delete;

  /// Construct a lattice with given size
  Lattice(const offsets_t& size, const neighbors_t& neighbors, const distances_t& distances);

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
  size_t GetNumSites() const { return numsites_; };

  /// Get nearest neighbors of site i
  const offsets_t& GetNeighbors(size_t i) const { return neighbors_[i]; }

  /// Convert coordinates to an offset
  size_t GetOffset(const coords_t& coords) const;
  size_t GetOffset(const coords_t& coords, const offsets_t& size) const;

  /// Convert an offset to coordinates
  coords_t GetCoordinates(size_t offset) const;
  coords_t GetCoordinates(size_t offset, const offsets_t& size) const;

  /// Distance betweeen two sites of the lattice
  double GetDistance(size_t a, size_t b) const;
  double GetDistance(size_t a, size_t b, size_t dir) const;

  /// Distance betweeen two sites of the lattice
  double GetDistanceSquared(size_t a, size_t b) const;
  double GetDistanceSquared(size_t a, size_t b, size_t dir) const;

  /// Factory function
  static Lattice* CreateLattice(const std::string& name, const offsets_t& size);

  /// Change a vector so that he will match the boundary conditions
  void EnforceBoundaries(coords_t& coords, const offsets_t& size) const;

  /// Change a vector so that he will match the boundary conditions
  void EnforceBoundaries(coords_t& coords) const;

  /// Check if two sites are neighbors
  bool AreNeighbors(size_t a, size_t b) const;

  /// Unique index for pairs of sites
  size_t PairIndex(size_t a, size_t b) const;

  /// Unique index for pairs of sites
  static size_t PairIndex(size_t a, size_t b, size_t numsites);

  /// Get the coordination number
  virtual size_t GetCoordination() const = 0;

protected:
  /// Create the vector of neighbors
  virtual neighbors_t CreateNeighbors(const offsets_t& size) const = 0;

  /// Create the vector of distances
  virtual distances_t GenerateDistances(const offsets_t& size) const = 0;

  /// Dimensionality of the lattice
  size_t dim_{0};

  /// Size of the lattice
  const offsets_t size_{};

  /// Number of sites in the lattice
  const size_t numsites_{0};

private:
  /// vector of nearest neighbors
  const neighbors_t neighbors_{};

  /// Vector of distances
  const std::vector<std::vector<double>> distances_;
}; // class Lattice

Lattice::Lattice(const offsets_t& size, const neighbors_t& neighbors, const distances_t& distances)
  : dim_(size.size())
  , size_(size)
  , numsites_(std::accumulate(size.begin(), size.end(), 1ul, std::multiplies<size_t>()))
  , neighbors_(neighbors)
  , distances_(distances)
{
}

size_t Lattice::PairIndex(size_t a, size_t b) const
{
  auto an = std::min(a,b);
  auto bn = std::max(a,b);

  return an * numsites_ + bn -  (an * (an + 1)) / 2;
}

size_t Lattice::PairIndex(size_t a, size_t b, size_t numsites)
{
  auto an = std::min(a,b);
  auto bn = std::max(a,b);

  return an * numsites + bn -  (an * (an + 1)) / 2;
}

double Lattice::GetDistance(size_t a, size_t b) const
{
  return std::sqrt(GetDistanceSquared(a,b));
}

double Lattice::GetDistance(size_t a, size_t b, size_t dir) const
{
  auto sign = (b > a) ? 1.0 : -1.0;
  return sign * distances_[PairIndex(a,b)][dir];
}

double Lattice::GetDistanceSquared(size_t a, size_t b, size_t dir) const
{
  return bwsl::square(GetDistance(a,b,dir));
}

double Lattice::GetDistanceSquared(size_t a, size_t b) const
{
  auto sum = 0.0;
  for (auto i=0ul; i < dim_; i++) {
    sum += GetDistanceSquared(a,b,i);
  }
  return sum;
}

size_t Lattice::GetOffset(const coords_t& coords) const
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

size_t Lattice::GetOffset(const coords_t& coords, const offsets_t& size) const
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

Lattice::coords_t Lattice::GetCoordinates(size_t offset) const
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

Lattice::coords_t Lattice::GetCoordinates(size_t offset, const offsets_t& size) const
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

bool Lattice::AreNeighbors(size_t a, size_t b) const
{
  assert(a < neighbors_.size());

  auto result = std::find(neighbors_[a].begin(), neighbors_[a].end(), b);

  return result != neighbors_[a].end();
}

} // namespace bwsl

#endif // BWSL_LATTICE_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
