cmake_minimum_required(VERSION 3.8)
project(rfr_v2)
 
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()
 
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
 
add_executable(gait_publisher src/gait_publisher.cpp)
ament_target_dependencies(gait_publisher rclcpp std_msgs sensor_msgs)
 
install(TARGETS gait_publisher
  DESTINATION lib/${PROJECT_NAME})
 
# Copy the leg model, launch file and RViz view into the install folder
install(DIRECTORY launch urdf rviz
  DESTINATION share/${PROJECT_NAME})
 
ament_package()
