//===-- Approx.hpp ------------------------------------- -------*- C++ -*-===//
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
/// \date       June, 2017
/// \brief      Definitions for the Approx Class
///
//===---------------------------------------------------------------------===//
#ifndef BWSL_APPROX_HPP
#define BWSL_APPROX_HPP

// std
#include <algorithm>
#include <cmath>
#include <limits>

namespace bwsl {

///
/// \brief      Smart type for comparison of floating point numbers
/// \author     Guido Masella (guido.masella@gmail.com)
///
class Approx
{
public:

  /// Default constructor
  Approx() = delete;

  /// Copy assignment operator
  Approx& operator=(const Approx& that) = delete;

  /// Constructor
  Approx(const double val);

  /// Copy constructor
  Approx(const Approx& that) = default;

  /// Default destructor
  virtual ~Approx() = default;

protected:
private:
  const double value_{};
  const double epsilon_{ std::numeric_limits<double>::epsilon() * 100.0 };

  friend bool operator==(Approx const& lhs, double rhs);
  friend bool operator!=(double lhs, Approx const& rhs);
  friend bool operator!=(Approx const& lhs, double rhs);
  friend bool operator<=(double lhs, Approx const& rhs);
  friend bool operator<=(Approx const& lhs, double rhs);
  friend bool operator>=(double lhs, Approx const& rhs);
  friend bool operator>=(Approx const& lhs, double rhs);
  friend bool operator==(double lhs, Approx const& rhs);
  friend bool operator<(double lhs, Approx const& rhs);
  friend bool operator>(double lhs, Approx const& rhs);
}; // class Approx

bool
operator==(double lhs, Approx const& rhs)
{
  return std::abs(lhs - rhs.value_) <=
         rhs.epsilon_ * std::max(std::abs(lhs), std::abs(rhs.value_));
}

bool
operator==(Approx const& lhs, double rhs)
{
  return operator==(rhs, lhs);
}

bool
operator!=(double lhs, Approx const& rhs)
{
  return !operator==(lhs, rhs);
}

bool
operator!=(Approx const& lhs, double rhs)
{
  return !operator==(rhs, lhs);
}

bool
operator<=(double lhs, Approx const& rhs)
{
  return lhs < rhs.value_ || lhs == rhs;
}

bool
operator<=(Approx const& lhs, double rhs)
{
  return lhs.value_ < rhs || lhs == rhs;
}

bool
operator>=(double lhs, Approx const& rhs)
{
  return lhs > rhs.value_ || lhs == rhs;
}

bool
operator>=(Approx const& lhs, double rhs)
{
  return lhs.value_ > rhs || lhs == rhs;
}

Approx::Approx(double val)
  : value_(val)
{
}

} // namespace bwsl

#endif // BWSL_APPROX_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
