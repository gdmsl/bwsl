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
  using coordinates_t = std::vector<int>;
  using realvec_t = std::vector<double>;

  /// Construct an abstract lattice
  Bravais(size_t dim_, realvec_t pvectors, realvec_t mvectors);

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

  /// Get the real space position of a point
  realvec_t GetRealspace(coordinates_t const& coords) const;

  /// Get the real space vector connecting two points on the lattice
  realvec_t GetVector(coordinates_t const& first,
                      coordinates_t const& second) const;

  /// Get the distance vector in real space between two points
  double GetDistance(coordinates_t const& first,
                     coordinates_t const& second) const;

  /// Generate the allowed momenta for a lattice of given size
  std::vector<realvec_t> GenMomenta(std::vector<size_t> const& sizes);

  /// Generate the coordinates on a lattice of given size
  std::vector<realvec_t> GenMinimumVectors(std::vector<size_t> const&);

protected:
private:
  /// Dimensionality
  size_t dim_{};

  /// Direct lattice vectors
  /// it is a dxd matrix
  realvec_t pvectors_{};

  /// Reciprocal lattice vectors
  /// it is a dxd matrix
  realvec_t mvectors_{};
}; // class Bravais

Bravais::Bravais(size_t dim_, realvec_t pvectors, realvec_t mvectors)
  : pvectors_(std::move(mvectors))
  , mvectors_(std::move(mvectors))
{}

Bravais::realvec_t
Bravais::GetRealspace(coordinates_t const& coords) const
{
  assert(coords.size() == dim_ && "Dimensions mismatch");
  auto p = Bravais::realvec_t(dim_, 0.0);
  for (auto i = 0ul; i < dim_; i++) {
    for (auto j = 0ul; j < dim_; j++) {
      p[i] += coords[i] * pvectors_[i * dim_ + j];
    }
  }
  return std::move(p);
}

Bravais::realvec_t
Bravais::GetVector(coordinates_t const& first,
                   coordinates_t const& second) const
{
  auto p = coordinates_t(dim_, 0.0);
  for (auto i = 0ul; i < dim_; i++) {
    p[i] = first[i] - second[i];
  }

  return GetRealspace(p);
}

double
Bravais::GetDistance(coordinates_t const& first,
                     coordinates_t const& second) const
{
  auto p = GetVector(first, second);
  auto d = 0.0;
  for (size_t i = 0; i < dim_; i++) {
    d += square(p[i]);
  }

  return std::sqrt(d);
}

const auto ChainLattice = Bravais(1ul, { 1.0 }, { 2 * M_PI });
const auto SquareLattice =
  Bravais(2ul, { 1.0, 0.0, 0.0, 1.0 }, { 2 * M_PI, 0.0, 0.0, 2 * M_PI });
const auto CubicLattice =
  Bravais(2ul,
          { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 },
          { 2 * M_PI, 0.0, 0.0, 0.0, 2 * M_PI, 0.0, 0.0, 0.0, 2 * M_PI });
const auto TriangularLattice = Bravais(
  3ul,
  { 1.0, 0.5, 0.0, std::sqrt(3.0) / 2.0 },
  { 2 * M_PI, 0.0, -2 * M_PI / std::sqrt(3.0), 4 * M_PI / std::sqrt(3.0) });

} // namespace bwsl

#endif // BWSL_BRAVAIS_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
