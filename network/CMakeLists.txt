cmake_minimum_required(VERSION 3.20)

project(network C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "-O3")

add_definitions(-DMAX_ROUND=6)
add_definitions(-DMAX_NEIGHBORS=6)
add_definitions(-DQUEUE_SIZE=32)
add_definitions(-DTIME_SLOT=100)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/netlib)

add_executable(${PROJECT_NAME}
        src/main.c
        src/netqueue.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        OUTPUT_NAME ${PROJECT_NAME})
target_include_directories(
        ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(
        ${PROJECT_NAME} PRIVATE netlib
)