#include "arl_slam/string_publisher.h"

StringPublisher::StringPublisher(ros::NodeHandle& nh)
	:nh_(nh)
	{
		string_publisher_ = nh_.advertise<std_msgs::String>("string", 1);
		timer_ = nh_.createTimer(ros::Duration(0.1), &StringPublisher::PublishStringCallback, this);
	}

void StringPublisher::PublishStringCallback(const ros::TimerEvent& event) 
{
	std_msgs::String string;
	std::stringstream ss;
  	ss << "test msg";
    string.data = ss.str();
    string_publisher_.publish(string);
}
