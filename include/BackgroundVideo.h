#pragma once

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/TextureRectangle>
#include <osg/io_utils>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/aruco.hpp>
#include <Eigen/Dense>

#include <string>

using std::string;

class BackgroundVideoSource
{
	cv::VideoCapture videocapture;
	cv::Mat frame;
	cv::Mat cam_matrix;
	cv::Mat dist_coeffs;
	cv::Ptr<cv::aruco::Dictionary> dictionary;
	cv::Ptr<cv::aruco::DetectorParameters> detector_params;
	bool cam_paramaters_ok;

	bool readDetectorParameters(const string &filename);
	bool readCameraParameters(const string &filename);
public:
	BackgroundVideoSource();
	~BackgroundVideoSource();
	cv::Mat getFrame();
	cv::Mat getCamMatrix() {
		return cam_matrix;
	}
	bool getExternalWorldViewMatrix(osg::Matrix& matrix) const; //TODO: Move to Background class (below)
};

class Background
{
	int image_width;
	int image_height;

	osg::ref_ptr<osg::Camera> background_camera;
	osg::Image* background_image;

	BackgroundVideoSource backgroundVideoSource;

public:
	Background();

	void Update();

	const osg::Matrixd& getPhysicalCameraMatrix() {
		auto opencv_cam_matrix = backgroundVideoSource.getCamMatrix();

		osg::Matrixd osg_cam_matrix(
			2.0 * opencv_cam_matrix.at<double>(0, 0) / image_width, 0.0, 0.0, 0.0,
			0.0, -2.0 * opencv_cam_matrix.at<double>(1, 1) / image_height, 0.0, 0.0,
			-1.0 + 2.0 * opencv_cam_matrix.at<double>(0, 2) / image_width, -1.0 + 2.0 * (image_height - opencv_cam_matrix.at<double>(1, 2)) / image_height, 1.0, 1.0,
			0.0, 0.0, -1.0, 0.0);

		return osg_cam_matrix;
	}

	osg::ref_ptr<osg::Camera> getBackgroundCamera() {
		return background_camera;
	}

	osg::Image* getBackgroundImage() {
		return background_image;
	}

	int getImageWidth() const { return image_width; }
	int getImageHeight() const { return image_height; }

	bool getExternalWorldViewMatrix(osg::Matrix& matrix) const {
		return backgroundVideoSource.getExternalWorldViewMatrix(matrix);
	}
};
