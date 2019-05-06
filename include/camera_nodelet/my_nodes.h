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
<<<<<<< HEAD
	float FPS = 30; //{15, 30, 60, 90}
=======
	float FPS = 60; //{15, 30, 60, 90}
>>>>>>> 918ae7f85a35edfb683b1fb2445f4bb28853a9bf
	uint32_t seq = 0;
	rs2::frameset frames;
	rs2::pipeline p;
	rs2::config c;
<<<<<<< HEAD
=======
	
>>>>>>> 918ae7f85a35edfb683b1fb2445f4bb28853a9bf
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
<<<<<<< HEAD
	ros::Subscriber rgb_sub;
	void drain_depth_callback(const sensor_msgs::Image::ConstPtr& msg);
	void drain_rgb_callback(const sensor_msgs::Image::ConstPtr& msg);
=======
	cv_bridge::CvImagePtr cv_ptr; //i don't know if I should put cv_ptr here or in the method scope
	void drain_callback(const sensor_msgs::Image::ConstPtr& msg);
>>>>>>> 918ae7f85a35edfb683b1fb2445f4bb28853a9bf
    };

    
}


#endif
