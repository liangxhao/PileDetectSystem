#pragma once

#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/calib3d.hpp>
#include "omp.h"
#include <shlwapi.h>

#include <thread>
#pragma comment(lib,"Shlwapi.lib") 

#define  WM_PROCESS_STEP WM_USER+101

using namespace std;
using namespace cv;

class CUndistorts
{
public:
	vector<String> imageNameList;
	Size imageSize;
	Mat cameraMatrix;
	Mat distCoeffs;

	String exportPath;
private:
	bool imageUndistort();
	HWND hwnd;
	
public:
	
	int getImageNum();

	bool runUndistort(HWND &Hwnd);
};

