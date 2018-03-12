#pragma once
#include <vector>
using namespace std;

class BlockCoords
{
public:
	BlockCoords();
	~BlockCoords();
public:
	void setCoords(vector<int> &index, vector<int> &xRcoords, vector<int> &yRcoords, vector<int> &radius);
	void setImageSize(int width, int height);
	void setExtendRatio(float ratio);
	//相对于大图的坐标dx、dy;图像块的尺寸w、h;圆心相对于图像块的坐标rx、ry
	int getNextBlockCoords(int &dx, int &dy, int &width, int &height,int &rx,int &ry);
	int getCurrentRadius();

private:
	//描述圆的坐标，相对于大图
	vector<int> index;
	vector<int> xRcoords;
	vector<int> yRcoords;
	vector<int> radius;
	//图像大小
	int width;
	int height;

	//返回当前的块坐标的序号
	int currentIndex = 0;
	int numCircle = 0;

	//在半径的基础上扩大的比例
	float extendRatio = 3.0;

};

