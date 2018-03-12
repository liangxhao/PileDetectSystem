#include "stdafx.h"
#include "ImageManage.h"
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "GeneralUtility.h"

using namespace cv;
//��ȡ�ļ���
CString CImageManage::getFileName(CString fullName)
{
	return fullName.Right(fullName.GetLength() - fullName.ReverseFind('\\') - 1);
}

//��ȡ�ļ���׺��
CString CImageManage::getFileExt(CString fileName)
{
	return fileName.Right(fileName.GetLength() - fileName.ReverseFind('.') - 1).MakeLower();
}

//��ȡ�ļ�·��
string CImageManage::getFilePath(string fileName)
{
	return fileName.substr(0, fileName.find_last_of('\\'));
}

//���ͼƬ�Ƿ����
bool CImageManage::isVaild(vector<string> &nameList)
{
	if (nameList.size() < this->minImageMum)
		return false;

	this->filePath = getFilePath(nameList[0]);
	return true;

	//return checkImageSizeIsSame(nameList);
}

bool CImageManage::checkImageSizeIsSame(vector<string> &nameList)
{
	//int num = nameList.size();
	//Mat img;
	//Size firstImageSz, currImageSz;

	//for (int i = 0; i < num; i++)
	//{
	//	img = imread(nameList[i]);
	//	if (i != 0)
	//	{
	//		
	//		currImageSz = img.size();
	//		if (currImageSz != firstImageSz)
	//			return false;
	//	}
	//	else
	//	{
	//		firstImageSz = img.size();
	//	}
	//}

	return true;
}

CString CImageManage::getFilePath()
{
	return CGeneralUtility::StringToCString(this->filePath);
}