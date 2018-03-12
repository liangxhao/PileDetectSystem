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
	const char* pCoordsFile;//����������ļ���
	const char* pExportFile;//�������ļ���

	const char* pImageName;//ͼƬ�ļ���

	int currentIndex = 0;

	//ͼƬ�ߴ�
	int width;
	int height;

	vector<int> index;//����ÿ��Բ�ı��


	BlockCoords *block;//��ȡ������
	ReadImageBlock *blockImage;//��ȡ��ͼ��

	void prepare();
	void filter(vector<int> &xRcoords, vector<int> &yRcoords, vector<int> &radius);
	

};

