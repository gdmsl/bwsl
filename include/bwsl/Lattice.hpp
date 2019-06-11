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

// bwsl
#include <bwsl/Approx.hpp>
#include <bwsl/Bravais.hpp>
#include <bwsl/MathUtils.hpp>
#include <bwsl/Pairs.hpp>

// std
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

namespace bwsl {

///
/// \brief      Representation of a Lattice
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
  Lattice(Bravais const& bravais,
          offsets_t const& size,
          bool openboundaries = false);

  /// Copy constructor
  Lattice(Lattice const& that) = default;

  /// Default destructor
  virtual ~Lattice() = default;

  /// Copy assignment operator
  Lattice& operator=(Lattice const& that) = delete;

  /// Get the dimensionality of the lattice
  size_t GetDim() const { return dim_; };

  /// Get the size of the lattice
  const offsets_t& GetSize() const { return size_; }

  /// Get the number of sites in the lattice
  size_t GetNumSites() const { return numsites_; };

  /// Get nearest neighbors of site i
  offsets_t const& GetNeighbors(size_t i) const { return neighbors_[i]; }

  /// Convert coordinates to an offset
  size_t GetOffset(coords_t const& coords) const
  {
    return ArrayToIndex(coords, size_);
  };

  /// Convert an offset to coordinates
  coords_t GetCoordinates(size_t offset) const
  {
    return IndexToArray<coords_t, offsets_t>(offset, size_);
  };

  /// Distance betweeen two sites of the lattice
  double GetDistance(size_t a, size_t b) const;

  /// Get the distacne vetctor between a and b
  realvec_t GetVector(size_t a, size_t b) const;

  /// Get the distance between a site and a copy of another
  double GetCopyDistance(size_t a, size_t b, coords_t copy) const;

  /// Change a vector so that he will match the boundary conditions
  void EnforceBoundaries(coords_t& coords) const;

  /// Check if the coordinates are within the boundaries
  bool IsInBoundaries(coords_t const& coords) const;

  /// Check if two sites are neighbors
  bool AreNeighbors(size_t a, size_t b) const;

  /// Unique index for pairs of sites
  size_t GetPairIndex(size_t a, size_t b) const;

  /// Destructure a pair and return the two indices
  std::pair<size_t, size_t> GetIndividualIndices(size_t pair) const;

  /// Get the coordination number
  size_t GetCoordination(size_t a) const;

  /// Get a site mapped by a distance on the lattice
  size_t GetMappedSite(size_t a, size_t b) const;

  /// Get an allowed momentum
  realvec_t GetMomentum(size_t a) const;

protected:
  /// Compute all the vectors cnnecting the origin site to the others
  std::vector<realvec_t> ComputeVectors(Bravais const& bravais) const;

  /// Compute the vector of distances
  realvec_t ComputeDistances() const;

  /// Create the vector of neighbors
  neighbors_t ComputeNeighbors(Bravais const& bravais) const;

  /// Compute the allowed momenta
  std::vector<realvec_t> ComputeMomenta(Bravais const& bravais) const;

  /// Dimensionality of the lattice
  size_t dim_{ 0 };

  /// Size of the lattice
  const offsets_t size_{};

  /// Number of sites in the lattice
  const size_t numsites_{ 0 };

  /// Number of pairs of lattice sites
  const size_t numpairs_{ 0 };

  /// Flag for closed/open boundary conditions
  const bool openboundaries_{ false };

private:
  /// Vector distances from the origin with minimum image convention
  const std::vector<realvec_t> distvector_{};

  /// All the distances on the lattice with minimum image convention
  const realvec_t distances_{};

  /// vector of nearest neighbors
  const neighbors_t neighbors_{};

  /// Allowed values momenta
  const std::vector<realvec_t> momenta_{};

}; // class Lattice

inline Lattice::Lattice(Bravais const& bravais,
                        offsets_t const& size,
                        bool openboundaries)
  : dim_(size.size())
  , size_(size)
  , numsites_(accumulate_product(size))
  , numpairs_(pairs::GetNumPairs(numsites_))
  , openboundaries_(openboundaries)
  , distvector_(ComputeVectors(bravais))
  , distances_(ComputeDistances())
  , neighbors_(ComputeNeighbors(bravais))
  , momenta_(ComputeMomenta(bravais))
{}

inline void
Lattice::EnforceBoundaries(coords_t& coords) const
{
  for (auto i = 0ul; i < dim_; i++) {
    auto s = size_[i];
    while (coords[i] < 0) {
      coords[i] += s;
    }
    while (coords[i] >= static_cast<long>(s)) {
      coords[i] -= s;
    }
  }
}

