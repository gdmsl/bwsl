//===-- LatticeFactory.hpp ------------------------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2017 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// \file
/// \version    0.1
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       May, 2017
/// \brief      Implementation for the LatticeFactory function
///
//===---------------------------------------------------------------------===//
#ifndef BWSL_LATTICEFACTORY_HPP
#define BWSL_LATTICEFACTORY_HPP

// bwsl
#include <Lattice.hpp>
#include <SquareLattice.hpp>

// std
#include <memory>

namespace bwsl {

std::shared_ptr<Lattice>
Lattice::CreateLattice(const std::string& name, const std::vector<size_t>& size)
{
  if (name == "square") {
    return std::make_shared<SquareLattice>(size);
  }
  return std::make_shared<SquareLattice>(size);
}

} // namespace bwsl

#endif // BWSL_LATTICEFACTORY_HPP

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
