cmake_minimun_required(VERSION 3.25.1)

project(AutoNet)

set(CMAKE_C_STANDARD 17)

add_compile_options(-Wall)

add_executable(distance src/distance.c)

target_link_libraries(src/distance.c PRIVATE pthread pigpio rt)
