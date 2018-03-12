#pragma once
#include <opencv2/opencv.hpp>
#include "ImageDesc.h"
using namespace std;

#ifndef ENABLE_LOG
#define ENABLE_LOG 1
#define LOGLN(msg) (cout<<msg << std::endl)
#endif


class GeoCorrection
{

private:
	//几何校正的多项式系数 n*6
	//x=a(0)+a(1)*X+a(2)*Y+a(3)*X^2+a(4)*X*Y+a(5)*Y^2
	//y=b(0)+b(1)*X+b(2)*Y+b(3)*X^2+b(4)*X*Y+b(5)*Y^2
	//a
	cv::Mat coeff_x;
	//b
	cv::Mat coeff_y;

	//图像坐标范围
	vector<LimitCoords> image_limits;

	//图片名称
	vector<string> imageNames;
	//图像尺寸
	cv::Size imageSize;

	//后缀，保存图片时在原图所在位置，加上后缀保存校正后的图像
	string suffix = "_cor.jpg";

private:

	//第idx张图，通过反算法，计算地理坐标对应在原图的像素坐标
	//mapping_x、mapping_y是得到的映射图（保存成矩阵，为了remap使用）
	void coordsToMapping(int idx, const LimitCoords &limit, cv::Mat &mapping_x, cv::Mat &mapping_y);
	void geoCorrect();
	
public:
	void setImageNames(const vector<string> &imageNames);
	void setImageLimits(const cv::Mat &limits_x, const cv::Mat &limits_y);
	void setCoeff(const cv::Mat &coeff_x, const cv::Mat &coeff_y);
	void correctImage(const cv::Mat &image, cv::Mat correctImage, int idx);
};

