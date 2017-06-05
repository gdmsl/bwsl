cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(bwsl VERSION 0.3 LANGUAGES C CXX)

#add cmake script files
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Default Build type to Debug if nothing else is specified
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Debug")
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(BUILD_WITH_GPROF)
    message(STATUS "Adding -pg to all the conmpilation flags")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
endif()

# Generate a CompilationDatabase (compile_commands.json file) for our build,
# for use by clang_complete, YouCompleteMe, etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

## C FLAGS
set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -ansi -pedantic -Wall -W -Wmissing-prototypes\
    -Wstrict-prototypes -Wconversion -Wshadow -Wpointer-arith -Wcast-qual\
    -Wcast-align -Wfloat-equal -Wwrite-strings -Wnested-externs\
    -fno-common")
set(CMAKE_C_FLAGS_RELEASE
    "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG
    "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG} -g")
set(CMAKE_C_FLAGS_MINSIZEREL
    "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_MINSIZEREL} -Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITDEBINFO
    "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELWITDEBINFO} -O3 -DNDEBUG -g")

# CXX FLAGS
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++14 -pedantic -Wall -W -Wconversion -Wshadow\
    -Wpointer-arith -Wcast-qual -Wcast-align -Wfloat-equal -Wwrite-strings\
    -fno-common")
set(CMAKE_CXX_FLAGS_RELEASE
    "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL
    "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITDEBINFO
    "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELWITDEBINFO} -O3 -DNDEBUG -g")

## OUPUT DIRECTORIES
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# openmp
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# Third Party Libraries Directories
set(DEPS_INSTALL_DIR "${CMAKE_BINARY_DIR}/usr" CACHE PATH "Dependencies install directory.")
set(DEPS_BIN_DIR "${DEPS_INSTALL_DIR}/bin" CACHE PATH "Dependencies binary install directory.")
set(DEPS_LIB_DIR "${DEPS_INSTALL_DIR}/lib" CACHE PATH "Dependencies library install directory.")
set(DEPS_BUILD_DIR "${CMAKE_BINARY_DIR}/build" CACHE PATH "Dependencies build directory.")
set(DEPS_DOWNLOAD_DIR "${DEPS_BUILD_DIR}/downloads" CACHE PATH "Dependencies download directory.")

# find make or cmake
include(FindMakePRG)

# External Projects
include(ExternalProject)

# Enable Testing
enable_testing()

# Build Catch
include(BuildCatch)
include_directories(SYSTEM ${CATCH_INCLUDE_DIRS})

# YAML
#if (NOT YAMLCPP_FOUND)
    #include(FindYamlCpp)
#endif()
#if (NOT YAMLCPP_FOUND)
    #include(BuildYamlCpp)
#else()
    #add_custom_target(yaml-cpp)
#endif()
#include_directories(SYSTEM ${YAMLCPP_INCLUDE_DIR})

# Include the directory where the headers of the third party dirs
# are
include_directories(SYSTEM ${DEPS_INSTALL_DIR}/include)

# boost library are required
set(Boost_USE_STATIC_LIBS     ON)
set(Boost_USE_MULTITHREADED   ON)
set(Boost_USE_STATIC_RUNTIME  ON)
find_package(Boost REQUIRED COMPONENTS program_options system filesystem)
include_directories(${Boost_INCLUDE_DIRS})
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
message(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")

# path of .h files
include_directories(src/)

# The LIBRARY libjohannes
add_library(
    bwsl
    STATIC
    src/Lattice.cpp
    #    src/Lattice.cpp
)

add_dependencies(bwsl ${THIRD_PARTY_DEPS})

include(BuildTests)

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
