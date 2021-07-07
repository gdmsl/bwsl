//===-- Accumulators.hpp ------------------------------------------------*- C++ -*-===//
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
/// @brief      Accumulators definitions
///
//===---------------------------------------------------------------------===//
#pragma once

#include <bwsl/accumulators/KnuthWelfordAccumulator.hpp>
#include <bwsl/accumulators/KahanAccumulator.hpp>
#include <bwsl/accumulators/NeumaierAccumulator.hpp>
#include <bwsl/accumulators/WestAccumulator.hpp>

namespace bwsl {

template<typename InputIt, typename Accumulator>
inline auto
apply_accumulator(InputIt begin, InputIt end, Accumulator& acc) -> Accumulator&
{
  while (begin != end) {
    acc.Add(*begin++);
  }
  return acc;
}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
