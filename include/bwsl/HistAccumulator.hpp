//===-- HistAccumulator.hpp ------------------------------------*- C++ -*-===//
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
/// @brief      Definitions for the HistAccumulator Class
///
//===---------------------------------------------------------------------===//
#pragma once

// bwsl
#include <bwsl/Accumulator.hpp>

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
  HistAccumulator(const HistAccumulator& that) = default;

  /// Default destructor
  virtual ~HistAccumulator() = default;

  /// Copy assignment operator
  HistAccumulator& operator=(const HistAccumulator& that) = default;

  /// Resize the histogram
  void Resize(size_t nbins);

  /// Reset the histogram
  void Reset();

  /// Add a measurement
  template<class T>
  void add(size_t idx, T val);

  /// Add a unitary measurement
  void add(size_t idx) { add(idx, 1.0); };

  /// Add a measurement and increase the number of bins
  template<class T>
  void force_add(size_t idx, T val);

  /// Add an unitary measurement and increase the number of bins
  void force_add(size_t idx) { force_add(idx, 1.0); };

  /// Get a single component result
  double GetResult(size_t idx) const;

  /// Get the result of all the components
  std::vector<double> GetResults() const;

  /// Get the results of all the components but divide by the one
  /// given.
  std::vector<double> GetResults(size_t idx) const;

  /// Get the number of measurements in total
  size_t GetCount() const { return count_; };

  /// Get the count of a single observable
  size_t GetCount(size_t idx) const { return acc_[idx].GetCount(); }

  /// Get the number of bins
  size_t GetNbins() const { return nbins_; };

protected:
  /// Number of bins
  size_t nbins_{ 0ul };

  /// Accumulators
  std::vector<Accumulator> acc_{};

  /// Number of measurements
  unsigned long count_{ 0ul };

  // serializaton
  friend class boost::serialization::access;

  /// Serialization method for the class
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version);

private:
}; // class HistAccumulator

inline HistAccumulator::HistAccumulator(size_t nbins)
  : nbins_(nbins)
  , acc_(nbins)
  , count_(0ul)
{}

inline void
HistAccumulator::Reset()
{
  for (auto& a : acc_) {
    a.Reset();
  }
  count_ = 0ul;
}

template<class T>
inline void
HistAccumulator::add(size_t idx, T val)
{
  acc_[idx].add(val);
  count_ += 1ul;
}

template<class T>
inline void
HistAccumulator::force_add(size_t idx, T val)
{
  while (idx >= nbins_) {
    acc_.emplace_back();
    nbins_++;
    assert(nbins_ = acc_.size());
  }
  acc_[idx].add(val);
  count_ += 1ul;
}

inline void
HistAccumulator::Resize(size_t nbins)
{
  nbins_ = nbins;
  acc_.resize(nbins);
}

inline double
HistAccumulator::GetResult(size_t idx) const
{
  auto& c = acc_[idx];

  if (c.GetCount() == 0ul) {
    return 0.0;
  }

  return c.GetResult() * (c.GetCount() / static_cast<double>(count_));
}

inline std::vector<double>
HistAccumulator::GetResults() const
{
  auto r = std::vector<double>(nbins_, 0.0);

  for (auto i = 0ul; i < nbins_; i++) {
    r[i] = GetResult(i);
  }

  return r;
}

inline std::vector<double>
HistAccumulator::GetResults(size_t idx) const
{
  auto r = GetResults();
  for (auto& v : r) {
    v /= r[idx];
  }
  return r;
}

template<class Archive>
inline void
HistAccumulator::serialize(Archive& ar, const unsigned int version)
{
  ar& nbins_;
  ar& acc_;
  ar& count_;
}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
