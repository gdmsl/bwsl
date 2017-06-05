ExternalProject_Add(catch
    PREFIX              ${DEPS_BUILD_DIR}
    SOURCE_DIR          ${CMAKE_SOURCE_DIR}/libs/catch
    #GIT_REPOSITORY      https://github.com/philsquared/Catch
    GIT_SUBMODULES      libs/catch
    GIT_TAG             master
    CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    TEST_COMMAND        make test
)

# CATCH Includes
set(CATCH_INCLUDE_DIRS ${DEPS_INSTALL_DIR}/include)

# GTest as a dependency
list(APPEND THIRD_PARTY_DEPS catch)

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
