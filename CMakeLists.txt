cmake_minimum_required(VERSION 3.12)

project(AutoNet)

set(CMAKE_C_STANDARD 17)

add_compile_options(-Wall)

add_executable(distance src/distance.c)

target_link_libraries(distance PRIVATE pthread pigpio rt)
