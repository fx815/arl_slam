#include "arl_slam/msg_converter_inv.h"

int main(int argc, char** argv){

  ros::init(argc, argv, "msg_converter_inv");
  ros::NodeHandle nh;

  MsgConverterInv msg_converter_inv(nh);

  ros::spin();
  return 0;

}