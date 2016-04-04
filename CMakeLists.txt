cmake_minimum_required (VERSION 2.6)
project (TARGA)

include_directories (.)
add_library (targa targa.c targa.h)


# Tests
add_executable (targa_test
  targa_test.c
  targa.c
  targa.h)

target_link_libraries (targa_test targa)

enable_testing()
add_test (NAME Load1         COMMAND targa_test "hello")

# doc
find_package (Doxygen)

if (DOXYGEN_FOUND)

  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

  add_custom_target (targa_doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM)

endif (DOXYGEN_FOUND)
