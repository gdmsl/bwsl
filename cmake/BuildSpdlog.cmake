ExternalProject_Add(spdlog
    PREFIX              ${DEPS_BUILD_DIR}
    SOURCE_DIR          ${CMAKE_SOURCE_DIR}/libs/spdlog
    #GIT_REPOSITORY      https://github.com/philsquared/Catch
    GIT_SUBMODULES      libs/spdlog
    GIT_TAG             master
    CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    TEST_COMMAND        make test
)

# Spdlog Includes
set(SPDLOG_INCLUDE_DIRS ${DEPS_INSTALL_DIR}/include)

# Spdlog as a dependency
list(APPEND THIRD_PARTY_DEPS spdlog)

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
