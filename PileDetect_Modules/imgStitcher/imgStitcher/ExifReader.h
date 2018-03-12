#pragma once

#include "gdal.h"
#include "gdal_priv.h"  
#include "iostream"


#include "boost/lexical_cast.hpp"  
#include "boost/algorithm/string.hpp"  

//using namespace boost;
//using namespace boost::algorithm;
using namespace std;

class ExifReader
{
public:
	//ExifReader();
	//~ExifReader();

	void readImageLonLat(vector<string> &filename,vector<double> &longitude, vector<double> &latitude);
	void readImageLonLatFromTxt(string &filename, vector<double> &longitude, vector<double> &latitude);

private:
	bool ExtractGPSInfo(char** papszMetadata, double &dLon, double &dLat, double &dHgt);
	std::vector<std::string> splitString(std::string str, std::string pattern);
	const double inf_negative = -1.0e10;
};

