#
#                                   BWSL
#
# libs/CMAkeLists.txt
# Third party libraries required by BWSL
#
# AUTHOR: Guido Masella (guido.masella@gmail.com)
#

# Boost {{{
if(MASTER_PROJECT)
  find_package(Boost REQUIRED)
  set_target_properties(Boost::boost PROPERTIES IMPORTED_GLOBAL TRUE)
endif()
# }}}

# Find Dependency Catch {{{
if (BWSL_TEST)
  message(STATUS "Compiling bundled CATCH")
  add_subdirectory(catch2)
endif()
# }}}

if (MASTER_PROJECT)
  message(STATUS "Adding bundled FMT")
  add_subdirectory(fmt EXCLUDE_FROM_ALL)
endif()

# vim: set ft=cmake ts=2 sts=2 et sw=2 tw=80 foldmarker={{{,}}} fdm=marker: #
