#pragma once
#include <iostream>
#include "ReadImageBlock.h"
#include "CircleInputFile.h"
#include "BlockCoords.h"
#include "FitSegmentCircle.h"


using namespace std;

class ExtractCircle
{
public:
	ExtractCircle(const char* pCoordsFile, const char* pImageName);
	~ExtractCircle();

public:
	void setExportFileName(const char* pExportFile);

	void run();


private:
	const char* pCoordsFile;//保存坐标的文件名
	const char* pExportFile;//输出结果文件名

	const char* pImageName;//图片文件名

	int currentIndex = 0;

	//图片尺寸
	int width;
	int height;

	vector<int> index;//保持每个圆的标号


	BlockCoords *block;//获取块坐标
	ReadImageBlock *blockImage;//获取块图像

	void prepare();
	void filter(vector<int> &xRcoords, vector<int> &yRcoords, vector<int> &radius);
	

};

