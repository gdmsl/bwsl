# LatticeTest {{{
add_executable(LatticeTest LatticeTest.cpp)
target_link_libraries(
    LatticeTest
    bwsl ${libs_LIBRARIES}
)
add_test(NAME Lattice COMMAND $<TARGET_FILE:LatticeTest>)
# }}}

# PairsTest {{{
add_executable(PairsTest PairsTest.cpp)
target_link_libraries(
    PairsTest
    bwsl ${libs_LIBRARIES}
)
add_test(NAME Pairs COMMAND $<TARGET_FILE:PairsTest>)
# }}}

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
