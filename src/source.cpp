#include "ros/ros.h"
#include <librealsense2/rs.hpp>
#include "camera_nodelet/helper.h"
#include "camera_nodelet/my_nodes.h"
#include <nodelet/nodelet.h>
#include "sensor_msgs/Image.h"
#include "sensor_msgs/image_encodings.h"

namespace camera{
    
    void source::onInit(){
	//getMTNodeHandle allows the all publishers/subscribers to run on multiple threads in the thread pool of nodelet manager.
	ros::NodeHandle& rs = getMTNodeHandle();

	//configure and initialize the camera
	//according to Intel RS documentation, the max FPS of rgb stream is 60 
    	c.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, FPS);
    	c.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_RGB8, (FPS>60)?60:FPS);
    	p.start(c);
	
	//define publishers
	depth_pub = rs.advertise<sensor_msgs::Image>("depth", 8);
        rgb_pub = rs.advertise<sensor_msgs::Image>("RGB", 8);

	//use timer to trigger callback
    	timer = rs.createTimer(ros::Duration(1/FPS), &source::timerCallback, this);
	NODELET_INFO("Camera source node onInit called\n");
    }
	
	
    void source::timerCallback(const ros::TimerEvent& event){
	sensor_msgs::Image depth_msg;
	sensor_msgs::Image rgb_msg;
	rs2::frameset frames = p.wait_for_frames();
	double frame_timestamp =  frames.get_timestamp(); //realsense timestamp in ms

	//we need to convert from millisecond to a [second-nanosecond] format 
	unsigned int second = frame_timestamp/1000; //obtain the "second" part
	unsigned int nanosecond = (frame_timestamp-1000*(double)(second))*1e6; //obtain the "nanosecond" part
	
	rs2::depth_frame depth = frames.get_depth_frame();
	rs2::video_frame color = frames.get_color_frame();

	unsigned int width = depth.get_width();
	unsigned int height = depth.get_height();

	unsigned int width_color = color.get_width();
	unsigned int height_color = color.get_height();
    
	const uint8_t* pixel_ptr = (const uint8_t*)(depth.get_data());
	const uint8_t* pixel_ptr_color = (const uint8_t*)(color.get_data());

	unsigned int pixel_amount = width*height;
	unsigned int pixel_amount_color = width_color*height_color;

	//as for the size of the vector, since depth image is of mono16 format, one pixel corresponds to 2 bytes; RGB is of rgb8 format, where one pixel is consisted of 3 channels, 1 byte for each channel leads to a total of 3 bytes/pixel.
	std::vector<uint8_t> depth_image(2*pixel_amount);
	std::vector<uint8_t> color_image(3*pixel_amount_color);

	//copy the pixels to our vectors
	memcpy(&depth_image[0], pixel_ptr, 2*pixel_amount*sizeof(uint8_t));
	memcpy(&color_image[0], pixel_ptr_color, 3*pixel_amount_color*sizeof(uint8_t));

	//prepare our message
        depth_msg.header.frame_id = std::to_string(seq);//this is the sequence number since start of the programme
	depth_msg.header.stamp.sec = second ;
	depth_msg.header.stamp.nsec = nanosecond;
	depth_msg.data = depth_image;
	depth_msg.height = height;
	depth_msg.width = width;
	depth_msg.encoding = sensor_msgs::image_encodings::MONO16;
	depth_msg.step = width*2; //each pixel is 2 bytes, so step, which stands for row length in bytes, should be two times "width".

	rgb_msg.header.frame_id = std::to_string(seq);
	rgb_msg.header.stamp.sec = second ;
	rgb_msg.header.stamp.nsec = nanosecond ;
	rgb_msg.data = color_image;
	rgb_msg.height = height_color;
	rgb_msg.width = width_color;
	rgb_msg.encoding = sensor_msgs::image_encodings::RGB8;
	rgb_msg.step = width_color*3;//each pixel is 3 bytes - red, green, and blue

	depth_pub.publish(depth_msg);
	rgb_pub.publish(rgb_msg);

	NODELET_DEBUG("Both streams published!");
	seq++;
    }
}
