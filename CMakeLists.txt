#
#                                   BWSl
#
# CMAkeLists.txt
#
# Guido Masella (guido.masella@gmail.com)
#

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(bwsl)

# Point CMake at any custom modules we may ship
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Set default build type.
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "CMAKE_BUILD_TYPE not given, defaulting to 'Debug'.")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

# Set available build types for CMake GUIs.
# A different build type can still be set by -DCMAKE_BUILD_TYPE=...
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
  STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

# Generate a compilation database used by many tools (clang, etc...)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CXX FLAGS
set(CMAKE_CXX_FLAGS
    "-std=c++14 -pedantic -Wall -Wextra -Wshadow -Wconversion -Wfloat-equal\
    -Wcast-qual -Wcast-align -Wpointer-arith -Wwrite-strings")
set(CMAKE_CXX_FLAGS_RELEASE       "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG         "-Og -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL    "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITDEBINFO "-O3 -DNDEBUG -g")

# Output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add Third Party libs
add_subdirectory(libs/)
get_directory_property(libs_INCLUDE_DIRS DIRECTORY libs
    DEFINITION libs_INCLUDE_DIRS)
get_directory_property(libs_LIBRARIES DIRECTORY libs
    DEFINITION libs_LIBRARIES)
get_directory_property(libs_TARGETS DIRECTORY libs
    DEFINITION THIRD_PARTY_DEPS)
message(STATUS "Libs include dirs: ${libs_INCLUDE_DIRS}")
message(STATUS "Libs libraries: ${libs_LIBRARIES}")
include_directories(SYSTEM ${libs_INCLUDE_DIRS})

# include library headers
include_directories(include/)

# add the swmc library
add_library(
    bwsl
    INTERFACE
)
add_dependencies(bwsl ${libs_TARGETS})

# Add Applications
add_subdirectory(applications/)

# Add tests
enable_testing()
add_subdirectory(test/)

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
