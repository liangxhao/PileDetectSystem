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
	//���ͼ������
	string compositeName;
	//ͼƬ����
	vector<string> imageNames;
	//��Ҫ��ת�ĽǶ�
	vector<double> imageAngles;
	//ԭʼͼ��ߴ�
	cv::Size imageSize=cv::Size(0,0);

	//���Ӻ����ͼ�ߴ�
	cv::Size compositeSize = cv::Size(0, 0);


	//��ת���ͼ��
	vector<ImageDesc> imageDesc;

	//��ͼ��Χ
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

