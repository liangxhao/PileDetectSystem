#include "main.h"



//����------ ģʽ,txt��,ͼ����,����ļ���....����
//����------ txt��,ͼ����
int main(int argc, char **argv)
{	
	//����Ҫ��4������
	if (argc < 3)
	{
		return 1;
	}


	string fileName(argv[1]);//�����txt��
	string imageName(argv[2]);//�����ͼƬ��


	compoiste(fileName, imageName);

	

	return 0;

}



void compoiste(string fileName, string compositeName)
{
	//string fileName = "G:/Temp/1.txt";
	//string compositeName = "G:/Temp/composite.tif";
	//string cameraParms = "F:/Projects/���˻���Ŀ/����궨/BoardChess/calibrator.xml";

	vector<string> imageName;
	vector<double> angles;
	cv::Mat limit_x;
	cv::Mat limit_y;

	readCompositeTxt(fileName, imageName, angles, limit_x, limit_y);

	Composite comp;

	comp.setCompositeName(compositeName);
	comp.setImageNames(imageName);
	comp.setImageAngles(angles);
	comp.setImageLimits(limit_x, limit_y);

	comp.compositeImage();

}