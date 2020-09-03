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
  Approx(double val);

  /// Copy constructor
  Approx(Approx const& that) = default;

  /// Default destructor
  virtual ~Approx() = default;

  /// Set the absolute value of the error
  Approx& SetAbs(double abs)
  {
    abs_ = std::abs(abs);
    return *this;
  };

  /// Set the relative value of the error
  Approx& SetRel(double rel)
  {
    rel_ = std::abs(rel);
    return *this;
  };

protected:
private:
  double value_{};
  double abs_{ 0.0 };
  double rel_{ std::numeric_limits<double>::epsilon() * 100 };

  friend bool operator==(Approx const& lhs, double rhs);
  friend bool operator==(double lhs, Approx const& rhs);
  friend bool operator!=(double lhs, Approx const& rhs);
  friend bool operator!=(Approx const& lhs, double rhs);
  friend bool operator<=(double lhs, Approx const& rhs);
  friend bool operator<=(Approx const& lhs, double rhs);
  friend bool operator>=(double lhs, Approx const& rhs);
  friend bool operator>=(Approx const& lhs, double rhs);
  friend bool operator<(Approx lhs, double const& rhs);
  friend bool operator<(double lhs, Approx const& rhs);
  friend bool operator>(Approx lhs, double const& rhs);
  friend bool operator>(double lhs, Approx const& rhs);
}; // class Approx

inline bool
operator==(double lhs, Approx const& rhs)
{
  return std::abs(lhs - rhs.value_) <=
         std::max(rhs.abs_, std::abs(rhs.rel_ * rhs.value_));
}

inline bool
operator==(Approx const& lhs, double rhs)
{
  return operator==(rhs, lhs);
}

inline bool
operator!=(double lhs, Approx const& rhs)
{
  return !(lhs == rhs);
}

inline bool
operator!=(Approx const& lhs, double rhs)
{
  return !(rhs == lhs);
}

inline bool
operator<=(double lhs, Approx const& rhs)
{
  return lhs < rhs.value_ || lhs == rhs;
}

inline bool
operator<=(Approx const& lhs, double rhs)
{
  return lhs.value_ < rhs || lhs == rhs;
}

inline bool
operator>=(double lhs, Approx const& rhs)
{
  return lhs > rhs.value_ || lhs == rhs;
}

inline bool
operator>=(Approx const& lhs, double rhs)
{
  return lhs.value_ > rhs || lhs == rhs;
}

inline Approx::Approx(double val)
  : value_(static_cast<double>(val))
{}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
