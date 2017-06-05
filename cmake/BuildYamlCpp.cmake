ExternalProject_Add(yaml-cpp
    PREFIX              ${DEPS_BUILD_DIR}
    SOURCE_DIR          ${CMAKE_SOURCE_DIR}/libs/yaml-cpp
    #GIT_REPOSITORY      https://github.com/jbeder/yaml-cpp
    GIT_SUBMODULES      libs/yaml-cpp
    GIT_TAG             master
    CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
        -DYAML_CPP_BUILD_TOOLS=OFF
        -DYAML_CPP_BUILD_CONTRIB=OFF
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    TEST_COMMAND        make test
)

# YAMLCPP as a dependency
list(APPEND THIRD_PARTY_DEPS yaml-cpp)

set(YAMLCPP_INCLUDE_DIR ${DEPS_INSTALL_DIR}/include)
set(YAMLCPP_LIBRARY ${DEPS_INSTALL_DIR}/lib/libyaml-cpp.a)

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
