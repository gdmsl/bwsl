//===-- KnuthWelfordAccumulator.hpp ----------------------------*- C++ -*-===//
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
/// @brief      Definitions for the KnuthWelfordAccumulator Class
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
class KnuthWelfordAccumulator
{
public:
  /// Default constructor
  KnuthWelfordAccumulator() = default;

  /// Copy constructor
  KnuthWelfordAccumulator(KnuthWelfordAccumulator const& that) = default;

  /// Move constructor
  KnuthWelfordAccumulator(KnuthWelfordAccumulator&& that) = default;

  /// Default destructor
  virtual ~KnuthWelfordAccumulator() = default;

  /// Copy assignment operator
  auto operator=(KnuthWelfordAccumulator const& that)
    -> KnuthWelfordAccumulator& = default;

  /// Copy assignment operator
  auto operator=(KnuthWelfordAccumulator&& that)
    -> KnuthWelfordAccumulator& = default;

  /// Add a measurement with unit weight
  auto Add(double m) -> void;

  /// Sum of the accumulated values
  [[nodiscard]] auto Sum() const -> double { return mean_ * Count(); };

  /// Average of the accumulated values
  [[nodiscard]] auto Mean() const -> double { return mean_; };

  /// Average of the accumulated values
  [[nodiscard]] auto Variance(bool corrected) const -> double;

  /// Average of the accumulated values
  [[nodiscard]] auto StandardDeviation(bool corrected) const -> double;

  /// Get the number of measurements
  [[nodiscard]] auto Count() const -> unsigned long { return count_; };

  /// Reset the accumulator to the initial state
  auto Reset() -> void;

protected:
private:
  /// Mean of the measurements
  double mean_{ 0.0 };

  /// Hold informations ofr the variance
  double m2_{ 0.0 };

  /// Number of measurements
  unsigned long count_{ 0UL };

  // serializaton
  friend class boost::serialization::access;

  /// Serialization method for the class
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version);
}; // class KnuthWelfordAccumulator

inline auto
KnuthWelfordAccumulator::Add(double x) -> void
{
#ifdef BWSL_ACCUMULATORS_CHECKS
  // protect against too many measurements
  if (count_ == std::numeric_limits<unsigned long>::max()) {
    throw exception::AccumulatorOverflow();
  }
#endif // BWSL_ACCUMULATORS_CHECKS

  count_ += 1ul;

  double delta = x - mean_;
  mean_ += delta / count_;
  double delta2 = x - mean_;
  m2_ += delta * delta2;
}

inline auto
KnuthWelfordAccumulator::Variance(bool corrected) const -> double
{
  if (count_ < 2) {
    return std::nan("");
  }

  auto ccount = corrected ? count_ - 1UL : count_;

  return m2_ / ccount;
}

inline auto
KnuthWelfordAccumulator::StandardDeviation(bool corrected) const -> double
{
  return std::sqrt(Variance(corrected));
}

inline auto
KnuthWelfordAccumulator::Reset() -> void
{
  mean_ = 0.0;
  count_ = 0UL;
}

template<class Archive>
inline void
KnuthWelfordAccumulator::serialize(Archive& ar,
                                   const unsigned int /* version */)
{
  // clang-format off
  ar & mean_;
  ar & m2_;
  ar & count_;
  // clang-format on
}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
