# CMake build script

cmake_minimum_required(VERSION 2.8.8) # for per-target include_directories

project(x2boost CXX C)

# CMake options
option(BUILD_SHARED_LIBS "Build x2boost as a shared library (DLL)" OFF)
# x2boost options
option(BUILD_FORCE_SHARED_CRT "Shared (DLL) version of C runtime library" OFF)
option(BUILD_XPILER "Build xpiler executable" ON)
option(BUILD_TEST "Build unit tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

if(WIN32)
  SET(CMAKE_USE_RELATIVE_PATHS true)
endif()

set(CMAKE_THREAD_PREFER_PTHREAD true)
find_package(Threads REQUIRED)

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
unset(Boost_LIBRARIES CACHE)
find_package(Boost REQUIRED COMPONENTS
  atomic
  chrono
  filesystem
  program_options
  regex
  thread
  unit_test_framework
)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

if(BUILD_SHARED_LIBS)
  list(APPEND x2boost_CDEFS "X2_AS_SHARED_LIB")
  if (MSVC)
    # Suppress warnings about DLL interface.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251 /wd4275")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4251 /wd4275")
  endif()
endif()

if(MSVC)
  # Prefer statically linked runtime library.
  foreach(flags
          CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
          CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
          CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
          CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    if(NOT BUILD_SHARED_LIBS AND NOT BUILD_FORCE_SHARED_CRT)
      string(REPLACE "/MD" "/MT" ${flags} "${${flags}}")
    endif()
  endforeach()
  # Prefer warning level 4.
  #foreach(flags
  #        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
  #        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
  #  string(REPLACE "/W3" "/W4" ${flags} "${${flags}}")
  #endforeach()

  # Suppress uninteresting warnings.
  add_definitions(/D_CRT_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_DEPRECATE)
  add_definitions(/D_CRT_NONSTDC_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE)
elseif(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")
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
set(x2boost_INCLUDES ${x2boost_INCLUDE_DIR})

add_subdirectory(src)

if(BUILD_XPILER)
  add_subdirectory(xpiler)
endif()

if(BUILD_TEST)
  add_subdirectory(test)
endif()

if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# EOF CMakeLists.txt
