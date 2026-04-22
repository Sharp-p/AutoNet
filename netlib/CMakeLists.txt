cmake_minimum_required(VERSION 3.20)

project(netlib C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "-O3")

add_library(${PROJECT_NAME} SHARED
        src/netlib.c)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        INTERFACE_NAME="wlan0"
        ETHERTYPE=0x1234)

set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        OUTPUT_NAME ${PROJECT_NAME})
target_include_directories(
        ${PROJECT_NAME} PUBLIC include)