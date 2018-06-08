#
#                                   BWSL
#
# libs/CMAkeLists.txt
# Third party libraries required by SWMC
#
# Guido Masella (guido.masella@gmail.com)
#

# Find Dependency Catch {{{
find_package(Catch)
if (NOT Catch_FOUND)
  message(STATUS "Compiling CATCH")
  add_subdirectory(catch2)
endif()
# }}}

# Find Dependency fmt {{{
#find_package(fmt)
#if (NOT FMT_FOUND)
  message(STATUS "Using bundled fmt")
  add_subdirectory(fmt EXCLUDE_FROM_ALL)
#endif()
#}}}

# Boost {{{
if(NOT ${CMAKE_VERSION} VERSION_LESS "3.11.0")
  find_package(Boost REQUIRED COMPONENTS filesystem)
  set_target_properties(Boost::filesystem PROPERTIES IMPORTED_GLOBAL TRUE)
endif()
# }}}

# vim: set ft=cmake ts=2 sts=2 et sw=2 tw=80 foldmarker={{{,}}} fdm=marker: #
