cmake_minimum_required(VERSION 3.29)
project(AutoNet C)

set(CMAKE_C_STANDARD 23)
#set(CMAKE_C_FLAGS "-O3")


add_compile_options(-Wall)

add_library(${PROJECT_NAME} STATIC
        src/main.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
        OUTPUT_NAME ${PROJECT_NAME})

target_include_directories(
        ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/include)