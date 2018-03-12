#include "BlockCoords.h"



BlockCoords::BlockCoords()
{
}


BlockCoords::~BlockCoords()
{
}

void BlockCoords::setCoords(vector<int> &index, vector<int> &xRcoords, vector<int> &yRcoords, vector<int> &radius)
{
	this->index = index;
	this->xRcoords = xRcoords;
	this->yRcoords = yRcoords;
	this->radius = radius;

	this->numCircle =(int) this->index.size();
}

void BlockCoords::setImageSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

void BlockCoords::setExtendRatio(float ratio)
{
	this->extendRatio = ratio;
}

int BlockCoords::getNextBlockCoords(int &dx, int &dy, int &width, int &height, int &rx, int &ry)
{
	//如果遍历完毕，则返回-1
	if (this->currentIndex == this->numCircle)
	{
		return -1;
	}

	int x = this->xRcoords[this->currentIndex];
	int y = this->yRcoords[this->currentIndex];
	int r = this->radius[this->currentIndex];

	//扩展的半径大小
	int d = (int)r*this->extendRatio;

	//图像块在左上角坐标
	dx = x - d;
	dy = y - d;
	dx = dx >= 0 ? dx : 0;
	dy = dy >= 0 ? dy : 0;

	//图像块的右下角坐标
	int rbx = dx + 2 * d;
	int rby = dy + 2 * d;
	rbx = rbx < this->width ? rbx : this->width - 1;
	rby = rby < this->height ? rby : this->height - 1;

	width = rbx - dx;
	height = rby - dy;

	rx = x - dx;
	ry = y - dy;

	return this->currentIndex++;


}

int BlockCoords::getCurrentRadius()
{
	int index = this->currentIndex - 1;
	if (index >= 0 && index < this->radius.size())
	{
		return this->radius[index];
	}

	return 0;
	 
}