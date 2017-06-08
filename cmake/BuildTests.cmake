# Lattice {{{
add_executable(LatticeTest tests/LatticeTest.cpp)
target_link_libraries(
    LatticeTest
    bwsl
)
add_test(Lattice bin/LatticeTest)
# }}}

# Pairs{{{
add_executable(PairsTest tests/PairsTest.cpp)
target_link_libraries(
    PairsTest
    bwsl
)
add_test(Pairs bin/PairsTest)
# }}}

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
