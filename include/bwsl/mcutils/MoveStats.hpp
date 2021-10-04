//===-- MoveStats.hpp ----------------------------------*- C++ -*-===//
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
/// @brief      Definitions for the MoveStats Class
///
//===---------------------------------------------------------------------===//
#pragma once

// bwsl
#include <bwsl/Accumulators.hpp>
#include <bwsl/mcutils/MoveResult.hpp>
#include <bwsl/mcutils/MoveStatus.hpp>

// fmt
#include <fmt/format.h>
#include <fmt/ostream.h>

// boost
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>

// std
#include <exception>
#include <ostream>
#include <string>

namespace bwsl::montecarlo {

namespace exception {
class MoveInvalidSequence : public std::exception
{
public:
  MoveInvalidSequence(std::string const& name)
    : message_(buildmessage(name))
  {}

  [[nodiscard]] auto what() const noexcept -> const char* override
  {
    return message_.c_str();
  }

protected:
  [[nodiscard]] static auto buildmessage(std::string const& name) -> std::string
  {

    return fmt::format("{} move: invalid sequence of proposals, accpetances or "
                       "rejections",
                       name);
  }

private:
  std::string message_{};
}; // class MoveInvalidSequence

class InvalidProbability : public std::exception
{
public:
  InvalidProbability(double prob)
    : message_(buildmessage(prob))
  {}

  [[nodiscard]] auto what() const noexcept -> const char* override
  {
    return message_.c_str();
  }

protected:
  [[nodiscard]] static auto buildmessage(double prob) -> std::string
  {
    return fmt::format("{} is not a valid probability", prob);
  }

private:
  std::string message_{};
}; // class InvalidProbability

} // namespace bwsl::montecarlo::exception

///
/// Keeps statistics of a Markov Chain Monte Carlo move.
///
class MoveStats
{
public:
  /// Default constructor
  MoveStats() = default;

  /// Copy constructor
  MoveStats(const MoveStats&) = default;

  /// Copy assignment operator
  MoveStats& operator=(const MoveStats&) = default;

  /// Move constructor
  MoveStats(MoveStats&&) = default;

  /// Move assignment operator
  MoveStats& operator=(MoveStats&&) = default;

  /// Construct a statistics for a named move
  MoveStats(std::string name);

  /// Default destructor
  virtual ~MoveStats() = default;

  /// Propose a move
  auto Propose() -> void { UpdateIfNotProposed(proposed_); };

  /// The proposed move has been accepted
  auto Accept(double prob) -> void { UpdateIfProposed(accepted_, prob); };

  /// The proposed move has been rejected
  auto Reject(double prob) -> void { UpdateIfProposed(rejected_, prob); };

  /// The proposed move is impossible to carry out.
  auto Impossible() -> void { UpdateIfProposed(impossible_, 0.0); };

  /// Add to the statistics
  auto Add(MoveResult const& res) -> void;

  /// Get the name of the move
  [[nodiscard]] auto GetName() const -> std::string { return name_; };

  /// Compute the acceptance from the collected statistics
  [[nodiscard]] auto GetAcceptedRatio() const -> double
  {
    return proposed_ == 0UL ? 0.0 : static_cast<double>(accepted_) / proposed_;
  };

  /// Compute the rejection ratio from the collected statistics
  [[nodiscard]] auto GetRejectedRatio() const -> double
  {
    return static_cast<double>(rejected_) / proposed_;
  };

  /// Compute the ratio of cancellations from the collected statistics
  [[nodiscard]] auto GetImpossibleRatio() const -> double
  {
    return static_cast<double>(impossible_) / proposed_;
  };

  /// Compute the average acceptance
  [[nodiscard]] auto GetAverageProbability() const -> double
  {
    return prob_.Mean();
  };

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
  accumulators::KahanAccumulator prob_{};

  /// Serialization
  friend class boost::serialization::access;

  /// Ostream operator
  friend auto operator<<(std::ostream& os, const MoveStats& dt)
    -> std::ostream&;

  //// Serialization method for the class
  template<class Archive>
  auto serialize(Archive& ar, const unsigned int version) -> void;
}; // class MoveStats

template<class Archive>
inline auto
MoveStats::serialize(Archive& ar, const unsigned int /* version */) -> void
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

inline MoveStats::MoveStats(std::string name)
  : name_(std::move(name))
{}

template<typename T>
inline auto
MoveStats::UpdateIfProposed(T& var, double prob) -> void
{
  if (!proposedflag_) {
    throw exception::MoveInvalidSequence(name_);
  }

  var += 1UL;
  proposedflag_ = false;
  prob_.Add(prob);
}

template<typename T>
inline auto
MoveStats::UpdateIfNotProposed(T& var) -> void
{
  if (proposedflag_) {
    throw exception::MoveInvalidSequence(name_);
  }

  var += 1UL;
  proposedflag_ = true;
}

inline auto
MoveStats::Reset() -> void
{
  proposed_ = 0UL;
  accepted_ = 0UL;
  rejected_ = 0UL;
  impossible_ = 0UL;
  proposedflag_ = false;
  prob_.Reset();
}

inline auto
MoveStats::Add(MoveResult const& res) -> void
{
  Propose();

  switch (res.GetStatus()) {
    case MoveStatus::Accepted:
      Accept(res.Probability());
      break;
    case MoveStatus::Rejected:
      Reject(res.Probability());
      break;
    case MoveStatus::Impossible:
      Impossible();
      break;
  }
}

inline auto
operator<<(std::ostream& os, const MoveStats& dt) -> std::ostream&
{
  fmt::print(os,
             "{}(accepted = {:.3e}, rejected = {:.3e}, impossible = {:.3e}, "
             "probability = {:.3e})",
             dt.name_,
             dt.GetAcceptedRatio(),
             dt.GetRejectedRatio(),
             dt.GetImpossibleRatio(),
             dt.GetAverageProbability());
  return os;
}
} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
