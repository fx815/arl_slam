#include "arl_slam/rotate_vision_pose.h"

int main(int argc, char** argv){

  ros::init(argc, argv, "rotate_vision_pose");
  ros::NodeHandle nh;

  RotatePose rotate_vision_pose(nh);
  
  ros::spin();
  return 0;
}
