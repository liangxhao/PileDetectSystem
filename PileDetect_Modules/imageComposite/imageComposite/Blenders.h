#pragma once

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "gdal.h"  
#include "gdal_priv.h" 

class Blenders
{


public:
	void setBlendersName(const char* pDstImgFileName) { this->pDstImgFileName = pDstImgFileName; }
	
	bool prepare(cv::Size size);
	void feed(cv::Mat &image, cv::Mat &mask, cv::Point2i &offset);
	bool blend();

private:
	//ͼ��ͨ����
	int nChannels = 3;
	//ͼ��ߴ�
	cv::Size imageSize;



	const char* pDstImgFileName;

	GDALDriver *pDriver = NULL;
	GDALDataset * pDataSet;

	//�Ƿ��ǵ�һ�ζ�дͼ��
	bool isFirstAccess = true;

	GDALRasterBand *pBand1 = NULL;
	GDALRasterBand *pBand2 = NULL;
	GDALRasterBand *pBand3 = NULL;



private:
	void readRaster(int dx, int dy, unsigned char* pdata1, unsigned char* pdata2,
		unsigned char* pdata3, int width, int height);
	void writeRaster(int dx, int dy, unsigned char* pdata1, unsigned char* pdata2,
		unsigned char* pdata3, int width, int height);


};

