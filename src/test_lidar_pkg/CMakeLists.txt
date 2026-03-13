cmake_minimum_required(VERSION 3.20)
project(test_lidar_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_srvs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# creating executable
add_executable(spinup_test src/spinup_test.cpp)
# adding the packages as dependencies to the executable
target_link_libraries(spinup_test PUBLIC
        ${sensor_msgs_TARGETS}
        ${std_srvs_TARGETS}
        rclcpp::rclcpp
        sensor_msgs::sensor_msgs_library
)
# ROS2 searches the nodes in lib/PROJECT_NAME so install the targets there
install(TARGETS spinup_test
        DESTINATION lib/${PROJECT_NAME}
)


ament_package()
