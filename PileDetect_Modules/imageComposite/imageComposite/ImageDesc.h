#pragma once
#include "opencv2/core.hpp"

//ͼ��Χ
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

//ͼ��λ��
struct ImageDesc
{
	//��ת���ͼ��ߴ��ٵ���
	cv::Size sz;
	//��ת���ͼ��ߴ�
	cv::Size rotateSz;

	LimitCoords limit;

	//�������ͼ��ƫ����
	int dx;
	int dy;

	//ͼ�����Էֱ���
	double scale = 1.0;
};


struct Point
{
	Point() { this->x = 0; this->y = 0; }
	Point(double x, double y) { this->x = x; this->y = y; }

	double x;
	double y;
};


//�Ƚ�min_x
bool compareX(ImageDesc &im1, ImageDesc &im2);



//�Ƚ�min_y
bool compareY(ImageDesc &im1, ImageDesc &im2);


