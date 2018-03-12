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
	//����ڴ�ͼ������dx��dy;ͼ���ĳߴ�w��h;Բ�������ͼ��������rx��ry
	int getNextBlockCoords(int &dx, int &dy, int &width, int &height,int &rx,int &ry);
	int getCurrentRadius();

private:
	//����Բ�����꣬����ڴ�ͼ
	vector<int> index;
	vector<int> xRcoords;
	vector<int> yRcoords;
	vector<int> radius;
	//ͼ���С
	int width;
	int height;

	//���ص�ǰ�Ŀ���������
	int currentIndex = 0;
	int numCircle = 0;

	//�ڰ뾶�Ļ���������ı���
	float extendRatio = 3.0;

};

