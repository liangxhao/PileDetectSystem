#include "ImageDesc.h"

//�Ƚ�min_x
bool compareX(ImageDesc &im1, ImageDesc &im2)
{
	return im1.limit.min_x < im2.limit.min_x;
}

//�Ƚ�min_y
bool compareY(ImageDesc &im1, ImageDesc &im2)
{
	return im1.limit.min_y < im2.limit.min_y;
}