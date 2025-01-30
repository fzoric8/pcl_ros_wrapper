#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/crop_box.h>
#include <pcl/common/transforms.h>
#include <pcl/point_cloud.h>

// Publisher for the filtered data
ros::Publisher pub;

// filter declarations
void voxelFilter(pcl::PCLPointCloud2ConstPtr cloudInPtr, pcl::PCLPointCloud2& cloudOut, double leafSize);
void boxFilter(pcl::PCLPointCloud2ConstPtr cloudInPtr, pcl::PCLPointCloud2& cloudOut); 

void  cloud_cb (const sensor_msgs::PointCloud2ConstPtr& cloud_msg)
{
  ROS_INFO_ONCE("PointCloud received: width = %d, height = %d", cloud_msg->width, cloud_msg->height);
  // Container for original & filtered data
  pcl::PCLPointCloud2* cloud = new pcl::PCLPointCloud2;
  pcl::PCLPointCloud2 cloud_filtered; 
  pcl::PCLPointCloud2ConstPtr cloudPtr(cloud);

  // Convert to PCL data type
  pcl_conversions::toPCL(*cloud_msg, *cloud);

  // Voxel filter
  voxelFilter(cloudPtr, cloud_filtered, 0.1f);
  boxFilter(cloudPtr, cloud_filtered);

  // Convert to ROS data type
  sensor_msgs::PointCloud2 output;
  // Copy header
  output.header = cloud_msg->header;
  output.header.frame_id = "camera_color_optical_frame";
  pcl_conversions::fromPCL(cloud_filtered, output);

  /*  pcl::PointCloud<pcl::PointXYZ>::Ptr ptr_transformed(new pcl::PointCloud<pcl::PointXYZ>);

  Eigen::Affine3f trans;
  trans << -1,   0,  0, 0.0,
           0,   -1,  0, 0.0,
           0,   0,  1, 0.0,
           0,   0,  0,  1;

  pcl::transformPointCloud(output, *ptr_transformed, trans);
  */
  // Publish the data
  pub.publish (output);

}

void voxelFilter(pcl::PCLPointCloud2ConstPtr cloudInPtr, pcl::PCLPointCloud2& cloudOut, double leafSize)
{
  //https://pointclouds.org/documentation/classpcl_1_1_voxel_grid.html
  pcl::VoxelGrid<pcl::PCLPointCloud2> sor;
  sor.setInputCloud (cloudInPtr);
  sor.setLeafSize (leafSize, leafSize, leafSize);
  int min_pts_per_voxel = 20; 
  sor.setMinimumPointsNumberPerVoxel(min_pts_per_voxel);
  //sor.setKeepOrganized(true);
  sor.filter (cloudOut);
}

void boxFilter(pcl::PCLPointCloud2ConstPtr cloudInPtr, pcl::PCLPointCloud2& cloudOut)
{
  // Test the PointCloud<PointT> method
  pcl::CropBox<pcl::PCLPointCloud2> cBF; 
  cBF.setInputCloud (cloudInPtr);
  Eigen::Vector4f min_pt (-0.5f, -0.5f, 0.0f, 1.0f);
  Eigen::Vector4f max_pt (0.5f, 0.5f, 3.0f, 1.0f);

  // Cropbox slighlty bigger then bounding box of points
  cBF.setMin (min_pt);
  cBF.setMax (max_pt);

  // Indices
  std::vector<int> indices;
  cBF.filter (indices);
  cBF.filter (cloudOut);
}

// Transform PCL: https://gist.github.com/LimHyungTae/ddd6f5cd6c2507a86388bd1b703e0cbb 

int main (int argc, char** argv)
{
  // Initialize ROS
  ros::init (argc, argv, "my_pcl_tutorial");
  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe ("/camera/depth/color/points", 1, cloud_cb);

  // Create a ROS publisher for the output point cloud
  pub = nh.advertise<sensor_msgs::PointCloud2> ("/filtered_pcl", 1);

  // Spin
  ros::spin ();
}
