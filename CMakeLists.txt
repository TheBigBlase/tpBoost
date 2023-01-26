cmake_minimum_required(VERSION 3.25)
project(tpboost)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(main
        src/utils.cpp
        src/main.cpp
        )

target_link_libraries(main ${Boost_LIBRARIES})
