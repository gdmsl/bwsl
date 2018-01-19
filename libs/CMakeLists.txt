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
include(BuildFMT)
list(APPEND libs_INCLUDE_DIRS ${FMT_INCLUDE_DIRS})
list(APPEND libs_LIBRARIES ${FMT_LIBRARY})

# Include the directory where the headers of the third party dirs
# are
include_directories(SYSTEM ${DEPS_INSTALL_DIR}/include)

# Boost
# TODO: download and install boost if required to
set(Boost_USE_STATIC_LIBS     ON)
set(Boost_USE_MULTITHREADED   ON)
set(Boost_USE_STATIC_RUNTIME  ON)
find_package(Boost REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
message(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
list(APPEND libs_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
list(APPEND libs_LIBRARIES ${Boost_LIBRARIES})

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
