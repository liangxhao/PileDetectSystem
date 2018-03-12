#include <iostream>
#include "opencv2/highgui.hpp"
#include "ExtractCircle.h"

using namespace std;


int main(int argv, char** argc)
{
	if (argv < 4)
	{
		cout << "Too few parameters!";
		return 1;
	}

	const char *pImageName = argc[1];
	const char *pCoordsName = argc[2];
	const char *pExportFile = argc[3];

	/*const char *pImageName = "C:/Users/liangxhao/Desktop/tmp/Program/b.jpg";
	const char *pCoordsName = "C:/Users/liangxhao/Desktop/tmp/1.txt";
	const char *pExportFile = "C:/Users/liangxhao/Desktop/tmp/out.txt";*/

	//Mat img = imread(pName);

	//imshow("sss", img);
	//waitKey(0);

	ExtractCircle extra = ExtractCircle(pCoordsName, pImageName);
	extra.setExportFileName(pExportFile);
	extra.run();
	return 0;

	//system("pause");
}