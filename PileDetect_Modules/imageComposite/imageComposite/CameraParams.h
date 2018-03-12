#pragma once
#include <opencv2/core.hpp>
#include <iostream>
#include <time.h>

using namespace std;


class CameraParams
{

private:
	float squareSize = 0;
	int IsFixMoreK = 0;

public:

	bool writeCalibratorFiles(string &filename, cv::Size &imageSize, cv::Size &boardSize, cv::Mat &cameraMatrix, cv::Mat &distCoeffs,
		double &rms, double &totalAvgErr, vector<float> &reprojErrs);
	bool readCalibratorFiles(string &filename, cv::Size &imageSize, cv::Mat &cameraMatrix, cv::Mat &distCoeffs);
};
