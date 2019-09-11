//===-- dumpmt.cpp ---------------------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2016-2010 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// @file
/// @author     Guido Masella (guido.masella@gmail.com)
/// @brief      Dump the status of a fresh initialized marsenne twister
///
//===---------------------------------------------------------------------===//

// bwsl
#include <bwsl/Lattice.hpp>

// std
#include <fstream>
#include <iostream>
#include <random>

using namespace std;

int
main (int ac, char **av)
{
  auto l = bwsl::Lattice(bwsl::SquareLattice, {4UL, 4UL}, false);

  for (auto i = 0UL; i < l.GetNumSites(); i++) {
    for (auto j = 0UL; j < l.GetNumSites(); j++) {
      cout << i << " " << j << " " << l.GetDistance(i, j);
      auto vec = l.GetVector(i, j);
      for (auto const& x : vec) {
        cout << " " << x;
      }
      cout << endl;
    }
  }

  return EXIT_SUCCESS;
}

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
