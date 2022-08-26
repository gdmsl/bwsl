//===-- FileUtils.hpp ------------------------------------------*- C++ -*-===//
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
/// @brief      Definitions for the class Foo
///
//===---------------------------------------------------------------------===//
#pragma once

// std
#include <fstream>
#include <string>

namespace bwsl {

///
/// Check if two files are equal.
/// This code is based on this answer
/// http://stackoverflow.com/a/37575457/4477378
///
auto
compare_files(const std::string& p1, const std::string& p2) -> bool
{
  std::ifstream f1(p1, std::ifstream::binary | std::ifstream::ate);
  std::ifstream f2(p2, std::ifstream::binary | std::ifstream::ate);

  if (f1.fail() || f2.fail()) {
    return false; // file problem
  }

  if (f1.tellg() != f2.tellg()) {
    return false; // size mismatch
  }

  // seek back to beginning and use equal to compare contents
  f1.seekg(0, std::ifstream::beg);
  f2.seekg(0, std::ifstream::beg);
  return equal(std::istreambuf_iterator<char>(f1.rdbuf()),
               std::istreambuf_iterator<char>(),
               std::istreambuf_iterator<char>(f2.rdbuf()));
}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
