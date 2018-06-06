#
#                                   BWSL
#
# Build Catch
# Third party libraries required by SWMC
#
# Guido Masella (guido.masella@gmail.com)
#
cmake_minimum_required(VERSION 3.5)

project(catch_builder LANGUAGES CXX)

include(ExternalProject)

find_package(Git REQUIRED)

ExternalProject_Add(catch_ex
  PREFIX
    ${CMAKE_BINARY_DIR}/deps/catch
  TIMEOUT
    10
  GIT_REPOSITORY
    https://github.com/philsquared/Catch.git
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    -DCMAKE_BUILD_TYPE=Release
    -DCATCH_ENABLE_WERROR=OFF
  TEST_COMMAND
    ""
    #make test
  UPDATE_COMMAND
    ${GIT_EXECUTABLE} pull
)

ExternalProject_Get_Property(catch_ex INSTALL_DIR)
add_library(Catch INTERFACE)
target_include_directories(Catch
  INTERFACE
    ${INSTALL_DIR}/include
  )
add_library(Catch::Catch ALIAS Catch)

# vim: set ft=cmake ts=2 sts=2 et sw=2 tw=80 foldmarker={{{,}}} fdm=marker: #
