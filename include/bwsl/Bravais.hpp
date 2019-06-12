//===-- Bravais.hpp --------------------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2016-2018 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// \file
/// \version    0.1
/// \author     Guido Masella (guido.masella@gmail.com)
/// \brief      Definitions for the Bravais Class
///
//===---------------------------------------------------------------------===//
#ifndef BWSL_BRAVAIS_HPP
#define BWSL_BRAVAIS_HPP

// bwsl
#include <bwsl/MathUtils.hpp>

// std
#include <cassert>
#include <cmath>
#include <vector>

namespace bwsl {

///
/// \brief Representation of an infinite bravais lattice
///
class Bravais
{
public:
  /// Coordinates of a point in the lattice
  using coords_t = std::vector<int>;

  /// Shorthand for real valued vectors
  using realvec_t = std::vector<double>;

  /// Vector of coordinates
  using neighbors_t = std::vector<int>;

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
  Bravais& operator=(Bravais const& that) = default;

  /// Copy assignment operator
  Bravais& operator=(Bravais&& that) = default;

  /// Get the dimensionality
  size_t GetDim() const { return dim_; };

  /// Get the coordination number
  size_t GetGamma() const { return gamma_; };

  /// Get the real space position of a point
  realvec_t GetRealSpace(coords_t const& coords) const;

  /// Get the coordinates of a realspace vector on the lattice basis
  realvec_t GetInverseVector(realvec_t const& realspace) const;

  /// Reciprocal space vector
  realvec_t GetReciprocalSpace(coords_t const& coords) const;

  /// Get the real space vector connecting two points on the lattice
  realvec_t GetVector(coords_t const& first, coords_t const& second) const;

  /// Get the distance vector in real space between two points
  double GetDistance(coords_t const& first, coords_t const& second) const;

  /// Get one of the neighbors of a lattice point
  Bravais::coords_t GetNeighbor(coords_t const& point, size_t idx) const;

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

Bravais::Bravais(size_t dim,
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
  assert(neighbors_.size() == gamma / 2ul * dim);
}

Bravais::realvec_t
Bravais::GetRealSpace(coords_t const& coords) const
{
  assert(coords.size() == dim_ && "Dimensions mismatch");
  auto p = Bravais::realvec_t(dim_, 0.0);
  for (auto i = 0ul; i < dim_; i++) {
    for (auto j = 0ul; j < dim_; j++) {
      p[i] += coords[i] * pvectors_[i * dim_ + j];
    }
  }
  return p;
}

inline Bravais::realvec_t
Bravais::GetInverseVector(realvec_t const& realspace) const
{
  assert(realspace.size() == dim_ && "Dimensions mismatch");
  auto p = Bravais::realvec_t(dim_, 0.0);
  for (auto i = 0ul; i < dim_; i++) {
    for (auto j = 0ul; j < dim_; j++) {
      p[i] += realspace[i] * pivectors_[i * dim_ + j];
    }
  }
  return p;
}

Bravais::realvec_t
Bravais::GetReciprocalSpace(coords_t const& coords) const
{
  assert(coords.size() == dim_ && "Dimensions mismatch");
  auto p = Bravais::realvec_t(dim_, 0.0);
  for (auto i = 0ul; i < dim_; i++) {
    for (auto j = 0ul; j < dim_; j++) {
      p[i] += coords[i] * 2 * M_PI * pivectors_[i * dim_ + j];
    }
  }
  return p;
}

Bravais::realvec_t
Bravais::GetVector(coords_t const& first, coords_t const& second) const
{
  auto p = coords_t(dim_, 0.0);
  for (auto i = 0ul; i < dim_; i++) {
    p[i] = second[i] - first[i];
  }

  return GetRealSpace(p);
}

inline double
Bravais::GetDistance(coords_t const& first, coords_t const& second) const
{
  auto p = GetVector(first, second);
  auto d = 0.0;
  for (size_t i = 0; i < dim_; i++) {
    d += square(p[i]);
  }

  return std::sqrt(d);
}

inline Bravais::coords_t
Bravais::GetNeighbor(coords_t const& point, size_t idx) const
{
  auto aidx = idx / 2ul;
  auto sidx = idx % 2ul == 0ul ? 1 : -1;
  auto n = point;

  for (auto i = 0ul; i < dim_; i++) {
    n[i] += sidx * neighbors_[aidx * dim_ + i];
  }
  return n;
}

// clang-format off
const auto ChainLattice = Bravais(
  1ul,
  2ul,
  { 1.0 },
  { 1.0 },
  { 1 }
);
const auto SquareLattice = Bravais(
  2ul,
  4ul,
  { 1.0, 0.0, 0.0, 1.0 },
  { 1.0, 0.0, 0.0, 1.0 },
  { 1, 0, 0, 1 }
);
const auto CubicLattice = Bravais(
  3ul,
  6ul,
  { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 },
  { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 },
  { 1, 0, 0, 0, 1, 0, 0, 0, 1 }
);
const auto TriangularLattice = Bravais(
  2ul,
  6ul,
  { 1.0, 0.5, 0.0, std::sqrt(3.0) / 2.0 },
  { 1.0, -1.0 / std::sqrt(3), 0, 2.0 / std::sqrt(3) },
  { 1, 0, 0, 1, 1, -1 }
);

} // namespace bwsl

#endif // BWSL_BRAVAIS_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
