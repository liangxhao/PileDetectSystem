#pragma once
#include "opencv2/core.hpp"

//图像范围
struct LimitCoords
{
	double min_x;
	double max_x;
	double min_y;
	double max_y;

	void setLimitX(double min_x, double max_x)
	{
		this->min_x = min_x;
		this->max_x = max_x;
	}

	void setLimitY(double min_y, double max_y)
	{
		this->min_y = min_y;
		this->max_y = max_y;
	}

	//double getXLimit()
	//{
	//	return max_x - min_x;
	//}

	//double getYLimit()
	//{
	//	return max_y - min_y;
	//}
};

//图像位置
struct ImageDesc
{
	//旋转后的图像尺寸再调整
	cv::Size sz;
	//旋转后的图像尺寸
	cv::Size rotateSz;

	LimitCoords limit;

	//相对于整图的偏移量
	int dx;
	int dy;

	//图像的相对分辨率
	double scale = 1.0;
};


struct Point
{
	Point() { this->x = 0; this->y = 0; }
	Point(double x, double y) { this->x = x; this->y = y; }

	double x;
	double y;
};


//比较min_x
bool compareX(ImageDesc &im1, ImageDesc &im2);



//比较min_y
bool compareY(ImageDesc &im1, ImageDesc &im2);


