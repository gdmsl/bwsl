//===-- Accumulator.hpp ----------------------------------------*- C++ -*-===//
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
/// \brief      Definitions for the Accumulator Class
///
//===---------------------------------------------------------------------===//
#ifndef BWSL_ACCUMULATOR_HPP
#define BWSL_ACCUMULATOR_HPP

// boost
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/version.hpp>

// std
#include <exception>
#include <limits>

namespace bwsl {

namespace exception {

class AccumulatorOverflow : public std::exception
{
  const char* what() const throw()
  {
    return "Accumulator has too many measurements";
  }
};
} // namespace bwsl::exception

///
/// Simple Accumulator
///
class Accumulator
{
public:
  /// Default constructor
  Accumulator() = default;

  /// Copy constructor
  Accumulator(const Accumulator& that) = default;

  /// Default destructor
  virtual ~Accumulator() = default;

  /// Copy assignment operator
  Accumulator& operator=(const Accumulator& that) = default;

  /// Add a measurement with some weight
  template<class T, class G>
  void add(T measure, G weight);

  /// Add a measurement with unit weight
  template<class T>
  void add(T measure);

  /// Get the result
  double GetResult() const;

  /// Get the number of measurements
  unsigned long GetCount() const;

  /// Reset the accumulator to the initial state
  void Reset();

protected:
private:
  /// Mean of the measurements
  double measure_{ 0.0 };

  /// Mean of the weights
  double weight_{ 0.0 };

  /// Number of measurements
  unsigned long count_{ 0 };

  // serializaton
  friend class boost::serialization::access;

  /// Serialization method for the class
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & measure_;
    ar & weight_;
    ar & count_;
  }
}; // class Accumulator

template<class T, class G>
inline void
Accumulator::add(T measure, G weight)
{
  double m = static_cast<double>(measure);
  double w = static_cast<double>(weight);
  double mw = m * w;

  double delta = mw - measure_;
  double deltaw = w - weight_;

  // protect against too many measurements
  if (count_ == std::numeric_limits<unsigned long>::max()) {
    throw exception::AccumulatorOverflow();
  }

  count_ += 1ul;
  measure_ += delta / count_;
  weight_ += deltaw / count_;
}

template<class T>
inline void
Accumulator::add(T measure)
{
  add(measure, 1.0);
}

inline double
Accumulator::GetResult() const
{
  return measure_ / weight_;
}

inline unsigned long
Accumulator::GetCount() const
{
  return count_;
}

inline void
Accumulator::Reset()
{
  measure_ = 0.0;
  weight_ = 0.0;
  count_ = 0ul;
}

} // namespace bwsl

#endif // BWSL_ACCUMULATOR_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
