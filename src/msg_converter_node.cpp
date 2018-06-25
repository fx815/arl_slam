#include "arl_slam/msg_converter.h"

int main(int argc, char** argv){

  ros::init(argc, argv, "msg_converter");
  ros::NodeHandle nh;

  MsgConverter msg_converter(nh);

  ros::spin();
  return 0;

}