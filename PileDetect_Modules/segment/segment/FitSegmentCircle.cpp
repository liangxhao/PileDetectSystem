#include "FitSegmentCircle.h"
void MatToArray(cv::Mat &src, emxArray_uint8_T* &dst)
{
	int iv0[2] = { src.rows, src.cols };
	dst = emxCreateND_uint8_T(2, *(int(*)[2])&iv0[0]);
	//自定义的类型数据是按列顺序放到一行上面存储
	for (int idx0 = 0; idx0 < dst->size[0U]; idx0++) {
		for (int idx1 = 0; idx1 < dst->size[1U]; idx1++) {
			dst->data[idx0 + dst->size[0] * idx1] = src.ptr<uchar>(idx0)[idx1];
		}
	}
}


FitSegmentCircle::FitSegmentCircle(Mat &img)
{
	this->img = img;
}


FitSegmentCircle::~FitSegmentCircle()
{
	this->img.release();
}

void FitSegmentCircle::setPriorParms(int rx,int ry, int radius)
{
	this->rx = rx;
	this->ry = ry;
	this->radius = radius;

}

void FitSegmentCircle::setSearchRatio(float ratio)
{
	this->searchRatio = ratio;
}


int FitSegmentCircle::getCircle(int &objX, int &objY, int &objRadius)
{
	fitCircle_initialize();
	//数据格式转换
	emxArray_uint8_T *arryImg = nullptr;
	MatToArray(this->img, arryImg);
	
	double _objX;
	double _objY;
	double _objRadius;
	double _flag;


	fitCircle(arryImg, this->rx, this->ry, this->radius,
		this->searchRatio, &_objX, &_objY, &_objRadius, &_flag);


	emxDestroyArray_uint8_T(arryImg);

	objX = (int)_objX;
	objY = (int)_objY;
	objRadius = (int)_objRadius;


	fitCircle_terminate();

	return (int)_flag;
}
