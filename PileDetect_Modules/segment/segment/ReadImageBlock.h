#pragma warning(disable: 4251)
#pragma once
#include "opencv2/core.hpp"

#include "gdal.h"  
#include "gdal_priv.h" 



class ReadImageBlock
{
public:
	ReadImageBlock(const char* pDstImgFileName);
	~ReadImageBlock();

	void prepare();
	void read(int dx, int dy, int width, int height, cv::Mat& img);
	void end();


	void getImageSize(int &width, int &height);


private:
	void readRaster(int dx, int dy, unsigned char* pdata1, unsigned char* pdata2,
		unsigned char* pdata3, int width, int height);
	void connectImage();

private:
	const char *pDstImgFileName;//ͼƬ����

	GDALDataset * pDataSet;

	int imgWidth=0; //ͼ����
	int imgHeight=0;  //ͼ��߶�
	int bandNum=0; //������

	//��������
	GDALRasterBand *pBand1 = NULL;
	GDALRasterBand *pBand2 = NULL;
	GDALRasterBand *pBand3 = NULL;
};

