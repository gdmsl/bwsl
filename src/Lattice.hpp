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
  Lattice(offsets_t const& size, neighbors_t const& neighbors, distances_t const& distances);

  /// Copy constructor
  Lattice(Lattice const& that) = default;

  /// Default destructor
  virtual ~Lattice() = default;

  /// Copy assignment operator
  Lattice& operator=(Lattice const& that) = default;

  /// Get the dimensionality of the lattice
  size_t GetDim() const { return dim_; };

  /// Get the size of the lattice
  const offsets_t& GetSize() const { return size_; }

  /// Get the number of sites in the lattice
  size_t GetNumSites() const { return numsites_; };

  /// Get nearest neighbors of site i
  /// Neighbors should have same positions for the same direction.
  /// For example moving from i to j=GetNeighbors(i)[2] and
  /// k = GetNeighbors(i)[2] is the same as moving in one direction
  offsets_t const& GetNeighbors(size_t i) const { return neighbors_[i]; }

  /// Convert coordinates to an offset
  size_t GetOffset(coords_t const& coords) const;
  size_t GetOffset(coords_t const& coords, offsets_t const& size) const;

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
  static Lattice* CreateLattice(std::string const& name, const offsets_t& size);

  /// Change a vector so that he will match the boundary conditions
  void EnforceBoundaries(coords_t& coords, offsets_t const& size) const;

  /// Change a vector so that he will match the boundary conditions
  void EnforceBoundaries(coords_t& coords) const;

  /// Check if two sites are neighbors
  bool AreNeighbors(size_t a, size_t b) const;

  /// Unique index for pairs of sites
  size_t GetPairIndex(size_t a, size_t b) const;

  /// Get the coordination number
  virtual size_t GetCoordination() const = 0;

  /// Get a vector of winding numbers, one for each direction, for
  /// a couple of sites
  virtual std::vector<int> GetWinding(size_t a, size_t b) const = 0;

  /// Get a site mapped by a distance on the lattice
  size_t GetMappedSite(size_t a, coords_t const& map) const;

  /// Get a random distance on the lattice
  template<class G>
  coords_t GetRandomDistance(G& rng) const;

  /// Get a random direction on the lattice
  template<class G>
  size_t GetRandomDirection(G& rng) const;

protected:
  /// Create the vector of neighbors
  virtual neighbors_t CreateNeighbors(offsets_t const& size) const = 0;

  /// Create the vector of distances
  virtual distances_t GenerateDistances(offsets_t const& size) const = 0;

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

Lattice::Lattice(offsets_t const& size, neighbors_t const& neighbors, distances_t const& distances)
  : dim_(size.size())
  , size_(size)
  , numsites_(std::accumulate(size.begin(), size.end(), 1ul, std::multiplies<size_t>()))
  , neighbors_(neighbors)
  , distances_(distances)
{
}

size_t Lattice::GetPairIndex(size_t a, size_t b) const
{
  auto an = std::min(a,b);
  auto bn = std::max(a,b);

  return an * numsites_ + bn -  (an * (an + 1)) / 2;
}

double Lattice::GetDistance(size_t a, size_t b) const
{
  return std::sqrt(GetDistanceSquared(a,b));
}

double Lattice::GetDistance(size_t a, size_t b, size_t dir) const
{
  auto sign = (b > a) ? 1.0 : -1.0;
  return sign * distances_[GetPairIndex(a,b)][dir];
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

size_t Lattice::GetOffset(coords_t const& coords) const
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

size_t Lattice::GetOffset(coords_t const& coords, offsets_t const& size) const
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

Lattice::coords_t Lattice::GetCoordinates(size_t offset, offsets_t const& size) const
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

void Lattice::EnforceBoundaries(coords_t& coords, offsets_t const& size) const
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

size_t Lattice::GetMappedSite(size_t a, coords_t const& map) const
{
  auto ac = GetCoordinates(a);

  sum_into(ac, map);

  EnforceBoundaries(ac);

  return GetOffset(ac);
}

template <class G>
Lattice::coords_t Lattice::GetRandomDistance(G& rng) const
{
  auto distance = coords_t(dim_, 0l);

  for (auto i = 0ul; i < dim_; i++) {
    auto s = static_cast<long>(size_[i])/2;
    auto distribution = std::uniform_int_distribution<long>(-s+1, s);
    distance[i] = distribution(rng);
  }

  return distance;
}

template<class G>
size_t Lattice::GetRandomDirection(G& rng) const
{
  static auto dist = std::uniform_int_distribution<size_t>(0,dim_*2ul-1);
  return dist(rng);
}

} // namespace bwsl

#endif // BWSL_LATTICE_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
