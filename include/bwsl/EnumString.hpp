//===-- EnumString.hpp -----------------------------------------*- C++ -*-===//
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
/// @brief      Definitions for the EnumString Class
///
//===---------------------------------------------------------------------===//
#pragma once

// boost
#include <boost/preprocessor.hpp>

// std
#include <exception>
#include <string>

///
/// Helper macro used in BWSL_DEFINE_ENUM_WITH_STRING_CONVERSIONS
///
#define BWSL_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE(r, data, elem)  \
  case elem:                                                                   \
    return BOOST_PP_STRINGIZE(elem);

///
/// Helper macro used in BWSL_DEFINE_ENUM_WITH_STRING_CONVERSIONS
///
#define BWSL_DEFINE_ENUM_WITH_STRING_CONVERSIONS_FROMSTRING_IF(r, data, elem)  \
  if (v == BOOST_PP_STRINGIZE(elem)) {                                         \
    n = elem;                                                                  \
    return;                                                                    \
  }

///
/// Macro for generating enum with string conversion.
///
/// The code was taken from https://stackoverflow.com/a/5094430
///
/// Usage:
/// @code
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
/// @endcode
///
#define BWSL_DEFINE_ENUM_WITH_STRING_CONVERSIONS(name, enumerators)            \
  enum name                                                                    \
  {                                                                            \
    BOOST_PP_SEQ_ENUM(enumerators)                                             \
  };                                                                           \
                                                                               \
  class EnumStringConversionFailed_##name : public std::exception              \
  {                                                                            \
  public:                                                                      \
    EnumStringConversionFailed_##name(std::string const& value)                \
      : value_(value)                                                          \
    {}                                                                         \
                                                                               \
    const char* what() const throw()                                           \
    {                                                                          \
      auto s = std::string("Unknown correspondance of ") + value_ +            \
               std::string(" for enum ") + name_;                              \
      return s.c_str();                                                        \
    }                                                                          \
                                                                               \
  private:                                                                     \
    const std::string name_{ BOOST_PP_STRINGIZE(name) };                       \
    std::string value_{ "" };                                                  \
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
        return "[ Unknown " BOOST_PP_STRINGIZE(name) " ]";                     \
    }                                                                          \
  }                                                                            \
                                                                               \
  inline void FromString(name& n, std::string const& v)                        \
  {                                                                            \
    BOOST_PP_SEQ_FOR_EACH(                                                     \
      BWSL_DEFINE_ENUM_WITH_STRING_CONVERSIONS_FROMSTRING_IF,                  \
      name,                                                                    \
      enumerators)                                                             \
    throw EnumStringConversionFailed_##name(v);                                \
  }

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
