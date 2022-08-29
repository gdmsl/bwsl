//===-- Bravais.hpp --------------------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2016-2022 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// @file
/// @author     Guido Masella (guido.masella@gmail.com)
/// @brief      Definitions for the Bravais Class
///
//===---------------------------------------------------------------------===//
#pragma once

// bwsl
#include <bwsl/MathUtils.hpp>

// std
#include <cassert>
#include <cmath>
#include <vector>

namespace bwsl {

///
/// Representation of an infinite bravais lattice.
///
class Bravais
{
public:
  /// Coordinates of a point in the lattice
  using coords_t = std::vector<long>;

  /// Shorthand for real valued vectors
  using realvec_t = std::vector<double>;

  /// Vector of coordinates
  using neighbors_t = std::vector<long>;

  /// Construct an abstract lattice
  Bravais(size_t dim_,
          size_t gamma,
          realvec_t pvectors,
          realvec_t pivectors,
          neighbors_t neighbors);

  /// Copy constructor
  Bravais(Bravais const& that) = default;

  /// Copy constructor
  Bravais(Bravais&& that) = default;

  /// Default destructor
  virtual ~Bravais() = default;

  /// Copy assignment operator
  auto operator=(Bravais const& that) -> Bravais& = default;

  /// Copy assignment operator
  auto operator=(Bravais&& that) -> Bravais& = default;

  /// Get the dimensionality
  [[nodiscard]] auto GetDim() const -> size_t { return dim_; };

  /// Get the coordination number
  [[nodiscard]] auto GetGamma() const -> size_t { return gamma_; };

  /// Get the real space position of a point
  [[nodiscard]] auto GetRealSpace(coords_t const& coords) const -> realvec_t;

  /// Get the coordinates of a realspace vector on the lattice basis
  [[nodiscard]] auto GetInverseVector(realvec_t const& realspace) const
    -> realvec_t;

  /// Reciprocal space vector
  [[nodiscard]] auto GetReciprocalSpace(coords_t const& coords) const
    -> realvec_t;

  /// Get the real space vector connecting two points on the lattice
  [[nodiscard]] auto GetVector(coords_t const& first,
                               coords_t const& second) const -> realvec_t;

  /// Get the distance vector in real space between two points
  [[nodiscard]] auto GetDistance(coords_t const& first,
                                 coords_t const& second) const -> double;

  [[nodiscard]] auto GetDistanceVector(coords_t const& first,
                                       coords_t const& second) const
    -> std::pair<double, realvec_t>;

  /// Get one of the neighbors of a lattice point
  [[nodiscard]] auto GetNeighbor(coords_t const& point, size_t idx) const
    -> Bravais::coords_t;

protected:
private:
  /// Dimensionality
  size_t dim_{};

  /// Coordination number
  size_t gamma_{};

  /// Direct lattice vectors
  /// it is a dxd matrix
  realvec_t pvectors_{};

  /// Inverse of the pvectors_ matrix
  realvec_t pivectors_{};

  /// Neighbors directions
  neighbors_t neighbors_{};
}; // class Bravais

inline Bravais::Bravais(size_t dim,
                        size_t gamma,
                        realvec_t pvectors,
                        realvec_t pivectors,
                        neighbors_t neighbors)
  : dim_(dim)
  , gamma_(gamma)
  , pvectors_(std::move(pvectors))
  , pivectors_(std::move(pivectors))
  , neighbors_(std::move(neighbors))
{
  assert(pvectors_.size() == square(dim));
  assert(pivectors_.size() == square(dim));
  assert(neighbors_.size() == gamma / 2UL * dim);
}

inline auto
Bravais::GetRealSpace(coords_t const& coords) const -> Bravais::realvec_t
{
  assert(coords.size() == dim_ && "Dimensions mismatch");
  auto p = Bravais::realvec_t(dim_, 0.0);
  for (auto i = 0UL; i < dim_; i++) {
    for (auto j = 0UL; j < dim_; j++) {
      p[i] += coords[j] * pvectors_[i + j*dim_];
    }
  }
  return p;
}

inline auto
Bravais::GetInverseVector(realvec_t const& realspace) const
  -> Bravais::realvec_t
{
  assert(realspace.size() == dim_ && "Dimensions mismatch");
  auto p = Bravais::realvec_t(dim_, 0.0);
  for (auto i = 0UL; i < dim_; i++) {
    for (auto j = 0UL; j < dim_; j++) {
      p[i] += realspace[i] * pivectors_[i * dim_ + j];
    }
  }
  return p;
}

inline auto
Bravais::GetReciprocalSpace(coords_t const& coords) const -> Bravais::realvec_t
{
  assert(coords.size() == dim_ && "Dimensions mismatch");
  auto p = Bravais::realvec_t(dim_, 0.0);
  for (auto i = 0UL; i < dim_; i++) {
    for (auto j = 0UL; j < dim_; j++) {
      p[i] += coords[j] * 2 * M_PI * pivectors_[i + j*dim_];
    }
  }
  return p;
}

inline auto
Bravais::GetVector(coords_t const& first, coords_t const& second) const
  -> Bravais::realvec_t
{
  auto p = coords_t(dim_, 0.0);
  for (auto i = 0UL; i < dim_; i++) {
    p[i] = second[i] - first[i];
  }

  return GetRealSpace(p);
}

inline auto
Bravais::GetDistance(coords_t const& first, coords_t const& second) const
  -> double
{
  auto p = GetVector(first, second);
  auto d = 0.0;
  for (size_t i = 0; i < dim_; i++) {
    d += square(p[i]);
  }

  return std::sqrt(d);
}

inline auto
Bravais::GetDistanceVector(coords_t const& first, coords_t const& second) const
  -> std::pair<double, Bravais::realvec_t>
{
  auto p = GetVector(first, second);
  auto d = 0.0;
  for (size_t i = 0; i < dim_; i++) {
    d += square(p[i]);
  }

  return std::make_pair(std::sqrt(d), p);
}

inline auto
Bravais::GetNeighbor(coords_t const& point, size_t idx) const
  -> Bravais::coords_t
{
  auto aidx = idx / 2UL;
  auto sidx = idx % 2UL == 0UL ? 1 : -1;
  auto n = point;

  for (auto i = 0UL; i < dim_; i++) {
    n[i] += sidx * neighbors_[aidx * dim_ + i];
  }
  return n;
}

// clang-format off
const auto ChainLattice = Bravais(
  1UL,
  2UL,
  { 1.0 },
  { 1.0 },
  { 1 }
);
const auto SquareLattice = Bravais(
  2UL,
  4UL,
  { 1.0, 0.0, 0.0, 1.0 },
  { 1.0, 0.0, 0.0, 1.0 },
  { 1, 0, 0, 1 }
);
const auto CubicLattice = Bravais(
  3UL,
  6UL,
  { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 },
  { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 },
  { 1, 0, 0, 0, 1, 0, 0, 0, 1 }
);
const auto TriangularLattice = Bravais(
  2UL,
  6UL,
  { 1.0, 0.0, 0.5, std::sqrt(3.0) / 2.0 },
  { 1.0, -1.0 / std::sqrt(3), 0, 2.0 / std::sqrt(3) },
  { 1, 0, 0, 1, 1, -1 }
);

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
