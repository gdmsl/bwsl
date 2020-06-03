//===-- MathUtils.hpp ------------------------------------------*- C++ -*-===//
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
/// @brief      Common math functions
///
//===---------------------------------------------------------------------===//
#pragma once

// std
#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <numeric>
#include <random>
#include <type_traits>
#include <vector>

namespace bwsl {

///
/// Type for indices of containers.
/// It is only an alias for std::size_t
///
using SizeType = std::size_t;

///
/// Type for distances between indices
/// It is only an alias for long
///
using DiffType = long;

///
/// Square functions
///
template<typename T>
inline T
square(T x)
{
  static_assert(std::is_arithmetic<T>::value, "Wrong type for square function");
  return x * x;
}

///
/// Cube functions
///
template<typename T>
inline T
cube(T x)
{
  static_assert(std::is_arithmetic<T>::value, "Wrong type for cube function");
  return x * x * x;
}

///
/// Product of all the elements of a container
///
template<class Container, class T = typename Container::value_type>
inline T
accumulate_product(const Container& v)
{
  return std::accumulate(
    v.begin(), v.end(), static_cast<T>(1), std::multiplies<T>());
}

///
/// Sum of all the elements of a container
///
template<class Container, class T = typename Container::value_type>
inline T
accumulate_sum(const Container& v)
{
  return std::accumulate(v.begin(), v.end(), static_cast<T>(0));
}

///
/// Return the maximum value in a container
///
template<class Container>
inline typename Container::value_type
max(const Container& v)
{
  return *std::max_element(v.begin(), v.end());
}

///
/// Return the minimum value in a container
///
template<class Container>
inline typename Container::value_type
min(const Container& v)
{
  return *std::min_element(v.begin(), v.end());
}

///
/// Square sum of the elements of a container
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
/// L2 norm
///
template<class Container>
inline double
l2norm(const Container& v)
{
  auto d = typename Container::value_type{ 0 };
  for (auto x : v) {
    d += square(x);
  }
  return sqrt(d);
}

///
/// Sign function
///
template<typename T>
inline int
sgn(T val)
{
  return (T(0) < val) - (val < T(0));
}

///
/// Do the difference between unsigned types or return `abs(a-b)`
///
template<typename T>
inline T
absdiff(T a, T b)
{
  return (a > b) ? (a - b) : (b - a);
}

///
/// Do the difference between unsigned types giving signed result
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
/// Compute the binomial coefficient `(n choose k)`.
/// Uses a multiplicative formula
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
/// Choose between the probabilities given
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
/// Choose between the comulative probabilities given
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
/// Choose with given probability
///
template<typename T = double, typename G>
inline bool
choose_with_probability(T prob, G& rng)
{
  auto udist = std::uniform_real_distribution<T>{ 0.0, 1.0 };
  return static_cast<bool>(udist(rng) < prob);
}

///
/// Add the second array to the first one
///
template<class Container>
inline Container&
sum_into(Container& left, const Container& right)
{
  std::transform(left.begin(),
                 left.end(),
                 right.begin(),
                 left.begin(),
                 std::plus<typename Container::value_type>());
  return left;
}

///
/// Subtract the second array to the first one
///
template<class Container>
inline Container&
subtract_into(Container& left, const Container& right)
{
  std::transform(left.begin(),
                 left.end(),
                 right.begin(),
                 left.begin(),
                 std::minus<typename Container::value_type>());
  return left;
}

///
/// Invert each component of a vector
///
template<class Container>
inline Container&
invert(Container& c)
{
  for (auto& x : c) {
    x = -x;
  }
  return c;
}

///
/// Invert each component of a vector
///
template<class Container>
inline Container
get_inverted(Container c)
{
  for (auto& x : c) {
    x = -x;
  }
  return c;
}

///
/// Transform coordinates to index
///
template<class C, class D>
inline size_t
array_to_index(C const& a, D const& size)
{
  static_assert(std::is_same<typename D::value_type, size_t>::value,
                "type must contain size_t values");
  static_assert(std::is_integral<typename C::value_type>::value,
                "Integral required.");
  auto dim = size.size();
  assert(a.size() == dim && "Dimensions not matching");

  auto index = 0UL;
  auto prod = accumulate_product(size);
  prod /= size[0UL];

  for (auto i = 0UL; i < dim; i++) {
    index += prod * static_cast<size_t>(a[i]);
    prod /= size[dim - (i + 1UL)];
  }

  return index;
}

///
/// Transform an index to coordinates
///
template<class C, class D>
inline C
index_to_array(size_t index, D const& size)
{
  static_assert(std::is_same<typename D::value_type, size_t>::value,
                "type must contain size_t values");
  static_assert(std::is_integral<typename C::value_type>::value,
                "Integral required.");
  auto dim = size.size();
  auto prod = 1ul;

  auto d = C(dim, 0);

  for (auto i = 1ul; i < dim; i++) {
    prod *= size[dim - i];
  }
  for (auto i = 0ul; i < dim; i++) {
    d[i] = static_cast<typename C::value_type>(index / prod);
    index = index % prod;

    prod /= size[dim - (i + 1UL)];
  }

  return d;
}

/// Interpolation-Binary search.
/// Mixed interpolation and binary search to squeeze the best possible
/// performances. Interpolation search is done until a window of 10000
/// elements is reached. For smaller windows a binary search is performed.
template<class ForwardIt, class T>
inline ForwardIt
upper_bound(ForwardIt first, ForwardIt last, const T& value)
{
  auto count = std::distance(first, last);
  auto it = first;
  auto to = it;
  std::advance(to, count - 1);

  while (count > 10000) {
    auto itval = *it;
    // check if the value is outside the lower bound of the search interval
    if (value < itval) {
      return it;
    } else if (!(itval < value)) { // value <= it (so only if they are equal)
      return ++it;
    }

    // check if the value is outside the upper bound of the search interval
    if (!(value < *to)) { // value >= to
      return ++to;
    }

    // compute the new position interpolating the position of the point
    const auto np = std::round((value - itval) / (*to - itval) * count);
    auto nt = it;
    std::advance(nt, np);

    if (!(value < *nt)) { // value >= *nt
      it = ++nt;
    } else {
      to = --nt;
    }
    count = std::distance(it, to);
  }
  return upper_bound(it, ++to, value);
}

/// Greater common denominator
template<typename T>
T
get_gcd(T a, T b)
{
  if (b == 0) {
    return a;
  }
  return get_gcd(b, a % b);
}

/// Heaviside theta function
template<typename Floating,
         std::enable_if_t<std::is_floating_point<Floating>::value, int> = 0>
Floating
heaviside(Floating x)
{
  return x >= 0.0 ? 1.0 : 0.0;
}

/// Heaviside theta function with half-maximum convention
template<typename Floating,
         std::enable_if_t<std::is_floating_point<Floating>::value, int> = 0>
Floating
heaviside2(Floating x)
{
  if (x == 0.0) {
    return 0.5;
  } else if (x > 0.0) {
    return 1.0;
  }

  return 0.0;
}

/// Linear interpolation between @p a and @p b for the parameter @p t.
/// If @p t is outside the range [0, 1] it performs an extrapolation
template<typename Floating>
Floating
lerp(Floating a, Floating b, Floating t)
{
  static_assert(std::is_floating_point<Floating>::value,
                "Required floating point type");
  return (1 - t) * a + t * b;
}

template<typename Floating, typename Integer>
Floating
lerpi(Integer i, Integer j, Floating a, Floating b)
{
  static_assert(std::is_integral<Integer>::value, "Required integral type");
  static_assert(std::is_floating_point<Floating>::value,
                "Required floating point type");
  auto t = static_cast<Floating>(i) / static_cast<Floating>(j);
  return lerp(a, b, t);
}

} // namespace bwsl

// vim: set ts=2 sts=2 et sw=2 tw=80: //
