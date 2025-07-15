cmake_minimum_required(VERSION 3.12)
project(AutoNet)

set(CMAKE_C_STANDARD 17)


add_compile_definitions(Debug)
add_compile_options(-Wall)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/BNet)

find_package(Threads REQUIRED)


add_executable(AutoNet
        src/main.c
        src/networkMan.c
        src/ultrasw.c
        src/ultrawb.c)

target_include_directories(AutoNet PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(AutoNet PRIVATE Threads::Threads BNet)

set_target_properties(AutoNet PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
