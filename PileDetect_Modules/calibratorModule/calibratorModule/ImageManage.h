#pragma once
#include <vector>
using namespace std;

class CImageManage
{

private:
	int minImageMum=0;
	string filePath;

public:
	CString getFileName(CString fullName);
	CString getFileExt(CString fileName);
	string getFilePath(string fileName);
	bool isVaild(vector<string> &nameList);

	CString getFilePath();

private:
	bool checkImageSizeIsSame(vector<string> &nameList);
};

