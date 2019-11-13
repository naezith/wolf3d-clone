cmake_minimum_required(VERSION 3.15)
project(wolf3d)

set(CMAKE_CXX_STANDARD 17)
find_package(SFML 2 REQUIRED network audio graphics window system)
add_executable(wolf3d main.cpp)
target_link_libraries(wolf3d sfml-graphics sfml-audio sfml-window)
set_target_properties(wolf3d PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

IF(NOT CMAKE_BUILD_TYPE MATCHES DEBUG)
    target_compile_options(wolf3d PUBLIC -Wall -Wextra -Ofast -march=native -ffast-math)
endif()