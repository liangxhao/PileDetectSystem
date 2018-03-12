#include "main.h"



//输入------ 模式,txt名,图像名,相机文件名....弃用
//输入------ txt名,图像名
int main(int argc, char **argv)
{	
	//至少要有4个参数
	if (argc < 3)
	{
		return 1;
	}


	string fileName(argv[1]);//输入的txt名
	string imageName(argv[2]);//输出的图片名


	compoiste(fileName, imageName);

	

	return 0;

}



void compoiste(string fileName, string compositeName)
{
	//string fileName = "G:/Temp/1.txt";
	//string compositeName = "G:/Temp/composite.tif";
	//string cameraParms = "F:/Projects/无人机项目/相机标定/BoardChess/calibrator.xml";

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