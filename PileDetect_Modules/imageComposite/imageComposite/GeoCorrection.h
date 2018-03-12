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
	//����У���Ķ���ʽϵ�� n*6
	//x=a(0)+a(1)*X+a(2)*Y+a(3)*X^2+a(4)*X*Y+a(5)*Y^2
	//y=b(0)+b(1)*X+b(2)*Y+b(3)*X^2+b(4)*X*Y+b(5)*Y^2
	//a
	cv::Mat coeff_x;
	//b
	cv::Mat coeff_y;

	//ͼ�����귶Χ
	vector<LimitCoords> image_limits;

	//ͼƬ����
	vector<string> imageNames;
	//ͼ��ߴ�
	cv::Size imageSize;

	//��׺������ͼƬʱ��ԭͼ����λ�ã����Ϻ�׺����У�����ͼ��
	string suffix = "_cor.jpg";

private:

	//��idx��ͼ��ͨ�����㷨��������������Ӧ��ԭͼ����������
	//mapping_x��mapping_y�ǵõ���ӳ��ͼ������ɾ���Ϊ��remapʹ�ã�
	void coordsToMapping(int idx, const LimitCoords &limit, cv::Mat &mapping_x, cv::Mat &mapping_y);
	void geoCorrect();
	
public:
	void setImageNames(const vector<string> &imageNames);
	void setImageLimits(const cv::Mat &limits_x, const cv::Mat &limits_y);
	void setCoeff(const cv::Mat &coeff_x, const cv::Mat &coeff_y);
	void correctImage(const cv::Mat &image, cv::Mat correctImage, int idx);
};

