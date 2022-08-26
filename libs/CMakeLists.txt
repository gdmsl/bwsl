#
#                                   BWSL
#
# libs/CMAkeLists.txt
# Third party libraries required by BWSL
#
# Guido Masella (guido.masella@gmail.com)
#

# boost
if (MASTER_PROJECT)
  find_package(Boost REQUIRED)
  set_target_properties(Boost::boost PROPERTIES IMPORTED_GLOBAL TRUE)
endif()

# catch
if (BWSL_TEST)
  message(STATUS "Compiling bundled CATCH")
  set(CATCH_BUILD_TESTING OFF CACHE BOOL "disable CATCH tests")
  add_subdirectory(catch)
endif()

# fmt
if (MASTER_PROJECT)
  message(STATUS "Adding bundled FMT")
  add_subdirectory(fmt EXCLUDE_FROM_ALL)
endif()

# vim: set ft=cmake ts=2 sts=2 et sw=2 tw=80 foldmarker={{{,}}} fdm=marker: #
