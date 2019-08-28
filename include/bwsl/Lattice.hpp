//===-- Lattice.hpp --------------------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2016-2019 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// @file
/// @author     Guido Masella (guido.masella@gmail.com)
/// @brief      Definitions for the Lattice Class
///
//===---------------------------------------------------------------------===//
#pragma once

// bwsl
#include <bwsl/Approx.hpp>
#include <bwsl/Bravais.hpp>
#include <bwsl/MathUtils.hpp>
#include <bwsl/Pairs.hpp>

// fmt
#include <fmt/ostream.h>
#include <fmt/format.h>

// std
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>

namespace bwsl {

///
/// Representation of a Lattice.
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
  Lattice() = default;

  /// Construct a lattice with given size from an infinite bravais lattice
  Lattice(Bravais const& bravais,
          offsets_t const& size,
          bool openboundaries = false);

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

  /// Get the vector spawning from site @p a to site @p b .
  realvec_t GetVector(size_t a, size_t b) const;

  /// Get the winding number times the system size for a hopping between the
  /// sites @p a and @p b .
  coords_t GetJump(size_t a, size_t b) const;

  /// Given an accumulator for the winding number return the total winding.
  /// The total winding is defined as the number of times we jump around the
  /// boundaries.
  coords_t GetWinding(coords_t jump) const;

  /// Get the real space coordinates of site @p a .
  realvec_t GetPosition(size_t a) const;

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

  /// Compute the structure factor given the occupations of the sites
  realvec_t ComputeSk(realvec_t occupations) const;

  /// Save the distances on a file
  void SaveDistances(std::string fname) const;

  /// Save the momenta on a file
  void SaveMomenta(std::string fname) const;

protected:
  /// Compute the positions of all the lattice points
  std::vector<realvec_t> ComputePositions(Bravais const& bravais) const;

  /// Compute the distance vectors
  std::vector<realvec_t> ComputeVectors(Bravais const& bravais) const;

  /// Compute the vector of distances
  realvec_t ComputeDistances(Bravais const& /*bravais*/) const;

  /// Create the vector of neighbors
  neighbors_t ComputeNeighbors(Bravais const& bravais) const;

  /// Compute the allowed momenta
  std::vector<realvec_t> ComputeMomenta(Bravais const& bravais) const;

  /// Dimensionality of the lattice
  size_t dim_{ 0 };

  /// Size of the lattice
  offsets_t size_{};

  /// Number of sites in the lattice
  size_t numsites_{ 0 };

  /// Number of pairs of lattice sites
  size_t numpairs_{ 0 };

  /// Flag for closed/open boundary conditions
  bool openboundaries_{ false };

private:
  /// Positions of all the sites
  /// Assuming that the first site has position `(0,0)`
  std::vector<realvec_t> position_{};

  /// All the distance vectors between pairs of sites
  std::vector<realvec_t> vectors_{};

  /// All the distances on the lattice with minimum image convention
  realvec_t distance_{};

  /// vector of nearest neighbors
  neighbors_t neighbors_{};

  /// Allowed values momenta
  std::vector<realvec_t> momenta_{};

}; // class Lattice

inline Lattice::Lattice(Bravais const& bravais,
                        offsets_t const& size,
                        bool openboundaries)
  : dim_(size.size())
  , size_(size)
  , numsites_(accumulate_product(size))
  , numpairs_(pairs::GetNumPairs(numsites_))
  , openboundaries_(openboundaries)
  , position_(ComputePositions(bravais))
  , vectors_(ComputeVectors(bravais))
  , distance_(ComputeDistances(bravais))
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
  assert(a < numsites_);
  assert(b < numsites_);
  return distance_[GetPairIndex(a, b)];
}

inline Lattice::realvec_t
Lattice::GetVector(size_t a, size_t b) const
{
  assert(a < numsites_);
  assert(b < numsites_);
  return vectors_[GetPairIndex(a, b)];
}

inline Lattice::coords_t
Lattice::GetJump(size_t a, size_t b) const
{
  assert(a < numsites_);
  assert(b < numsites_);
  auto cb = GetCoordinates(b);
  subtract_into(cb, GetCoordinates(a));

  for (auto i = 0UL; i < dim_; i++) {
    auto& cbi = cb[i];
    auto si = size_[i];

    if (cbi > static_cast<int>(si / 2)) {
      cbi -= si;
    }
    if (cbi <= -static_cast<int>(si / 2)) {
      cbi += si;
    }
  }

  return cb;
}

inline Lattice::coords_t
Lattice::GetWinding(Lattice::coords_t jumps) const
{
  assert(jumps.size() == dim_);

  std::transform(jumps.begin(),
                 jumps.end(),
                 size_.begin(),
                 jumps.begin(),
                 std::divides<typename coords_t::value_type>());

  return jumps;
}

