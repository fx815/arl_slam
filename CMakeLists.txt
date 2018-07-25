cmake_minimum_required(VERSION 2.8.3)
project(arl_slam)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS geometry_msgs roscpp rospy std_msgs cmake_modules tf)
find_package(Eigen REQUIRED)

## Include directory for header files
include_directories(./include ${Eigen_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS})

## Declare a catkin package
catkin_package(
DEPENDS Eigen 
INCLUDE_DIRS include ${Eigen_INCLUDE_DIRS}
CATKIN_DEPENDS tf
)

add_executable(msg_converter_node src/msg_converter_node.cpp src/msg_converter.cpp)
target_link_libraries(msg_converter_node ${catkin_LIBRARIES})

add_executable(msg_converter_inv_node src/msg_converter_inv_node.cpp src/msg_converter_inv.cpp)
target_link_libraries(msg_converter_inv_node ${catkin_LIBRARIES})


add_executable(string_publisher_node src/string_publisher_node.cpp src/string_publisher.cpp)
target_link_libraries(string_publisher_node ${catkin_LIBRARIES})


add_executable(rotate_vision_pose_node src/rotate_vision_pose_node.cpp src/rotate_vision_pose.cpp)
target_link_libraries(rotate_vision_pose_node ${catkin_LIBRARIES})
