#
#                                   BWSL
#
# libs/CMAkeLists.txt
# Third party libraries required by BWSL
#
# Guido Masella (guido.masella@gmail.com)
#

# External Projects
include(ExternalProject)

# Point CMake at any custom modules we may ship
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/libs/cmake")

# Useful directories for management of third party libraries
set(DEPS_INSTALL_DIR "${CMAKE_BINARY_DIR}/usr" CACHE PATH "Dependencies install directory.")
set(DEPS_BIN_DIR "${DEPS_INSTALL_DIR}/bin" CACHE PATH "Dependencies binary install directory.")
set(DEPS_LIB_DIR "${DEPS_INSTALL_DIR}/lib" CACHE PATH "Dependencies library install directory.")
set(DEPS_BUILD_DIR "${CMAKE_BINARY_DIR}/build" CACHE PATH "Dependencies build directory.")
set(DEPS_DOWNLOAD_DIR "${DEPS_BUILD_DIR}/downloads" CACHE PATH "Dependencies download directory.")

# Catch 2 is our choice for the testing framework
include(BuildCatch)
list(APPEND libs_INCLUDE_DIRS ${CATCH_INCLUDE_DIRS})

# We use FMT for fast output formatting and pretty printing
add_subdirectory(fmt)
target_link_libraries(bwsl
  INTERFACE
    fmt::fmt-header-only
  )

# vim: set ft=cmake ts=2 sts=2 et sw=2 tw=80 foldmarker={{{,}}} fdm=marker: #
