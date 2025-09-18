cmake_minimum_required(VERSION 3.12)
project(AutoNet)

set(CMAKE_C_STANDARD 17)


add_compile_definitions(Debug)
add_compile_options(-Wall -pthread -lpigpio -lrt)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/BNet)

find_package(Threads REQUIRED)


add_executable(AutoNet
        src/main.c
        src/networkMan.c
        src/ultrasw.c
        src/ultrawb.c)

add_executable(distanceSensor
        src/distance.c
)

target_include_directories(AutoNet PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(AutoNet PRIVATE Threads::Threads)

set_target_properties(AutoNet PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

target_include_directories(distanceSensor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(distanceSensor PRIVATE Threads::Threads)

set_target_properties(distanceSensor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
