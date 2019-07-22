//===-- Approx.hpp ------------------------------------- -------*- C++ -*-===//
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
/// @brief      Definitions for the Approx Class
///
//===---------------------------------------------------------------------===//
#pragma once

// std
#include <algorithm>
#include <cmath>
#include <limits>

namespace bwsl {

///
/// Smart type for comparison of floating point numbers
///
class Approx
{
public:
  /// Default constructor
  Approx() = delete;

  /// Copy assignment operator
  Approx& operator=(Approx const& that) = delete;

  /// Constructor
  template <typename T>
  Approx(T val);

  /// Copy constructor
  Approx(Approx const& that) = default;

  /// Default destructor
  virtual ~Approx() = default;

  Approx& SetEpsilon(double epsilon) { epsilon_ = epsilon; return *this; };

protected:
private:
  double value_{};
  double epsilon_{std::numeric_limits<double>::epsilon() * 100};

  template <typename T>
  friend bool operator==(Approx const& lhs, T rhs);
  template <typename T>
  friend bool operator==(T lhs, Approx const& rhs);
  template <typename T>
  friend bool operator!=(double lhs, Approx const& rhs);
  template <typename T>
  friend bool operator!=(Approx const& lhs, T rhs);
  template <typename T>
  friend bool operator<=(T lhs, Approx const& rhs);
  template <typename T>
  friend bool operator<=(Approx const& lhs, T rhs);
  template <typename T>
  friend bool operator>=(T lhs, Approx const& rhs);
  template <typename T>
  friend bool operator>=(Approx const& lhs, T rhs);
  template <typename T>
  friend bool operator<(Approx lhs, T const& rhs);
  template <typename T>
  friend bool operator<(T lhs, Approx const& rhs);
  template <typename T>
  friend bool operator>(Approx lhs, T const& rhs);
  template <typename T>
  friend bool operator>(T lhs, Approx const& rhs);
}; // class Approx

template <typename T>
inline bool
operator==(T lhs, Approx const& rhs)
{
  auto lhs_v = static_cast<double>(lhs);
  return std::abs(lhs_v - rhs.value_) <=
         rhs.epsilon_ * std::max(lhs_v, rhs.value_);
}

template <typename T>
inline bool
operator==(Approx const& lhs, T rhs)
{
  return operator==(rhs, lhs);
}

template <typename T>
inline bool
operator!=(T lhs, Approx const& rhs)
{
  return !(lhs == rhs);
}

template <typename T>
inline bool
operator!=(Approx const& lhs, T rhs)
{
  return !(rhs == lhs);
}

template <typename T>
inline bool
operator<=(T lhs, Approx const& rhs)
{
  auto lhs_v = static_cast<double>(lhs);
  return lhs_v < rhs.value_ || lhs_v == rhs;
}

template <typename T>
inline bool
operator<=(Approx const& lhs, T rhs)
{
  auto rhs_v = static_cast<double>(rhs);
  return lhs.value_ < rhs_v || lhs == rhs_v;
}

template <typename T>
inline bool
operator>=(T lhs, Approx const& rhs)
{
  auto lhs_v = static_cast<double>(lhs);
  return lhs_v > rhs.value_ || lhs_v == rhs;
}

template <typename T>
inline bool
operator>=(Approx const& lhs, T rhs)
{
  auto rhs_v = static_cast<double>(rhs);
  return lhs.value_ > rhs_v || lhs == rhs_v;
}

template <typename T>
inline Approx::Approx(T val)
  : value_(static_cast<double>(val))
{}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
