ExternalProject_Add(pcg-cpp
    PREFIX              ${DEPS_BUILD_DIR}
    #GIT_REPOSITORY      https://github.com/imneme/pcg-cpp
    SOURCE_DIR          ${CMAKE_SOURCE_DIR}/libs/pcg-cpp
    GIT_SUBMODULES      libs/pcg-cpp
    GIT_TAG             master
    CONFIGURE_COMMAND   ""
    BUILD_COMMAND       ""
    TEST_COMMAND        ""
    INSTALL_COMMAND     ""
)

ExternalProject_Get_Property(pcg-cpp source_dir)
set(PCGCPP_INCLUDE_DIRS ${source_dir}/include)

list(APPEND THIRD_PARTY_DEPS pcg-cpp)

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
