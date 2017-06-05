# Lattice {{{
add_executable(LatticeTest tests/LatticeTest.cpp)
target_link_libraries(
    LatticeTest
    bwsl
)
add_test(Lattice bin/LatticeTest)
# }}}

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
