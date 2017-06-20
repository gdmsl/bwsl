//===-- Approx.hpp ------------------------------------------------*- C++
//-*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2017 Guido Masella. All Rights Reserved.
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
template<typename T = double>
class Approx
{
public:
  /// Value Type
  using value_type = T;

  /// Default constructor
  Approx() = delete;

  /// Copy assignment operator
  Approx& operator=(const Approx& that) = delete;

  /// Constructor
  Approx(const value_type val);

  /// Copy constructor
  Approx(const Approx& that) = default;

  /// Default destructor
  virtual ~Approx() = default;

protected:
private:
  const value_type value_{};
  const value_type epsilon_{ std::numeric_limits<T>::epsilon() * 100 };

  friend bool operator==(Approx const& lhs, T rhs);
  friend bool operator!=(T lhs, Approx const& rhs);
  friend bool operator!=(Approx const& lhs, T rhs);
  friend bool operator<=(T lhs, Approx const& rhs);
  friend bool operator<=(Approx const& lhs, T rhs);
  friend bool operator>=(T lhs, Approx const& rhs);
  friend bool operator>=(Approx const& lhs, T rhs);
  friend bool operator==(T lhs, Approx const& rhs);
  friend bool operator<(T lhs, Approx const& rhs);
  friend bool operator>(T lhs, Approx const& rhs);
}; // class Approx

template<typename T>
bool
operator==(T lhs, Approx<T> const& rhs)
{
  return std::abs(lhs - rhs.value_) <=
         rhs.epsilon_ * std::max(std::abs(lhs), std::abs(rhs.value_));
}

template<typename T>
bool
operator==(Approx<T> const& lhs, T rhs)
{
  return operator==(rhs, lhs);
}

template<typename T>
bool
operator!=(T lhs, Approx<T> const& rhs)
{
  return !operator==(lhs, rhs);
}

template<typename T>
bool
operator!=(Approx<T> const& lhs, T rhs)
{
  return !operator==(rhs, lhs);
}

template<typename T>
bool
operator<=(T lhs, Approx<T> const& rhs)
{
  return lhs < rhs.value_ || lhs == rhs;
}

template<typename T>
bool
operator<=(Approx<T> const& lhs, T rhs)
{
  return lhs.value_ < rhs || lhs == rhs;
}

template<typename T>
bool
operator>=(T lhs, Approx<T> const& rhs)
{
  return lhs > rhs.value_ || lhs == rhs;
}

template<typename T>
bool
operator>=(Approx<T> const& lhs, T rhs)
{
  return lhs.value_ > rhs || lhs == rhs;
}

} // namespace bwsl

#endif // BWSL_APPROX_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
