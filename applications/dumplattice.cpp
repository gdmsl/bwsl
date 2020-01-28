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

void
printjw(std::vector<int> const& jvec, std::vector<int> const& wvec) {
    for (auto const& x : jvec) {
      cout << " " << x;
    }
    for (auto const& x : wvec) {
      cout << " " << x;
    }
}

int
main(int ac, char **av)
{
  auto l = bwsl::Lattice(bwsl::SquareLattice, {4UL, 4UL}, false);

  cout << "i j distance x y" << endl;
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

  cout << endl;

  cout << "i j jx jy wx wy" << endl;
  for (auto i = 0UL; i < l.GetNumSites(); i++) {
    for (auto j = 0UL; j < l.GetNumSites(); j++) {
      cout << i << " " << j;
      auto jvec = l.GetJump(i, j);
      auto wvec = l.GetWinding(jvec);
      printjw(jvec, wvec);
      cout << endl;
    }
  }

  cout << " jx jy wx wy" << endl;
  auto jump = std::vector<int>{4, 4};
  assert(jump[0] == 4);
  assert(jump[1] == 4);
  auto wind = l.GetWinding(jump);
  printjw(jump, wind);
  cout << endl;

  jump[0] = -4;
  jump[1] = -4;
  wind = l.GetWinding(jump);
  printjw(jump, wind);
  cout << endl;

  jump[0] = -6;
  jump[1] = -6;
  wind = l.GetWinding(jump);
  printjw(jump, wind);
  cout << endl;

  jump[0] = -6;
  jump[1] = 10;
  wind = l.GetWinding(jump);
  printjw(jump, wind);
  cout << endl;

  return EXIT_SUCCESS;
}

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
