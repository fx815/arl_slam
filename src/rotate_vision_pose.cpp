#include "arl_slam/rotate_vision_pose.h"

RotatePose::RotatePose(ros::NodeHandle& nh)
	:nh_(nh)
	{
		pose_subscriber_ = nh_.subscribe("vision_pose_in",1,&RotatePose::RotatePoseCallback, this);
  		pose_publisher_ = nh_.advertise<geometry_msgs::PoseStamped>("vision_pose_out", 1);
	}

void RotatePose::RotatePoseCallback(const geometry_msgs::PoseStamped::ConstPtr &poseStamp) 
{
	//set scale
	scale_ = 1.0;

	//drift of vision w.r.t world
	//q_wv_.setIdentity();
	q_wv_.w() = 0.707; //0.696;
    q_wv_.x() = 0.0;//0.092; //0.123;
    q_wv_.y() = 0.0;//0.092; //-0.123;
    q_wv_.z() = -0.707; //0.696;
	p_wv_.setZero();

	//rotation of imu w.r.t camera, from kalibr
	q_ic_.x() = 0.62035007271;
	q_ic_.y() = 0.613169181942;
	q_ic_.z() = 0.353778138141;
	q_ic_.w() = 0.337683832274;

	//translation of imu w.r.t camera, from kalibr
	p_ic_[0] = 0.00105466959468;
	p_ic_[1] = -0.00908823206352;
	p_ic_[2] = -0.01196203291;

	//rotation of camera w.r.t vision, from rovioli
	q_cv_.x() = poseStamp->pose.orientation.x;
	q_cv_.y() = poseStamp->pose.orientation.y;
	q_cv_.z() = poseStamp->pose.orientation.z;
	q_cv_.w() = poseStamp->pose.orientation.w;

	//translation of camera w.r.t vision, from rovioli
	p_vc_[0] = poseStamp->pose.position.x;
	p_vc_[1] = poseStamp->pose.position.y;
	p_vc_[2] = poseStamp->pose.position.z;

	//rotation of imu w.r.t world
	q_ = (q_ic_ * q_cv_.conjugate() * q_wv_).conjugate();
	q_.normalize();

	//translation of imu w.r.t world
	p_ = p_wv_ + q_wv_.conjugate().toRotationMatrix() * p_vc_ / scale_
        - q_.toRotationMatrix() * p_ic_;

    //broadcast transform for visualization
    transform.setOrigin(tf::Vector3(p_[0], p_[1], p_[2]));
    transform.setRotation(tf::Quaternion(q_.x(), q_.y(), q_.z(), q_.w()));
    tf_broadcaster_.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "state"));

    //publish new transformed pose
    pose_out_.pose.position.x = p_[0]; 
    pose_out_.pose.position.y = p_[1];
    pose_out_.pose.position.z = p_[2];

    pose_out_.pose.orientation.x = q_.x();
    pose_out_.pose.orientation.y = q_.y();
    pose_out_.pose.orientation.z = q_.z();
    pose_out_.pose.orientation.w = q_.w();

    pose_out_.header.stamp = ros::Time::now();

	/*
	//get the pos part of the pose
	v_[0] = poseStamp->pose.position.x;
    v_[1] = poseStamp->pose.position.y;
    v_[2] = poseStamp->pose.position.z;
   
   	//get vector part of the quaternion
    u_[0] = poseStamp->pose.orientation.x; 
    u_[1] = poseStamp->pose.orientation.y; 
    u_[2] = poseStamp->pose.orientation.z; 

    //extract the scalar part of the quaternion
    s_ = poseStamp->pose.orientation.w; 

    //do the math
    v_prime_ = 2.0 * u_.dot(v_) * u_
          + (s_*s_ - u_.dot(u_)) * v_
          + 2.0 * s_ * u_.cross(v_);

    pos_out_.pose.position = v_prime_[0]
    */

    pose_publisher_.publish(pose_out_);

}
