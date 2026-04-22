cmake_minimum_required(VERSION 3.20)

project(lidar_data CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-O3")

include(FetchContent)

FetchContent_Declare(
        rplidar_sdk_src
        GIT_REPOSITORY https://github.com/Slamtec/rplidar_sdk.git
        GIT_TAG        bbf8b415488db06175bfad24ddc2465d7f78fbf0 # snapshot v2.1.0
)

FetchContent_MakeAvailable(rplidar_sdk_src)

set(RPLIDAR_SDK_ROOT "${rplidar_sdk_src_SOURCE_DIR}/sdk")

file(GLOB RPLIDAR_DATAUNPACKER_SOURCES
        "${RPLIDAR_SDK_ROOT}/src/dataunpacker/*.cpp"
        "${RPLIDAR_SDK_ROOT}/src/dataunpacker/unpacker/*.cpp"
)

set(RPLIDAR_SOURCES
        "${RPLIDAR_SDK_ROOT}/src/sl_lidar_driver.cpp"
        "${RPLIDAR_SDK_ROOT}/src/hal/thread.cpp"
        "${RPLIDAR_SDK_ROOT}/src/sl_crc.cpp"
        "${RPLIDAR_SDK_ROOT}/src/sl_serial_channel.cpp"
        "${RPLIDAR_SDK_ROOT}/src/sl_lidarprotocol_codec.cpp"
        "${RPLIDAR_SDK_ROOT}/src/sl_async_transceiver.cpp"
        "${RPLIDAR_SDK_ROOT}/src/sl_tcp_channel.cpp"
        "${RPLIDAR_SDK_ROOT}/src/sl_udp_channel.cpp"
        ${RPLIDAR_DATAUNPACKER_SOURCES}
)

list(APPEND RPLIDAR_SOURCES
        "${RPLIDAR_SDK_ROOT}/src/arch/linux/net_serial.cpp"
        "${RPLIDAR_SDK_ROOT}/src/arch/linux/net_socket.cpp"
        "${RPLIDAR_SDK_ROOT}/src/arch/linux/timer.cpp"
)
add_library(rplidar_sdk STATIC ${RPLIDAR_SOURCES})

target_include_directories(rplidar_sdk PUBLIC
        "${RPLIDAR_SDK_ROOT}/include"
        "${RPLIDAR_SDK_ROOT}/src"
        "${RPLIDAR_SDK_ROOT}/src/dataunpacker"
        "${RPLIDAR_SDK_ROOT}/src/dataunpacker/unpacker"
)

target_compile_features(rplidar_sdk PUBLIC cxx_std_11)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(rplidar_sdk PRIVATE -O3)
endif()


add_library(${PROJECT_NAME} SHARED
        src/lidar_data.cpp)


set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        OUTPUT_NAME ${PROJECT_NAME})

target_include_directories(
        ${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PRIVATE autonet_types rplidar_sdk)
