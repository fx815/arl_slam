#include "arl_slam/msg_converter_inv.h"

MsgConverterInv::MsgConverterInv(ros::NodeHandle& nh)
    : nh_(nh)
{
  transform_stamped_subscriber_ = nh_.subscribe("transform_in",1,&MsgConverterInv::MsgConverterInvCallback, this);
  pose_stamped_publisher_ = nh_.advertise<geometry_msgs::PoseStamped>("pose_out", 1);
}

void MsgConverterInv::MsgConverterInvCallback(const geometry_msgs::TransformStamped::ConstPtr &transStamp)
  {
    geometry_msgs::PoseStamped poseStamp;

    poseStamp.pose.position.x = transStamp->transform.translation.x;
    poseStamp.pose.position.y = transStamp->transform.translation.y;
    poseStamp.pose.position.z = transStamp->transform.translation.z;
    poseStamp.pose.orientation.x = transStamp->transform.rotation.x;
    poseStamp.pose.orientation.y = transStamp->transform.rotation.y;
    poseStamp.pose.orientation.z = transStamp->transform.rotation.z;
    poseStamp.pose.orientation.w = transStamp->transform.rotation.w;
    //poseStamp.header = transStamp->header;
    poseStamp.header.stamp = ros::Time::now();

    //transStamp.child_frame_id = "imu";

    pose_stamped_publisher_.publish(poseStamp);
  } 
