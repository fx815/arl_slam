#include "arl_slam/msg_converter.h"

MsgConverter::MsgConverter(ros::NodeHandle& nh)
    : nh_(nh)
{
  pose_stamped_subscriber_ = nh_.subscribe("pose_in",1,&MsgConverter::MsgConverterCallback, this);
  transform_stamped_publisher_ = nh_.advertise<geometry_msgs::TransformStamped>("transform_out", 1);
}

void MsgConverter::MsgConverterCallback(const geometry_msgs::PoseStamped::ConstPtr &poseStamp)
  {
    geometry_msgs::TransformStamped transStamp;

    transStamp.transform.translation.x = poseStamp->pose.position.x;
    transStamp.transform.translation.y = poseStamp->pose.position.y;
    transStamp.transform.translation.z = poseStamp->pose.position.z;
    transStamp.transform.rotation.x = poseStamp->pose.orientation.x;
    transStamp.transform.rotation.y = poseStamp->pose.orientation.y;
    transStamp.transform.rotation.z = poseStamp->pose.orientation.z;
    transStamp.transform.rotation.w = poseStamp->pose.orientation.w;
    //transStamp.header = poseStamp->header;
    transStamp.header.stamp = ros::Time::now();
    //transStamp.child_frame_id = "imu";

    transform_stamped_publisher_.publish(transStamp);
  } 

/*
geometry_msgs::TransformStamped transStamp;

void convert_cb(const geometry_msgs::PoseStamped::ConstPtr &poseStamp)
  {
    transStamp.transform.translation.x = poseStamp->pose.position.x;
    transStamp.transform.translation.y = poseStamp->pose.position.y;
    transStamp.transform.translation.z = poseStamp->pose.position.z;
    transStamp.transform.rotation.x = poseStamp->pose.orientation.x;
    transStamp.transform.rotation.y = poseStamp->pose.orientation.y;
    transStamp.transform.rotation.z = poseStamp->pose.orientation.z;
    transStamp.transform.rotation.w = poseStamp->pose.orientation.w;
    transStamp.header = poseStamp->header;
    transStamp.child_frame_id = "imu";
  }

int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "msg_converter");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("maplab_rovio/T_G_I", 1000, convert_cb);
  ros::Publisher pub = n.advertise<geometry_msgs::TransformStamped>("msg_converter/T_G_I", 1000);

  ros::Rate loop_rate(30);

  while (ros::ok())
  {

    pub.publish(transStamp);

    ros::spinOnce();

    loop_rate.sleep();
  }

  ros::spin();

  return 0;

} 
*/