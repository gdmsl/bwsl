//===-- RelationalUtils.hpp ------------------------------------*- C++ -*-===//
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
/// @brief      Macros for fast definition of relational operators
///
//===---------------------------------------------------------------------===//
#pragma once

/// Define the relational operators starting as friends in a class
#define FDEFINEOVERLOADRELATIONAL(T, S)                                        \
  friend bool operator==(T const& a, S const& b);                              \
  friend bool operator!=(T const& a, S const& b);                              \
  friend bool operator<(T const& a, S const& b);                               \
  friend bool operator<=(T const& a, S const& b);                              \
  friend bool operator>(T const& a, S const& b);                               \
  friend bool operator>=(T const& a, S const& b);

/// Overload the relational operators starting from an already implemented
/// `operator<`
#define OVERLOADRELATIONAL(T, S)                                               \
  inline bool operator==(T const& a, S const& b)                               \
  {                                                                            \
    return !(a < b) && !(b < a);                                               \
  }                                                                            \
  inline bool operator!=(T const& a, S const& b)                               \
  {                                                                            \
    return !(a == b);                                                          \
  }                                                                            \
  inline bool operator<=(T const& a, S const& b)                               \
  {                                                                            \
    return !(b < a);                                                           \
  }                                                                            \
  inline bool operator>(T const& a, S const& b)                                \
  {                                                                            \
    return b < a;                                                              \
  }                                                                            \
  inline bool operator>=(T const& a, S const& b)                               \
  {                                                                            \
    return !(a < b);                                                           \
  }

/// Overload the relational operators starting from the already implemented
/// `operator<` and `operator==`
#define OVERLOADRELATIONALNOEQ(T, S)                                           \
  inline bool operator!=(T const& a, S const& b)                               \
  {                                                                            \
    return !(a == b);                                                          \
  }                                                                            \
  inline bool operator<=(T const& a, S const& b)                               \
  {                                                                            \
    return !(b < a);                                                           \
  }                                                                            \
  inline bool operator>(T const& a, S const& b)                                \
  {                                                                            \
    return b < a;                                                              \
  }                                                                            \
  inline bool operator>=(T const& a, S const& b)                               \
  {                                                                            \
    return !(a < b);                                                           \
  }

/// Overload the relational operators starting from an already implemented
/// `operator<`
#define TEMPLATEOVERLOADRELATIONAL(A, B, T)                                    \
  template<class T>                                                            \
  inline bool operator==(A const& a, B const& b)                               \
  {                                                                            \
    return !(a < b) && !(b < a);                                               \
  }                                                                            \
  template<class T>                                                            \
  inline bool operator!=(A const& a, B const& b)                               \
  {                                                                            \
    return !(a == b);                                                          \
  }                                                                            \
  template<class T>                                                            \
  inline bool operator<=(A const& a, B const& b)                               \
  {                                                                            \
    return !(b < a);                                                           \
  }                                                                            \
  template<class T>                                                            \
  inline bool operator>(A const& a, B const& b)                                \
  {                                                                            \
    return b < a;                                                              \
  }                                                                            \
  template<class T>                                                            \
  inline bool operator>=(A const& a, B const& b)                               \
  {                                                                            \
    return !(a < b);                                                           \
  }

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
