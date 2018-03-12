#pragma once

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "gdal.h"  
#include "gdal_priv.h" 


using namespace cv;
namespace st {

	class CV_EXPORTS Blenders
	{
	public:
		virtual ~Blenders() {}

		enum { NO, Other };
		static Ptr<Blenders> createDefault(int type);
		char* findImageTypeGDAL(const char *pDstImgFileName);

		bool prepare(const std::vector<Point> &corners, const std::vector<Size> &sizes);
		/** @overload */
		virtual bool prepare(Rect dst_roi);


		/** @brief Processes the image.
		@param img Source image
		@param mask Source image mask
		@param tl Source image top-left corners
		*/
		virtual void feed(InputArray img, InputArray mask, Point tl);

		

		/** @brief Blends and returns the final pano.
		@param dst Final pano
		*/
		//virtual void blend(UMat& dst);

		virtual void readRaster(int dx, int dy, unsigned char* pdata1, unsigned char* pdata2,
			unsigned char* pdata3, int width, int height);

		virtual void writeRaster(int dx, int dy, unsigned char* pdata1, unsigned char* pdata2,
			unsigned char* pdata3, int width, int height);

		virtual bool blend();


		void setOutputFileName(const char* dst_name) { pDstImgFileName = dst_name; }
		void setChannels(int nChannels) { this->nChannels = nChannels; }

	protected:
		virtual void feedImage(InputArray img, InputArray mask, Point tl);
		//int width;
		//int height;
		
		bool IsFirstAccess=true;
		int nChannels;

		Rect dst_roi_;

		const char* pDstImgFileName;

		GDALDriver *pDriver = NULL;
		//GDALDriver *pDstDriver = NULL;
		GDALDataset * pDataSet;


		GDALRasterBand *pBand1 = NULL;
		GDALRasterBand *pBand2 = NULL;
		GDALRasterBand *pBand3 = NULL;


		unsigned char* pdata1 = NULL;
		unsigned char* pdata2 = NULL;
		unsigned char* pdata3 = NULL;

	};

}

