cmake_minimum_required(VERSION 3.12)

project(TokenRingEmulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(headers)

set(SOURCES
    main.cpp
    impl/base_agent.cpp
    impl/data.cpp
    impl/echo_agent.cpp
    impl/emulator.cpp
    impl/main_agent.cpp
    impl/simple_agent.cpp
)

add_executable(TokenRing ${SOURCES})