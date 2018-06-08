#
#                                   BWSL
#
# CMAkeLists.txt
#
# Guido Masella (guido.masella@gmail.com)
#

cmake_minimum_required(VERSION 3.8)

project(bwl VERSION 1 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Third-Party libraries
add_subdirectory(libs)

add_library(bwsl INTERFACE)
add_library(bwsl::bwsl ALIAS bwsl)

target_compile_features(bwsl
  INTERFACE
    cxx_range_for
    cxx_strong_enums
    cxx_auto_type
    cxx_decltype
    cxx_static_assert
    cxx_return_type_deduction
    cxx_nullptr
    cxx_override
    cxx_std_17
  )

target_include_directories(bwsl
  INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  )

# Applications
add_subdirectory(applications)

# Get the git version
execute_process(COMMAND git describe --tags --dirty=-dirty
    OUTPUT_VARIABLE BWSL_OUTPUT_STRIP_TRAILING_WHITESPACE)
set(BWSL_BUILD_TYPE "${CMAKE_BUILD_TYPE}")

# Get the compileation flags used
include(GetCompileFlags)
get_compile_flags(BWSL_CFLAGS)

# Add configs
add_subdirectory(config/)

# Testing
enable_testing()
add_subdirectory(test)

# vim: set ft=cmake ts=2 sts=2 et sw=2 tw=80 foldmarker={{{,}}} fdm=marker: #
