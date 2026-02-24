cmake_minimum_required(VERSION 3.29)
project(Test C)

set(CMAKE_C_STANDARD 23)
#set(CMAKE_C_FLAGS "-O3")


add_compile_options(-Wall)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/network)

add_executable(${PROJECT_NAME}
        src/main.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
        OUTPUT_NAME test)

target_link_libraries(${PROJECT_NAME} PRIVATE AutoNet)