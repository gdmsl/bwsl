//===-- Lattice.hpp --------------------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2016-2018 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// \file
/// \version    0.3
/// \author     Guido Masella (guido.masella@gmail.com)
/// \brief      Definitions for the Lattice Class
///
//===---------------------------------------------------------------------===//
#ifndef BWSL_LATTICE_HPP
#define BWSL_LATTICE_HPP

// std
#include <algorithm>
#include <cassert>
#include <cmath>
#include <memory>
#include <vector>

// bwsl
#include <bwsl/Approx.hpp>
#include <bwsl/Bravais.hpp>
#include <bwsl/MathUtils.hpp>

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
  using coords_t = std::vector<int>;

  /// Vector of offsets
  using offsets_t = std::vector<size_t>;

  /// Vector of neighbors
  using neighbors_t = std::vector<offsets_t>;

  /// Shorthand for real valued vectors
  using realvec_t = std::vector<double>;

  /// Default constructor
  Lattice() = delete;

  /// Construct a lattice with given size from an infinite bravais lattice
  Lattice(Bravais const& bravais, offsets_t const& size);

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
  offsets_t const& GetNeighbors(size_t i) const { return neighbors_[i]; }

  /// Convert coordinates to an offset
  size_t GetOffset(coords_t const& coords) const;
  size_t GetOffset(coords_t const& coords, offsets_t const& size) const;

  /// Convert an offset to coordinates
  coords_t GetCoordinates(size_t offset) const;
  coords_t GetCoordinates(size_t offset, offsets_t const& size) const;

  /// Distance betweeen two sites of the lattice
  double GetDistance(size_t a, size_t b) const;

  /// Get the distacne vetctor between a and b
  realvec_t GetVector(size_t a, size_t b) const;

  /// Change a vector so that he will match the boundary conditions
  void EnforceBoundaries(coords_t& coords) const;
  void EnforceBoundaries(coords_t& coords, offsets_t const& size) const;

  /// Check if two sites are neighbors
  bool AreNeighbors(size_t a, size_t b) const;

  /// Unique index for pairs of sites
  size_t GetPairIndex(size_t a, size_t b) const;

  /// Get the coordination number
  size_t GetCoordination(size_t a) const;

  /// Get a site mapped by a distance on the lattice
  size_t GetMappedSite(size_t a, size_t b) const;

  /// Get an allowed momentum
  realvec_t GetMomentum(size_t a) const;

protected:
  /// Compute all the vectors cnnecting the origin site to the others
  std::vector<realvec_t> ComputeVectors(Bravais const& bravais,
                                        offsets_t const& size) const;

  /// Compute the vector of distances
  std::vector<realvec_t> ComputeDistances(
    std::vector<realvec_t> const& distvector,
    offsets_t const& size) const;

  /// Create the vector of neighbors
  neighbors_t ComputeNeighbors(std::vector<realvec_t> const& distances) const;

  /// Compute the allowed momenta
  std::vector<realvec_t> ComputeMomenta(Bravais const& bravais,
                                        offsets_t const& size) const;

  /// Dimensionality of the lattice
  size_t dim_{ 0 };

  /// Size of the lattice
  const offsets_t size_{};

  /// Number of sites in the lattice
  const size_t numsites_{ 0 };

private:
  /// Vector distances from the origin with minimum image convention
  const std::vector<realvec_t> distvector_{};

  /// All the distances on the lattice with minimum image convention
  const std::vector<realvec_t> distances_{};

  /// vector of nearest neighbors
  const neighbors_t neighbors_{};

  /// Allowed values momenta
  const std::vector<realvec_t> momenta_{};

}; // class Lattice

Lattice::Lattice(Bravais const& bravais, offsets_t const& size)
  : dim_(size.size())
  , size_(size)
  , numsites_(
      std::accumulate(size.begin(), size.end(), 1ul, std::multiplies<size_t>()))
  , distvector_(ComputeVectors(bravais, size))
  , distances_(ComputeDistances(distvector_, size))
  , neighbors_(ComputeNeighbors(distances_))
  , momenta_(ComputeMomenta(bravais, size))
{}

size_t
Lattice::GetPairIndex(size_t a, size_t b) const
{
  auto an = std::min(a, b);
  auto bn = std::max(a, b);

  return an * numsites_ + bn - (an * (an + 1)) / 2;
}

size_t
Lattice::GetOffset(coords_t const& coords) const
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

size_t
Lattice::GetOffset(coords_t const& coords, offsets_t const& size) const
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

