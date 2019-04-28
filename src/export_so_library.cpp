#include <pluginlib/class_list_macros.h>
#include <nodelet/nodelet.h>
#include "camera_nodelet/my_nodes.h"

PLUGINLIB_EXPORT_CLASS(camera::source, nodelet::Nodelet);
PLUGINLIB_EXPORT_CLASS(camera::drain, nodelet::Nodelet);
