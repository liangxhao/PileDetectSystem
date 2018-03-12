#include "Blenders.h"





bool Blenders::prepare(cv::Size size)
{
	this->imageSize = size;
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");


	pDriver = GetGDALDriverManager()->GetDriverByName("GTIFF");
	if (pDriver == NULL)
	{
		return false;
	}

	
	char **ppMetaData = NULL;
	ppMetaData = CSLSetNameValue(ppMetaData, "BIGTIFF", "IF_NEEDED");

	pDataSet = pDriver->Create(pDstImgFileName, this->imageSize.width, this->imageSize.height, this->nChannels, GDT_Byte, ppMetaData);


	pBand1 = pDataSet->GetRasterBand(3);
	pBand2 = pDataSet->GetRasterBand(2);
	pBand3 = pDataSet->GetRasterBand(1);

	return true;
}


void Blenders::feed(cv::Mat &image, cv::Mat &mask, cv::Point2i &offset)
{
	//resize(image,image,cv::Size(image.rows / 5, image.cols / 5));
	//cv::imshow("image", image);
	//cv::waitKey(0);

	//Æ«ÒÆ
	int dx = offset.x;
	int dy = offset.y;

	//Í¼Ïñ³ß´ç
	int width = image.cols;
	int height = image.rows;

	int length = width*height;
	unsigned char* pdata1 = new unsigned char[length];
	unsigned char* pdata2 = new unsigned char[length];
	unsigned char* pdata3 = new unsigned char[length];

	unsigned char* pd1 = pdata1;
	unsigned char* pd2 = pdata2;
	unsigned char* pd3 = pdata3;

	if (this->isFirstAccess)
	{
		memset(pdata1, 0, length);
		memset(pdata2, 0, length);
		memset(pdata3, 0, length);
		this->isFirstAccess = false;
	}
	else
	{
		readRaster(dx, dy, pdata1, pdata2, pdata3, width, height);
	}


	int image_step = image.step;
	uchar *pImage = image.data;

	int mask_step = mask.step;
	uchar *pMask = mask.data;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{

			// ÔÝÊ±ÆúÓÃ
			/*
			if(pMask[j])
			{
				*pd1 = pImage[3 * j];
				*pd2 = pImage[3 * j + 1];
				*pd3 = pImage[3 * j + 2];
			}

			pd1++;
			pd2++;
			pd3++;
			*/

			if (pImage[3 * j] != 0 && pImage[3 * j + 1]!=0 && pImage[3 * j + 2]!=0)
			{
				*pd1 = pImage[3 * j];
				*pd2 = pImage[3 * j + 1];
				*pd3 = pImage[3 * j + 2];
			}
			pd1++;
			pd2++;
			pd3++;
			

		}
		pImage += image_step;
		pMask += mask_step;
	}



	writeRaster(dx, dy, pdata1, pdata2, pdata3, width, height);


	if (NULL != pdata1)
	{
		delete[] pdata1;
		pdata1 = NULL;
		pd1 = NULL;
	}

	if (NULL != pdata2)
	{
		delete[] pdata2;
		pdata2 = NULL;
		pd2 = NULL;
	}

	if (NULL != pdata3)
	{
		delete[] pdata3;
		pdata3 = NULL;
		pd3 = NULL;
	}

}


void Blenders::readRaster(int dx, int dy, unsigned char* pdata1, unsigned char* pdata2,
	unsigned char* pdata3, int width, int height)
{

	pBand1->RasterIO(GF_Read, dx, dy, width, height, pdata1, width, height, GDT_Byte, 1, 0);

	pBand2->RasterIO(GF_Read, dx, dy, width, height, pdata2, width, height, GDT_Byte, 1, 0);

	pBand3->RasterIO(GF_Read, dx, dy, width, height, pdata3, width, height, GDT_Byte, 1, 0);

}


void Blenders::writeRaster(int dx, int dy, unsigned char* pdata1, unsigned char* pdata2,
	unsigned char* pdata3, int width, int height)
{

	pBand1->RasterIO(GF_Write, dx, dy, width, height, pdata1, width, height, GDT_Byte, 1, 0);

	pBand2->RasterIO(GF_Write, dx, dy, width, height, pdata2, width, height, GDT_Byte, 1, 0);

	pBand3->RasterIO(GF_Write, dx, dy, width, height, pdata3, width, height, GDT_Byte, 1, 0);

}

bool Blenders::blend()
{
	
	GDALClose(pDataSet);
	return true;

}