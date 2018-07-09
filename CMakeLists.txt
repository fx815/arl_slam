cmake_minimum_required(VERSION 2.8.3)
project(arl_slam)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS geometry_msgs roscpp rospy std_msgs)

## Declare a catkin package
catkin_package()

## Include directory for header files
include_directories(include ${catkin_INCLUDE_DIRS})

## add libraries
add_library(msg_converter src/msg_converter.cpp)
add_library(string_publisher src/string_publisher.cpp)

add_executable(msg_converter_node src/msg_converter_node.cpp)
target_link_libraries(msg_converter_node ${catkin_LIBRARIES} msg_converter)

add_executable(string_publisher_node src/string_publisher_node.cpp)
target_link_libraries(string_publisher_node ${catkin_LIBRARIES} string_publisher)
