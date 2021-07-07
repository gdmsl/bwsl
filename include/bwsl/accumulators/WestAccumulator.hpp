//===-- WestAccumulator.hpp ------------------------------------*- C++ -*-===//
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
/// @brief      Definitions for the WestAccumulator Class
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

namespace bwsl::accumulators {

///
/// Accumulator using West algorithm.
/// It computed weighted mean and variance.
///
class WestAccumulator
{
public:
  /// Default constructor
  WestAccumulator() = default;

  /// Copy constructor
  WestAccumulator(WestAccumulator const& that) = default;

  /// Move constructor
  WestAccumulator(WestAccumulator&& that) = default;

  /// Default destructor
  virtual ~WestAccumulator() = default;

  /// Copy assignment operator
  auto operator=(WestAccumulator const& that) -> WestAccumulator& = default;

  /// Copy assignment operator
  auto operator=(WestAccumulator&& that) -> WestAccumulator& = default;

  /// Add a measurement with unit weight
  auto Add(double m, double w) -> void;

  /// Sum of the accumulated values
  [[nodiscard]] auto Sum() const -> double { return mean_ * sum_weights_; };

  /// Average of the accumulated values
  [[nodiscard]] auto Mean() const -> double { return mean_; };

  /// Biased estimation of the variance
  [[nodiscard]] auto PopulationVariance() const -> double;

  /// Unbiased estimation of the variance
  /// To be used in the case the weights are frequencies weights.
  [[nodiscard]] auto SampleFrequencyVariance() const -> double;

  /// Unbiased estimation of the variance
  /// To be used in the case the weights are reliability weights.
  [[nodiscard]] auto SampleReliabilityVariance() const -> double;

  /// Get the number of measurements
  [[nodiscard]] auto Count() const -> unsigned long { return count_; };

  /// Reset the accumulator to the initial state
  auto Reset() -> void;

protected:
private:
  /// Sum of the weights
  double sum_weights_{ 0.0 };

  /// Sum of the squared weights
  double sum_weights2_{ 0.0 };

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
  void serialize(Archive& ar,  unsigned int version);
}; // class WestAccumulator

inline auto
WestAccumulator::Add(double m, double w) -> void
{
#ifdef BWSL_ACCUMULATORS_CHECKS
  // protect against too many measurements
  if (count_ == std::numeric_limits<unsigned long>::max()) {
    throw exception::AccumulatorOverflow();
  }
#endif // BWSL_ACCUMULATORS_CHECKS

  count_ += 1UL;

  sum_weights_ += w;
  sum_weights2_ += w * w;
  double oldmean = mean_;
  mean_ += (w / sum_weights_) * (m - oldmean);
  m2_ += w * (m - oldmean) * (m - mean_);
}

inline auto
WestAccumulator::PopulationVariance() const -> double
{
  if (count_ < 2) {
    return std::nan("");
  }

  return m2_ / sum_weights_;
}

inline auto
WestAccumulator::SampleFrequencyVariance() const -> double
{
  if (count_ < 2) {
    return std::nan("");
  }

  return m2_ / (sum_weights_ - 1);
}

inline auto
WestAccumulator::SampleReliabilityVariance() const -> double
{
  if (count_ < 2) {
    return std::nan("");
  }

  return m2_ / (sum_weights_ - sum_weights2_ / sum_weights_);
}

inline auto
WestAccumulator::Reset() -> void
{
  sum_weights_ = 0.0;
  sum_weights2_ = 0.0;
  mean_ = 0.0;
  m2_ = 0.0;
  count_ = 0UL;
}

template<class Archive>
inline void
WestAccumulator::serialize(Archive& ar, const unsigned int /* version */)
{
  // clang-format off
  ar & sum_weights_;
  ar & sum_weights2_;
  ar & mean_;
  ar & m2_;
  ar & count_;
  // clang-format on
}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
