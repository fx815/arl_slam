#ifndef STRING_PUBLISHER_H
#define STRING_PUBLISHER_H

#include <ros/ros.h>
#include "std_msgs/String.h"
#include <sstream>

class StringPublisher{
	public:
		StringPublisher(ros::NodeHandle& nh);
		void PublishStringCallback(const ros::TimerEvent& event);
	private:
		ros::NodeHandle nh_;
		ros::Publisher string_publisher_;
		ros::Timer timer_;
};

#endif