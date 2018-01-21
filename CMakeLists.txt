#
#                                   BWSl
#
# CMAkeLists.txt
#
# Guido Masella (guido.masella@gmail.com)
#

cmake_minimum_required(VERSION 3.5)

project(bwsl LANGUAGES CXX VERSION 1.0)

add_library(bwsl INTERFACE)
add_library(bwsl::bwsl ALIAS bwsl)

target_compile_features(bwsl
  INTERFACE
    cxx_range_for
    cxx_strong_enums
)

target_include_directories(bwsl INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_subdirectory(libs)

add_subdirectory(applications)

enable_testing()
add_subdirectory(test)

# vim: set ft=cmake ts=2 sts=2 et sw=2 tw=80 foldmarker={{{,}}} fdm=marker: #
