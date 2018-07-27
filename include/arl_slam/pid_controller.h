#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <ros/ros.h>
//#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>
#include <trajectory_msgs/MultiDOFJointTrajectory.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>

class pidControl{
	public:
		pidControl(ros::NodeHandle& nh);

	private:
		ros::NodeHandle nh_;
		ros::Subscriber odom_subscriber_;
		ros::Publisher command_publisher_;
		nav_msgs::Odometry odom_in_;
		//Eigen::Matrix<double, 3, 1> 
		//double scale_;
		//tf::Transform transform;
		//tf::TransformBroadcaster tf_broadcaster_;

		void poseCallback(const geometry_msgs::PoseStamped::ConstPtr& pose);
		void trajectoryCallback(const geometry_msgs::PoseStamped::ConstPtr& pose);
		void poseCallback(const trajectory_msgs::MultiDOFJointTrajectory::ConstPtr& pose);

};

#endif