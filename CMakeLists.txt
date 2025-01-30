cmake_minimum_required(VERSION 3.0.2)
project(pcl_ros_wrapper)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  pcl_conversions
  pcl_ros
  roscpp
  sensor_msgs
)

## Find PCL
find_package(PCL REQUIRED)

## Declare a C++ executable
add_executable(pcl_ros_filter src/pcl_ros_filter.cpp)

## Link against catkin and PCL libraries
target_link_libraries(pcl_ros_filter
  ${catkin_LIBRARIES}
  ${PCL_LIBRARIES}
)

## Add include directories
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
)

## Export dependencies
catkin_package(
  INCLUDE_DIRS ${PCL_INCLUDE_DIRS}
  LIBRARIES pcl_ros_wrapper
  CATKIN_DEPENDS pcl_conversions pcl_ros roscpp sensor_msgs
  DEPENDS PCL
)


## Install target so ROS can find it
install(TARGETS pcl_ros_filter
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)