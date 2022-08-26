//===-- NaiveInteger.hpp ---------------------------------------*- C++ -*-===//
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
/// @brief      Definitions for the NaiveInteger Class
///
//===---------------------------------------------------------------------===//
#pragma once

// bwsl
#include <bwsl/accumulators/AccumulatorsExceptions.hpp>

// boost
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>

// std
#include <cmath>
#include <exception>
#include <limits>

namespace bwsl {

///
/// Accumulator using KnuthWelford algorithm
///
class NaiveInteger
{
public:
  /// Default constructor
  NaiveInteger() = default;

  /// Copy constructor
  NaiveInteger(NaiveInteger const& that) = default;

  /// Move constructor
  NaiveInteger(NaiveInteger&& that) = default;

  /// Default destructor
  virtual ~NaiveInteger() = default;

  /// Copy assignment operator
  auto operator=(NaiveInteger const& that) -> NaiveInteger& = default;

  /// Copy assignment operator
  auto operator=(NaiveInteger&& that) -> NaiveInteger& = default;

  /// Add a measurement with unit weight
  auto Add(long) -> void;

  /// Sum of the accumulated values
  [[nodiscard]] auto Sum() const -> long { return sum_; };

  /// Average of the accumulated values
  [[nodiscard]] auto Mean() const -> double
  {
    return static_cast<double>(sum_) / Count();
  };

  /// Variance of the accumulated values
  [[nodiscard]] auto Variance(bool corrected) const -> double;

  /// Scaled variance of the accumulated values
  [[nodiscard]] auto ScaledVariance() const -> double;

  /// Standard deviation of the accumulated values
  [[nodiscard]] auto StandardDeviation(bool corrected) const -> double;

  /// Get the number of measurements
  [[nodiscard]] auto Count() const -> unsigned long { return count_; };

  /// Reset the accumulator to the initial state
  auto Reset() -> void;

protected:
private:
  /// Accumulates the sum of the given values
  long sum_{ 0L };

  /// Accumulates the squares of the given values
  long sum2_{ 0L };

  /// Number of measurements
  unsigned long count_{ 0UL };

  // serializaton
  friend class boost::serialization::access;

  /// Serialization method for the class
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version);
}; // class NaiveInteger

inline auto
NaiveInteger::Add(long x) -> void
{
#ifdef BWSL_ACCUMULATORS_CHECKS
  // protect against too many measurements
  if (count_ == std::numeric_limits<unsigned long>::max()) {
    throw exception::AccumulatorOverflow();
  }
  if (std::numeric_limits<unsigned long>::max() - sum_ <= x) {
    throw exception::AccumulatorOverflow();
  }
  if (std::numeric_limits<unsigned long>::max() < sum2_ <= x * x) {
    throw exception::AccumulatorOverflow();
  }
#endif // BWSL_ACCUMULATORS_CHECKS

  count_ += 1ul;

  sum_ += x;
  sum2_ += x * x;
}

inline auto
NaiveInteger::Variance(bool corrected) const -> double
{
  if (count_ < 2) {
    return std::nan("");
  }

  auto ccount = corrected ? count_ - 1UL : count_;

  return static_cast<double>(sum2_ - sum_ * sum_) / (count_ * ccount);
}

inline auto
NaiveInteger::ScaledVariance() const -> double
{
  if (count_ < 2) {
    return std::nan("");
  }

  return static_cast<double>(sum2_ - sum_ * sum_) / count_;
}

inline auto
NaiveInteger::StandardDeviation(bool corrected) const -> double
{
  return std::sqrt(Variance(corrected));
}

inline auto
NaiveInteger::Reset() -> void
{
  sum_ = 0L;
  sum2_ = 0L;
  count_ = 0UL;
}

template<class Archive>
inline void
NaiveInteger::serialize(Archive& ar, const unsigned int /* version */)
{
  // clang-format off
  ar & sum_;
  ar & sum2_;
  ar & count_;
  // clang-format on
}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
