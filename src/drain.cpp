#include "ros/ros.h"
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>
#include "camera_nodelet/helper.h"
#include "camera_nodelet/my_nodes.h"
#include <nodelet/nodelet.h>
#include "sensor_msgs/Image.h"
#include "sensor_msgs/image_encodings.h"
#include "cv_bridge/cv_bridge.h"

namespace camera{
    void drain::onInit(){
	ros::NodeHandle& rs = getMTNodeHandle();
	depth_sub = rs.subscribe<sensor_msgs::Image>("depth", 8, &drain::drain_depth_callback, this);
	rgb_sub = rs.subscribe<sensor_msgs::Image>("RGB", 8, &drain::drain_rgb_callback, this);
	NODELET_INFO("Camera drain node created\n");
    }
    
    void drain::drain_depth_callback(const sensor_msgs::Image::ConstPtr& msg){
	cv_bridge::CvImagePtr cv_ptr;
	cv_bridge::CvImageConstPtr cv_const_ptr;
	
	try
	{
	    cv_const_ptr = cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::MONO16);
	    //cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::MONO16);
	    //NODELET_INFO("cv bridge successful\n");
      
	}
	catch (cv_bridge::Exception& e)
	{
	    NODELET_ERROR("cv_bridge exception: %s", e.what());
	    return;
	}

	std::string myStr =  "received depth frame"+(msg->header.frame_id)+"\n";
	const char* c = myStr.c_str();
	NODELET_INFO(c);

	unsigned int seconds = msg->header.stamp.sec;
	unsigned int nanoseconds = msg->header.stamp.nsec;
	std::stringstream ss;
	ss<<std::setw(10)<<std::setfill('0')<<seconds<<"."<<std::setw(9)<<std::setfill('0')<<nanoseconds;
	myStr = "/media/nvidia/ExtremeSSD/depth_images/"+ss.str()+".png";
	cv::imwrite(myStr, cv_const_ptr->image);

        myStr = "saved depth frame"+(msg->header.frame_id)+"\n";
	const char* c2 = myStr.c_str();
	NODELET_INFO(c2);
    }

    void drain::drain_rgb_callback(const sensor_msgs::Image::ConstPtr& msg){
	cv_bridge::CvImagePtr cv_ptr;
	cv_bridge::CvImageConstPtr cv_const_ptr;
	
	try
	{
	    cv_const_ptr = cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::BGR8); //the CV Bridge will auto-convert from RGB to BGR format
	    //cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
 
	}
	catch (cv_bridge::Exception& e)
	{
	    NODELET_ERROR("cv_bridge exception: %s", e.what());
	    return;
	}

	std::string myStr =  "received rgb frame"+(msg->header.frame_id)+"\n";
	const char* c = myStr.c_str();
	NODELET_INFO(c);

	unsigned int seconds = msg->header.stamp.sec;
	unsigned int nanoseconds = msg->header.stamp.nsec;
	std::stringstream ss;
	ss<<std::setw(10)<<std::setfill('0')<<seconds<<"."<<std::setw(9)<<std::setfill('0')<<nanoseconds;
	myStr = "/media/nvidia/ExtremeSSD/rgb_images/"+ss.str()+".jpg";
	cv::imwrite(myStr, cv_const_ptr->image);

        myStr = "saved color frame"+(msg->header.frame_id)+"\n";
	const char* c2 = myStr.c_str();
	NODELET_INFO(c2);
    }
}
