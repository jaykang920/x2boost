# CMake build script for x2boost examples

cmake_minimum_required(VERSION 2.8.8) # for per-target include_directories

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

add_executable(headfirst2 ${headfirst_SRC} ${headfirst_INC}
  ${headfirst_SRC_DIR}/headfirst2.cpp
)
add_dependencies(headfirst2 headfirst_xpiled)

add_executable(headfirst3_client ${headfirst_SRC} ${headfirst_INC}
  ${headfirst_SRC_DIR}/headfirst3_client.cpp
)
add_dependencies(headfirst3_client headfirst_xpiled)

add_executable(headfirst3_server ${headfirst_SRC} ${headfirst_INC}
  ${headfirst_SRC_DIR}/headfirst3_server.cpp
)
add_dependencies(headfirst3_server headfirst_xpiled)

# examples common

set(example_TARGETS
  "headfirst1"
  "headfirst2"
  "headfirst3_client"
  "headfirst3_server"
)

foreach(target ${example_TARGETS})
  target_link_libraries(${target} x2boost ${Boost_LIBRARIES})

  set_target_properties(${target} PROPERTIES
    DEBUG_POSTFIX _d
    MINSIZEREL_POSTFIX _mr
    RELWITHDEBINFO_POSTFIX _rd
    COMPILE_DEFINITIONS "${headfirst_CDEFS}"
    RUNTIME_OUTPUT_DIRECTORY "${examples_BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${examples_BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${examples_BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${examples_BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${examples_BIN_DIR}"
  )
  set_property(TARGET ${target} APPEND PROPERTY
    INCLUDE_DIRECTORIES "${headfirst_INCLUDES}"
  )
endforeach()

# EOF CMakeLists.txt
