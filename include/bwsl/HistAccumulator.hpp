//===-- HistAccumulator.hpp ------------------------------------*- C++ -*-===//
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
/// @brief      Definitions for the HistAccumulator Class
///
//===---------------------------------------------------------------------===//
#pragma once

// bwsl
#include <bwsl/Accumulators.hpp>

// boost
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>

// std
#include <vector>

namespace bwsl {

///
/// Accumulate histogram statistic
///
class HistAccumulator
{
public:
  /// Default constructor
  HistAccumulator() = default;

  /// Construct an histogram
  HistAccumulator(size_t nbins);

  /// Copy constructor
  HistAccumulator(HistAccumulator const& that) = default;

  /// Move constructor
  HistAccumulator(HistAccumulator&& that) = default;

  /// Default destructor
  virtual ~HistAccumulator() = default;

  /// Copy assignment operator
  auto operator=(HistAccumulator const& that) -> HistAccumulator& = default;

  /// Move assignment operator
  auto operator=(HistAccumulator&& that) -> HistAccumulator& = default;

  /// Resize the histogram
  void Resize(size_t nbins);

  /// Reset the histogram
  void Reset();

  /// Add a measurement
  template<class T>
  auto Add(size_t idx, T val) -> void;

  /// Add a unitary measurement
  auto Add(size_t idx) -> void { Add(idx, 1.0); };

  /// Add a measurement and increase the number of bins
  template<class T>
  auto ForceAdd(size_t idx, T val) -> void;

  /// Add an unitary measurement and increase the number of bins
  auto ForceAdd(size_t idx) -> void { ForceAdd(idx, 1.0); };

  /// Get a single component result
  [[nodiscard]] auto GetResult(size_t idx) const -> double;

  /// Get the result of all the components
  [[nodiscard]] auto GetResults() const -> std::vector<double>;

  /// Get the results of all the components but divide by the one
  /// given.
  [[nodiscard]] auto GetResults(size_t idx) const -> std::vector<double>;

  /// Get the number of measurements in total
  [[nodiscard]] auto GetCount() const -> size_t { return count_; };

  /// Get the count of a single observable
  [[nodiscard]] auto GetCount(size_t idx) const -> size_t { return acc_[idx].Count(); }

  /// Get the number of bins
  [[nodiscard]] auto GetNbins() const -> size_t { return nbins_; };

protected:
private:
  /// Number of bins
  size_t nbins_{ 0UL };

  /// Accumulators
  std::vector<bwsl::accumulators::NeumaierAccumulator> acc_{};

  /// Number of measurements
  unsigned long count_{ 0UL };

  // serializaton
  friend class boost::serialization::access;

  /// Serialization method for the class
  template<class Archive>
  void serialize(Archive& ar,  unsigned int version);
}; // class HistAccumulator

inline HistAccumulator::HistAccumulator(size_t nbins)
  : nbins_(nbins)
  , acc_(nbins)
{}

inline auto
HistAccumulator::Reset() -> void
{
  for (auto& a : acc_) {
    a.Reset();
  }
  count_ = 0UL;
}

template<class T>
inline auto
HistAccumulator::Add(size_t idx, T val) -> void
{
  acc_[idx].Add(val);
  count_ += 1UL;
}

template<class T>
inline auto
HistAccumulator::ForceAdd(size_t idx, T val) -> void
{
  while (idx >= nbins_) {
    acc_.emplace_back();
    nbins_++;
    assert(nbins_ == acc_.size());
  }
  acc_[idx].Add(val);
  count_ += 1UL;
}

inline void
HistAccumulator::Resize(size_t nbins)
{
  nbins_ = nbins;
  acc_.resize(nbins);
}

inline auto
HistAccumulator::GetResult(size_t idx) const -> double
{
  auto const& c = acc_[idx];

  if (c.Count() == 0UL) {
    return 0.0;
  }

  return c.Sum() / static_cast<double>(count_);
}

inline auto
HistAccumulator::GetResults() const -> std::vector<double>
{
  auto r = std::vector<double>(nbins_, 0.0);

  for (auto i = 0UL; i < nbins_; i++) {
    r[i] = GetResult(i);
  }

  return r;
}

inline auto
HistAccumulator::GetResults(size_t idx) const -> std::vector<double>
{
  auto r = GetResults();
  for (auto& v : r) {
    v /= r[idx];
  }
  return r;
}

template<class Archive>
inline auto
HistAccumulator::serialize(Archive& ar, const unsigned int /* version */) -> void
{
  ar& nbins_;
  ar& acc_;
  ar& count_;
}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
