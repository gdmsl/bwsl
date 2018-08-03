//===-- HistAccumulator.hpp ------------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2016-2018 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// \file
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       July, 2018
/// \brief      Definitions for the HistAccumulator Class
///
//===---------------------------------------------------------------------===//
#ifndef BWSL_HISTACCUMULATOR_HPP
#define BWSL_HISTACCUMULATOR_HPP

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
  HistAccumulator() = delete;

  /// Construct an histogram
  HistAccumulator(size_t nbins);

  /// Copy constructor
  HistAccumulator(const HistAccumulator& that) = default;

  /// Default destructor
  virtual ~HistAccumulator() = default;

  /// Copy assignment operator
  HistAccumulator& operator=(const HistAccumulator& that) = default;

  /// Reset the histogram
  void Reset();

  /// Add a measurement
  template<class T>
  void add(size_t idx, T val);

  /// Add a unitary measurement
  void add(size_t idx) { add(idx, 1.0); };

  /// Get a single component result
  double GetResult(size_t idx) const;

  /// Get the result of all the components
  std::vector<double> GetResult() const;

  /// Get the number of measurements in total
  size_t GetCount() const { return count_; };

  /// Get the count of a single observable
  size_t GetCount(size_t idx) const { return acc_[idx].GetCount(); }

  /// Get the number of bins
  size_t GetNbins() const { return nbins_; };

protected:
private:
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
}; // class HistAccumulator

HistAccumulator::HistAccumulator(size_t nbins)
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

double
HistAccumulator::GetResult(size_t idx) const
{
  auto& c = acc_[idx];

  if (c.GetCount() == 0ul) {
    return 0.0;
  }

  return c.GetResult() * (c.GetCount() / static_cast<double>(count_));
}

std::vector<double>
HistAccumulator::GetResult() const
{
  auto r = std::vector<double>(nbins_, 0.0);

  for (auto i = 0ul; i < nbins_; i++) {
    r[i] = GetResult(i);
  }

  return std::move(r);
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

#endif // BWSL_HISTACCUMULATOR_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
