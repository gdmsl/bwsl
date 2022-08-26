//===-- HyperCubicGrid.hpp -------------------------------------*- C++ -*-===//
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
/// @brief      Definitions for the HyperCubicGrid Class
///
//===---------------------------------------------------------------------===//
#pragma once

// bwsl
#include <bwsl/MathUtils.hpp>
#include <bwsl/Pairs.hpp>

/// std
#include <vector>

namespace bwsl {

///
/// Hypercubic Grid
///
class HyperCubicGrid
{
public:
  /// Coordinates
  using coords_t = std::vector<long>;

  /// Sizes of the grid
  using gridsize_t = std::vector<size_t>;

  /// Type for the site indices
  using index_t = size_t;

  /// Type of the boundaries
  enum class boundaries_t
  {
    Open,
    Closed,
  };

  /// Default constructor
  HyperCubicGrid() = default;

  /// Copy constructor
  HyperCubicGrid(const HyperCubicGrid&) = default;

  /// Move constructor
  HyperCubicGrid(HyperCubicGrid&&) = default;

  /// Copy assignment operator
  auto operator=(const HyperCubicGrid&) -> HyperCubicGrid& = default;

  /// Move assignment operator
  auto operator=(HyperCubicGrid&&) -> HyperCubicGrid& = default;

  /// Constructor
  HyperCubicGrid(gridsize_t const& size, boundaries_t boundaries);

  /// Default destructor
  virtual ~HyperCubicGrid() = default;

  /// Get the number of dimension of the grid
  [[nodiscard]] auto GetDim() const -> size_t { return dim_; }

  /// Check if the grid has open boundaries
  [[nodiscard]] auto HasOpenBoundaries() const -> bool
  {
    return boundaries_ == boundaries_t::Open;
  }

  /// Check if the grid has open boundaries
  [[nodiscard]] auto HasClosedBoundaries() const -> bool
  {
    return boundaries_ == boundaries_t::Closed;
  }

  /// Get the size of the grid
  [[nodiscard]] auto GetSize() const -> std::vector<size_t> const&
  {
    return size_;
  }

  /// Get the site i mapping (a, b) to (0, i)
  [[nodiscard]] auto GetMappedSite(index_t a, index_t b) const -> index_t;

  /// Get the site i mapping (0, i) to (a, b)
  [[nodiscard]] auto GetUnMappedSite(index_t i, index_t a) const -> index_t;

  /// Convert an offset to coordinates
  [[nodiscard]] auto GetCoordinates(index_t offset) const -> coords_t;

  /// Convert coordinates to an offset
  [[nodiscard]] auto GetIndex(coords_t const& coords) const -> index_t;

  /// Change the coordinates so that they will match the boundary conditions
  auto EnforceBoundaries(coords_t& coords) const -> void;

  /// Check if the vector is on the grid
  [[nodiscard]] auto IsOnGrid(coords_t const& coords) const -> bool;

  /// Destructure a pair and return the two indices
  [[nodiscard]] auto GetIndividualIndices(index_t pair) const
    -> std::pair<index_t, index_t>;

  /// Unique index for pairs of sites
  [[nodiscard]] auto GetPairIndex(index_t a, index_t b) const -> index_t;

  /// Get the number of sites on the grid
  [[nodiscard]] auto GetNumSites() const -> size_t { return numsites_; }

  /// Check if the index is valid
  [[nodiscard]] auto IndexIsValid(index_t i) const -> bool
  {
    return i < numsites_;
  }

  /// Get the boundaries
  [[nodiscard]] auto GetBoundaries() const -> boundaries_t
  {
    return boundaries_;
  }

  /// Get this
  [[nodiscard]] auto GetGrid() const -> HyperCubicGrid { return *this; }

protected:
  /// Check the dimensions
  template<class C>
  auto HasSameDimension(C const& v) const -> bool
  {
    return v.size() == dim_;
  }

private:
  /// DImensionality of the grid
  size_t dim_{ 0UL };

  /// Size of the grid
  gridsize_t size_{};

  /// Number of sites on the grid
  size_t numsites_{ 0UL };

  /// Number of possible pairs of sites on the grid
  size_t numpairs_{ 0UL };

  /// Boundary conditions
  boundaries_t boundaries_{ boundaries_t::Open };
}; // class HyperCubicGrid

inline HyperCubicGrid::HyperCubicGrid(gridsize_t const& size,
                                      boundaries_t boundaries)
  : dim_(size.size())
  , size_(size)
  , numsites_(accumulate_product(size))
  , numpairs_(pairs::GetNumPairs(numsites_))
  , boundaries_(boundaries)
{
}

inline auto
HyperCubicGrid::GetCoordinates(index_t offset) const -> coords_t
{
  return index_to_array<coords_t, gridsize_t>(offset, size_);
}

inline auto
HyperCubicGrid::GetIndex(coords_t const& coords) const -> index_t
{
  return array_to_index(coords, size_);
}

inline auto
HyperCubicGrid::GetMappedSite(index_t a, index_t b) const -> index_t
{
  auto cb = GetCoordinates(b);
  subtract_into(cb, GetCoordinates(a));
  EnforceBoundaries(cb);
  return GetIndex(cb);
}

inline auto
HyperCubicGrid::GetUnMappedSite(index_t i, index_t a) const -> index_t
{
  auto ca = GetCoordinates(a);
  sum_into(ca, GetCoordinates(i));
  EnforceBoundaries(ca);
  return GetIndex(ca);
}

inline auto
HyperCubicGrid::EnforceBoundaries(coords_t& coords) const -> void
{
  assert(coords.size() == dim_);
  if (HasClosedBoundaries()) {
    for (auto i = 0UL; i < dim_; i++) {
      auto s = static_cast<long>(size_[i]);
      while (coords[i] < 0) {
        coords[i] += s;
      }
      while (coords[i] >= static_cast<long>(s)) {
        coords[i] -= s;
      }
    }
  }
}

inline auto
HyperCubicGrid::IsOnGrid(coords_t const& coords) const -> bool
{
  assert(coords.size() == dim_);
  for (auto i = 0UL; i < dim_; i++) {
    if (coords[i] < 0L || static_cast<size_t>(coords[i]) >= size_[i]) {
      return false;
    }
  }
  return true;
}

inline auto
HyperCubicGrid::GetPairIndex(index_t a, index_t b) const -> index_t
{
  return bwsl::pairs::GetPairIndex(a, b, numsites_);
}

inline auto
HyperCubicGrid::GetIndividualIndices(size_t pair) const
  -> std::pair<index_t, index_t>
{
  return bwsl::pairs::GetPair(pair, numsites_);
}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
