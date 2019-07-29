#include "BackgroundVideo.h"

#include <vector>
#include <opencv2/aruco.hpp>
#include <opencv2/highgui.hpp>

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

using std::vector;

const int image_width = 256;
const int image_height = 170;
const float marker_length = 1.0f;
const int aruco_dictionary_id = 4;
const int aruco_label_id = 27;
const string detector_params_filename = "C:/p/src/HWOSGOCV/HWOSG-Build/cam-calib.yml";
//unsigned char image_data[3 * image_width * image_height];

BackgroundVideoSource::BackgroundVideoSource() :
	videocapture(0)
{
	if (!videocapture.isOpened())
	{
		frame = cv::Mat(image_height, image_width, CV_8UC3);
		cv::randu(frame, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
	}
	else {
		videocapture >> frame;
	}
	dictionary =
		cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME(aruco_dictionary_id));

	//readDetectorParameters();
	detector_params = cv::aruco::DetectorParameters::create();

	cam_paramaters_ok = readCameraParameters(detector_params_filename);
}


BackgroundVideoSource::~BackgroundVideoSource()
{
}

cv::Mat BackgroundVideoSource::getFrame()
{
	if (!videocapture.isOpened())
	{
		cv::randu(frame, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
	}
	else {
		videocapture >> frame;

		//Debug: project 0,0,10
		vector<cv::Point3f> points3d = { cv::Point3f(0.0f, 0.0f, 10.0f) };
		vector<cv::Point3f> tvec = { cv::Point3f(0.0f, 0.0f, 0.0f) };
		vector<cv::Point3f> rvec = { cv::Point3f(0.0f, 0.0f, 0.0f) };
		vector<cv::Point2f> points2d;
		vector<double> distortion_coefficients;
		//TODO Check with and without distortion coefficients
		//projectPoints(InputArray objectPoints, InputArray rvec, InputArray tvec, InputArray cameraMatrix, InputArray distCoeffs, OutputArray imagePoints, OutputArray jacobian=noArray(), double aspectRatio=0 )
		cv::projectPoints(points3d, rvec, tvec, cam_matrix, distortion_coefficients, points2d);

		cv::Point left(points2d[0].x - 50, points2d[0].y), 
			right(points2d[0].x + 50, points2d[0].y), 
			up(points2d[0].x, points2d[0].y - 50), 
			down(points2d[0].x, points2d[0].y + 50);
		cv::line(frame, left, right, cv::Scalar(0, 255, 0));
		cv::line(frame, up, down, cv::Scalar(0, 255, 0));
			
		cv::line(frame, cv::Point(0,0), cv::Point(0,479), cv::Scalar(0, 255, 0));
		cv::line(frame, cv::Point(0, 0), cv::Point(639, 0), cv::Scalar(0, 255, 0));
		cv::line(frame, cv::Point(639, 479), cv::Point(639, 0), cv::Scalar(0, 255, 0));
		cv::line(frame, cv::Point(639, 479), cv::Point(0, 479), cv::Scalar(0, 255, 0));
	}
	return frame;
}

bool BackgroundVideoSource::readCameraParameters(const string &filename) {
	cv::FileStorage fs(filename, cv::FileStorage::READ);
	if (!fs.isOpened())
		return false;
	fs["camera_matrix"] >> cam_matrix;
	fs["distortion_coefficients"] >> dist_coeffs;
	return true;
}

/**
 */
bool BackgroundVideoSource::readDetectorParameters(const string &filename) {
	cv::FileStorage fs(filename, cv::FileStorage::READ);
	if (!fs.isOpened())
		return false;
	fs["adaptiveThreshWinSizeMin"] >> detector_params->adaptiveThreshWinSizeMin;
	fs["adaptiveThreshWinSizeMax"] >> detector_params->adaptiveThreshWinSizeMax;
	fs["adaptiveThreshWinSizeStep"] >> detector_params->adaptiveThreshWinSizeStep;
	fs["adaptiveThreshConstant"] >> detector_params->adaptiveThreshConstant;
	fs["minMarkerPerimeterRate"] >> detector_params->minMarkerPerimeterRate;
	fs["maxMarkerPerimeterRate"] >> detector_params->maxMarkerPerimeterRate;
	fs["polygonalApproxAccuracyRate"] >> detector_params->polygonalApproxAccuracyRate;
	fs["minCornerDistanceRate"] >> detector_params->minCornerDistanceRate;
	fs["minDistanceToBorder"] >> detector_params->minDistanceToBorder;
	fs["minMarkerDistanceRate"] >> detector_params->minMarkerDistanceRate;
	fs["cornerRefinementMethod"] >> detector_params->cornerRefinementMethod;
	fs["cornerRefinementWinSize"] >> detector_params->cornerRefinementWinSize;
	fs["cornerRefinementMaxIterations"] >> detector_params->cornerRefinementMaxIterations;
	fs["cornerRefinementMinAccuracy"] >> detector_params->cornerRefinementMinAccuracy;
	fs["markerBorderBits"] >> detector_params->markerBorderBits;
	fs["perspectiveRemovePixelPerCell"] >> detector_params->perspectiveRemovePixelPerCell;
	fs["perspectiveRemoveIgnoredMarginPerCell"] >> detector_params->perspectiveRemoveIgnoredMarginPerCell;
	fs["maxErroneousBitsInBorderRate"] >> detector_params->maxErroneousBitsInBorderRate;
	fs["minOtsuStdDev"] >> detector_params->minOtsuStdDev;
	fs["errorCorrectionRate"] >> detector_params->errorCorrectionRate;
	return true;
}


Background::Background()
{
	//Set up background image
	background_camera = new osg::Camera();

	background_camera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	background_camera->setViewMatrix(osg::Matrix::identity());
	background_camera->setProjectionMatrix(osg::Matrix::ortho2D(-1.0, 1.0, -1.0, 1.0f));
	background_camera->setProjectionResizePolicy(osg::Camera::ProjectionResizePolicy::FIXED);
	background_camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	background_camera->setRenderOrder(osg::Camera::NESTED_RENDER);
	background_camera->setAllowEventFocus(false);
	osg::StateSet* cameraStateSet = background_camera->getOrCreateStateSet();
	cameraStateSet->setRenderBinDetails(1, "RenderBin");
	cameraStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	auto stateset = geode->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	background_image = new osg::Image();
	cv::Mat frame = backgroundVideoSource.getFrame();

	image_width = frame.cols;
	image_height = frame.rows;

	background_image->setImage(frame.cols, frame.rows, 3,
		GL_RGB, GL_BGR, GL_UNSIGNED_BYTE,
		(uchar*)(frame.data),
		osg::Image::AllocationMode::NO_DELETE, 1);
	background_image->dirty();

	auto texturedQuad = osg::createTexturedQuadGeometry(
		osg::Vec3(-1.0f, 1.0f, 0.f),
		osg::Vec3(2.0f, 0.0f, 0.f),
		osg::Vec3(0.0f, -2.0f, 0.f),
		0.f,
		0.f,
		background_image->s(),
		background_image->t());
	auto textureRect = new osg::TextureRectangle(background_image);
	textureRect->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	textureRect->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	textureRect->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
	textureRect->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
	texturedQuad->getOrCreateStateSet()->setTextureAttributeAndModes(0, textureRect, osg::StateAttribute::ON);
	texturedQuad->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

	geode->addDrawable(texturedQuad);

	if (!background_camera->addChild(geode))
	{
		osg::notify(osg::NotifySeverity::WARN) << "Failed to add geode" << std::endl;
	}
}

void Background::Update() {
	cv::Mat frame = backgroundVideoSource.getFrame();
	background_image->setImage(frame.cols, frame.rows, 3,
		GL_RGB, GL_BGR, GL_UNSIGNED_BYTE,
		(uchar*)(frame.data),
		osg::Image::AllocationMode::NO_DELETE, 1);
	background_image->dirty();
}


bool BackgroundVideoSource::getExternalWorldViewMatrix(osg::Matrix& matrix) const
{
	vector< int > ids;
	vector< vector< cv::Point2f > > corners, rejected;
	vector< cv::Vec3d > rvecs, tvecs;

	// detect markers and estimate pose
	cv::aruco::detectMarkers(frame, dictionary, corners, ids, detector_params, rejected);
	if (ids.size() > 0) {
		//FIXME: Only estimate posion for target aruco tag
		cv::aruco::estimatePoseSingleMarkers(corners, marker_length, cam_matrix, dist_coeffs, rvecs,
			tvecs);
	}
	else {
		return false; //didn't find any ARUCO tags 
	}

	int found_idx = -1;
	for (int i = 0; i < ids.size(); ++i) {
		if (ids[i] == aruco_label_id) {
			found_idx = i;
			break;
		}
	}
	if (found_idx == -1) {
		return false; //didn't find target ARUCO tag
	}

	cv::Vec3d translation_vector = tvecs[found_idx];
	cv::Vec3d rotation_vector = rvecs[found_idx];

	//FIXME: It requires taking  cam_matrix and  into account

	double angle = sqrt(rotation_vector[0] * rotation_vector[0] +
		rotation_vector[1] * rotation_vector[1] +
		rotation_vector[2] * rotation_vector[2]);

	osg::Vec3d axis(rotation_vector[0] / angle,
		rotation_vector[1] / angle,
		rotation_vector[2] / angle);

	matrix = osg::Matrix::rotate(angle, axis); 

	auto translation = osg::Matrix::translate(translation_vector[0], translation_vector[1], translation_vector[2]);
	matrix *= translation;

	return true;
}