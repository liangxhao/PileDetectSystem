#include "stdafx.h"
#include "StringManage.h"


std::string stringGetFileNameNotExt(const std::string &str)
{
	int pos = (int)str.find_last_of("//");
	int counts = (int)str.find_last_of(".");
	std::string name = str.substr(pos + 1, counts - pos - 1);

	return  name;
}