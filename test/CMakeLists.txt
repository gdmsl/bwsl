#== CMakeLists.txt ---------------------------------------------------------==#
#
#                       BeagleWarlord's Support Library
#
# Copyright 2016-2022 Guido Masella. All Rights Reserved.
# See LICENSE file for details.
#
#==------------------------------------------------------------------------==#
#
# Guido Masella (guido.masella@gmail.com)
#
#==------------------------------------------------------------------------==#

# LatticeTest
add_executable(LatticeTest LatticeTest.cpp)
target_link_libraries(LatticeTest
  PRIVATE
    bwsl
    Catch2::Catch2WithMain
    fmt-header-only
  )
target_compile_options(LatticeTest
  PRIVATE
    -W -Wall -Wpedantic -Wextra
  )
add_test(NAME bwsl.Lattice COMMAND $<TARGET_FILE:LatticeTest>)

# HyperCubicGrid
add_executable(HyperCubicGridTest HyperCubicGridTest.cpp)
target_link_libraries(HyperCubicGridTest
  PRIVATE
    bwsl
    Catch2::Catch2WithMain
  )
target_compile_options(HyperCubicGridTest
  PRIVATE
    -W -Wall -Wpedantic -Wextra
  )
add_test(NAME bwsl.HyperCubicGrid COMMAND $<TARGET_FILE:HyperCubicGridTest>)

# EnumString
add_executable(EnumString EnumString.cpp)
target_link_libraries(EnumString
  PRIVATE
    bwsl
    Catch2::Catch2WithMain
  )
target_compile_options(EnumString
  PRIVATE
    -W -Wall -Wpedantic -Wextra
  )
add_test(NAME bwsl.EnumString COMMAND $<TARGET_FILE:EnumString>)

# PairsTest
add_executable(PairsTest PairsTest.cpp)
target_link_libraries(PairsTest
  PRIVATE
    bwsl
    Catch2::Catch2WithMain
  )
target_compile_options(PairsTest
  PRIVATE
    -W -Wall -Wpedantic -Wextra
  )
add_test(NAME bwsl.Pairs COMMAND $<TARGET_FILE:PairsTest>)

# RationalNumTest
add_executable(RationalNumTest RationalNumTest.cpp)
target_link_libraries(RationalNumTest
  PRIVATE
    bwsl
    Catch2::Catch2WithMain
    fmt-header-only
  )
target_compile_options(RationalNumTest
  PRIVATE
    -W -Wall -Wpedantic -Wextra
  )
add_test(NAME bwsl.RationalNum COMMAND $<TARGET_FILE:RationalNumTest>)

# HistAccumulatorTest
add_executable(HistAccumulatorTest HistAccumulatorTest.cpp)
target_link_libraries(HistAccumulatorTest
  PRIVATE
    bwsl
    Catch2::Catch2WithMain
  )
target_compile_options(HistAccumulatorTest
  PRIVATE
    -W -Wall -Wpedantic -Wextra
  )
add_test(NAME bwsl.HistAccumulator COMMAND $<TARGET_FILE:HistAccumulatorTest>)

# KahanAccumulatorTest
add_executable(KahanAccumulatorTest KahanAccumulatorTest.cpp)
target_link_libraries(KahanAccumulatorTest
  PRIVATE
    bwsl
    Catch2::Catch2WithMain
  )
target_compile_options(KahanAccumulatorTest
  PRIVATE
    -W -Wall -Wpedantic -Wextra
  )
add_test(NAME bwsl.KahanAccumulator COMMAND $<TARGET_FILE:KahanAccumulatorTest>)

# NeumaierAccumulatorTest
add_executable(NeumaierAccumulatorTest NeumaierAccumulatorTest.cpp)
target_link_libraries(NeumaierAccumulatorTest
  PRIVATE
    bwsl
    Catch2::Catch2WithMain
  )
target_compile_options(NeumaierAccumulatorTest
  PRIVATE
    -W -Wall -Wpedantic -Wextra
  )
add_test(NAME bwsl.NeumaierAccumulator COMMAND $<TARGET_FILE:NeumaierAccumulatorTest>)

# MoveStatsTest
add_executable(MoveStatsTest MoveStatsTest.cpp)
target_link_libraries(MoveStatsTest
  PRIVATE
    bwsl
    Catch2::Catch2WithMain
    fmt-header-only
  )
target_compile_options(MoveStatsTest
  PRIVATE
    -W -Wall -Wpedantic -Wextra
  )
add_test(NAME bwsl.MoveStats COMMAND $<TARGET_FILE:MoveStatsTest>)

# vim: set ft=cmake ts=2 sts=2 et sw=2 tw=80 foldmarker={{{,}}} fdm=marker: #
