# CATCH2 Finder for cmake
#
# Guido Masella (guido.masella@gmail.com)
#

find_path(Catch_INCLUDE_DIR catch.hpp)
mark_as_advanced(Catch_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Catch
  REQUIRED_VARS Catch_INCLUDE_DIR
  )

if(Catch_FOUND AND NOT TARGET Catch::Catch)
  add_library(Catch::Catch INTERFACE IMPORTED)
  set_target_properties(Catch::Catch PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    INTERFACE_INCLUDE_DIRECTORIES "${Catch_INCLUDE_DIR}"
    )
endif()

# vim: set ft=cmake ts=2 sts=2 et sw=2 tw=80 foldmarker={{{,}}} fdm=marker: #

