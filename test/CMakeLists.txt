# CMake build script for x2boost unit test

cmake_minimum_required(VERSION 2.8.8) # for per-target include_directories

set(x2test_BIN_DIR "${x2boost_SOURCE_DIR}/test/bin")

set(x2test_CDEFS "${x2boost_CDEFS}")
set(x2test_INCLUDES "${x2boost_INCLUDES}")

set(x2test_SRC_DIR "${x2boost_SOURCE_DIR}/test")

set(x2test_INC
)

set(x2test_SRC
  "${x2test_SRC_DIR}/buffer_pool_test.cpp"
  "${x2test_SRC_DIR}/buffer_test.cpp"
  "${x2test_SRC_DIR}/ranged_int_pool_test.cpp"

  "${x2test_SRC_DIR}/main.cpp"
)

add_executable(x2test ${x2test_SRC} ${x2test_INC})
target_link_libraries(x2test x2boost ${Boost_LIBRARIES})

set_target_properties(x2test PROPERTIES
  DEBUG_POSTFIX _d
  MINSIZEREL_POSTFIX _mr
  RELWITHDEBINFO_POSTFIX _rd
  COMPILE_DEFINITIONS "${x2test_CDEFS}"
  RUNTIME_OUTPUT_DIRECTORY "${x2test_BIN_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${x2test_BIN_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${x2test_BIN_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${x2test_BIN_DIR}"
)
set_property(TARGET x2test APPEND PROPERTY
  INCLUDE_DIRECTORIES "${x2test_INCLUDES}"
)

# EOF CMakeLists.txt
