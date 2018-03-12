// photoStitch.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
//#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
//#include "opencv2/stitching.hpp"
#include <io.h>
#include "stitching.h"
#include <fstream>

using namespace std;
using namespace cv;


/*
void getFiles(string path, vector<string>& files)
{
	//文件句柄  
	intptr_t   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				//if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				//	getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
*/

int main(int argc,char **argv)
{
	if (argc < 2)
	{
		cout << "无法打开工程文件";
		return 0;

	}

	string filepath(argv[1]);
	vector<string> images_name;


	ifstream infile(filepath);
	if (!infile.is_open())
	{
		cout << "无法打开工程文件";
		return 0;
	}

	
	string projectPath = filepath.substr(0, filepath.find_last_of('/'));//工作路径

	int num = 0;//图片个数
	char tmp[200];

	infile >> tmp;
	num = atoi(tmp);


	for (int i = 0; i < num; i++) {
		infile >> tmp;
		images_name.push_back(string(tmp));
		cout << images_name[i]<<endl;
	}
	infile.close();


	//string output = projectPath + "//result.tif";

	string output(argv[2]);//输出图片名称

	


	const char *dst_name = output.c_str();


	st::Stitcher stitcher = st::Stitcher::createDefault();
	stitcher.setOutputFileName(dst_name);
	stitcher.setPathProject(projectPath);
	
	//设置图像坐标保存文件
	if (argc == 4)
	{
		string exifFileName(argv[3]);
		stitcher.setExifFileName(exifFileName);
	}


	projectPath.clear();
	

	//st::Stitcher::Status status = stitcher.stitch(images_name, st::Stitcher::Stage::ESTIMATE_PARMS);

	st::Stitcher::Status status = stitcher.stitch(images_name);
	
	if (status != st::Stitcher::OK)
	{
		cout << "Can't stitch images, error code = " << int(status) << endl;
		return -1;
	}
	cout << "Successfully!" << endl;
	return 0;
}

