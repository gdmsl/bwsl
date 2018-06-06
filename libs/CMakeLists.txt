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

# Find Dependency pcg-cpp {{{
#add_subdirectory(pcg-cpp)
# }}}

# Find Dependency yaml-cpp {{{
#find_package(yaml-cpp)
#if (NOT yaml-cpp_FOUND)
  #message(STATUS "Compiling YAML-CPP")
  #add_subdirectory(yaml-cpp)
#endif()
# }}}

# Find Dependency fmt {{{
#find_package(fmt)
#if (NOT FMT_FOUND)
  #message(STATUS "Using bundled fmt lib")
  #add_subdirectory(fmt)
#endif()
#}}}

# Boost {{{
#find_package(Boost REQUIRED COMPONENTS program_options filesystem)
#set_target_properties(Boost::program_options PROPERTIES IMPORTED_GLOBAL TRUE)
#set_target_properties(Boost::filesystem PROPERTIES IMPORTED_GLOBAL TRUE)
# }}}

# vim: set ft=cmake ts=2 sts=2 et sw=2 tw=80 foldmarker={{{,}}} fdm=marker: #
