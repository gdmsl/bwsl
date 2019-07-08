//===-- LogSpace.hpp -------------------------------------------*- C++ -*-===//
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
/// @brief      Definitions of the LogSpace class
///
//===---------------------------------------------------------------------===//
#pragma once

//bwsl
#include <bwsl/MathUtils.hpp>

// std
#include <vector>
#include <algorithm>
#include <cassert>
#include <cmath>

namespace bwsl {

///
/// Generate logarithmically spaced numbers.
///
template <typename T>
class LogSpace
{
public:
  /// Constructor
  LogSpace(long start, long stop, unsigned long steps, double base);

  /// Construct using exact times
  LogSpace(T first, T last, unsigned long steps);

  /// Function call operator for generate usage
  T operator()();

  /// Deference operator overloading
  T operator*() { return current_value_; };

  /// Infix increment
  LogSpace& operator++();

  /// Postfix increment
  LogSpace operator++(int);

  /// Returns a LogSpace at the beginning of the interval
  LogSpace begin();

  /// Returns a LogSpace at the end of the interval
  LogSpace end();

  /// Return a vector filled with values
  std::vector<T> Collect(unsigned long n);

protected:
private:
  /// The step to take at each iteration
  T base_;

  /// The current value
  T current_value_;

  /// First value
  T first_;

  /// Last value
  T last_;

  /// Number of steps to take
  unsigned long nsteps_;

  /// The actual step
  unsigned long step_;

  /// Compare two LogSpaces and check for equality
  friend bool operator==(const LogSpace<T>& a, const LogSpace<T>& b)
  {
    return static_cast<bool>(a.step_ == b.step_);
  };

  /// Compare two LogSpaces and check for inequality
  friend bool operator!=(const LogSpace<T>& a, const LogSpace<T>& b)
  {
    return !(a == b);
  }
}; // class LogSpace

template <typename T>
LogSpace<T>::LogSpace(long start, long stop, unsigned long steps, double base)
  : nsteps_(steps)
  , step_(0)
{
  assert(start < stop);
  first_ = pow(base, start);
  last_ = pow(base, stop);
  base_ = pow(M_E, (log(last_) - log(first_)) / (static_cast<double>(steps)));
  current_value_ = pow(base, start);
}

template <typename T>
LogSpace<T>::LogSpace(T first, T last, unsigned long steps)
  : first_(first)
  , last_(last)
  , nsteps_(steps)
  , step_(0)
{
  assert(first < last);
  base_ = pow(M_E, (log(last_) - log(first_)) / (static_cast<double>(steps)));
  current_value_ = first;
}

template <typename T>
T
LogSpace<T>::operator()()
{
  T retval = current_value_;
  operator++();
  return retval;
}

template <typename T>
LogSpace<T>& LogSpace<T>::operator++()
{
  current_value_ *= base_;
  ++step_;
  return *this;
}

template <typename T>
LogSpace<T> LogSpace<T>::operator++(int)
{
  LogSpace<T> tmp(*this);
  operator++();
  return tmp;
}

template <typename T>
LogSpace<T>
LogSpace<T>::begin()
{
  LogSpace<T> tmp(*this);
  tmp.current_value_ = first_;
  tmp.step_ = 0;
  return tmp;
}

template <typename T>
LogSpace<T>
LogSpace<T>::end()
{
  LogSpace<T> tmp(*this);
  tmp.current_value_ = last_;
  tmp.step_ = nsteps_;
  return tmp;
}

template <typename T>
std::vector<T>
LogSpace<T>::Collect(unsigned long n)
{
  std::vector<T> retval;
  retval.reserve(n);
  generate_n(std::back_inserter(retval), n, *this);
  return retval;
}

} // namespace bwsl

// vim: set ts=2 sts=2 et sw=2 tw=80: //
