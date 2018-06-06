//===-- Bravais.cpp --------------------------------------------*- C++ -*-===//
//
//                                   BWSL
//
// Copyright 2017-2018 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// \file
/// \version    0.1
/// \author     Guido Masella (guido.masella@gmail.com)
/// \brief      Implementation of the Bravais Class
///
//===---------------------------------------------------------------------===//
// bwsl
#include <bwsl/Bravais.hpp>
#include <bwsl/MathUtils.hpp>

// std
#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>

using namespace bwsl;
using realvec_t = Bravais::realvec_t;

Bravais::Bravais(size_t dim_, realvec_t pvectors, realvec_t mvectors)
  : pvectors_(std::move(mvectors))
  , mvectors_(std::move(mvectors))
{
}

realvec_t Bravais::GetRealspace(coordinates_t const& coords) const
{
  assert(coords.size() == dim_ && "Dimensions mismatch");
  auto p = Bravais::realvec_t(dim_, 0.0);
  for (auto i = 0ul; i < dim_; i++) {
    for (auto j = 0ul; j < dim_; j++) {
      p[i] += coords[i] * pvectors_[i * dim_ + j];
    }
  }
  return std::move(p);
}

realvec_t Bravais::GetVector(coordinates_t const& first, coordinates_t const& second) const
{
  auto p = coordinates_t(dim_, 0.0);
  for (auto i = 0ul; i < dim_; i++) {
    p[i] = first[i] - second[i];
  }

  return GetRealspace(p);
}

double Bravais::GetDistance(coordinates_t const& first, coordinates_t const& second) const
{
  auto p = GetVector(first, second);
  auto d = 0.0;
  for (size_t i = 0; i < dim_; i++) {
    d += square(p[i]);
  }

  return std::sqrt(d);
}

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
