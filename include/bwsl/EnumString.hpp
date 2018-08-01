//===-- EnumString.hpp -----------------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2016-2018 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// \file
/// \version    0.1
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       May, 2017
/// \brief      Definitions for the EnumString Class
///
//===---------------------------------------------------------------------===//
#ifndef BWSL_ENUMSTRING_HPP
#define BWSL_ENUMSTRING_HPP

// boost
#include <boost/preprocessor.hpp>

// std
#include <string>

///
/// Helper macro used in BWSL_DEFINE_ENUM_WITH_STRING_CONVERSIONS
///
#define BWSL_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE(r, data, elem)  \
  case elem:                                                                   \
    return BOOST_PP_STRINGIZE(elem);

///
/// Macro for generating enum with string conversion.
///
/// The code was taken from https://stackoverflow.com/a/5094430
///
/// Usage:
/// \code
/// #include <iostream>
///
/// BWSL_DEFINE_ENUM_WITH_STRING_CONVERSIONS(OS_type, (Linux)(Apple)(Windows))
///
/// int main()
/// {
///   OS_type t = Windows;
///   std::cout << ToString(t) << " " << ToString(Apple) << std::endl;
/// }
///
/// \endcode
///
#define BWSL_DEFINE_ENUM_WITH_STRING_CONVERSIONS(name, enumerators)            \
  enum name                                                                    \
  {                                                                            \
    BOOST_PP_SEQ_ENUM(enumerators)                                             \
  };                                                                           \
                                                                               \
  inline std::string ToString(name v)                                          \
  {                                                                            \
    switch (v) {                                                               \
      BOOST_PP_SEQ_FOR_EACH(                                                   \
        BWSL_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE,                \
        name,                                                                  \
        enumerators)                                                           \
      default:                                                                 \
        return "[Unknown " BOOST_PP_STRINGIZE(name) "]";                       \
    }                                                                          \
  }

#endif // BWSL_ENUMSTRING_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //