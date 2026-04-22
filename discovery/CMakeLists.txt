cmake_minimum_required(VERSION 3.20)

project(discovery C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "-O3")

add_library(${PROJECT_NAME} SHARED
        src/discovery.c)

target_compile_definitions(${PROJECT_NAME} PUBLIC RECV_BUFFER_SIZE=16) #number of packets

set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        OUTPUT_NAME ${PROJECT_NAME})

target_include_directories(
        ${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PRIVATE autonet_types)
