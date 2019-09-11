# =========================================================================== #
#                                   BWSL
# =========================================================================== #
#
# applications/CMAkeLists.txt
# Applications and examples included in the BWSL
#
# AUTHOR: Guido Masella (guido.masella@gmail.com)
# =========================================================================== #

# dumpmt {{{
add_executable(dumpmt dumpmt.cpp)
target_link_libraries(
    dumpmt
    bwsl::bwsl
    )
# }}}

# dumplattice {{{
add_executable(dumplattice dumplattice.cpp)
target_link_libraries(
    dumplattice
    bwsl::bwsl
    )
# }}}

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
