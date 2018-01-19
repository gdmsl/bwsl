# dumpmt {{{
add_executable(dumpmt dumpmt.cpp)
target_link_libraries(
    dumpmt
    bwsl ${libs_LIBRARIES}
)
# }}}

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
