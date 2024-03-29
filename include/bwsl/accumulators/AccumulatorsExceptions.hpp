//===-- AccumulatorsExceptions.hpp -----------------------------*- C++ -*-===//
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
/// @brief      Definitions exceptions for accumulators
///
//===---------------------------------------------------------------------===//
#pragma once

// std
#include <exception>

namespace bwsl::exception {

class AccumulatorOverflow : public std::exception
{
  [[nodiscard]] auto what() const noexcept -> const char* override
  {
    return "Too many measurements added to the accumulator";
  }
};

} // namespace bwsl::exception

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
