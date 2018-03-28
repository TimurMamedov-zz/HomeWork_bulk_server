cmake_minimum_required(VERSION 3.9)

find_package(Threads)

set(HEADERS commands_storage.h threadsafe_queue.h solver.h solvers.h exeption_class.h)
set(SOURCES commands_storage.cpp solvers.cpp)

add_library(async STATIC
    ${HEADERS}
    ${SOURCES}
)

set_target_properties (async PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_link_libraries(async
  ${CMAKE_THREAD_LIBS_INIT}
  )

target_include_directories(
    async
    INTERFACE .
    )
