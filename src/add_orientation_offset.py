#!/usr/bin/env python

#
#  Title:        add_orientation_offset.py
#  Description:  ROS module to add a constant offset to incoming IMU messages.
#                WARNING, use this module ONLY to initialize Rovio with NPOSE=0, as ONLY IMU orientation is processed,
#                accelerations and gyros are not affected!!!
#  Convention:   q_out =  q_in * q_offset

import rospy
from geometry_msgs.msg import PoseStamped
import tf.transformations as tf
import math


class AddOrientationOffset:
    def __init__(self):

        self.i = 0

        rospy.Subscriber('maplab_rovio/T_G_I', PoseStamped, self.pose_callback)

        self.pub_pose_out = rospy.Publisher('maplab_rovio/T_G_I/Init',PoseStamped, queue_size=10)

        rospy.spin()

    def pose_callback(self, msg):

        self.i += 1

        if self.i == 1:
            q_in = [msg.pose.orientation.x,
                           msg.pose.orientation.y,
                           msg.pose.orientation.z,
                           msg.pose.orientation.w]

            euler_offset_rpy = tf.transformations.euler_from_quaternion(q_in)
            roll = -euler_offset_rpy[0]
            pitch = -euler_offset_rpy[1]
            yaw = -euler_offset_rpy[2]
            self.q_offset = tf.transformations.quaternion_from_euler(roll, pitch, yaw)
            q_new = tf.quaternion_multiply(q_in,self.q_offset)

            pose_out = msg
            pose_out.pose.orientation.x = q_new[0]
            pose_out.pose.orientation.y = q_new[1]
            pose_out.pose.orientation.z = q_new[2]
            pose_out.pose.orientation.w = q_new[3]

            self.pub_pose_out.publish(pose_out) 

        else:
            q_in = [msg.pose.orientation.x,
                           msg.pose.orientation.y,
                           msg.pose.orientation.z,
                           msg.pose.orientation.w]

            q_new = tf.quaternion_multiply(q_in,self.q_offset)

            pose_out = msg
            pose_out.pose.orientation.x = q_new[0]
            pose_out.pose.orientation.y = q_new[1]
            pose_out.pose.orientation.z = q_new[2]
            pose_out.pose.orientation.w = q_new[3]

            self.pub_pose_out.publish(pose_out) 

if __name__ == '__main__':
    rospy.init_node('add_pose_offset')

    # Go to class functions that do all the heavy lifting. Do error checking.
    try:
        add_orientation_offset = AddOrientationOffset()
    except rospy.ROSInterruptException:
        pass