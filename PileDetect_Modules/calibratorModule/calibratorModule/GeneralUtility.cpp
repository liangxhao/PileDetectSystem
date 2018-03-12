#include "stdafx.h"
#include "GeneralUtility.h"


string  CGeneralUtility::CStringToString(CString str)
{
	LPCWSTR pwszSrc = str.GetBuffer(str.GetLength());
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen <= 0) return std::string("");
	char* pszDst = new char[nLen];
	if (NULL == pszDst) return std::string("");
	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;
	std::string strTemp(pszDst);
	delete[] pszDst;
	return strTemp;
}

CString CGeneralUtility::StringToCString(string str)
{
	CString strTemp(str.c_str());
	return strTemp;
}
