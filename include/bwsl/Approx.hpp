//===-- Approx.hpp ------------------------------------- -------*- C++ -*-===//
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

  /// Copy constructor
  Approx(Approx const& that) = default;

  /// Move constructor
  Approx(Approx&& that) = default;

  /// Copy assignment operator
  auto operator=(Approx const& that) -> Approx& = delete;

  /// Move assignment operator
  auto operator=(Approx&& that) -> Approx& = delete;

  /// Default destructor
  virtual ~Approx() = default;

  /// Constructor
  Approx(double val);

  /// Set the absolute value of the error
  auto SetAbs(double abs) -> Approx&
  {
    abs_ = std::abs(abs);
    return *this;
  };

  /// Set the relative value of the error
  auto SetRel(double rel) -> Approx&
  {
    rel_ = std::abs(rel);
    return *this;
  };

protected:
private:
  double value_{};
  double abs_{ 0.0 };
  double rel_{ std::numeric_limits<double>::epsilon() * 100 };

  friend auto operator==(Approx const& lhs, double rhs) -> bool;
  friend auto operator==(double lhs, Approx const& rhs) -> bool;
  friend auto operator!=(double lhs, Approx const& rhs) -> bool;
  friend auto operator!=(Approx const& lhs, double rhs) -> bool;
  friend auto operator<=(double lhs, Approx const& rhs) -> bool;
  friend auto operator<=(Approx const& lhs, double rhs) -> bool;
  friend auto operator>=(double lhs, Approx const& rhs) -> bool;
  friend auto operator>=(Approx const& lhs, double rhs) -> bool;
  friend auto operator<(Approx lhs, double const& rhs) -> bool;
  friend auto operator<(double lhs, Approx const& rhs) -> bool;
  friend auto operator>(Approx lhs, double const& rhs) -> bool;
  friend auto operator>(double lhs, Approx const& rhs) -> bool;
}; // class Approx

inline auto
operator==(double lhs, Approx const& rhs) -> bool
{
  return std::abs(lhs - rhs.value_) <=
         std::max(rhs.abs_, std::abs(rhs.rel_ * rhs.value_));
}

inline auto
operator==(Approx const& lhs, double rhs) -> bool
{
  return operator==(rhs, lhs);
}

inline auto
operator!=(double lhs, Approx const& rhs) -> bool
{
  return !(lhs == rhs);
}

inline auto
operator!=(Approx const& lhs, double rhs) -> bool
{
  return !(rhs == lhs);
}

inline auto
operator<=(double lhs, Approx const& rhs) -> bool
{
  return lhs < rhs.value_ || lhs == rhs;
}

inline auto
operator<=(Approx const& lhs, double rhs) -> bool
{
  return lhs.value_ < rhs || lhs == rhs;
}

inline auto
operator>=(double lhs, Approx const& rhs) -> bool
{
  return lhs > rhs.value_ || lhs == rhs;
}

inline auto
operator>=(Approx const& lhs, double rhs) -> bool
{
  return lhs.value_ > rhs || lhs == rhs;
}

inline Approx::Approx(double val)
  : value_(static_cast<double>(val))
{}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
