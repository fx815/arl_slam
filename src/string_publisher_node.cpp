#include "arl_slam/string_publisher.h"

int main(int argc, char** argv){

  ros::init(argc, argv, "string_publisher");
  ros::NodeHandle nh;

  StringPublisher string_publisher(nh);
  
  ros::spin();
  return 0;
}
