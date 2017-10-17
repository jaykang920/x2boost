# CMake build script for x2boost samples

cmake_minimum_required(VERSION 2.8.8) # for per-target include_directories

set(samples_BIN_DIR "${x2boost_SOURCE_DIR}/samples/bin")

# helloworld

set(helloworld_CDEFS "${x2boost_CDEFS}")
set(helloworld_INCLUDES "${x2boost_INCLUDES}")

set(helloworld_SRC_DIR "${x2boost_SOURCE_DIR}/samples/helloworld")

add_custom_target(helloworld_xpiled
  xpiler ${helloworld_SRC_DIR}
)

set(helloworld_INC
  "${helloworld_SRC_DIR}/hello_world.hpp"
  "${helloworld_SRC_DIR}/helloworld_cases.hpp"
)

set(helloworld_SRC
  "${helloworld_SRC_DIR}/hello_world.cpp"
  "${helloworld_SRC_DIR}/HelloWorld.xml"
)

add_executable(helloworld1 ${helloworld_SRC} ${helloworld_INC}
  ${helloworld_SRC_DIR}/helloworld1.cpp
)
add_dependencies(helloworld1 helloworld_xpiled)

add_executable(helloworld2 ${helloworld_SRC} ${helloworld_INC}
  ${helloworld_SRC_DIR}/helloworld2.cpp
)
add_dependencies(helloworld2 helloworld_xpiled)

add_executable(helloworld3_client ${helloworld_SRC} ${helloworld_INC}
  ${helloworld_SRC_DIR}/helloworld3_client.cpp
)
add_dependencies(helloworld3_client helloworld_xpiled)

add_executable(helloworld3_server ${helloworld_SRC} ${helloworld_INC}
  ${helloworld_SRC_DIR}/helloworld3_server.cpp
)
add_dependencies(helloworld3_server helloworld_xpiled)

# samples common

set(sample_TARGETS
  "helloworld1"
  "helloworld2"
  "helloworld3_client"
  "helloworld3_server"
)

foreach(target ${sample_TARGETS})
  target_link_libraries(${target} x2boost ${Boost_LIBRARIES})

  set_target_properties(${target} PROPERTIES
    DEBUG_POSTFIX _d
    MINSIZEREL_POSTFIX _mr
    RELWITHDEBINFO_POSTFIX _rd
    COMPILE_DEFINITIONS "${helloworld_CDEFS}"
    RUNTIME_OUTPUT_DIRECTORY "${samples_BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${samples_BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${samples_BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${samples_BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${samples_BIN_DIR}"
  )
  set_property(TARGET ${target} APPEND PROPERTY
    INCLUDE_DIRECTORIES "${helloworld_INCLUDES}"
  )
endforeach()

# EOF CMakeLists.txt
