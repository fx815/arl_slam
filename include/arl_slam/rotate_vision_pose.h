#ifndef ROTATE_VISION_POSE_H
#define ROTATE_VISION_POSE_H

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <tf/transform_broadcaster.h>

class RotatePose{
	public:
		RotatePose(ros::NodeHandle& nh);

	private:
		ros::NodeHandle nh_;
		ros::Subscriber pose_subscriber_;
		ros::Publisher pose_publisher_;
		geometry_msgs::PoseStamped pose_out_;
		Eigen::Matrix<double, 3, 1> p_, p_wv_, p_vc_, p_ic_;
		Eigen::Quaternion<double> q_, q_wv_, q_ic_, q_cv_;
		double scale_;
		tf::Transform transform;
		tf::TransformBroadcaster tf_broadcaster_;

		void RotatePoseCallback(const geometry_msgs::PoseStamped::ConstPtr& poseStamp);
};

#endif