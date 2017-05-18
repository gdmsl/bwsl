//===-- FileUtils.hpp ------------------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2016-2017 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// \file
/// \version    1.0
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       June, 2016
/// \brief      Definitions for the class Foo
///
//===---------------------------------------------------------------------===//
#ifndef BWSL_FILEUTILS_HPP
#define BWSL_FILEUTILS_HPP

// std
#include <fstream>
#include <string>

namespace bwsl {

///
/// Check if two files are equal.
/// This code is based on this answer
/// http://stackoverflow.com/a/37575457/4477378
///
bool compare_files(const std::string& p1, const std::string& p2)
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

#endif // BWSL_FILEUTILS_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
