#include "stdafx.h"
#include "opencv2/stitching/detail/util.hpp"
#include "Blenders.h"
#include "util_log.h"
#include "opencv2/highgui.hpp"
namespace st {
	Ptr<Blenders> Blenders::createDefault(int type)
	{
		if (type == NO)
			return makePtr<Blenders>();

		CV_Error(Error::StsBadArg, "unsupported blending method");
		return Ptr<Blenders>();
	}

	bool Blenders::prepare(const std::vector<Point> &corners, const std::vector<Size> &sizes)
	{
		return prepare(detail::resultRoi(corners, sizes));
	}

	char* Blenders::findImageTypeGDAL(const char *pDstImgFileName)
	{
		char *dstExtension = _strlwr(strrchr((char*)pDstImgFileName, '.') + 1);
		char *Gtype = NULL;

		if (0 == strcmp(dstExtension, "jpg")) Gtype = "JPEG";
		else if (0 == strcmp(dstExtension, "png")) Gtype = "PNG";
		else if (0 == strcmp(dstExtension, "tif")) Gtype = "GTiff";

		else Gtype = NULL;

		return Gtype;
	}


	bool Blenders::prepare(Rect dst_roi)
	{

		GDALAllRegister();
		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
		//char *GType = NULL;
		//GType = findImageTypeGDAL(pDstImgFileName);
		//if (GType == NULL)
		//{ 
		//	LOGLN(" unsupported file suffixes");
		//	return false; 
		//}

		
		pDriver = GetGDALDriverManager()->GetDriverByName("GTIFF");
		if (pDriver == NULL) 
		{ 
			LOGLN(" Create image failed");
			return false; 
		}

		dst_roi_ = dst_roi;
		char **ppMetaData = NULL;
		ppMetaData=CSLSetNameValue(ppMetaData, "BIGTIFF", "IF_NEEDED");
		
		pDataSet = pDriver->Create(pDstImgFileName, dst_roi.width, dst_roi.height, nChannels, GDT_Byte, ppMetaData);
		

	    pBand1 = pDataSet->GetRasterBand(3);
		pBand2 = pDataSet->GetRasterBand(2);
		pBand3 = pDataSet->GetRasterBand(1);
		return true;
	}

	
	void Blenders::feed(InputArray _img, InputArray _mask, Point tl)
	{
		try {
			feedImage(_img, _mask, tl);
		}

		catch (...)
		{

			LOGLN("feed current image failed! ")

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
	}


	void Blenders::feedImage(InputArray _img, InputArray _mask, Point tl)
	{
		Mat img = _img.getMat();

		int width = img.cols;
		int height = img.rows;

		int dx = tl.x - dst_roi_.x;
		int dy = tl.y - dst_roi_.y;

		int length = width*height;
		unsigned char* pdata1 = new unsigned char[length];
		unsigned char* pdata2 = new unsigned char[length];
		unsigned char* pdata3 = new unsigned char[length];


		unsigned char* pd1 = pdata1;
		unsigned char* pd2 = pdata2;
		unsigned char* pd3 = pdata3;

		if (IsFirstAccess)
		{
			memset(pdata1, 0, length);
			memset(pdata2, 0, length);
			memset(pdata3, 0, length);
			IsFirstAccess = false;
		}
		else
		{
			readRaster(dx, dy, pdata1, pdata2, pdata3, width, height);

		}


		Mat mask = _mask.getMat();

		for (int y = 0; y < height; y++)
		{
			const Point3_<short> *src_row = img.ptr<Point3_<short> >(y);
			uchar* mask_row = mask.ptr<uchar>(y);

			for (int x = 0; x < width; x++)
			{
				if (mask_row[x])
				{
					*pd1 = (uchar)src_row[x].x;
					*pd2 = (uchar)src_row[x].y;
					*pd3 = (uchar)src_row[x].z;
				}

				pd1++;
				pd2++;
				pd3++;
			}
		}

		writeRaster(dx, dy, pdata1, pdata2, pdata3, width, height);


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

	bool Blenders::blend()
	{
		//char *GType = findImageTypeGDAL(pDstImgFileName);
		//GDALDriver *pDstDriver = NULL;

		//pDstDriver = (GDALDriver *)GDALGetDriverByName(GType);
		//if (pDstDriver == NULL) {
		//	LOGLN(" Create image failed");
		//	return false;
		//}

		//pDstDriver->CreateCopy(pDstImgFileName, pMemDataSet, FALSE, NULL, NULL, NULL);

		GDALClose(pDataSet);

		return true;
	}


	void Blenders::readRaster(int dx, int dy, unsigned char* pdata1, unsigned char* pdata2,
								unsigned char* pdata3 ,int width, int height)
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

}