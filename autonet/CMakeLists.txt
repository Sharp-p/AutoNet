cmake_minimum_required(VERSION 3.20)

project(autonet C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "-O3")


add_library(autonet_types INTERFACE)
target_include_directories(autonet_types INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/shared/include)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/netlib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/discovery)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/clock_sync)

add_executable(${PROJECT_NAME}
        src/main.c)

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        OUTPUT_NAME ${PROJECT_NAME})

target_include_directories(
        ${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME}
        PRIVATE autonet_types netlib discovery clock_sync)
