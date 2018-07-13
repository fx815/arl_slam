#!/usr/bin/env python

import rospy
from nav_msgs.msg import Odometry
import tf
import math


class AddOrientationOffset:
    def __init__(self):

        self.i = 0

        rospy.Subscriber('msf_core/odometry', Odometry, self.pose_callback)

        self.pub_pose_out = rospy.Publisher('msf_core/odometry/corrected',Odometry, queue_size=10)

        rospy.spin()

    def pose_callback(self, msg):

        self.i += 1

        if self.i == 1:
            print ("add_orientation_offset initialized")
            q_in = [msg.pose.pose.orientation.x,
                    msg.pose.pose.orientation.y,
                    msg.pose.pose.orientation.z,
                    msg.pose.pose.orientation.w]

            euler_init_rpy = tf.transformations.euler_from_quaternion(q_in)
            roll = euler_init_rpy[0]
            pitch = euler_init_rpy[1]
            yaw = euler_init_rpy[2]
            #self.roll_offset = -roll
            #self.pitch_offset = -pitch
            self.yaw_offset = -yaw

            #roll_out = roll + self.roll_offset
            #pitch_out = pitch + self.pitch_offset
            yaw_out = yaw + self.yaw_offset

            q_new = tf.transformations.quaternion_from_euler(roll,pitch,yaw_out)

            pose_out = msg
            pose_out.pose.pose.orientation.x = q_new[0]
            pose_out.pose.pose.orientation.y = q_new[1]
            pose_out.pose.pose.orientation.z = q_new[2]
            pose_out.pose.pose.orientation.w = q_new[3]

            
            transform_pub = tf.TransformBroadcaster()
            transform_pub.sendTransform((pose_out.pose.pose.position.x, pose_out.pose.pose.position.y, pose_out.pose.pose.position.z),
                     q_new,
                     rospy.Time.now(),
                     "state_corrected",
                     "world")

            self.pub_pose_out.publish(pose_out) 

        else:
            q_in = [msg.pose.pose.orientation.x,
                    msg.pose.pose.orientation.y,
                    msg.pose.pose.orientation.z,
                    msg.pose.pose.orientation.w]

            euler_in_rpy = tf.transformations.euler_from_quaternion(q_in)
            roll = euler_in_rpy[0]
            pitch = euler_in_rpy[1]
            yaw = euler_in_rpy[2]

            #roll_out = roll + self.roll_offset
            #pitch_out = pitch + self.pitch_offset
            yaw_out = yaw + self.yaw_offset

            q_new = tf.transformations.quaternion_from_euler(roll,pitch,yaw_out)

            pose_out = msg
            pose_out.pose.pose.orientation.x = q_new[0] 
            pose_out.pose.pose.orientation.y = q_new[1]
            pose_out.pose.pose.orientation.z = q_new[2]
            pose_out.pose.pose.orientation.w = q_new[3]
            
            transform_pub = tf.TransformBroadcaster()
            transform_pub.sendTransform((pose_out.pose.pose.position.x, pose_out.pose.pose.position.y, pose_out.pose.pose.position.z),
                     q_new,
                     rospy.Time.now(),
                     "state_corrected",
                     "world")

            self.pub_pose_out.publish(pose_out) 
        

if __name__ == '__main__':
    rospy.init_node('add_pose_offset')

    # Go to class functions that do all the heavy lifting. Do error checking.
    try:
        add_orientation_offset = AddOrientationOffset()
    except rospy.ROSInterruptException:
        pass