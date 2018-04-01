
import keras
from keras.models import Sequential, model_from_json

import numpy as np
from numpy import inf
import cv2

import roslib
import rospy
import rosbag
from std_msgs.msg import Int32, String
from sensor_msgs.msg import CompressedImage
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist

is_frame_changed = 0
is_lidar_changed = 0

scan_data = np.array([])
image_data = np.array([])

def callback(data):
    global is_lidar_changed
    global scan_data

    np_scan = np.array(data.ranges)

    # print np_scan.shape

    scan_data = np.concatenate((np_scan[0:30], np_scan[-31:-1]), axis=0)

    scan_data[np.isinf(scan_data)] = 1.0

    # print scan_data.shape

    is_lidar_changed = 1

def image_callback(ros_data):
    global is_frame_changed
    global image_data
    np_arr = np.fromstring(ros_data.data, np.uint8)
    image_np = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)
    image_data = cv2.resize(image_np, (320, 240))
    is_frame_changed = 1


if __name__ == '__main__':

    rospy.init_node('predict_node')
    rospy.Subscriber("/scan", LaserScan, callback)
    rospy.Subscriber("/camera/rgb/image_raw/compressed",  CompressedImage, image_callback,  queue_size = 1)
    pub = rospy.Publisher('/cmd_vel', Twist, queue_size=10)

    json_file = open('my_files/my_model.json', 'r')
    loaded_model_json = json_file.read()
    json_file.close()
    loaded_model = model_from_json(loaded_model_json)
    # load weights into new model
    loaded_model.load_weights("my_files/my_model.h5")

    loaded_model.compile(loss='mean_squared_error', optimizer=keras.optimizers.Adam(lr=0.0001), metrics=["accuracy"])

    cmd_data = Twist()

    while not rospy.is_shutdown():

        if (is_frame_changed == 1) and (is_lidar_changed == 1):

            # print scan_data

            # cv2.imshow('image', image_data)
            # cv2.waitKey(10)

            isFrameChanged = 0
            isLidarChanged = 0

            image_data = image_data.astype('float32')
            image_data /= 255.0
            image_data = np.reshape(image_data, [1, 240, 320, 3])
            scan_data = np.reshape(scan_data, [1, 60])

            cmd_pre =  loaded_model.predict([image_data, scan_data])

            print cmd_pre[0]

            cmd_data.linear.x = cmd_pre[0][0]
            cmd_data.angular.z = cmd_pre[0][1]

            pub.publish(cmd_data)

    cmd_data.linear.x = 0
    cmd_data.angular.z = 0

    pub.publish(cmd_data)
