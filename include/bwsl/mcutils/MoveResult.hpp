//===-- MoveResult.hpp ------------------------------------------------*- C++
//-*-===//
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
/// @brief      Definitions for the MoveResult Class
///
//===---------------------------------------------------------------------===//
#pragma once

#include <bwsl/mcutils/MoveStatus.hpp>

namespace bwsl::montecarlo {

///
/// Useful wrapper for result type of Markov Chain Monte Carlo moves
///
class MoveResult
{
public:
  /// Default constructor
  MoveResult() = default;

  /// Copy constructor
  MoveResult(const MoveResult&) = default;

  /// Copy assignment operator
  MoveResult& operator=(const MoveResult&) = default;

  /// Move constructor
  MoveResult(MoveResult&&) = default;

  /// Move assignment operator
  MoveResult& operator=(MoveResult&&) = default;

  /// Default destructor
  virtual ~MoveResult() = default;

  /// Check whether the move has been accepted
  auto Accepted() const -> bool { return status_ == MoveStatus::Accepted; };

  /// Check whether the move has been rejected
  auto Rejected() const -> bool { return status_ == MoveStatus::Rejected; };

  /// Check whether the move has been accepted
  auto Impossible() const -> bool { return status_ == MoveStatus::Impossible; };

  /// Return the probability of the move
  [[nodiscard]] auto Probability() const -> double { return prob_; };

  /// Get the status
  [[nodiscard]] auto GetStatus() const -> MoveStatus { return status_; };

protected:
private:
  /// Returned status of the move
  MoveStatus status_{ MoveStatus::Impossible };

  /// Returned probability of the move
  double prob_{ 0.0 };
}; // class MoveResult

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
