#ifndef _LOCALIZATION_H_
#define _LOCALIZATION_H_

#include <ros/ros.h>
#include <ros/package.h>

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>

#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <tf/tf.h>

#include <sensor_msgs/Image.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/MagneticField.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/FluidPressure.h>
#include <nav_msgs/Path.h>

#include <opencv2/opencv.hpp>

#include "sensor_msgs/Joy.h"

#include "SLAM.h"
#include "SLAM_includes.h"
#include "InterfaceStructs.h"

//#include "onboard_localization/ControllerOut.h"

#include <dynamic_reconfigure/server.h>
#include <vio_ros/vio_rosConfig.h>

#include <duo3d_ros/Duo3d.h>
#include <vio_ros/vio_vis.h>

#include <vector>
#include <cstdio>

#include <boost/circular_buffer.hpp>

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"

#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/UInt32.h"

#include "onboard_localization/PositionReference.h"

#include "Precision.h"

class Localization
{
public:
	Localization();
	~Localization();

private:
	// Visualization topics
	ros::Publisher vio_vis_pub;
	ros::Publisher vio_vis_reset_pub;

	int vis_publish_delay;
	bool SLAM_reset_flag;
	int display_tracks_cnt;
	int max_clicks_;
	int clear_queue_counter;
	double fps_duo;
	int vio_cnt;
	int vision_subsample;

	cv::Mat darkCurrentL;
	cv::Mat darkCurrentR;
	bool use_dark_current;

	DUOParameters cameraParams;
	NoiseParameters noiseParams;
	ControllerGains controllerGains;
	VIOParameters vioParams;
	ros::NodeHandle nh_;

	ros::Subscriber duo_sub;
	ros::Subscriber joy_sub_;
	ros::Subscriber position_reference_sub_;

	ros::Publisher controller_pub;
	ros::Publisher duo_processed_pub;
	dynamic_reconfigure::Server<vio_ros::vio_rosConfig> dynamic_reconfigure_server;

	ros::Publisher timing_SLAM_pub;
	ros::Publisher timing_feature_tracking_pub;
	ros::Publisher timing_total_pub;

	ros::Time prev_time_;
	std::vector<int> update_vec_;
	geometry_msgs::Pose pose;

	unsigned int num_points_;
	bool show_camera_image_;
	int image_visualization_delay;
	RobotState robot_state;

	std::vector<FloatType> h_u_apo;
	std::vector<FloatType> map;
	std::vector<AnchorPose> anchor_poses;

	void duo3dCb(const duo3d_ros::Duo3d& msg);
	void joystickCb(const sensor_msgs::Joy::ConstPtr& msg);
	void positionReferenceCb(const onboard_localization::PositionReference& msg);

	void update(double dt, const duo3d_ros::Duo3d &msg, bool debug_publish, bool show_image);

	void getIMUData(const sensor_msgs::Imu& imu, VIOMeasurements& meas);

	ros::Publisher vis_pub_;
	void updateVis(RobotState &robot_state, std::vector<AnchorPose> &anchor_poses, std::vector<FloatType> &map, std::vector<int> &updateVect, const duo3d_ros::Duo3d &duo_msg, std::vector<FloatType> &z_l, bool show_image);

	ReferenceCommand referenceCommand;
	bool change_reference;
	tf::Quaternion camera2world; // the rotation that transforms a vector in the camera frame to one in the world frame

	tf::TransformListener tf_listener_;
	void getViconPosition(void);
	std::vector<double> vicon_pos;
	std::vector<double> vicon_quaternion;

	void dynamicReconfigureCb(vio_ros::vio_rosConfig &config, uint32_t level);

};

#endif /* _LOCALIZATION_H_ */
