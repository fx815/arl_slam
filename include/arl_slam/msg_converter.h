#ifndef MSG_CONVERTER_H
#define MSG_CONVERTER_H

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TransformStamped.h>

class MsgConverter{
	public:
		MsgConverter(ros::NodeHandle& nh);

	private:
		ros::NodeHandle nh_;
		ros::Subscriber pose_stamped_subscriber_;
		ros::Publisher transform_stamped_publisher_;

		void MsgConverterCallback(const geometry_msgs::PoseStamped::ConstPtr& poseStamp);
};

#endif