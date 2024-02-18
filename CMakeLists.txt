cmake_minimum_required(VERSION 3.27)
project(DLGG_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_subdirectory(dlgg)

add_executable(DLGG_cpp main.cpp
)
target_link_libraries(DLGG_cpp
        dlgg::game
        dlgg::agent
        dlgg::util
)