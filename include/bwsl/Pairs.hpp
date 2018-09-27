//===-- Pairs.hpp ----------------------------------------------*- C++ -*-===//
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
/// \date       September, 2018
/// \brief      Definitions for the Pairs
///
//===---------------------------------------------------------------------===//
#pragma once

// std
#include <utility>
#include <cmath>
#include <cstdlib>

namespace bwsl {

namespace pairs {

///
/// Get the index of a pair from the values of the two single indices
///
inline size_t
GetPairIndex(size_t a, size_t b, size_t maxval, bool noself = false)
{
  auto i = b * maxval + a;
  if (noself) {
    i -= i / maxval;
  }
  return i;
}

///
/// Get the index of the pair from the pair of indices
///
inline size_t
GetPairIndex(const std::pair<size_t, size_t> pair,
             const size_t maxval,
             const bool noself = false)
{
  return GetPairIndex(pair.first, pair.second, maxval, noself);
}

///
/// Get a pair of indices from the index of the pair
///
inline std::pair<size_t, size_t>
GetPair(size_t i, const size_t maxval, const bool noself = false)
{
  if (noself) {
    i += i / (maxval - 1ul);
  }
  return std::make_pair(i % maxval, i / maxval);
}

///
/// Get the number of indices of pairs of unsigned values
///
inline size_t
GetNumPairs(size_t maxval, bool noself = false)
{
  return maxval * (noself ? maxval - 1ul : maxval + 1ul) / 2ul;
}

} // namespace pairs

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
