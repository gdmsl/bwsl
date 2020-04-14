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

// bwsl
#include <bwsl/Exceptions.hpp>
#include <bwsl/MathUtils.hpp>
#include <bwsl/RelationalUtils.hpp>

// fmt
#include <fmt/format.h>

// std
#include <iostream>
#include <sstream>

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

  /// Explicit conversion to double
  explicit operator double() { return static_cast<double>(num_) / den_; };

  /// Explicit conversion to integer
  explicit operator T() { return num_ / den_; };

  /// @name Relational Operators
  /// @{
  template<class G>
  friend bool operator<(RationalNum<G> const& rhs, RationalNum<G> const& lhs);

  template<class G>
  friend bool operator<(RationalNum<G> const& rhs, G const& lhs);

  template<class G>
  friend bool operator<(G const& rhs, RationalNum<G> const& lhs);
  /// @}

  /// @name Arithmetic operations
  /// @{
  template<class G>
  friend RationalNum<G> operator+(RationalNum<G> const& lhs,
                                  RationalNum<G> const& rhs);

  template<class G>
  friend RationalNum<G> operator-(RationalNum<G> const& lhs,
                                  RationalNum<G> const& rhs);

  template<class G>
  friend RationalNum<G> operator/(RationalNum<G> const& lhs,
                                  RationalNum<G> const& rhs);

  template<class G>
  friend RationalNum<G> operator*(RationalNum<G> const& lhs,
                                  RationalNum<G> const& rhs);

  template<class G>
  friend RationalNum<G> operator+(RationalNum<G> const& lhs, G const& rhs);

  template<class G>
  friend RationalNum<G> operator-(RationalNum<G> const& lhs, G const& rhs);

  template<class G>
  friend RationalNum<G> operator/(RationalNum<G> const& lhs, G const& rhs);

  template<class G>
  friend RationalNum<G> operator*(RationalNum<G> const& lhs, G const& rhs);

  template<class G>
  friend RationalNum<G> operator+(G const& lhs, RationalNum<G> const& rhs);

  template<class G>
  friend RationalNum<G> operator-(G const& lhs, RationalNum<G> const& rhs);

  template<class G>
  friend RationalNum<G> operator/(G const& lhs, RationalNum<G> const& rhs);

  template<class G>
  friend RationalNum<G> operator*(G const& lhs, RationalNum<G> const& rhs);
  /// @}

  /// @name Stream operators
  /// @{
  template<class G>
  friend std::ostream& operator<<(std::ostream& out, RationalNum<G> const&);

  template<class G>
  friend std::istream& operator>>(std::istream& in, RationalNum<G>&);
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
  for (auto i = 0UL; i < 2UL; i++) {
    in >> sep;
    if (sep != '/') {
      in.setstate(std::ios_base::failbit);
      return in;
    }
  }
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
  auto g = get_gcd(num_, den_);
  num_ = num_ / g;
  den_ = den_ / g;
}

template<class G>
inline bool
operator<(const RationalNum<G>& lhs, const RationalNum<G>& rhs)
{
  return lhs.num_ * rhs.den_ < rhs.num_ * lhs.den_;
}

template<class G>
inline bool
operator<(const RationalNum<G>& lhs, const G& rhs)
{
  return lhs.num_ / lhs.den_ < rhs;
}

template<class G>
inline bool
operator<(const G& lhs, const RationalNum<G>& rhs)
{
  return lhs < rhs.num_ / rhs.den_;
}

TEMPLATEOVERLOADRELATIONAL(RationalNum<G>, RationalNum<G>, G)
TEMPLATEOVERLOADRELATIONAL(RationalNum<G>, G, G)
TEMPLATEOVERLOADRELATIONAL(G, RationalNum<G>, G)

template<class G>
inline RationalNum<G>
operator+(RationalNum<G> const& lhs, RationalNum<G> const& rhs)
{
  return RationalNum<G>(lhs.num_ * rhs.den_ + rhs.num_ * lhs.den_,
                        lhs.den_ * rhs.den_);
}

template<class G>
inline RationalNum<G>
operator-(RationalNum<G> const& lhs, RationalNum<G> const& rhs)
{
  return RationalNum<G>(lhs.num_ * rhs.den_ - rhs.num_ * lhs.den_,
                        lhs.den_ * rhs.den_);
}

template<class G>
inline RationalNum<G>
operator/(RationalNum<G> const& lhs, RationalNum<G> const& rhs)
{
  return RationalNum<G>(lhs.num_ * rhs.den_, rhs.num_ * lhs.den_);
}

template<class G>
inline RationalNum<G>
operator*(RationalNum<G> const& lhs, RationalNum<G> const& rhs)
{
  return RationalNum<G>(lhs.num_ * rhs.num_, lhs.den_ * rhs.den_);
}

template<class G>
inline RationalNum<G>
operator+(RationalNum<G> const& lhs, G const& rhs)
{
  return RationalNum<G>(lhs.num_ + rhs * lhs.den_, lhs.den_);
}

template<class G>
inline RationalNum<G>
operator-(RationalNum<G> const& lhs, G const& rhs)
{
  return RationalNum<G>(lhs.num_ - rhs * lhs.den_, lhs.den_);
}

template<class G>
inline RationalNum<G>
operator/(RationalNum<G> const& lhs, G const& rhs)
{
  return RationalNum<G>(lhs.num_, lhs.den_ * rhs);
}

template<class G>
inline RationalNum<G>
operator*(RationalNum<G> const& lhs, G const& rhs)
{
  return RationalNum<G>(lhs.num_ * rhs, lhs.den_);
}

template<class G>
inline RationalNum<G>
operator+(G const& lhs, RationalNum<G> const& rhs)
{
  return rhs + lhs;
}

template<class G>
inline RationalNum<G>
operator-(G const& lhs, RationalNum<G> const& rhs)
{
  return RationalNum<G>{lhs * rhs.den_ - rhs.num_, rhs.den_};
}

template<class G>
inline RationalNum<G>
operator*(G const& lhs, RationalNum<G> const& rhs)
{
  return rhs * lhs;
}

template<class G>
inline RationalNum<G>
operator/(G const& lhs, RationalNum<G> const& rhs)
{
  return RationalNum<G>{lhs * rhs.den_, rhs.num_};
}

template<class T>
inline void
FromString(std::string const& str, RationalNum<T>& rhs)
{
  auto iss = std::istringstream{ str };
  iss >> rhs;

  if (iss.fail()) {
    throw bwsl::exception::BadParsing{};
  }
}

template<class T>
inline std::string
ToString(RationalNum<T> const& rhs)
{
  return fmt::format("{}", rhs);
}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
