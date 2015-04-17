# CMake build script for xpiler

cmake_minimum_required(VERSION 2.8.8) # for per-target include_directories

# project x2boost

set(xpiler_CDEFS "${x2boost_CDEFS}")

set(xpiler_SRC_DIR "${x2boost_SOURCE_DIR}/xpiler")

set(xpiler_INC
  "${xpiler_SRC_DIR}/boost_formatter.hpp"
  "${xpiler_SRC_DIR}/definition.hpp"
  "${xpiler_SRC_DIR}/document.hpp"
  "${xpiler_SRC_DIR}/handler.hpp"
  "${xpiler_SRC_DIR}/options.hpp"
  "${xpiler_SRC_DIR}/formatter.hpp"
  "${xpiler_SRC_DIR}/pre.hpp"
  "${xpiler_SRC_DIR}/types.hpp"
  "${xpiler_SRC_DIR}/xml_handler.hpp"
  "${xpiler_SRC_DIR}/xpiler.hpp"
)

set(xpiler_SRC
  "${xpiler_SRC_DIR}/boost_formatter.cpp"
  "${xpiler_SRC_DIR}/main.cpp"
  "${xpiler_SRC_DIR}/options.cpp"
  "${xpiler_SRC_DIR}/types.cpp"
  "${xpiler_SRC_DIR}/xml_handler.cpp"
  "${xpiler_SRC_DIR}/xpiler.cpp"
)

add_executable(xpiler ${xpiler_SRC} ${xpiler_INC})
target_link_libraries(xpiler ${Boost_LIBRARIES})

set_target_properties(xpiler PROPERTIES
  DEBUG_POSTFIX _d
  MINSIZEREL_POSTFIX _mr
  RELWITHDEBINFO_POSTFIX _rd
  COMPILE_DEFINITIONS "${xpiler_CDEFS}"
)

# EOF CMakeLists.txt
