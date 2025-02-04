cmake_minimum_required(VERSION 3.0.2)
project(pcl_ros_wrapper)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  pcl_conversions
  pcl_ros
  roscpp
  rospy
  sensor_msgs
  dynamic_reconfigure
)

find_package(PCL REQUIRED)

## Dynamic reconfigure
generate_dynamic_reconfigure_options(
  cfg/PclFilter.cfg
)

## Add include directories
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  cfg
)

## Export dependencies
catkin_package(
  LIBRARIES pcl_ros_wrapper
  CATKIN_DEPENDS pcl_conversions pcl_ros roscpp rospy sensor_msgs dynamic_reconfigure
)

## Declare the executable
add_executable(pcl_ros_filter src/pcl_ros_filter.cpp)
## Link against required libraries
target_link_libraries(pcl_ros_filter
  ${catkin_LIBRARIES}
  ${PCL_LIBRARIES}
)
## Ensure dynamic reconfigure is built before our node
add_dependencies(pcl_ros_filter ${PROJECT_NAME}_gencfg)

## Ensure ROS can find and install the executable
install(TARGETS pcl_ros_filter
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

