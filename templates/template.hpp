//===-- Foo.hpp ------------------------------------------------*- C++ -*-===//
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
/// \brief      Definitions for the Foo Class
///
//===---------------------------------------------------------------------===//
#ifndef BWSL_TEMPLATE_HPP
#define BWSL_TEMPLATE_HPP

namespace bwsl {

///
/// \brief      A Class
/// \version    0.1
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       May, 2017
///
/// Detailed description
///
class Foo {
 public:
   /// Default constructor
   Foo() = default;

   /// Copy constructor
   Foo(const Foo& that) = default;

   /// Default destructor
   virtual ~Foo() = default;

   /// Copy assignment operator
   Foo& operator=(const Foo& that) = default;
 protected:
 private:
}; // class Foo

}

#endif // BWSL_TEMPLATE_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
