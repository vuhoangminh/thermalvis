/*! \file	ros_resources.hpp
 *  \brief	Declarations for ROS inclusion requirements and ROS-related tools.
*/

#ifdef _BUILD_FOR_ROS_
#ifndef _THERMALVIS_ROS_RESOURCES_H_
#define _THERMALVIS_ROS_RESOURCES_H_

#include "general_resources.hpp"
#include "opencv_resources.hpp"

/***** ROS Stuff *****/
#include "ros/ros.h"
#include <std_msgs/Float32.h>
//#include <../../opt/ros/fuerte/include/ros/node_handle.h>
#include "ros/node_handle.h"
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/SetCameraInfo.h>
#include <sensor_msgs/image_encodings.h>
#include <camera_calibration_parsers/parse_ini.h>
#include <cv_bridge/cv_bridge.h>

#include <ros/ros.h>
#include <image_transport/image_transport.h>

#include <tf/transform_broadcaster.h>

#include <visualization_msgs/MarkerArray.h>

#include <dynamic_reconfigure/server.h>

namespace enc = sensor_msgs::image_encodings;

void changemode(int);
int  kbhit(void);
double timeDiff(ros::Time time1, ros::Time time2);
ros::Time findAverageTime(ros::Time time1, ros::Time time2);

#define MAX_RVIZ_DISPLACEMENT 	1000

#ifdef _BUILD_FOR_ROS_ // Should definitely be defined if this file is called..
void displayMessage(string msg, int msg_code = MESSAGE_NORMAL);
#endif

#endif
#endif