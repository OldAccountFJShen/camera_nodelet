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
	ros::NodeHandle& rs = getPrivateNodeHandle();
	depth_sub = rs.subscribe<sensor_msgs::Image>("depth", 8, &drain::drain_callback, this);
	NODELET_INFO("Camera drain node created\n");
    }
    
    void drain::drain_callback(const sensor_msgs::Image::ConstPtr& msg){
	//NODELET_INFO("DEPTH IMAGE RECEIVED");
	try
	{
	    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::MONO16);
	    //NODELET_INFO("cv bridge successful\n");
	
	}
	catch (cv_bridge::Exception& e)
	{
	    NODELET_ERROR("cv_bridge exception: %s", e.what());
	    return;
	}

	std::string str;
	str = "/media/nvidia/ExtremeSSD/depth_images/"+msg->header.frame_id+".jpg";
	cv::imwrite(str, cv_ptr->image);
	//NODELET_INFO("Image written\n");
    }
}
