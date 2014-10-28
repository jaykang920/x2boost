# CMake build script for x2boost

cmake_minimum_required(VERSION 2.8.8) # for per-target include_directories

# CMake options
option(BUILD_SHARED_LIBS "Build x2boost as a shared library (DLL)." OFF)
# x2boost options
option(BUILD_FORCE_SHARED_CRT "Shared (DLL) version of C runtime library." OFF)
option(BUILD_XPILER "Build xpiler executable." ON)

if(WIN32)
    SET(CMAKE_USE_RELATIVE_PATHS true)
endif()

# Project module path
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/cmake")
include(build/cmake/internal_utils.cmake)

project(x2boost CXX C)

set(CMAKE_THREAD_PREFER_PTHREAD true)
find_package(Threads REQUIRED)

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
unset(Boost_LIBRARIES CACHE)
find_package(Boost REQUIRED COMPONENTS
  filesystem
  program_options
  system
  thread
)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

if(BUILD_SHARED_LIBS)
  string_append(x2boost_CDEFS "X2_AS_SHARED_LIB" ";")
  if (MSVC)
    # Suppress warnings about DLL interface.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251 /wd4275")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4251 /wd4275")
  endif()
endif()

if(MSVC)
  # Prefer statically linked runtime library.
  foreach(flag_var
          CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
          CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
          CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
          CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    if(NOT BUILD_SHARED_LIBS AND NOT BUILD_FORCE_SHARED_CRT)
      string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif()
  endforeach()
  # Prefer warning level 4.
  foreach(flag_var
          CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
          CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    string(REPLACE "/W3" "/W4" ${flag_var} "${${flag_var}}")
  endforeach()

  # Suppress uninteresting warnings.
  add_definitions(/D_CRT_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_DEPRECATE)
  add_definitions(/D_CRT_NONSTDC_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE)
endif()

set(x2boost_LIB_DIR ${x2boost_SOURCE_DIR}/lib)
set(x2boost_BIN_DIR ${x2boost_SOURCE_DIR}/bin)

set(CMAKE_DEBUG_POSTFIX _d)
set(CMAKE_MINSIZEREL_POSTFIX _mr)
set(CMAKE_RELWITHDEBINFO_POSTFIX _rd)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${x2boost_LIB_DIR})
foreach(output_config ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${output_config} OUTPUTCONFIG)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${x2boost_LIB_DIR})
endforeach()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${x2boost_BIN_DIR})
foreach(output_config ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${output_config} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${x2boost_BIN_DIR})
endforeach()

set(x2boost_INCLUDE_DIR "${x2boost_SOURCE_DIR}/include")
string_append(x2boost_INCLUDES "${x2boost_INCLUDE_DIR}" ";")
string_append(x2boost_INCLUDES "${x2boost_SOURCE_DIR}" ";")

# x2boost header files

set(x2boost_INC_DIR "${x2boost_INCLUDE_DIR}/x2boost")

set(x2boost_INC
  "${x2boost_INC_DIR}/binder.hpp"
  "${x2boost_INC_DIR}/case.hpp"
  "${x2boost_INC_DIR}/cell.hpp"
  "${x2boost_INC_DIR}/event.hpp"
  "${x2boost_INC_DIR}/event_sink.hpp"
  "${x2boost_INC_DIR}/fingerprint.hpp"
  "${x2boost_INC_DIR}/flow.hpp"
  "${x2boost_INC_DIR}/handler.hpp"
  "${x2boost_INC_DIR}/hub.hpp"
  "${x2boost_INC_DIR}/link.hpp"
  "${x2boost_INC_DIR}/pre.hpp"
  "${x2boost_INC_DIR}/queue.hpp"
  "${x2boost_INC_DIR}/x2boost.hpp"
)
set(x2boost_INC_FLOWS
  "${x2boost_INC_DIR}/flows/event_based_flow.hpp"
  "${x2boost_INC_DIR}/flows/multi_threaded_flow.hpp"
  "${x2boost_INC_DIR}/flows/single_threaded_flow.hpp"
)
set(x2boost_INC_ASIO_LINKS
  "${x2boost_INC_DIR}/links/asio_link/asio_client.hpp"
  "${x2boost_INC_DIR}/links/asio_link/asio_link.hpp"
  "${x2boost_INC_DIR}/links/asio_link/asio_server.hpp"
)
set(x2boost_INC_QUEUES
  "${x2boost_INC_DIR}/queues/lockfree_queue.hpp"
  "${x2boost_INC_DIR}/queues/synchronized_queue.hpp"
)
set(x2boost_INC_UTIL
  "${x2boost_INC_DIR}/util/deleter.hpp"
  "${x2boost_INC_DIR}/util/log.hpp"
  "${x2boost_INC_DIR}/util/shared_object.hpp"
)

list(APPEND x2boost_ALL_INC ${x2boost_INC})
list(APPEND x2boost_ALL_INC ${x2boost_INC_FLOWS})
list(APPEND x2boost_ALL_INC ${x2boost_INC_ASIO_LINKS})
list(APPEND x2boost_ALL_INC ${x2boost_INC_QUEUES})
list(APPEND x2boost_ALL_INC ${x2boost_INC_UTIL})

# x2boost source files

set(x2boost_SRC_DIR "${x2boost_SOURCE_DIR}/src")

set(x2boost_SRC
  "${x2boost_SRC_DIR}/binder.cpp"
  "${x2boost_SRC_DIR}/case.cpp"
  "${x2boost_SRC_DIR}/cell.cpp"
  "${x2boost_SRC_DIR}/event.cpp"
  "${x2boost_SRC_DIR}/fingerprint.cpp"
  "${x2boost_SRC_DIR}/flow.cpp"
  "${x2boost_SRC_DIR}/hub.cpp"
  "${x2boost_SRC_DIR}/main.cpp"
)
set(x2boost_SRC_FLOWS
  "${x2boost_SRC_DIR}/flows/event_based_flow.cpp"
  "${x2boost_SRC_DIR}/flows/single_threaded_flow.cpp"
)

list(APPEND x2boost_ALL_SRC ${x2boost_SRC})
list(APPEND x2boost_ALL_SRC ${x2boost_SRC_FLOWS})

# Source groups
source_group("Header Files" FILES ${x2boost_INC})
source_group("Header Files\\flows" FILES ${x2boost_INC_FLOWS})
source_group("Header Files\\links")
source_group("Header Files\\links\\asio_link" FILES ${x2boost_INC_ASIO_LINKS})
source_group("Header Files\\queues" FILES ${x2boost_INC_QUEUES})
source_group("Header Files\\util" FILES ${x2boost_INC_UTIL})
source_group("Source Files" FILES ${x2boost_SRC})
source_group("Source Files\\flows" FILES ${x2boost_SRC_FLOWS})

add_library(x2boost ${x2boost_ALL_SRC} ${x2boost_ALL_INC})
set_target_properties(x2boost PROPERTIES
  COMPILE_DEFINITIONS "${x2boost_CDEFS}"
)
set_property(TARGET x2boost APPEND PROPERTY
  INCLUDE_DIRECTORIES "${x2boost_INCLUDES}"
)
if(BUILD_SHARED_LIBS)
  set_target_properties(x2boost PROPERTIES DEFINE_SYMBOL X2BOOST_EXPORTS)
endif()

if(BUILD_XPILER)
  set(xpiler_INCLUDES "${x2boost_INCLUDES}")
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
  set_property(TARGET xpiler APPEND PROPERTY
    INCLUDE_DIRECTORIES "${xpiler_INCLUDES}"
  )
endif()

# EOF CMakeLists.txt
