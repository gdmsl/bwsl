//===-- KahanAccumulator.hpp -----------------------------------*- C++ -*-===//
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
/// @brief      Definitions for the KahanAccumulator Class
///
//===---------------------------------------------------------------------===//
#pragma once

// bwsl
#include <bwsl/accumulators/AccumulatorsExceptions.hpp>

// boost
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>

// std
#include <limits>

namespace bwsl::accumulators {

///
/// Accumulator following the Kahan summation algorithm
///
class KahanAccumulator
{
public:
  /// Default constructor
  KahanAccumulator() = default;

  /// Copy constructor
  KahanAccumulator(KahanAccumulator const& that) = default;

  /// Copy constructor
  KahanAccumulator(KahanAccumulator&& that) = default;

  /// Default destructor
  virtual ~KahanAccumulator() = default;

  /// Copy assignment operator
  auto operator=(KahanAccumulator const& that) -> KahanAccumulator& = default;

  /// Copy assignment operator
  auto operator=(KahanAccumulator&& that) -> KahanAccumulator& = default;

  /// Add a number to the sum
  auto Add(double x) -> void;

  /// Return the final result
  [[nodiscard]] auto Sum() const -> double { return sum_; };

  /// Return the final result
  [[nodiscard]] auto Mean() const -> double { return sum_ / count_; };

  /// Get the number of values added
  [[nodiscard]] auto Count() const -> unsigned long { return count_; };

  /// Reset the accumulator
  auto Reset() -> void;

protected:
private:
  /// Accumulator for the sum
  double sum_{ 0.0 };

  /// Keep track of correction
  double c_{ 0.0 };

  /// Number of values added
  unsigned long count_{ 0UL };

  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive& ar, unsigned int version);
}; // class KahanAccumulator

inline auto
KahanAccumulator::Add(double x) -> void
{
#ifdef BWSL_ACCUMULATORS_CHECKS
  if (count_ == std::numeric_limits<unsigned long>::max()) {
    throw exception::AccumulatorOverflow();
  }
#endif // BWSL_ACCUMULATORS_CHECKS

  auto y = x - c_;
  auto t = sum_ + y;
  c_ = (t - sum_) - y;
  sum_ = t;
  count_++;
}

inline auto
KahanAccumulator::Reset() -> void
{
  sum_ = 0.0;
  c_ = 0.0;
  count_ = 0.0;
}

template<class Archive>
void
KahanAccumulator::serialize(Archive& ar, const unsigned int /* version */)
{
  // clang-format off
  ar & sum_;
  ar & c_;
  ar & count_;
  // clang-format on
}

} // namespace bwsl::accumulators

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
