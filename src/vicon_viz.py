#!/usr/bin/env python

import roslib 
import rospy 
import math
import tf 
import matplotlib.pyplot as plt 


def changeOdomAxis(odom_pos, odom_rpy):
	# note do this after already getting the euler form quaternion 
	newpos = [odom_pos[2], -odom_pos[0], -odom_pos[1]]
	newrpy = [odom_rpy[2], -odom_rpy[0], -odom_rpy[1]]
	return newpos, newrpy 

def computeRef(odom_pos, odom_ori, vic_pos, vic_ori): #compute transform from odom to vicon
	# return (x_diff, y_diff, z_diff), (y_diff, p_diff, r_diff)
	odom_rpy = tf.transformations.euler_from_quaternion(odom_ori)
	# o_pos, o_rpy = changeOdomAxis(odom_pos, odom_rpy)
	o_pos, o_rpy = odom_pos, odom_rpy
	vic_rpy = tf.transformations.euler_from_quaternion(vic_ori)
	trans_diff = tuple([vic_pos[i] - o_pos[i] for i in range(len(o_pos))])
	rot_diff = tuple([vic_rpy[j] - o_rpy[j] for j in range(len(o_rpy))])
	return trans_diff, rot_diff

def transformOdomData(odom_pos, odom_ori, trans_diff, rot_diff): # transform the odom data to the vicon frame
	odom_rpy = tf.transformations.euler_from_quaternion(odom_ori)
	# o_pos, o_rpy = changeOdomAxis(odom_pos, odom_rpy)
	o_pos, o_rpy = odom_pos, odom_rpy
	rpy = [o_rpy[i] + rot_diff[i] for i in range(len(rot_diff))]  
	pos = [o_pos[j] + trans_diff[j] for j in range(len(trans_diff))]
	return pos, rpy

def findError(new_odom_pos, new_odom_rpy, vic_pos, vic_rpy): # error in odom 
	error_pos = [new_odom_pos[i] - vic_pos[i] for i in range(len(vic_pos))]
	error_rpy = [new_odom_rpy[j] - vic_rpy[j] for j in range(len(vic_rpy))]
	print(error_pos)
	return error_pos, error_rpy

def findDistance(error_pos): # distance error b/t odom and vicon
	return math.sqrt(error_pos[0]**2 + error_pos[1]**2 + error_pos[2]**2)

def rads2degs(rad):
	degrees = rad/6.28*360
	return degrees
	
class PlotAndStore(object):
	# store data and create plots
	def __init__(self, sets_of_data, downsample_factor=30):
		# sets of data should be an integer
		self.data = [[] for i in range(sets_of_data)]
		self.n = [0 for i in range(sets_of_data)]
		self.downsample_factor = downsample_factor

	def add_datapt(self, data_index, data_pt):
		if self.n[data_index] % self.downsample_factor == 0:
			self.data[data_index].append(data_pt)
		self.n[data_index] += 1

	def show_result(self, TitlesList, yAxisList):
		for i in range(len(self.data)):
			plt.plot(self.data[i])
			plt.ylabel(yAxisList[i])
			plt.title(TitlesList[i])
			plt.show()
			data_len = len(self.data[i])
			print(TitlesList[i])
			print('max value: {}'.format(str(max(self.data[i]))))
			avg = sum(self.data[i])/data_len
			print('avg value: {}'.format(str(avg)))
			avg_1 = sum(self.data[i][0:int(data_len/2)])/float(int((data_len/2)))
			avg_2 = sum(self.data[i][int(data_len/2):])/float(int((data_len/2)))
			print('first half avg value: {}'.format(str(avg_1)))
			print('second half avg value: {}'.format(str(avg_2))) 

if __name__ == '__main__':
	vicon_pos_0 = None # xyz from vicon 
	vicon_ori_0 = None # rpy from vicon 

	odom_pos_0 = None # xyz from rovioli odom 
	odom_ori_0 = None # rpy from rovioli odom 

	rospy.init_node('vicon_viz')
	listener = tf.TransformListener()
	rate = rospy.Rate(30.0)

	trans_diff = None # difference in the linear position 
	rot_diff = None # difference in the angular position 

	analyzer = PlotAndStore(6)

	while not rospy.is_shutdown():
		try:
			#get initialized baseframes from vicon and odom respectively
			(trans_v,rot_v) = listener.lookupTransform('vicon/brett/brett', '/world', rospy.Time(0))
			(trans_o, rot_o) = listener.lookupTransform('/state', '/world', rospy.Time(0))
			if vicon_pos_0 == None or vicon_ori_0 == None: # find the first vicon transform for reference 
				vicon_pos_0 = trans_v
				vicon_ori_0 = rot_v

			if odom_pos_0 == None or odom_ori_0 == None: # keep first odom transform 
				odom_pos_0 = trans_o
				odom_ori_0 = rot_o

			if trans_diff == None or rot_diff == None: # haven't called computeRef yet
				if odom_pos_0 != None and odom_ori_0 != None and vicon_ori_0 != None and vicon_pos_0 != None: # initialized first transform from vicon and odom
					# compute reference 
					trans_diff, rot_diff = computeRef(odom_pos_0, odom_ori_0, vicon_pos_0, vicon_ori_0)
					print("reading...")
			else:
				rot_rpy = tf.transformations.euler_from_quaternion(rot_v) 
				pos, rpy = transformOdomData(trans_o, rot_o, trans_diff, rot_diff)
				error_pos, error_rpy = findError(pos, rpy, trans_v, rot_rpy)
				dist = findDistance(error_pos)
				analyzer.add_datapt(0, abs(error_pos[0]))
				analyzer.add_datapt(1, abs(error_pos[1]))
				analyzer.add_datapt(2, abs(error_pos[2]))
				analyzer.add_datapt(3, abs(error_rpy[0]))
				analyzer.add_datapt(4, abs(error_rpy[1]))
				analyzer.add_datapt(5, abs(error_rpy[2]))

		except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
			continue
		rate.sleep()
analyzer.show_result(['x-error', 'y-error', 'z-error', 'roll', 'pitch', 'yaw'], ['m', 'm', 'm','rad', 'rad', 'rad'])