//===-- ObservableGroup.hpp ------------------------------------*- C++ -*-===//
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
/// @brief      Definitions for the ObservableGroup Class
///
//===---------------------------------------------------------------------===//
#pragma once

// bwsl
#include <bwsl/Accumulator.hpp>

// fmt
#include <fmt/format.h>
#include <fmt/ostream.h>

// std
#include <map>
#include <string>

namespace bwsl {

/// Description
template<typename Index_t>
class ObservableGroup
{
public:
  /// Default constructor
  ObservableGroup() = default;

  /// Copy constructor
  ObservableGroup(const ObservableGroup&) = default;

  /// Copy assignment operator
  ObservableGroup& operator=(const ObservableGroup&) = default;

  /// Move constructor
  ObservableGroup(ObservableGroup&&) = default;

  /// Move assignment operator
  ObservableGroup& operator=(ObservableGroup&&) = default;

  /// Default destructor
  virtual ~ObservableGroup() = default;

  /// Constructor
  ObservableGroup(std::string output_file);

  /// Constructor
  ObservableGroup(std::string output_file, std::vector<Index_t> const& indices);

  /// Measure an observable
  void Measure(Index_t idx, double val);

  /// Print the headers to the file
  void PrintHeaders() const;

  /// Print the results and reset all the accumulators
  void PrintAndReset();

  ObservableGroup& AddObservable(Index_t key);

protected:
private:
  /// Name of the associated output file
  std::string output_file_;

  /// Storage for the accumulators
  std::map<Index_t, bwsl::Accumulator> accumulator_{};
}; // class ObservableGroup

template<typename Index_t>
ObservableGroup<Index_t>::ObservableGroup(std::string output_file)
  : output_file_(output_file)
{}

template<typename Index_t>
ObservableGroup<Index_t>::ObservableGroup(std::string output_file,
                                          std::vector<Index_t> const& indices)
  : output_file_(output_file)
{
  for (auto i : indices) {
    accumulator_.try_emplace(i);
  }
}

template<typename Index_t>
inline void
ObservableGroup<Index_t>::Measure(Index_t idx, double val)
{
  accumulator_.at(idx).add(val);
}

template<typename Index_t>
inline void
ObservableGroup<Index_t>::PrintHeaders() const
{
  auto out = std::ofstream(output_file_.c_str(), std::ios::trunc);

  auto it = accumulator_.begin();
  fmt::print(out, "{}", it->first);
  while (++it != accumulator_.end()) {
    fmt::print(out, ",{}", it->first);
  }
  fmt::print(out, "\n");
}

template<typename Index_t>
inline void
ObservableGroup<Index_t>::PrintAndReset()
{
  auto out = std::ofstream(output_file_.c_str(), std::ios::app);

  auto it = accumulator_.begin();
  fmt::print(out, "{}", it->second.GetResult());
  it->second.Reset();
  while (++it != accumulator_.end()) {
    fmt::print(out, ",{}", it->second.GetResult());
    it->second.Reset();
  }
  fmt::print(out, "\n");
}

template<typename Index_t>
inline ObservableGroup<Index_t>&
ObservableGroup<Index_t>::AddObservable(Index_t key)
{
  accumulator_.try_emplace(key);
  return *this;
}

} // namespace bwsl

// vim: set ft=cpp ts=2 sts=2 et sw=2 tw=80: //
