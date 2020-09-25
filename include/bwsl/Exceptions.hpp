//===-- Foo.hpp ------------------------------------------------*- C++ -*-===//
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
/// @brief      Definitions for the Foo Class
///
//===---------------------------------------------------------------------===//
#pragma once

// std
#include <exception>
#include <string>
#include <utility>

namespace bwsl::exception {

/// Exception for missing parameters
class BadParsing : public std::exception
{
public:
  /// Default constructor
  BadParsing() = default;

  [[nodiscard]] const char* what() const noexcept override
  {
    return "BadParsing";
  }

private:
}; // class BadParsing

} // namespace bwsl::exception

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
