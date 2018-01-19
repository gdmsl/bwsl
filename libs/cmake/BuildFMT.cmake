#
#                                   BWSL
#
# Build FMT
# Third party libraries required by BWSl
#
# Guido Masella (guido.masella@gmail.com)
#

ExternalProject_Add(fmt
    PREFIX              ${DEPS_BUILD_DIR}
    SOURCE_DIR          ${CMAKE_SOURCE_DIR}/libs/fmt
    GIT_SUBMODULES      libs/fmt
    GIT_TAG             master
    CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
                        -DCMAKE_BUILD_TYPE=Release
    TEST_COMMAND        make test
)

# FMT includes and libraries
set(FMT_INCLUDE_DIR ${DEPS_INSTALL_DIR}/include)
set(FMT_LIBRARY ${DEPS_INSTALL_DIR}/lib64/libfmt.a)

# Add FMT as a dependency
list(APPEND THIRD_PARTY_DEPS fmt)

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
