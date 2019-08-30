//===-- RationalNum.hpp ----------------------------------------*- C++ -*-===//
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
/// @brief      Definitions for the RationalNum Class
///
//===---------------------------------------------------------------------===//
#pragma once

/// bwsl
#include <bwsl/RelationalUtils.hpp>

// std
#include <iostream>

namespace bwsl {

/// Class for managing rational numbers
template<class T = long>
class RationalNum
{
public:
  /// Default constructor
  RationalNum() = default;

  /// Copy constructor
  RationalNum(const RationalNum&) = default;

  /// Copy assignment operator
  RationalNum& operator=(const RationalNum&) = default;

  /// Move constructor
  RationalNum(RationalNum&&) = default;

  /// Move assignment operator
  RationalNum& operator=(RationalNum&&) = default;

  /// Default destructor
  virtual ~RationalNum() = default;

  /// Constructor
  RationalNum(T num, T den);

  /// Conversion to double
  operator double() const { return static_cast<double>(num_) / den_; };

  /// Conversion operator to integer type
  operator T() const { return num_ / den_; };

  /// @name Relational Operators
  /// @{
  friend bool operator<(RationalNum<T> const& rhs, RationalNum<T> const& lhs);
  friend bool operator<(RationalNum<T> const& rhs, T const& lhs);
  friend bool operator<(T const& rhs, RationalNum<T> const& lhs);
  /// @}

  /// @name Arithmetic operations
  /// @{
  friend RationalNum<T> operator+(RationalNum<T> const& lhs,
                                  RationalNum<T> const& rhs);
  friend RationalNum<T> operator-(RationalNum<T> const& lhs,
                                  RationalNum<T> const& rhs);
  friend RationalNum<T> operator/(RationalNum<T> const& lhs,
                                  RationalNum<T> const& rhs);
  friend RationalNum<T> operator*(RationalNum<T> const& lhs,
                                  RationalNum<T> const& rhs);
  friend RationalNum<T> operator+(RationalNum<T> const& lhs, T const& rhs);
  friend RationalNum<T> operator-(RationalNum<T> const& lhs, T const& rhs);
  friend RationalNum<T> operator/(RationalNum<T> const& lhs, T const& rhs);
  friend RationalNum<T> operator*(RationalNum<T> const& lhs, T const& rhs);
  friend RationalNum<T> operator+(T const& lhs, RationalNum<T> const& rhs);
  friend RationalNum<T> operator-(T const& lhs, RationalNum<T> const& rhs);
  friend RationalNum<T> operator/(T const& lhs, RationalNum<T> const& rhs);
  friend RationalNum<T> operator*(T const& lhs, RationalNum<T> const& rhs);
  /// @}

  /// @name Stream operators
  /// @{
  friend std::ostream& operator<<(std::ostream& out, RationalNum<T> const&);
  friend std::istream& operator>>(std::istream& in, RationalNum<T>&);
  /// @}

  /// Check if the number is an integer
  bool IsInt() const { return num_ % den_ == 0; }

  /// Simplify the stored values
  void Simplify();

  /// Get the numerator
  T GetNum() const { return num_; };

  /// Get the denominator
  T GetDen() const { return den_; };

protected:
private:
  /// Numerator
  T num_{ 0 };

  /// Denominator
  T den_{ 1 };
}; // class RationalNum

using RationalLong = RationalNum<long>;
using RationalInt = RationalNum<int>;

template<class T>
inline std::ostream&
operator<<(std::ostream& out, RationalNum<T> const& rhs)
{
  out << rhs.num_ << "//" << rhs.den_;
  return out;
}

template<class T>
inline std::istream&
operator>>(std::istream& in, RationalNum<T>& rhs)
{
  auto sep = char{};
  in >> rhs.num_;
  in >> sep;
  in >> sep;
  in >> rhs.den_;

  return in;
}

template<class T>
inline RationalNum<T>::RationalNum(T num, T den)
  : num_(num)
  , den_(den)
{
  Simplify();
}

template<class T>
inline void
RationalNum<T>::Simplify()
{
  auto g = GetGCD(num_, den_);
  num_ = num_ / g;
  den_ = den_ / g;
}

template<class T>
inline bool
operator<(const RationalNum<T>& lhs, const RationalNum<T>& rhs)
{
  return lhs.num_ * rhs.den_ < rhs.num_ * lhs.den_;
}

template<class T>
inline bool
operator<(const RationalNum<T>& lhs, const T& rhs)
{
  return lhs.num_ / lhs.den_ < rhs;
}

template<class T>
inline bool
operator<(const T& lhs, const RationalNum<T>& rhs)
{
  return lhs < rhs.num_ / rhs.den_;
}

template<class T>
inline RationalNum<T>
operator+(RationalNum<T> const& lhs, RationalNum<T> const& rhs)
{
  return RationalNum<T>(lhs.num_ * rhs.den_ + rhs.num_ * lhs.den_,
                        lhs.den_ * rhs.den_);
}

template<class T>
inline RationalNum<T>
operator-(RationalNum<T> const& lhs, RationalNum<T> const& rhs)
{
  return RationalNum<T>(lhs.num_ * rhs.den_ - rhs.num_ * lhs.den_,
                        lhs.den_ * rhs.den_);
}

template<class T>
inline RationalNum<T>
operator/(RationalNum<T> const& lhs, RationalNum<T> const& rhs)
{
  return RationalNum<T>(lhs.num_ * rhs.den_, rhs.num_ * lhs.den_);
}

template<class T>
inline RationalNum<T> operator*(RationalNum<T> const& lhs,
                                RationalNum<T> const& rhs)
{
  return RationalNum<T>(lhs.num_ * rhs.num_, lhs.den_ * rhs.den_);
}

template<class T>
inline RationalNum<T>
operator+(RationalNum<T> const& lhs, T const& rhs)
{
  return RationalNum<T>(lhs.num_ + rhs * lhs.den_, lhs.den_);
}

template<class T>
inline RationalNum<T>
operator-(RationalNum<T> const& lhs, T const& rhs)
{
  return RationalNum<T>(lhs.num_ - rhs * lhs.den_, lhs.den_);
}

template<class T>
inline RationalNum<T>
operator/(RationalNum<T> const& lhs, T const& rhs)
{
  return RationalNum<T>(lhs.num_, lhs.den_ * rhs);
}

template<class T>
inline RationalNum<T> operator*(RationalNum<T> const& lhs, T const& rhs)
{
  return RationalNum<T>(lhs.num_ * rhs, lhs.den_);
}

template<class T>
inline RationalNum<T>
operator+(T const& lhs, RationalNum<T> const& rhs)
{
  return rhs + lhs;
}

template<class T>
inline RationalNum<T>
operator-(T const& lhs, RationalNum<T> const& rhs)
{
  return rhs - lhs;
}

template<class T>
inline RationalNum<T>
operator/(T const& lhs, RationalNum<T> const& rhs)
{
  return rhs / lhs;
}

template<class T>
inline RationalNum<T> operator*(T const& lhs, RationalNum<T> const& rhs)
{
  return rhs * lhs;
}

TEMPLATEOVERLOADRELATIONAL(RationalNum<T>, RationalNum<T>, T)
TEMPLATEOVERLOADRELATIONAL(RationalNum<T>, T, T)
TEMPLATEOVERLOADRELATIONAL(T, RationalNum<T>, T)

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
