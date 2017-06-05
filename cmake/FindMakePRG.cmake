# make or gmake
find_program(MAKE_PRG NAMES gmake make)
if(MAKE_PRG)
    execute_process(
        COMMAND "${MAKE_PRG}" --version
        OUTPUT_VARIABLE MAKE_VERSION_INFO)
    if(NOT "${OUTPUT_VARIABLE}" MATCHES ".*GNU.*")
        unset(MAKE_PRG)
    endif()
endif()
if(NOT MAKE_PRG)
    message(FATAL_ERROR "GNU Make is required to build the dependencies.")
else()
    message(STATUS "Found GNU Make at ${MAKE_PRG}")
endif()

# vim: set ft=cmake ts=4 sts=4 et sw=4 tw=80 foldmarker={{{,}}} fdm=marker: #
