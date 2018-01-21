#
#                                   BWSL
#
# Build Catch
# Third party libraries required by BWSL
#
# Guido Masella (guido.masella@gmail.com)
#

ExternalProject_Add(catch_ex
  PREFIX
    ${DEPS_BUILD_DIR}
  SOURCE_DIR
    ${CMAKE_SOURCE_DIR}/libs/catch2
  GIT_SUBMODULES
    libs/catch2
  GIT_TAG
    master
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
    -DCMAKE_BUILD_TYPE=Release
    -DCATCH_ENABLE_WERROR=OFF
  TEST_COMMAND
    make test
)

add_library(catch INTERFACE)
add_library(catch::catch ALIAS catch)
target_include_directories(catch
  INTERFACE
    ${DEPS_INSTALL_DIR}/include
  )
add_dependencies(catch
  PUBLIC
    catch_ex
  )
# vim: set ft=cmake ts=2 sts=2 et sw=2 tw=80 foldmarker={{{,}}} fdm=marker: #
