cmake_minimum_required(VERSION 3.10)
project(ZooERP)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(Zoo
    src/animals.cpp
    src/zoo.cpp
    src/di_container.cpp
    main.cpp
)