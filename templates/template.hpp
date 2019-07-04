//===-- Foo.hpp ------------------------------------------------*- C++ -*-===//
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
/// @brief      Definitions for the Foo Class
///
//===---------------------------------------------------------------------===//
#pragma once

namespace bwsl {

///
/// Description
///
class Foo
{
public:
  /// Default constructor
  Foo() = default;

  /// Copy constructor
  Foo(const Foo&) = default;

  /// Copy assignment operator
  Foo& operator=(const Foo&) = default;

  /// Move constructor
  Foo(Foo&&) = default;

  /// Move assignment operator
  Foo& operator=(Foo&&) = default;

  /// Default destructor
  virtual ~Foo() = default;

protected:
private:
}; // class Foo

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
