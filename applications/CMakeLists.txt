#== CMakeLists.txt ---------------------------------------------------------==#
#
#                       BeagleWarlord's Support Library
#
# Copyright 2016-2022 Guido Masella. All Rights Reserved.
# See LICENSE file for details.
#
#==------------------------------------------------------------------------==#
#
# Guido Masella (guido.masella@gmail.com)
#
#==------------------------------------------------------------------------==#

# dumpmt {{{
add_executable(dumpmt dumpmt.cpp)
target_link_libraries(
    dumpmt
    bwsl::bwsl
    )
# }}}

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
