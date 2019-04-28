#ifndef CAMERA_NODELET__MY_NODES
#define CAMERA_NODELET__MY_NODES

#include "ros/ros.h"
#include <librealsense2/rs.hpp>
#include "camera_nodelet/helper.h"
#include <nodelet/nodelet.h>
#include "sensor_msgs/Image.h"
#include "sensor_msgs/image_encodings.h"
#include <stdio.h>
#include "cv_bridge/cv_bridge.h"
#include <opencv2/opencv.hpp>

namespace camera{

    class source : public nodelet::Nodelet{
    public:
	source(){
	    NODELET_INFO("camera source node constructed\n");
	};
	~source(){
	    NODELET_INFO("camera source node destrcuted\n");
	}; 
	virtual void onInit();

    private:
	ros::Publisher depth_pub;
	ros::Publisher rgb_pub;
	ros::Timer timer;
	float FPS = 60; //{15, 30, 60, 90}
	uint32_t seq = 0;
	rs2::frameset frames;
	rs2::pipeline p;
	rs2::config c;
	
	void timerCallback(const ros::TimerEvent& event);
    };


 
    class drain : public nodelet::Nodelet{
    public:
	drain(){
	    NODELET_INFO("camera drain node constructed\n");
	};
	~drain(){
	    NODELET_INFO("camera drain node destructed\n");
	};
	virtual void onInit();
    private:
	ros::Subscriber depth_sub;
	cv_bridge::CvImagePtr cv_ptr;
	void drain_callback(const sensor_msgs::Image::ConstPtr& msg);
    };

    
}


#endif
