#include "stdafx.h"
#include "Undistorts.h"

#include "GeneralUtility.h"


int CUndistorts::getImageNum()
{
	return (int)imageNameList.size();
}



bool CUndistorts::imageUndistort()
{
	CString path = CGeneralUtility::StringToCString(exportPath);
	int num = (int)imageNameList.size();



#pragma omp parallel for 
	for (int i = 0; i < num; i++) {
		Mat img = imread(imageNameList[i]);
		Mat out;

		cv::undistort(img, out, cameraMatrix, distCoeffs);

		//去除路径获取文件名
		CString fileFullNameCStr = CGeneralUtility::StringToCString(imageNameList[i]);
		CString fileNameCStr= fileFullNameCStr.Right(fileFullNameCStr.GetLength() - fileFullNameCStr.ReverseFind('\\') - 1);
		CString outFullNameCStr = path + "\\" + fileNameCStr;

		string outName = CGeneralUtility::CStringToString(outFullNameCStr);
		imwrite(outName, out);

		//CString parms = _T("-TagsFromFile ") + fileFullNameCStr + _T(" ") + outFullNameCStr+_T(" -overwrite_original_in_place");


		//ShellExecute(NULL, _T("open"), _T("exiftool.exe"), parms, _T(""), SW_HIDE);

		PostMessage(this->hwnd, WM_PROCESS_STEP, 0, (LPARAM)1);

		

	}
	
	return true;
	
}

bool CUndistorts::runUndistort(HWND &hwnd)
{
	this->hwnd = hwnd;

	Mat tmp = imread(imageNameList[0]);
	if (tmp.size() != imageSize)
	{
		return false;
	}
	tmp.release();


	thread t(&CUndistorts::imageUndistort,this);
	t.detach();

	return true;
}