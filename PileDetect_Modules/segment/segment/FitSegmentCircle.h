#pragma once

#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "./SegmentCircle/fitCircle.h"
#include "./SegmentCircle/fitCircle_emxAPI.h"

using namespace cv;

class FitSegmentCircle
{
public:
	FitSegmentCircle(Mat &img);
	~FitSegmentCircle();

public:
	void setPriorParms(int rx,int ry, int radius);
	void setSearchRatio(float ratio);

	int getCircle(int &objX, int &objY, int &objRadius);

private:
	Mat img;
	float searchRatio = 2.0;
	int rx;
	int ry;
	int radius;
};

