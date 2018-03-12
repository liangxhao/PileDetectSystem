#pragma once

#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "DetectPoints/emxTypes.h"
#include "DetectPoints/DetectCheckerboardPoints.h"
#include <time.h>
#include <thread>

#define  WM_PROCESS_STEP WM_USER+101

//#pragma comment(lib, "winmm.lib ")

using namespace std;
using namespace cv;

class CCameraCalibration
{

public:
	double squareSize = 30;

	double sigma = 2;
	double minCornerMetric = 0.15;

	Size boardSize;
	Size imageSize;
	vector<string> imageNameList;
	vector<vector<Point2f>> imagePointsList;
	vector<Point2i> boardSizeList;
	vector<bool> imageIsVaild;
	bool IsFixK3 = 0;
	bool IsSizeAuto = 0;

public:
	Mat cameraMatrix;
	Mat distCoeffs;
	double rms=-1;
	double totalAvgErr=0;
	vector<float> reprojErrs;

private:
	void matToArray(emxArray_uint8_T* &dst, cv::Mat &src);
	void detectCheckerboardPoints(emxArray_uint8_T *img, vector<Point2f> &imagePoints, double *boardSize);
	void chessboardWorldCorners(vector<Point3f>& corners);

	void drawChessboardCorners(InputOutputArray _image, Size patternSize, InputArray _corners,
		bool patternWasFound);

	void cvDrawChessboardCorners(CvArr* _image, CvSize pattern_size,
		CvPoint2D32f* corners, int count, int found);



	double computeReprojectionErrors(const vector<vector<Point3f>>& objectPoints,
		const vector<vector<Point2f> >& imagePoints,
		const vector<Mat>& rvecs, const vector<Mat>& tvecs,
		const Mat& cameraMatrix, const Mat& distCoeffs,
		vector<float>& perViewErrors);



	HWND hwnd;
	bool PointsOnImagesList();
	
	bool checkPointsSizeIsSame();
	void verticalSortPoints();

public:


	void drawPointsOnImage(Mat &img,string &imageName,int flag);//flag=1表示已经选取角点
	bool clcCalibration();
	int getNumImage();
	void autoGetChessSize();

	void runDetect(HWND &hwnd);
	void runCalibration(HWND &hwnd);

	bool calibrationSucess = false;
};

