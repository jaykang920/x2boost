# CMake build script for x2boost

cmake_minimum_required(VERSION 2.8.8) # for per-target include_directories

# x2boost header files

set(x2boost_INC_DIR "${x2boost_INCLUDE_DIR}/x2boost")

set(x2boost_INC
  "${x2boost_INC_DIR}/binder.hpp"
  "${x2boost_INC_DIR}/buffer.hpp"
  "${x2boost_INC_DIR}/case.hpp"
  "${x2boost_INC_DIR}/cell.hpp"
  "${x2boost_INC_DIR}/deserializer.hpp"
  "${x2boost_INC_DIR}/event.hpp"
  "${x2boost_INC_DIR}/event_factory.hpp"
  "${x2boost_INC_DIR}/event_queue.hpp"
  "${x2boost_INC_DIR}/event_sink.hpp"
  "${x2boost_INC_DIR}/fingerprint.hpp"
  "${x2boost_INC_DIR}/flow.hpp"
  "${x2boost_INC_DIR}/handler.hpp"
  "${x2boost_INC_DIR}/hub.hpp"
  "${x2boost_INC_DIR}/link.hpp"
  "${x2boost_INC_DIR}/pre.hpp"
  "${x2boost_INC_DIR}/serializer.hpp"
  "${x2boost_INC_DIR}/x2boost.hpp"
)
set(x2boost_INC_FLOWS
  "${x2boost_INC_DIR}/flows/event_based_flow.hpp"
  "${x2boost_INC_DIR}/flows/multi_threaded_flow.hpp"
  "${x2boost_INC_DIR}/flows/single_threaded_flow.hpp"
)
set(x2boost_INC_LINKS
  "${x2boost_INC_DIR}/links/asio_link.hpp"
  "${x2boost_INC_DIR}/links/asio_tcp_client.hpp"
  "${x2boost_INC_DIR}/links/asio_tcp_link.hpp"
  "${x2boost_INC_DIR}/links/asio_tcp_link_session.hpp"
  "${x2boost_INC_DIR}/links/asio_tcp_server.hpp"
)
set(x2boost_INC_QUEUES
  "${x2boost_INC_DIR}/queues/synchronized_event_queue.hpp"
)
set(x2boost_INC_UTIL
  "${x2boost_INC_DIR}/util/buffer_pool.hpp"
  "${x2boost_INC_DIR}/util/deleter.hpp"
  "${x2boost_INC_DIR}/util/ranged_int_pool.hpp"
  "${x2boost_INC_DIR}/util/ref_counted.hpp"
  "${x2boost_INC_DIR}/util/string.hpp"
  "${x2boost_INC_DIR}/util/trace.hpp"
)

list(APPEND x2boost_ALL_INC ${x2boost_INC})
list(APPEND x2boost_ALL_INC ${x2boost_INC_FLOWS})
list(APPEND x2boost_ALL_INC ${x2boost_INC_LINKS})
list(APPEND x2boost_ALL_INC ${x2boost_INC_QUEUES})
list(APPEND x2boost_ALL_INC ${x2boost_INC_UTIL})

# x2boost source files

set(x2boost_SRC_DIR "${x2boost_SOURCE_DIR}/src")

set(x2boost_SRC
  "${x2boost_SRC_DIR}/binder.cpp"
  "${x2boost_SRC_DIR}/buffer.cpp"
  "${x2boost_SRC_DIR}/case.cpp"
  "${x2boost_SRC_DIR}/cell.cpp"
  "${x2boost_SRC_DIR}/deserializer.cpp"
  "${x2boost_SRC_DIR}/event.cpp"
  "${x2boost_SRC_DIR}/event_factory.cpp"
  "${x2boost_SRC_DIR}/fingerprint.cpp"
  "${x2boost_SRC_DIR}/flow.cpp"
  "${x2boost_SRC_DIR}/hub.cpp"
  "${x2boost_SRC_DIR}/link.cpp"
  "${x2boost_SRC_DIR}/main.cpp"
  "${x2boost_SRC_DIR}/serializer.cpp"
)
set(x2boost_SRC_FLOWS
  "${x2boost_SRC_DIR}/flows/event_based_flow.cpp"
  "${x2boost_SRC_DIR}/flows/single_threaded_flow.cpp"
)
set(x2boost_SRC_LINKS
  "${x2boost_SRC_DIR}/links/asio_link.cpp"
)
set(x2boost_SRC_UTIL
  "${x2boost_SRC_DIR}/util/buffer_pool.cpp"
  "${x2boost_SRC_DIR}/util/trace.cpp"
)

list(APPEND x2boost_ALL_SRC ${x2boost_SRC})
list(APPEND x2boost_ALL_SRC ${x2boost_SRC_FLOWS})
list(APPEND x2boost_ALL_SRC ${x2boost_SRC_LINKS})
list(APPEND x2boost_ALL_SRC ${x2boost_SRC_UTIL})

# Source groups
source_group("Header Files" FILES ${x2boost_INC})
source_group("Header Files\\flows" FILES ${x2boost_INC_FLOWS})
source_group("Header Files\\links" FILES ${x2boost_INC_LINKS})
source_group("Header Files\\queues" FILES ${x2boost_INC_QUEUES})
source_group("Header Files\\util" FILES ${x2boost_INC_UTIL})
source_group("Source Files" FILES ${x2boost_SRC})
source_group("Source Files\\flows" FILES ${x2boost_SRC_FLOWS})
source_group("Source Files\\links")
source_group("Source Files\\links" FILES ${x2boost_SRC_LINKS})
source_group("Source Files\\util" FILES ${x2boost_SRC_UTIL})

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

# EOF CMakeLists.txt
