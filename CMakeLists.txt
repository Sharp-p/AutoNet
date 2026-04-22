cmake_minimum_required(VERSION 3.20)

project(autonet C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "-O3")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-O3")


add_library(autonet_types INTERFACE)
target_include_directories(autonet_types INTERFACE
        shared/include)

add_subdirectory(netlib)
add_subdirectory(discovery)
add_subdirectory(clock_sync)
add_subdirectory(lidar_data)

add_executable(${PROJECT_NAME}
        src/main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        OUTPUT_NAME ${PROJECT_NAME})

target_include_directories(
        ${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME}
        PRIVATE autonet_types netlib discovery clock_sync lidar_data)
