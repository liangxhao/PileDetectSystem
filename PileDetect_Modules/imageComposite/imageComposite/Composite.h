#pragma once

#include "vector"
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "ImageDesc.h"
#include "Blenders.h"
#include "CameraParams.h"
#include <fstream>  

#ifndef ENABLE_LOG
#define ENABLE_LOG 1
#define LOGLN(msg) (cout<<msg << std::endl)
#endif

using namespace std;
//using namespace cv;
class Composite
{
private:
	//输出图像名称
	string compositeName;
	//图片名称
	vector<string> imageNames;
	//需要旋转的角度
	vector<double> imageAngles;
	//原始图像尺寸
	cv::Size imageSize=cv::Size(0,0);

	//叠加后的整图尺寸
	cv::Size compositeSize = cv::Size(0, 0);


	//旋转后的图像
	vector<ImageDesc> imageDesc;

	//大图像范围
	Point imageLimit_min;
	Point imageLimit_max;

	//cv::Point2f imageLimit_min;
	//cv::Point2f imageLimit_max;



	string cameraFile="";

private:
	void clcCompositeSize();
	void clcCompositeSizeX(vector<ImageDesc> &_imageDesc);
	void clcCompositeSizeY(vector<ImageDesc> &_imageDesc);

	void clcOffsets();
	void outputCoords();
	void getUndistMask(cv::Mat &mask, cv::Mat &mask_undist);
	void unifyResolution();

public:
	void setCompositeName(string compositeName) {this->compositeName = compositeName;}
	void setImageNames(const vector<string> &names);
	void setImageAngles(const vector<double> &angles);
	void setImageLimits(const cv::Mat &limits_x,const cv::Mat &limits_y);

	void setCameraFIle(string cameraFile) { this->cameraFile = cameraFile; }

	void compositeImage();

	//void outputCoords();
};

