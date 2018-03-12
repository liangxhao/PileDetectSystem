// exifcoords.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>  
#include <fstream>  
#include <vector>
#include "ExifReader.h"

using namespace std;


int main(int argc,char** argv)
{
	if (argc < 2)
	{
		return 0;
	}

	string filepath(argv[1]);

	ifstream infile(filepath);
	if (!infile.is_open())
	{
		return 0;
	}

	vector<string> imageName;//图片名称
	//string projectPath;//工作路径
	int num = 0;//图片个数

	
	
	//projectPath = filepath.substr(0, filepath.find_last_of('//'));

	char tmp[200];
	infile >> tmp;
	num = atoi(tmp);


	for (int i = 0; i < num; i++) {
		infile >> tmp;
		imageName.push_back(string(tmp));
	}
	infile.close();

	ExifReader ex = ExifReader();
	vector<double> longitude;
	vector<double> latitude;

	ex.readImageLonLat(imageName, longitude, latitude);

	//string path = projectPath + "//points.txt";

	string path(argv[2]);


	ofstream outfile(path);
	if (outfile)
	{
		outfile << num << endl;
		for (int i = 0; i < num; i++)
		{
			char lon[30], lat[30];
			sprintf(lon, "%.20f", longitude[i]);
			sprintf(lat, "%.20f", latitude[i]);

			outfile << lon << "," << lat << endl;

		}
		outfile.close();
	}

	return 0;
}

