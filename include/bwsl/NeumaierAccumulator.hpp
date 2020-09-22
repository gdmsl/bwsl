//===-- NeumaierAccumulator.hpp --------------------------------*- C++ -*-===//
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
/// @brief      Definitions for the NeumaierAccumulator Class
///
//===---------------------------------------------------------------------===//
#pragma once

// boost
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>

namespace bwsl {

///
/// Accumulator following the Neumaier summation algorithm
///
class NeumaierAccumulator {
 public:
   /// Default constructor
   NeumaierAccumulator() = default;

   /// Copy constructor
   NeumaierAccumulator(NeumaierAccumulator const& that) = default;

   /// Copy constructor
   NeumaierAccumulator(NeumaierAccumulator&& that) = default;

   /// Default destructor
   virtual ~NeumaierAccumulator() = default;

   /// Copy assignment operator
   auto operator=(NeumaierAccumulator const& that) -> NeumaierAccumulator& = default;

   /// Copy assignment operator
   auto operator=(NeumaierAccumulator&& that) -> NeumaierAccumulator& = default;

   /// Add a number to the sum
   template<typename T>
   auto Add(T x) -> void;

   /// Return the final result
   [[nodiscard]] auto GetResult() const -> double { return sum_ + c_; };

   /// Get the number of values added
   [[nodiscard]] auto GetCount() const -> unsigned long { return count_; };

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
}; // class NeumaierAccumulator

template<typename T>
inline auto
NeumaierAccumulator::Add(T x) -> void
{
  auto t = sum_ + x;
  if (std::abs(sum_) >= std::abs(x)) {
    c_ += (sum_ - t) + x;
  } else {
    c_ += (x - t) + sum_;
  }
  sum_ = t;
  count_++;
}

inline auto
NeumaierAccumulator::Reset() -> void
{
  sum_ = 0.0;
  c_ = 0.0;
    count_ = 0.0;
}

template<class Archive>
void
NeumaierAccumulator::serialize(Archive& ar, const unsigned int /* version */)
{
  // clang-format off
  ar & sum_;
  ar & c_;
  ar & count_;
  // clang-format on
}

} // namespace swmc

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
