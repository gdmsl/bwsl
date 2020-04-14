//===-- MarkovChainStats.hpp ----------------------------------*- C++ -*-===//
//
//                      BeagleWarlord's Support Library
//
// Copyright 2016-2020 Guido Masella. All Rights Reserved.
// See LICENSE file for details
//
//===---------------------------------------------------------------------===//
///
/// @file
/// @author     Guido Masella (guido.masella@gmail.com)
/// @brief      Definitions for the MarkovChainStats Class
///
//===---------------------------------------------------------------------===//
#pragma once

// bwsl
#include <bwsl/Accumulator.hpp>

// fmt
#include <fmt/format.h>

// boost
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>

// std
#include <exception>
#include <string>

namespace bwsl {

namespace exception {
class MoveInvalidSequence : public std::exception
{
public:
  MoveInvalidSequence(std::string name)
    : name_(name)
  {}

  const char* what() const throw()
  {
    return fmt::format("{} move: invalid sequence of proposals, accpetances or "
                       "rejections",
                       name_)
      .c_str();
  }

private:
  std::string name_{ "Unknown" };
};

class InvalidProbability : public std::exception
{
public:
  InvalidProbability(double prob)
    : prob_(prob)
  {}
  const char* what() const throw()
  {
    return fmt::format("{} is not a valid probability", prob_).c_str();
  }

private:
  double prob_{};
};
}

///
/// Keeps statistics of a Markov Chain Monte Carlo move.
///
class MarkovChainStats
{
public:
  /// Default constructor
  MarkovChainStats() = default;

  /// Copy constructor
  MarkovChainStats(const MarkovChainStats&) = default;

  /// Copy assignment operator
  MarkovChainStats& operator=(const MarkovChainStats&) = default;

  /// Move constructor
  MarkovChainStats(MarkovChainStats&&) = default;

  /// Move assignment operator
  MarkovChainStats& operator=(MarkovChainStats&&) = default;

  /// Construct a statistics for a named move
  MarkovChainStats(std::string name);

  /// Default destructor
  virtual ~MarkovChainStats() = default;

  /// Propose a move
  auto Propose() -> void { UpdateIfNotProposed(proposed_); };

  /// The proposed move has been accepted
  auto Accept(double prob) -> void { UpdateIfProposed(accepted_, prob); };

  /// The proposed move has been rejected
  auto Reject(double prob) -> void { UpdateIfProposed(rejected_, prob);};

  /// The proposed move is impossible to carry out.
  auto Impossible() -> void { UpdateIfProposed(impossible_, 0.0); };

  /// Compute the acceptance from the collected statistics
  auto GetAcceptedRatio() const -> double
  {
    return static_cast<double>(accepted_) / proposed_;
  };

  /// Compute the rejection ratio from the collected statistics
  auto GetRejectedRatio() const -> double
  {
    return static_cast<double>(rejected_) / proposed_;
  };

  /// Compute the ratio of cancellations from the collected statistics
  auto GetImpossibleRatio() const -> double
  {
    return static_cast<double>(impossible_) / proposed_;
  };

  /// Compute the average acceptance
  auto GetAverageProbability() const -> double { return prob_.GetResult(); };

  /// Reset all the counters
  auto Reset() -> void;

protected:
  template<typename T>
  auto UpdateIfProposed(T& var, double prob) -> void;

  template<typename T>
  auto UpdateIfNotProposed(T& varm) -> void;


private:
  /// Name for identify the move
  std::string name_{ "Unknown" };

  /// Checks to se if a move has been proposed but not accepted or rejected yet
  bool proposedflag_{ false };

  /// Number of times the move has been proposed
  unsigned long proposed_{ 0UL };

  /// Number of times the move has been accepted
  unsigned long accepted_{ 0UL };

  /// Number of times the move has been rejected
  unsigned long rejected_{ 0UL };

  /// Number of times the move has been canceled
  unsigned long impossible_{ 0UL };

  /// Probability
  Accumulator prob_{};

  // serialization
  friend class boost::serialization::access;

  //// Serialization method for the class
  template<class Archive>
  auto serialize(Archive& ar, const unsigned int version) -> void;
}; // class MarkovChainStats

template<class Archive>
inline auto
MarkovChainStats::serialize(Archive& ar, const unsigned int /* version */)
  -> void
{
  // clang-format off
  ar & prob_;
  ar & proposed_;
  ar & accepted_;
  ar & rejected_;
  ar & impossible_;
  ar & proposedflag_;
  // clang-format on
}

inline MarkovChainStats::MarkovChainStats(std::string name)
  : name_(std::move(name))
{}

template<typename T>
inline auto
MarkovChainStats::UpdateIfProposed(T& var, double prob) -> void
{
  if (!proposedflag_) {
    throw exception::MoveInvalidSequence(name_);
  }

  var += 1UL;
  proposedflag_ = false;
  prob_.add(prob);
}

template<typename T>
inline auto
MarkovChainStats::UpdateIfNotProposed(T& var) -> void
{
  if (proposedflag_) {
    throw exception::MoveInvalidSequence(name_);
  }

  var += 1UL;
  proposedflag_ = true;
}

inline auto
MarkovChainStats::Reset() -> void
{
  proposed_ = 0UL;
  accepted_ = 0UL;
  rejected_ = 0UL;
  impossible_ = 0UL;
  proposedflag_ = false;
  prob_.Reset();
}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