inline bool
Lattice::IsInBoundaries(coords_t const& coords) const
{
  assert(coords.size() == dim_);
  for (auto i = 0ul; i < dim_; i++) {
    if (coords[i] < 0 || coords[i] >= static_cast<long>(size_[i])) {
      return false;
    }
  }
  return true;
}

inline double
Lattice::GetDistance(size_t a, size_t b) const
{
  return distances_[GetPairIndex(a, b)];
}

inline Lattice::realvec_t
Lattice::GetVector(size_t a, size_t b) const
{
  return distvector_[GetPairIndex(a, b)];
}

inline bool
Lattice::AreNeighbors(size_t a, size_t b) const
{
  assert(a < neighbors_.size());

  auto result = std::find(neighbors_[a].begin(), neighbors_[a].end(), b);

  return result != neighbors_[a].end();
}

inline std::vector<Lattice::realvec_t>
Lattice::ComputeVectors(Bravais const& bravais) const
{
  auto p = std::vector<realvec_t>{};

  const auto imgsize = offsets_t(dim_, 3);
  const auto nimg = accumulate_product(imgsize);

  for (auto pair = 0ul; pair < numpairs_; pair++) {
    auto [i, j] = GetIndividualIndices(pair);
    auto ci = GetCoordinates(i);
    auto cj = GetCoordinates(j);
    auto mindist = bravais.GetDistance(ci, cj);
    auto minvec = bravais.GetVector(ci, cj);

    // search for the minimum distance across the first shell of
    // periodic images if we are with closed boundary condition
    if (!openboundaries_) {
      for (auto k = 0ul; k < nimg; k++) {
        auto img = IndexToArray<coords_t, offsets_t>(k, imgsize);
        auto cjm = coords_t(cj);
        for (auto l = 0ul; l < dim_; l++) {
          cjm[l] += size_[l] * (img[l] - 1);
        }
        auto dist = bravais.GetDistance(ci, cjm);
        if (dist < mindist) {
          mindist = dist;
          minvec = bravais.GetVector(ci, cjm);
        }
      }
    }

    /// just add the minimum image to the vector of distances
    p.push_back(minvec);
  }
  return p;
}

inline Lattice::realvec_t
Lattice::ComputeDistances() const
{
  auto p = realvec_t{};
  for (auto const& dv : distvector_) {
    auto d = 0.0;
    for (auto x : dv) {
      d += square(x);
    }
    p.push_back(std::sqrt(d));
  }
  return p;
}

inline Lattice::neighbors_t
Lattice::ComputeNeighbors(Bravais const& bravais) const
{
  auto p = neighbors_t{};
  const auto gamma = bravais.GetGamma();

  for (auto i = 0ul; i < numsites_; i++) {
    auto nn = offsets_t{};
    auto ci = GetCoordinates(i);
    for (auto j = 0ul; j < gamma; j++) {
      auto cj = bravais.GetNeighbor(ci, j);

      // add the sites to the list of neighbors accordingly with the
      // boundary conditions set
      if (!openboundaries_ || IsInBoundaries(cj)) {
        EnforceBoundaries(cj);
        nn.push_back(GetOffset(cj));
      }
    }
    p.push_back(std::move(nn));
  }

  return p;
}

inline std::vector<Lattice::realvec_t>
Lattice::ComputeMomenta(Bravais const& bravais) const
{
  auto p = std::vector<realvec_t>{};

  // with open boundary conditions the momenta are not defined
  if (openboundaries_) {
    return p;
  }

  // in a lattice with periodic boundary conditions the number
  // of reciprocal lattice vectors are the same number as the
  // sites of the direct lattice
  for (auto i = 0ul; i < numsites_; i++) {
    auto ci = GetCoordinates(i);
    for (auto k = 0ul; k < ci.size(); k++) {
      ci[k] -= size_[k] / 2ul + 1;
    }
    auto kappa = bravais.GetReciprocalSpace(ci);
    for (auto k = 0ul; k < kappa.size(); k++) {
      kappa[k] /= size_[k];
    }
    p.push_back(kappa);
  }

  return p;
}

inline size_t
Lattice::GetPairIndex(size_t a, size_t b) const
{
  return bwsl::pairs::GetPairIndex(a, b, numsites_);
}

inline std::pair<size_t, size_t>
Lattice::GetIndividualIndices(size_t pair) const
{
  return bwsl::pairs::GetPair(pair, numsites_);
}

inline double
Lattice::GetCopyDistance(size_t a, size_t b, coords_t copy) const
{
  auto dv = GetVector(a, b);
  auto d = 0.0;
  for (auto i = 0ul; i < dim_; i++) {
    d += square(dv[i] + copy[i] * size_[i]);
  }
  return std::sqrt(d);
}

} // namespace bwsl

#endif // BWSL_LATTICE_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
