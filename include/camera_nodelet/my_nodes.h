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

/*
 *Define two classes: source and drain that act as our nodelets. Both inherits the base class "Nodelet::nodelet"
 *camera::source fetches new frames from the camera and sends them to the topics
 *camera::drain collects depth and rgb frames from the topics and stores them to file system
 *TODO: TAKE THE BURDEN OF SAVING FILES OFF FROM THE SUBSCRIBER CALLBACK, AND DO IT WITH FORK
 */

namespace camera{

    class source : public nodelet::Nodelet{
    public:
	source(){
	    NODELET_INFO("camera source node constructed\n");
	};
	~source(){
	    NODELET_INFO("camera source node destrcuted\n");
	}; 
	virtual void onInit();//mandatory initialization function for all nodelets

    private:
	ros::Publisher depth_pub;
	ros::Publisher rgb_pub;
	ros::Timer timer;
	float FPS = 30; //{15, 30, 60, 90}; this FPS value should be send in via command line parameters in the future
	uint32_t seq = 0;
	
	//rs2::frameset frames;
	rs2::pipeline p;
	rs2::config c;
	
	//callback function that transmits frames to the topics
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
	virtual void onInit();//mandatory initialization function for all nodelets
    private:
	ros::Subscriber depth_sub;
	ros::Subscriber rgb_sub;

	//ConstPtr& is necessary for nodelets to work
	void drain_depth_callback(const sensor_msgs::Image::ConstPtr& msg);
	void drain_rgb_callback(const sensor_msgs::Image::ConstPtr& msg);
    };

    
}


#endif
