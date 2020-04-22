//===-- MoveStatus.hpp ------------------------------------------------*- C++ -*-===//
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
/// @brief      Definitions for the MoveStatus Class
///
//===---------------------------------------------------------------------===//
#pragma once

namespace bwsl::montecarlo {

///
/// Status flags for moves
///
enum class MoveStatus : char
{
  Accepted = 'A',
  Rejected = 'R',
  Impossible = 'I',
}; // enum class MoveStatus

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
