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
#include <bwsl/HyperCubicGrid.hpp>

// fmt
#include <fmt/format.h>
#include <fmt/ostream.h>

// std
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace bwsl {

///
/// Representation of a Lattice.
///
class Lattice : public HyperCubicGrid
{
public:
  /// Vector of offsets
  using vectorindex_t = std::vector<index_t>;

  /// Vector of neighbors
  using neighbors_t = std::vector<vectorindex_t>;

  /// Shorthand for real valued vectors
  using realvec_t = std::vector<double>;

  /// Default constructor
  Lattice() = default;

  /// Construct a lattice with given size from an infinite bravais lattice
  Lattice(Bravais const& bravais,
          gridsize_t const& size,
          boundaries_t openboundaries = boundaries_t::Closed);

  /// Copy constructor
  Lattice(Lattice const& that) = default;

  /// Default destructor
  virtual ~Lattice() = default;

  /// Copy assignment operator
  Lattice& operator=(Lattice const& that) = default;

  /// Get nearest neighbors of site i
  auto GetNeighbors(index_t i) const -> vectorindex_t const&
  {
    return neighbors_[i];
  }

  /// Distance betweeen two sites of the lattice
  auto GetDistance(index_t a, index_t b) const -> double;

  /// Get the vector spawning from site @p a to site @p b .
  auto GetVector(index_t a, index_t b) const -> realvec_t;

  /// Get the winding number times the system size for a hopping between the
  /// sites @p a and @p b .
  auto GetJump(size_t a, size_t b) const -> coords_t;

  /// Given an accumulator for the winding number return the total winding.
  /// The total winding is defined as the number of times we jump around the
  /// boundaries.
  auto GetWinding(coords_t jump) const -> coords_t;

  /// Get the real space coordinates of site @p a .
  auto GetPosition(index_t a) const -> realvec_t;

  /// Change a vector so that he will match the boundary conditions
  auto EnforceBoundaries(coords_t& coords) const -> void;

  /// Check if two sites are neighbors
  auto AreNeighbors(index_t a, index_t b) const -> bool;

  /// Get the coordination number
  index_t GetCoordination(index_t a) const;

  /// Get the coordination number
  index_t GetCoordination() const;

  /// Get an allowed momentum
  realvec_t GetMomentum(size_t a) const;

  /// Compute the structure factor given the occupations of the sites
  auto ComputeSk(realvec_t occupations) const -> realvec_t;

  /// Save the distances on a file
  auto SaveDistances(std::string fname) const -> void;

  /// Save the positions on a file
  auto SavePositions(std::string fname) const -> void;

  /// Save the momenta on a file
  auto SaveMomenta(std::string fname) const -> void;

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
                        gridsize_t const& size,
                        boundaries_t boundaries)
  : HyperCubicGrid(size, boundaries)
  , position_(ComputePositions(bravais))
  , vectors_(ComputeVectors(bravais))
  , distance_(ComputeDistances(bravais))
  , neighbors_(ComputeNeighbors(bravais))
  , momenta_(ComputeMomenta(bravais))
{}

inline void
Lattice::EnforceBoundaries(coords_t& coords) const
{
  for (auto i = 0ul; i < GetDim(); i++) {
    auto s = GetSize()[i];
    while (coords[i] < 0) {
      coords[i] += s;
    }
    while (coords[i] >= static_cast<long>(s)) {
      coords[i] -= s;
    }
  }
}

inline double
Lattice::GetDistance(size_t a, size_t b) const
{
  assert(IndexIsValid(a) && IndexIsValid(b));
  auto s = GetMappedSite(a, b);
  return distance_[s];
}

inline auto
Lattice::GetVector(size_t a, size_t b) const -> realvec_t
{
  assert(IndexIsValid(a) && IndexIsValid(b));
  auto s = GetMappedSite(a, b);
  return vectors_[s];
}

inline auto
Lattice::GetJump(size_t a, size_t b) const -> coords_t
{
  assert(IndexIsValid(a) && IndexIsValid(b));
  auto cb = GetCoordinates(b);
  subtract_into(cb, GetCoordinates(a));

  for (auto i = 0UL; i < GetDim(); i++) {
    auto& cbi = cb[i];
    auto si = GetSize()[i];

    if (cbi > static_cast<int>(si / 2)) {
      cbi -= si;
    }
    if (cbi <= -static_cast<int>(si / 2)) {
      cbi += si;
    }
  }

  return cb;
}

inline auto
Lattice::GetWinding(coords_t jumps) const -> coords_t
{
  assert(HasSameDimension(jumps));

  std::transform(jumps.begin(),
                 jumps.end(),
                 GetSize().cbegin(),
                 jumps.begin(),
                 std::divides<typename coords_t::value_type>());

  return jumps;
}

inline Lattice::realvec_t
Lattice::GetPosition(index_t a) const
{
  assert(IndexIsValid(a));
  return position_[a];
}

inline bool
Lattice::AreNeighbors(index_t a, index_t b) const
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
  for (auto i = 0UL; i < GetNumSites(); i++) {
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
  auto p = std::vector<realvec_t>(GetNumSites(), realvec_t(GetDim(), 0.0));
  const auto imgsize = gridsize_t(GetDim(), 3);
  const auto nimg = accumulate_product(imgsize);

  const auto c0 = GetCoordinates(0UL);
  for (auto site = 0UL; site < GetNumSites(); site++) {
    auto cs = GetCoordinates(site);

    // minimum distance found
    auto [mindist, minvec] = bravais.GetDistanceVector(c0, cs);

    // search for the minimum distance across the first shell of
    // periodic images if we are with closed boundary condition
    if (HasClosedBoundaries()) {
      for (auto k = 0ul; k < nimg; k++) {
        auto img = index_to_array<coords_t, gridsize_t>(k, imgsize);
        auto csm = coords_t(cs);
        for (auto m = 0UL; m < GetDim(); m++) {
          csm[m] += (img[m] - 1) * GetSize()[m];
        }
        auto [dist, vec] = bravais.GetDistanceVector(c0, csm);

        if (dist < mindist) {
          mindist = dist;
          minvec = vec;
        }
      }
    }
    p[site] = minvec;
  }
  return p;
}

inline Lattice::neighbors_t
Lattice::ComputeNeighbors(Bravais const& bravais) const
{
  auto p = neighbors_t{};
  const auto gamma = bravais.GetGamma();

  for (auto i = 0ul; i < GetNumSites(); i++) {
    auto nn = gridsize_t{};
    auto ci = GetCoordinates(i);
    for (auto j = 0ul; j < gamma; j++) {
      auto cj = bravais.GetNeighbor(ci, j);

      // add the sites to the list of neighbors accordingly with the
      // boundary conditions set
      if (HasClosedBoundaries() || IsOnGrid(cj)) {
        EnforceBoundaries(cj);
        nn.push_back(GetIndex(cj));
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
  if (HasOpenBoundaries()) {
    return p;
  }

  // in a lattice with periodic boundary conditions the number
  // of reciprocal lattice vectors are the same number as the
  // sites of the direct lattice
  for (auto i = 0ul; i < GetNumSites(); i++) {
    auto ci = GetCoordinates(i);
    for (auto k = 0ul; k < ci.size(); k++) {
      ci[k] -= GetSize()[k] / 2ul - 1;
    }
    auto kappa = bravais.GetReciprocalSpace(ci);
    for (auto k = 0ul; k < kappa.size(); k++) {
      kappa[k] /= GetSize()[k];
    }
    p.push_back(kappa);
  }

  return p;
}

inline auto
Lattice::ComputeSk(realvec_t occupations) const -> realvec_t
{
  auto n = GetNumSites();
  auto sk = std::vector<double>(n, 0.0);

  if (HasOpenBoundaries()) {
    return sk;
  }

  for (auto i = 0UL; i < GetNumSites(); i++) {
    auto const k = momenta_[i];
    auto im = 0.0;
    auto re = 0.0;

    for (auto j = 0UL; j < n; j++) {
      auto const x = GetVector(0, j);
      auto prod = 0.0;
      for (auto q = 0UL; q < GetDim(); q++) {
        prod += k[q] * x[q];
      }
      im += sin(prod) * occupations[j];
      re += cos(prod) * occupations[j];
    }
    sk[i] = (square(im) + square(re)) / square(n);
  }

  return sk;
}

inline auto
Lattice::SavePositions(std::string fname) const -> void
{
  auto out = std::ofstream{ fname.c_str() };

  fmt::print(out, "i");
  for (auto i = 0UL; i < GetDim(); i++) {
    fmt::print(out, ",x{}", i);
  }
  fmt::print(out, "\n");

  for (auto i = 0UL; i < GetNumSites(); i++) {
    auto vec = GetPosition(i);
    fmt::print(out, "{}", i);
    for (auto const& v : vec) {
      fmt::print(out, ",{}", v);
    }
    fmt::print(out, "\n");
  }
}

inline auto
Lattice::SaveDistances(std::string fname) const -> void
{
  auto out = std::ofstream{ fname.c_str() };

  fmt::print(out, "i");
  for (auto i = 0UL; i < GetDim(); i++) {
    fmt::print(out, ",d{}", i);
  }
  fmt::print(out, "\n");

  for (auto i = 0UL; i < GetNumSites(); i++) {
    auto vec = GetVector(0, i);
    fmt::print(out, "{}", i);
    for (auto const& v : vec) {
      fmt::print(out, ",{}", v);
    }
    fmt::print(out, "\n");
  }
}

inline void
Lattice::SaveMomenta(std::string fname) const
{
  auto out = std::ofstream{ fname.c_str() };

  fmt::print(out, "i");
  for (auto i = 0UL; i < GetDim(); i++) {
    fmt::print(out, ",k{}", i);
  }
  fmt::print(out, "\n");

  for (auto i = 0UL; i < GetNumSites(); i++) {
    fmt::print(out, "{}", i);
    for (auto const& k : momenta_[i]) {
      fmt::print(out, ",{}", k);
    }
    fmt::print(out, "\n");
  }
}

inline auto
Lattice::GetCoordination(size_t a) const -> size_t
{
  return neighbors_[a].size();
}

inline auto
Lattice::GetCoordination() const -> size_t
{
  return neighbors_[0].size();
}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
