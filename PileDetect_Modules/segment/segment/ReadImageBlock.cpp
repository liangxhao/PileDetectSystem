#include "ReadImageBlock.h"

ReadImageBlock::ReadImageBlock(const char* pDstImgFileName)
{
	this->pDstImgFileName = pDstImgFileName;
}


ReadImageBlock::~ReadImageBlock()
{
	

}

void ReadImageBlock::getImageSize(int &width, int &height)
{
	if (this->pDataSet == NULL)
	{
		this->connectImage();
		this->end();
	}
	
	width = this->imgWidth;
	height = this->imgHeight;

}

void ReadImageBlock::connectImage()
{
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

	pDataSet = (GDALDataset *)GDALOpen(this->pDstImgFileName, GA_ReadOnly);
	this->imgWidth = pDataSet->GetRasterXSize();   //图像宽度
	this->imgHeight = pDataSet->GetRasterYSize();  //图像高度
	this->bandNum = pDataSet->GetRasterCount();    //波段数
}

//打开图像
void ReadImageBlock::prepare()
{
	
	this->connectImage();
	
	pBand1 = pDataSet->GetRasterBand(3);
	pBand2 = pDataSet->GetRasterBand(2);
	pBand3 = pDataSet->GetRasterBand(1);

}
void ReadImageBlock::end()
{
	GDALClose(pDataSet);
}

//读取图像，得到3个unsigned char*
void ReadImageBlock::readRaster(int dx, int dy, unsigned char* pdata1, unsigned char* pdata2,
	unsigned char* pdata3, int width, int height)
{
	pBand1->RasterIO(GF_Read, dx, dy, width, height, pdata1, width, height, GDT_Byte, 1, 0);

	pBand2->RasterIO(GF_Read, dx, dy, width, height, pdata2, width, height, GDT_Byte, 1, 0);

	pBand3->RasterIO(GF_Read, dx, dy, width, height, pdata3, width, height, GDT_Byte, 1, 0);
}


void ReadImageBlock::read(int dx, int dy, int width, int height, cv::Mat& img)
{

	int length = width*height;
	unsigned char* pdata1 = new unsigned char[length];
	unsigned char* pdata2 = new unsigned char[length];
	unsigned char* pdata3 = new unsigned char[length];

	unsigned char* pd1 = pdata1;
	unsigned char* pd2 = pdata2;
	unsigned char* pd3 = pdata3;


	this->readRaster(dx, dy, pdata1, pdata2, pdata3, width, height);


	img.create( height, width, CV_8UC3);
	img.setTo(cv::Scalar(0, 0, 0));

	int nr = img.rows; // number of rows
	int nc = img.cols * img.channels(); // total number of elements per line

	for (int j = 0; j < nr; j++) {
		uchar* data = img.ptr<uchar>(j);

		for (int i = 0; i < nc; i+=3) {
			*data++ = *pd1++;
			*data++ = *pd2++;
			*data++ = *pd3++;
		}               
	}



	if (NULL != pdata1)
	{
		delete[] pdata1;
		pdata1 = NULL;
	}

	if (NULL != pdata2)
	{
		delete[] pdata2;
		pdata2 = NULL;
	}

	if (NULL != pdata3)
	{
		delete[] pdata3;
		pdata3 = NULL;
	}

}
