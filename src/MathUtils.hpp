//===-- MathUtils.hpp ------------------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2016 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// \file
/// \version    0.1
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       May, 2017
/// \brief      Common math functions
///
//===---------------------------------------------------------------------===//
#ifndef BWSL_MATHUTILS_HPP
#define BWSL_MATHUTILS_HPP

// std
#include <algorithm>
#include <cassert>
#include <functional>
#include <numeric>
#include <random>
#include <type_traits>
#include <vector>

namespace bwsl {

///
/// \brief      Type for indices of containers
/// \version    2.0
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       June, 2016
///
/// It is only an alias for std::size_t
///
using SizeType = std::size_t;

///
/// \brief      Type for distances between indices
/// \version    2.0
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       March, 2017
///
/// It is only an alias for long
///
using DiffType = long;

///
/// \brief      Square functions
/// \version    2.0
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       June, 2016
///
template<typename T>
inline T
square(T x)
{
  static_assert(std::is_arithmetic<T>::value, "Wrong type for square function");
  return x * x;
}

///
/// \brief      Cube functions
/// \version    2.0
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       June, 2016
///
template<typename T>
inline T
cube(T x)
{
  static_assert(std::is_arithmetic<T>::value, "Wrong type for cube function");
  return x * x * x;
}

///
/// \brief      Product of all the elements of a container
/// \version    1.0
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       April, 2016
///
template<class Container, class T>
inline T
accumulate_product(const Container& v)
{
  return (T)std::accumulate(v.begin(), v.end(), 1, std::multiplies<T>());
}

///
/// \brief      Square sum of the elements of a container
/// \version    1.0
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       April, 2016
///
template<class Container, class T>
inline T
sum_squared(const Container& v)
{
  T d = 0;
  for (auto x : v) {
    d += square<T>(x);
  }
  return d;
}

///
/// \brief      Do the difference between unsigned types or return abs(a-b)
/// \version    1.0
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       June, 2016
///
template<typename T>
inline T
absdiff(T a, T b)
{
  return (a > b) ? (a - b) : (b - a);
}

///
/// \brief      Do the difference between unsigned types giving signed result
/// \version    1.0
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       June, 2016
///
template<typename T, typename R>
inline R
signeddiff(T a, T b)
{
  static_assert(!std::is_signed<T>::value,
                "Non valid unsigned type for signeddifference");
  static_assert(std::is_signed<R>::value,
                "Non valid unsigned type for signeddifference");
  return (a > b) ? (R)(a - b) : -(R)(b - a);
}

///
/// \brief      Compute the binomial coefficient (n choose k).
/// \details    Uses a multiplicative formula
/// \version    1.0
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       October, 2016
///
template<typename T = long>
inline double
cbinomial(T n, T k)
{
  T res = 1;

  if (k > n) {
    return 0;
  }

  // this is because (n choose k) == (n choose n-k)
  if (k > n - k) {
    k = n - k;
  }

  for (T i = 1; i <= k; ++i) {
    res *= n + 1 - i;
    res /= i;
  }

  return res;
}

///
/// \brief    Choose between the probabilities given
/// \version  1.0
/// \author   Guido Masella (guido.masella@gmail.com)
/// \date     March, 2017
///
template<class T = std::vector<double>, typename G>
inline SizeType
choose_between(const T& probs, G& rng)
{
  auto comul = probs;

  std::partial_sum(probs.begin(), probs.end(), comul.begin());

  std::uniform_real_distribution<typename T::value_type> udist(0.0,
                                                               comul.back());
  auto rnd = udist(rng);

  typename T::iterator choice =
    std::upper_bound(comul.begin(), comul.end(), rnd);

  return static_cast<SizeType>(std::distance(comul.begin(), choice));
}

///
/// \brief    Choose between the probabilities given
/// \version  1.0
/// \author   Guido Masella (guido.masella@gmail.com)
/// \date     March, 2017
///
template<class T = std::vector<double>, typename G>
inline SizeType
choose_between_psums(const T& comul, G& rng)
{
  std::uniform_real_distribution<typename T::value_type> udist(0.0,
                                                               comul.back());
  auto rnd = udist(rng);

  typename T::const_iterator choice =
    std::upper_bound(comul.cbegin(), comul.cend(), rnd);

  return static_cast<SizeType>(std::distance(comul.cbegin(), choice));
}


///
/// \brief    Choose with given probability
/// \version  1.0
/// \author   Guido Masella (guido.masella@gmail.com)
/// \date     March, 2017
///
template<typename T = double, typename G>
inline bool
choose_with_probability(T prob, G& rng)
{
  auto udist = std::uniform_real_distribution<T>{ 0.0, 1.0 };
  return static_cast<bool>(udist(rng) < prob);
}

} // namespace bwsl

#endif // MATHUTILS_HPP

// vim: set ts=2 sts=2 et sw=2 tw=80: //
