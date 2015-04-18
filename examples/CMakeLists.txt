# CMake build script for examples

cmake_minimum_required(VERSION 2.8.8) # for per-target include_directories

# project x2boost

set(examples_BIN_DIR "${x2boost_SOURCE_DIR}/examples/bin")

# headfirst

set(headfirst_CDEFS "${x2boost_CDEFS}")
set(headfirst_INCLUDES "${x2boost_INCLUDES}")

set(headfirst_SRC_DIR "${x2boost_SOURCE_DIR}/examples/headfirst")

add_custom_target(headfirst_xpiled
  xpiler ${headfirst_SRC_DIR}
)

set(headfirst_INC
  "${headfirst_SRC_DIR}/head_first.hpp"
  "${headfirst_SRC_DIR}/headfirst_cases.hpp"
)

set(headfirst_SRC
  "${headfirst_SRC_DIR}/head_first.cpp"
  "${headfirst_SRC_DIR}/HeadFirst.xml"
)

add_executable(headfirst1 ${headfirst_SRC} ${headfirst_INC}
  ${headfirst_SRC_DIR}/headfirst1.cpp
)
add_dependencies(headfirst1 headfirst_xpiled)
target_link_libraries(headfirst1 ${Boost_LIBRARIES} x2boost)

set_target_properties(headfirst1 PROPERTIES
  DEBUG_POSTFIX _d
  MINSIZEREL_POSTFIX _mr
  RELWITHDEBINFO_POSTFIX _rd
  COMPILE_DEFINITIONS "${headfirst_CDEFS}"
  RUNTIME_OUTPUT_DIRECTORY "${examples_BIN_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${examples_BIN_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${examples_BIN_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${examples_BIN_DIR}"
)
set_property(TARGET headfirst1 APPEND PROPERTY
  INCLUDE_DIRECTORIES "${headfirst_INCLUDES}"
)

# EOF CMakeLists.txt
