//===-- Pairs.hpp ----------------------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2016-2020 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// @file
/// @author     Guido Masella (guido.masella@gmail.com)
/// @brief      Definitions for the Pairs
///
//===---------------------------------------------------------------------===//
#pragma once

// bwsl
#include <bwsl/MathUtils.hpp>

// std
#include <cmath>
#include <cstdlib>
#include <utility>

namespace bwsl::pairs {

///
/// Get the index of a pair from the values of the two single indices
///
inline size_t
GetPairIndex(size_t a, size_t b, size_t maxval)
{
  auto i = b * maxval + a;
  return i;
}

///
/// Get the index of the pair from the pair of indices
///
inline size_t
GetPairIndex(const std::pair<size_t, size_t> pair, const size_t maxval)
{
  return GetPairIndex(pair.first, pair.second, maxval);
}

///
/// Get a pair of indices from the index of the pair
///
inline std::pair<size_t, size_t>
GetPair(size_t i, const size_t maxval)
{
  return std::make_pair(i % maxval, i / maxval);
}

///
/// Get the number of indices of pairs of unsigned values
///
inline size_t
GetNumPairs(size_t maxval)
{
  return square(maxval);
}

} // namespace bwsl::pairs

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
