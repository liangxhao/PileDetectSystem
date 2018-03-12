#include "CameraParams.h"


bool CameraParams::writeCalibratorFiles(string &filename, cv::Size &imageSize, cv::Size &boardSize,
	cv::Mat &cameraMatrix, cv::Mat &distCoeffs,
	double &rms, double &totalAvgErr, vector<float> &reprojErrs)
{
	cv::FileStorage fs(filename, cv::FileStorage::WRITE); // Read the settings
	if (!fs.isOpened())
	{
		return false;
	}

	time_t tt;
	time(&tt);
	struct tm t2 = { 0 };
	localtime_s(&t2, &tt);
	char buf[1024];
	strftime(buf, sizeof(buf) - 1, "%c", &t2);

	fs << "calibration_time" << buf;

	fs << "image_width" << imageSize.width;
	fs << "image_height" << imageSize.height;
	fs << "board_width" << boardSize.width;
	fs << "board_height" << boardSize.height;
	fs << "square_size" << squareSize;

	fs << "Fix_k4_k5_k6" << IsFixMoreK;

	fs << "camera_matrix" << cameraMatrix;

	fs << "distortion_coefficients" << distCoeffs;

	fs << "RMS" << rms;

	fs << "avg_reprojection_error" << totalAvgErr;

	if (!reprojErrs.empty())
		fs << "per_view_reprojection_errors" << cv::Mat(reprojErrs);

	fs.release();

	return true;

}


bool CameraParams::readCalibratorFiles(string &filename, cv::Size &imageSize, cv::Mat &cameraMatrix, cv::Mat &distCoeffs)
{
	cv::FileStorage fs(filename, cv::FileStorage::READ); // Read the settings
	if (!fs.isOpened())
	{
		return false;
	}

	fs["image_width"] >> imageSize.width;
	fs["image_height"] >> imageSize.height;


	fs["camera_matrix"]>> cameraMatrix;

	fs["distortion_coefficients"] >> distCoeffs;

	fs.release();
	return true;
}