#include <ros/ros.h>
// PCL specific includes
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl_ros/transforms.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>

ros::Publisher pub;

void 
cloud_cb (const sensor_msgs::PointCloud2ConstPtr& msg)
{
  pcl::PCLPointCloud2::Ptr cloud (new pcl::PCLPointCloud2 ());
  pcl::PCLPointCloud2::Ptr cloud_filtered (new pcl::PCLPointCloud2 ());
  //pcl_conversions::copyPointCloud2MetaData(*msg,*cloud);
  pcl_conversions::toPCL(*msg,*cloud);


  std::cerr << "PointCloud before filtering: " << cloud->width * cloud->height 
       << " data points (" << pcl::getFieldsList (*cloud) << ")."<<std::endl;

  // Create the filtering object
  pcl::VoxelGrid<pcl::PCLPointCloud2> sor;
  sor.setInputCloud (cloud);
  sor.setLeafSize (0.01f, 0.01f, 0.01f);
  sor.filter (*cloud_filtered);
  std::cerr << "PointCloud after filtering: " << cloud_filtered->width * cloud_filtered->height 
       << " data points (" << pcl::getFieldsList (*cloud_filtered) << ")."<<std::endl;

  // Create a container for the data.
  //sensor_msgs::PointCloud2 output;

  // Do data processing here...
  //output = *input;

  // Publish the data.
  pub.publish (*cloud_filtered);
}

int
main (int argc, char** argv)
{
  // Initialize ROS
  ros::init (argc, argv, "voxel_filter");
  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe ("/camera/depth/points", 1, cloud_cb);

  // Create a ROS publisher for the output point cloud
  pub = nh.advertise<sensor_msgs::PointCloud2> ("output", 1);

  // Spin
  ros::spin ();
}
