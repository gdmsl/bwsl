//===-- LinSpace.hpp -------------------------------------------*- C++ -*-===//
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
/// @brief      Definitions of the LinSpace class
///
//===---------------------------------------------------------------------===//
#pragma once

// std
#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>

namespace bwsl {

///
/// Generate sequence of equally spaced numbers
///
template<typename T>
class LinSpace
{
public:
  /// Construct using exact times
  LinSpace(T first, T last, unsigned long steps);

  /// Function call operator for generate usage
  T operator()();

  /// Deference operator overloading
  T operator*() { return current_value_; };

  /// Infix increment
  LinSpace& operator++();

  /// Postfix increment
  LinSpace operator++(int);

  /// Returns a LinSpace at the beginning of the interval
  LinSpace begin();

  /// Returns a LinSpace at the end of the interval
  LinSpace end();

  /// Return a vector filled with values
  std::vector<T> Collect(unsigned long n);

protected:
private:
  /// The current value
  T current_value_;

  /// First value
  T first_;

  /// Last value
  T last_;

  // stepsize
  T stepsize_;

  /// Number of steps to take
  unsigned long nsteps_;

  /// The actual step
  unsigned long step_;

  /// Compare two LinSpaces and check for equality
  friend bool operator==(const LinSpace<T>& a, const LinSpace<T>& b);

  /// Compare two LinSpaces and check for inequality
  friend bool operator!=(const LinSpace<T>& a, const LinSpace<T>& b);
}; // class LinSpace

template<typename T>
LinSpace<T>::LinSpace(T first, T last, unsigned long steps)
  : current_value_(first)
  , first_(first)
  , last_(last)
  , nsteps_(steps)
  , step_(0)
{
  assert(first < last);
  stepsize_ = (last - first) / static_cast<T>(nsteps_);
}

template<typename T>
T
LinSpace<T>::operator()()
{
  T retval = current_value_;
  operator++();
  return retval;
}

template<typename T>
LinSpace<T>&
LinSpace<T>::operator++()
{
  ++step_;
  current_value_ = first_ + stepsize_ * static_cast<T>(step_);
  return *this;
}

template<typename T>
LinSpace<T>
LinSpace<T>::operator++(int)
{
  LinSpace<T> tmp(*this);
  operator++();
  return tmp;
}

template<typename T>
LinSpace<T>
LinSpace<T>::begin()
{
  LinSpace<T> tmp(*this);
  tmp.current_value_ = first_;
  tmp.step_ = 0;
  return tmp;
}

template<typename T>
LinSpace<T>
LinSpace<T>::end()
{
  LinSpace<T> tmp(*this);
  tmp.current_value_ = last_;
  tmp.step_ = nsteps_;
  return tmp;
}

template<typename T>
std::vector<T>
LinSpace<T>::Collect(unsigned long n)
{
  std::vector<T> retval;
  retval.reserve(n);
  generate_n(std::back_inserter(retval), n, *this);
  return retval;
}

template<typename T>
bool
operator==(const LinSpace<T>& a, const LinSpace<T>& b)
{
  return a.step_ == b.step_;
};

template<typename T>
bool
operator!=(const LinSpace<T>& a, const LinSpace<T>& b)
{
  return !(a == b);
}

} // namespace bwsl

// vim: set ts=2 sts=2 et sw=2 tw=80: //
