ExternalProject_Add(bwsl
    PREFIX              ${DEPS_BUILD_DIR}
    #GIT_REPOSITORY      https://github.com/imneme/pcg-cpp
    SOURCE_DIR          ${CMAKE_SOURCE_DIR}/libs/bwsl
    GIT_SUBMODULES      libs/bwsl
    GIT_TAG             master
    CONFIGURE_COMMAND   ""
    BUILD_COMMAND       ""
    TEST_COMMAND        ""
    INSTALL_COMMAND     ""
)

ExternalProject_Get_Property(bwsl source_dir)
set(BWSL_INCLUDE_DIRS ${source_dir}/src)

list(APPEND THIRD_PARTY_DEPS bwsl)

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
