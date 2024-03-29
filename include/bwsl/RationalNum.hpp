//===-- RationalNum.hpp ----------------------------------------*- C++ -*-===//
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
  auto operator=(const RationalNum&) -> RationalNum& = default;

  /// Move constructor
  RationalNum(RationalNum&&) noexcept = default;

  /// Move assignment operator
  auto operator=(RationalNum&&) noexcept -> RationalNum& = default;

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
  friend auto operator<(RationalNum<G> const& rhs, RationalNum<G> const& lhs)
    -> bool;

  template<class G>
  friend auto operator<(RationalNum<G> const& rhs, G const& lhs) -> bool;

  template<class G>
  friend auto operator<(G const& rhs, RationalNum<G> const& lhs) -> bool;
  /// @}

  /// @name Arithmetic operations
  /// @{
  template<class G>
  friend auto operator+(RationalNum<G> const& lhs, RationalNum<G> const& rhs)
    -> RationalNum<G>;

  template<class G>
  friend auto operator-(RationalNum<G> const& lhs, RationalNum<G> const& rhs)
    -> RationalNum<G>;

  template<class G>
  friend auto operator/(RationalNum<G> const& lhs, RationalNum<G> const& rhs)
    -> RationalNum<G>;

  template<class G>
  friend auto operator*(RationalNum<G> const& lhs, RationalNum<G> const& rhs)
    -> RationalNum<G>;

  template<class G>
  friend auto operator+(RationalNum<G> const& lhs, G const& rhs)
    -> RationalNum<G>;

  template<class G>
  friend auto operator-(RationalNum<G> const& lhs, G const& rhs)
    -> RationalNum<G>;

  template<class G>
  friend auto operator/(RationalNum<G> const& lhs, G const& rhs)
    -> RationalNum<G>;

  template<class G>
  friend auto operator*(RationalNum<G> const& lhs, G const& rhs)
    -> RationalNum<G>;

  template<class G>
  friend auto operator+(G const& lhs, RationalNum<G> const& rhs)
    -> RationalNum<G>;

  template<class G>
  friend auto operator-(G const& lhs, RationalNum<G> const& rhs)
    -> RationalNum<G>;

  template<class G>
  friend auto operator/(G const& lhs, RationalNum<G> const& rhs)
    -> RationalNum<G>;

  template<class G>
  friend auto operator*(G const& lhs, RationalNum<G> const& rhs)
    -> RationalNum<G>;
  /// @}

  /// @name Stream operators
  /// @{
  template<class G>
  friend auto operator<<(std::ostream& out, RationalNum<G> const&)
    -> std::ostream&;

  template<class G>
  friend auto operator>>(std::istream& in, RationalNum<G>&) -> std::istream&;
  /// @}

  /// Check if the number is an integer
  [[nodiscard]] auto IsInt() const -> bool { return num_ % den_ == 0; }

  /// Simplify the stored values
  void Simplify();

  /// Get the numerator
  auto GetNum() const -> T { return num_; };

  /// Get the denominator
  auto GetDen() const -> T { return den_; };

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
inline auto
operator<<(std::ostream& out, RationalNum<T> const& rhs) -> std::ostream&
{
  out << rhs.num_ << "//" << rhs.den_;
  return out;
}

template<class T>
inline auto
operator>>(std::istream& in, RationalNum<T>& rhs) -> std::istream&
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
inline auto
operator<(const RationalNum<G>& lhs, const RationalNum<G>& rhs) -> bool
{
  return lhs.num_ * rhs.den_ < rhs.num_ * lhs.den_;
}

template<class G>
inline auto
operator<(const RationalNum<G>& lhs, const G& rhs) -> bool
{
  return lhs.num_ / lhs.den_ < rhs;
}

template<class G>
inline auto
operator<(const G& lhs, const RationalNum<G>& rhs) -> bool
{
  return lhs < rhs.num_ / rhs.den_;
}

TEMPLATEOVERLOADRELATIONAL(RationalNum<G>, RationalNum<G>, G)
TEMPLATEOVERLOADRELATIONAL(RationalNum<G>, G, G)
TEMPLATEOVERLOADRELATIONAL(G, RationalNum<G>, G)

template<class G>
inline auto
operator+(RationalNum<G> const& lhs, RationalNum<G> const& rhs)
  -> RationalNum<G>
{
  return RationalNum<G>(lhs.num_ * rhs.den_ + rhs.num_ * lhs.den_,
                        lhs.den_ * rhs.den_);
}

template<class G>
inline auto
operator-(RationalNum<G> const& lhs, RationalNum<G> const& rhs)
  -> RationalNum<G>
{
  return RationalNum<G>(lhs.num_ * rhs.den_ - rhs.num_ * lhs.den_,
                        lhs.den_ * rhs.den_);
}

template<class G>
inline auto
operator/(RationalNum<G> const& lhs, RationalNum<G> const& rhs)
  -> RationalNum<G>
{
  return RationalNum<G>(lhs.num_ * rhs.den_, rhs.num_ * lhs.den_);
}

template<class G>
inline auto
operator*(RationalNum<G> const& lhs, RationalNum<G> const& rhs)
  -> RationalNum<G>
{
  return RationalNum<G>(lhs.num_ * rhs.num_, lhs.den_ * rhs.den_);
}

template<class G>
inline auto
operator+(RationalNum<G> const& lhs, G const& rhs) -> RationalNum<G>
{
  return RationalNum<G>(lhs.num_ + rhs * lhs.den_, lhs.den_);
}

template<class G>
inline auto
operator-(RationalNum<G> const& lhs, G const& rhs) -> RationalNum<G>
{
  return RationalNum<G>(lhs.num_ - rhs * lhs.den_, lhs.den_);
}

template<class G>
inline auto
operator/(RationalNum<G> const& lhs, G const& rhs) -> RationalNum<G>
{
  return RationalNum<G>(lhs.num_, lhs.den_ * rhs);
}

template<class G>
inline auto
operator*(RationalNum<G> const& lhs, G const& rhs) -> RationalNum<G>
{
  return RationalNum<G>(lhs.num_ * rhs, lhs.den_);
}

template<class G>
inline auto
operator+(G const& lhs, RationalNum<G> const& rhs) -> RationalNum<G>
{
  return rhs + lhs;
}

template<class G>
inline auto
operator-(G const& lhs, RationalNum<G> const& rhs) -> RationalNum<G>
{
  return RationalNum<G>{ lhs * rhs.den_ - rhs.num_, rhs.den_ };
}

template<class G>
inline auto
operator*(G const& lhs, RationalNum<G> const& rhs) -> RationalNum<G>
{
  return rhs * lhs;
}

template<class G>
inline auto
operator/(G const& lhs, RationalNum<G> const& rhs) -> RationalNum<G>
{
  return RationalNum<G>{ lhs * rhs.den_, rhs.num_ };
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
inline auto
ToString(RationalNum<T> const& rhs) -> std::string
{
  return fmt::format("{}", rhs);
}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
