#
#                                   BWSL
#
# CMAkeLists.txt
#
# Guido Masella (guido.masella@gmail.com)
#

cmake_minimum_required(VERSION 3.8)

# Determine if bwsl is built as a subproject (using add_subdirectory)
# or if it is the master project.
set(MASTER_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(MASTER_PROJECT ON)
endif ()

# Set the default CMAKE_BUILD_TYPE to Release.
# This should be done before the project command since the latter can set
# CMAKE_BUILD_TYPE itself (it does so for nmake).
if (NOT CMAKE_BUILD_TYPE)
  join(doc "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or "
           "CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING ${doc})
endif ()

# Options that control generation of various targets.
option(BWSL_TEST "Generate the test target." ${MASTER_PROJECT})

project(bwl VERSION 1 LANGUAGES CXX)

# Output the build type for checking
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

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
if (BWSL_TEST)
  enable_testing()
  add_subdirectory(test)
endif()

# vim: set ft=cmake ts=2 sts=2 et sw=2 tw=80 foldmarker={{{,}}} fdm=marker: #
