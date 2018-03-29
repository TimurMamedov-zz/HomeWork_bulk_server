cmake_minimum_required(VERSION 3.5)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(bulk_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(bulk_server VERSION 0.0.1)
endif()

add_subdirectory(CommandsStorage)

if(WIN32)
    set(BOOST_ROOT          "C:/boost/boost_1_62_0/boost_mingw53")
    set(Boost_USE_STATIC_LIBS        ON)
    set(Boost_USE_MULTITHREADED ON)
endif()

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads)

aux_source_directory(. SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST} bulk_server.h bulk_session.h)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
#        COMPILE_DEFINITIONS BOOST_ASIO_ENABLE_HANDLER_TRACKING
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
        )

set_target_properties (${PROJECT_NAME} PROPERTIES
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
        )

if(WIN32)
        target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif()

target_link_libraries(${PROJECT_NAME}
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        CommandsStorage
        )
