cmake_minimum_required(VERSION 3.3)
project(Sound)

set(CMAKE_CXX_STANDARD 14)
find_package(SDL2 REQUIRED)

add_executable(Sound
        src/sound.cpp)

target_link_libraries(Sound
        PRIVATE SDL2::SDL2)