Lattice::coords_t
Lattice::GetCoordinates(size_t offset) const
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

Lattice::coords_t
Lattice::GetCoordinates(size_t offset, offsets_t const& size) const
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

void
Lattice::EnforceBoundaries(coords_t& coords, offsets_t const& size) const
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

void
Lattice::EnforceBoundaries(coords_t& coords) const
{
  EnforceBoundaries(coords, size_);
}

size_t
Lattice::GetMappedSite(size_t a, size_t b) const
{
  auto ac = GetCoordinates(a);
  auto bc = GetCoordinates(b);

  subtract_into(ac, bc);

  EnforceBoundaries(ac);

  return GetOffset(ac);
}

double
Lattice::GetDistance(size_t a, size_t b) const
{
  return distances_[a][b];
}

Lattice::realvec_t
Lattice::GetVector(size_t a, size_t b) const
{
  auto j = GetMappedSite(b,a);
  return distvector_[j];
}

bool
Lattice::AreNeighbors(size_t a, size_t b) const
{
  assert(a < neighbors_.size());

  auto result = std::find(neighbors_[a].begin(), neighbors_[a].end(), b);

  return result != neighbors_[a].end();
}

std::vector<Lattice::realvec_t>
Lattice::ComputeVectors(Bravais const& bravais, offsets_t const& size) const
{
  auto p = std::vector<realvec_t>{};
  auto n = accumulate_product(size);
  auto d = size.size();

  auto c0 = GetCoordinates(0ul, size);
  const auto imgsize = offsets_t(d, 3);
  const auto nimg = accumulate_product(imgsize);

  for (auto i = 0ul; i < n; i++) {
    auto mindist = std::numeric_limits<double>::infinity();
    auto minvec = realvec_t{};
    auto ci = GetCoordinates(i, size);
    for (auto j = 0ul; j < nimg; j++) {
      auto img = GetCoordinates(j, imgsize);
      auto cim = coords_t(ci);
      for (auto k = 0ul; k < d; k++) {
        cim[k] += size[k] * (img[k] - 1);
      }
      auto dist = bravais.GetDistance(c0, cim);
      if (dist < mindist) {
        mindist = dist;
        minvec = bravais.GetVector(c0, cim);
      }
    }
    p.push_back(minvec);
  }

  return std::move(p);
}

std::vector<Lattice::realvec_t>
Lattice::ComputeDistances(std::vector<realvec_t> const& distvector,
                          offsets_t const& size) const
{
  auto n = accumulate_product(size);
  auto p = std::vector<realvec_t>{};
  for (auto i = 0ul; i < n; i++) {
    auto v = realvec_t{};
    for (auto j = 0ul; j < n; j++) {
      auto jn = GetMappedSite(j, i);
      auto d = 0.0;
      for (auto x : distvector[jn]) {
        d += square(x);
      }
      v.push_back(std::sqrt(d));
    }
    p.push_back(std::move(v));
  }
  return std::move(p);
}

Lattice::neighbors_t
Lattice::ComputeNeighbors(std::vector<realvec_t> const& distances) const
{
  auto p = neighbors_t{};

  // nearest neighbors of one sites are defined to be all the sites to have
  // the minimum distance to that site
  for (auto i = 0ul; i < distances.size(); i++) {
    auto nn = offsets_t{};
    auto mindist = std::numeric_limits<double>::infinity();
    for (auto j = 0ul; j < distances[i].size(); j++) {
      if (i == j) {
        continue;
      }
      auto const& dist = distances[i][j];
      // we use approx to avoid damage dealth by roundoff errors while
      // computing the distances
      if (Approx(dist) == mindist) {
        nn.push_back(j);
      } else if (dist < mindist) {
        mindist = dist;
        nn.clear();
      }
    }
    p.push_back(std::move(nn));
  }

  return std::move(p);
}

std::vector<Lattice::realvec_t>
Lattice::ComputeMomenta(Bravais const& bravais, offsets_t const& size) const
{
  auto p = std::vector<realvec_t>{};
  const auto n = accumulate_product(size);

  for (auto i = 0ul; i < n; i++) {
    auto ci = GetCoordinates(i, size);
    for (auto k = 0ul; k < ci.size(); k++) {
      ci[k] -= size[k]/2ul+1;
    }
    auto kappa = bravais.GetReciprocalSpace(ci);
    for (auto k = 0ul; k < kappa.size(); k++) {
      kappa[k] /= size[k];
    }
    p.push_back(kappa);
  }

  return std::move(p);
}

} // namespace bwsl

#endif // BWSL_LATTICE_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
