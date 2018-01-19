#
#                                   BWSL
#
# Build Catch
# Third party libraries required by BWSL
#
# Guido Masella (guido.masella@gmail.com)
#

ExternalProject_Add(catch
    PREFIX              ${DEPS_BUILD_DIR}
    SOURCE_DIR          ${CMAKE_SOURCE_DIR}/libs/catch2
    #GIT_REPOSITORY      https://github.com/philsquared/Catch2
    GIT_SUBMODULES      libs/catch2
    GIT_TAG             master
    CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
                        -DCMAKE_BUILD_TYPE=Release
                        -DCATCH_ENABLE_WERROR=OFF
    TEST_COMMAND        make test
)

# CATCH Includes
set(CATCH_INCLUDE_DIRS ${DEPS_INSTALL_DIR}/include)

# Catch as a dependency
list(APPEND THIRD_PARTY_DEPS catch)

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
