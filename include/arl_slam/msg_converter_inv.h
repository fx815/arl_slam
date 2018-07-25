#ifndef MSG_CONVERTER_INV_H
#define MSG_CONVERTER_INV_H

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TransformStamped.h>

class MsgConverterInv{
	public:
		MsgConverterInv(ros::NodeHandle& nh);

	private:
		ros::NodeHandle nh_;
		ros::Publisher pose_stamped_publisher_;
		ros::Subscriber transform_stamped_subscriber_;

		void MsgConverterInvCallback(const geometry_msgs::TransformStamped::ConstPtr& transStamp);
};

#endif