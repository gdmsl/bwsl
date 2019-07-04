//===-- dumpmt.cpp ---------------------------------------------*- C++ -*-===//
//
//                       BeagleWarlord's Support Library
//
// Copyright 2016-2010 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// \file
/// \version    0.1
/// \author     Guido Masella (guido.masella@gmail.com)
/// \date       May, 2017
/// \brief      Dump the status of a fresh initialized marsenne twister
///
//===---------------------------------------------------------------------===//

// std
#include <fstream>
#include <iostream>
#include <random>

int
main (int ac, char **av)
{
  if (ac < 2) {
    return EXIT_FAILURE;
  }

  auto seed = static_cast<unsigned long>(atoi(av[1]));

  auto rng = std::mt19937_64{seed};

  std::cout << rng << std::endl;

  return EXIT_SUCCESS;
}

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
