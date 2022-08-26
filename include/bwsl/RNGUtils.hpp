//===-- RNGUtils.hpp.hpp ---------------------------------------*- C++ -*-===//
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
/// @brief      Utilities for Random number generators
///
//===---------------------------------------------------------------------===//
#pragma once

// boost
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>

// std
#include <random>
#include <sstream>
#include <string>

namespace boost::serialization {

// The following code (from START CODE to END CODE)has been taken and modified
// from StackOverflow It is released on StackOverflow under the
// LICENSE CC BY-SA 3.0
// Link to the original answer: https://stackoverflow.com/a/45873005
// Link to the user profile: https://stackoverflow.com/users/85371/sehe
// Changes include formatting and stylistics changes.
// START CODE

#define MT_TPARAMS                                                             \
  typename UIntType, size_t w, size_t n, size_t m, size_t r, UIntType a,       \
    size_t u, UIntType d, size_t s, UIntType b, size_t t, UIntType c,          \
    size_t l, UIntType f
#define MT_TARGLIST UIntType, w, n, m, r, a, u, d, s, b, t, c, l, f

template<typename Ar, MT_TPARAMS>
inline auto
load(Ar& ar, std::mersenne_twister_engine<MT_TARGLIST>& mt, unsigned) -> void
{
  std::string text;
  ar& text;
  std::istringstream iss(text);

  if (!(iss >> mt))
    throw std::invalid_argument("mersenne_twister_engine state");
}

template<typename Ar, MT_TPARAMS>
inline auto
save(Ar& ar, std::mersenne_twister_engine<MT_TARGLIST> const& mt, unsigned)
  -> void
{
  std::ostringstream oss;
  if (!(oss << mt))
    throw std::invalid_argument("mersenne_twister_engine state");
  std::string text = oss.str();
  ar& text;
}

template<typename Ar, MT_TPARAMS>
inline auto
serialize(Ar& ar,
          std::mersenne_twister_engine<MT_TARGLIST>& mt,
          unsigned version) -> void
{
  if (typename Ar::is_saving())
    save(ar, mt, version);
  else
    load(ar, mt, version);
}

#undef MT_TPARAMS
#undef MT_TARGLIST
// END CODE

} // namespace boost::serialization

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
