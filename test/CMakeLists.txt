# add compiler options to build
add_compile_options(-W -Wpedantic -Wall -Wextra)

# LatticeTest {{{
add_executable(LatticeTest LatticeTest.cpp)
target_link_libraries(LatticeTest
  PRIVATE
    bwsl
    catch::catch
  )
add_test(NAME bwsl.Lattice COMMAND $<TARGET_FILE:LatticeTest>)
# }}}

# PairsTest {{{
add_executable(PairsTest PairsTest.cpp)
target_link_libraries(PairsTest
  PRIVATE
    bwsl
    catch::catch
  )
add_test(NAME bwsl.Pairs COMMAND $<TARGET_FILE:PairsTest>)
# }}}

# vim: set ft=cmake ts=2 sts=2 et sw=2 tw=80 foldmarker={{{,}}} fdm=marker: #
