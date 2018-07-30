#include "arl_slam/pid_controller.h"

pidControl::pidControl(ros::NodeHandle& nh)
	:nh_(nh)
	{
		odom_subscriber_ = nh_.subscribe("odom_in",1,&pidControl::OdomCallback, this);
		trajectory_subscriber_ = nh_.subscribe("trajectory_in",1,&pidControl::TrajectoryCallback, this);
  		pose_publisher_ = nh_.advertise<geometry_msgs::PoseStamped>("vision_pose_out", 1);
	}

void pidControl::pidControlCallback(const nav_msgs::Odometry::ConstPtr &odom) 
{

// Simple PD control law - following desired motion (xd, dxd, ddxd)
Vector3d desiredThrust = e3*m*g - ddxd*m + (dx - dxd)*k_x + (x - xd)*k_v; //e3*m*g gravity force

// Compute throttle, roll angle, pitch angle and yaw rate commands
QuadrotorCommand computeQuadrotorCommand(Vector3d desiredThrust, double currentYawAngle, double desiredYaw) {
	QuadrotorCommand quadrotorCommand; 

	Matrix3d R_yaw = rpy2R(0.0, 0.0, currentYawAngle);		// Rotation matrix with only yaw motion
	Vector3d Gamma_temp = R_yaw.transpose()*desiredThrust;

	quadrotorCommand.thrustValue = std::sqrt(desiredThrust.dot(desiredThrust));
	quadrotorCommand.rollAngle = asin(Gamma_temp(1) / (-quadrotorCommand.thrustValue));
	quadrotorCommand.pitchAngle = atan2(Gamma_temp(0) / quadrotorCommand.thrustValue, Gamma_temp(2) / quadrotorCommand.thrustValue);
	quadrotorCommand.yawAngle = desiredYaw;//yawControlGain * (currentYawAngle - (45 * PI / 180 + currentYawAngle));
	return quadrotorCommand;

}