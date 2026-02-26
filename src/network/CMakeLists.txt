cmake_minimum_required(VERSION 3.20)

project(NetLib C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "-O3")

add_definitions(-DINTERFACE_NAME="wlan0")
add_definitions(-DETHERTYPE=0x1234)

add_library(${PROJECT_NAME} STATIC
        src/netlib.c)

set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
        OUTPUT_NAME ${PROJECT_NAME})
target_include_directories(
        ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/include)