inline Lattice::realvec_t
Lattice::GetPosition(size_t a) const
{
  assert(a < numsites_);
  return position_[a];
}

inline bool
Lattice::AreNeighbors(size_t a, size_t b) const
{
  assert(a < neighbors_.size());

  auto result = std::find(neighbors_[a].begin(), neighbors_[a].end(), b);

  return result != neighbors_[a].end();
}

inline std::vector<Lattice::realvec_t>
Lattice::ComputePositions(Bravais const& bravais) const
{
  auto p = std::vector<realvec_t>{};
  auto c0 = GetCoordinates(0);
  for (auto i = 0UL; i < numsites_; i++) {
    auto x = bravais.GetVector(c0, GetCoordinates(i));
    p.push_back(x);
  }

  return p;
}

inline Lattice::realvec_t
Lattice::ComputeDistances(Bravais const& /*bravais*/) const
{
  auto p = realvec_t{};
  std::transform(vectors_.begin(),
                 vectors_.end(),
                 std::back_inserter(p),
                 [](realvec_t const& x) -> double {
                   return sqrt(sum_squared<realvec_t, double>(x));
                 });
  return p;
}

inline std::vector<Lattice::realvec_t>
Lattice::ComputeVectors(Bravais const& bravais) const
{
  auto p = std::vector<realvec_t>(numpairs_, realvec_t(dim_, 0.0));
  const auto imgsize = offsets_t(dim_, 3);
  const auto nimg = accumulate_product(imgsize);

  const auto c0 = GetCoordinates(0UL);
  for (auto pair = 0UL; pair < numpairs_; pair++) {
    auto [i, j] = GetIndividualIndices(pair);
    if (i > j) {
      continue;
    }
    auto ci = GetCoordinates(i);
    auto cj = GetCoordinates(j);
    auto pair2 = GetPairIndex(j, i);

    subtract_into(cj, ci);
    EnforceBoundaries(cj);

    // minimum distance found
    auto [mindist, minvec] = bravais.GetDistanceVector(c0, cj);

    // search for the minimum distance across the first shell of
    // periodic images if we are with closed boundary condition
    if (!openboundaries_) {
      for (auto k = 0ul; k < nimg; k++) {
        auto img = IndexToArray<coords_t, offsets_t>(k, imgsize);
        auto cjm = coords_t(cj);
        for (auto m = 0UL; m < dim_; m++) {
          cjm[m] += (img[m] - 1) * size_[m];
        }
        auto [dist, vec] = bravais.GetDistanceVector(c0, cjm);

        if (dist < mindist) {
          mindist = dist;
          minvec = vec;
        }
      }
    }
    p[pair] = minvec;
    p[pair2] = minvec;
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

inline size_t
Lattice::GetMappedSite(size_t a, size_t b) const
{
  auto cb = GetCoordinates(b);
  subtract_into(cb, GetCoordinates(a));
  EnforceBoundaries(cb);
  return GetOffset(cb);
}

inline Lattice::realvec_t
Lattice::ComputeSk(realvec_t occupations) const
{
  auto sk = std::vector<double>(numsites_, 0.0);

  if (openboundaries_) {
    return sk;
  }

  for (auto i = 0UL; i < numsites_; i++) {
    auto const k = GetMomentum(i);
    auto im = 0.0;
    auto re = 0.0;

    for (auto j = 0UL; j < numsites_; j++) {
      auto const x = GetVector(0, i);
      auto prod = 0.0;
      for (auto q = 0UL; q < dim_; q++) {
        prod += k[q] * x[q];
      }
      im += sin(prod) * occupations[j];
      re += cos(prod) * occupations[j];
    }
    sk[i] = (square(im) + square(re)) / square(numsites_);
  }

  return sk;
}

inline void
Lattice::SaveDistances(std::string fname) const {
  auto out = std::ofstream{fname.c_str()};

  for (auto i = 0UL; i < numsites_; i++) {
    auto vec = GetVector(0, i);
    fmt::print(out, "{}", i);
    for (auto const& v : vec) {
      fmt::print(out, ",{}", v);
    }
    fmt::print(out, "\n");
  }
}

inline void
Lattice::SaveMomenta(std::string fname) const {
  auto out = std::ofstream{fname.c_str()};

  for (auto i = 0UL; i < numsites_; i++) {
    fmt::print(out, "{}", i);
    for (auto const& k : momenta_[i]) {
      fmt::print(out, ",{}", k);
    }
    fmt::print(out, "\n");
  }

}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
