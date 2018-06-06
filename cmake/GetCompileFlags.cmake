function(get_compile_flags _compile_flags)
  # Create template akin to CMAKE_C_COMPILE_OBJECT.
  set(compile_flags "<CMAKE_CXX_COMPILER> <CXXFLAGS> <BUILD_TYPE_CFLAGS>")

  # Get CXX compiler.
  string(REPLACE
    "<CMAKE_CXX_COMPILER>"
    "${CMAKE_CXX_COMPILER}"
    compile_flags
    "${compile_flags}")

  # Get general CXX flags.
  string(REPLACE
    "<CXXFLAGS>"
    "${CMAKE_CXX_FLAGS}"
    compile_flags
    "${compile_flags}")

  # Get CXX flags specific to build type.
  string(TOUPPER "${CMAKE_BUILD_TYPE}" build_type)
  string(REPLACE
    "<BUILD_TYPE_CFLAGS>"
    "${CMAKE_CXX_FLAGS_${build_type}}"
    compile_flags
    "${compile_flags}")

  # Clean duplicate whitespace.
  string(REPLACE
    "  "
    " "
    compile_flags
    "${compile_flags}")

  set(${_compile_flags} "${compile_flags}" PARENT_SCOPE)
endfunction()
