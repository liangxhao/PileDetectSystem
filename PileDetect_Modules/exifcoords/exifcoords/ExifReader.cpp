#include "stdafx.h"
#include "ExifReader.h"

bool ExifReader::ExtractGPSInfo(char** papszMetadata, double &dLon, double &dLat, double &dHgt)
{
if (CSLCount(papszMetadata) <= 0)
return false;

char** papszMetadataGPS = NULL;
for (int i = 0; papszMetadata[i] != NULL; i++)
{
	//if (EQUALN(papszMetadata[i], "EXIF_GPS", 8))
	if (_strnicmp(papszMetadata[i], "EXIF_GPS", 8) == 0)
		papszMetadataGPS = CSLAddString(papszMetadataGPS, papszMetadata[i]);
}

int iGPSCount = CSLCount(papszMetadataGPS);
if (iGPSCount <= 0)
{
	CSLDestroy(papszMetadataGPS);
	return false;
}

bool bIsNorth = true;
bool bIsEast = true;

for (int i = 0; papszMetadataGPS[i] != NULL; i++)
{
	vector<string> vSplitStr;
	split(vSplitStr, papszMetadataGPS[i], boost::is_any_of("="));  //使用=拆分字符串  
	if (vSplitStr.empty() || vSplitStr.size() != 2)
		continue;

	string strName = vSplitStr[0];  //取出标识符  
	string strValue = vSplitStr[1]; //取出值  
	if (strName.empty() || strValue.empty())
		continue;

	if (strName == "EXIF_GPSAltitude")   //获取海拔  
	{
		vector<string> vSplitValue;
		boost::split(vSplitValue, strValue, boost::is_any_of("( )"), boost::token_compress_on);  //使用( )拆分字符串  
		if (vSplitValue.size() != 3)
			dHgt = 0;
		else
			dHgt = boost::lexical_cast<double>(vSplitValue[1]);
	}
	else if (strName == "EXIF_GPSLongitude")     //获取经度  
	{
		vector<string> vSplitValue;
		boost::split(vSplitValue, strValue, boost::is_any_of("( )"), boost::token_compress_on);  //使用( )拆分字符串  
		if (vSplitValue.size() != 5)
			dLon = 0;
		else
			dLon = boost::lexical_cast<double>(vSplitValue[1]) + boost::lexical_cast<double>(vSplitValue[2]) / 60.0 + boost::lexical_cast<double>(vSplitValue[3]) / 3600.0;
	}
	else if (strName == "EXIF_GPSLatitude")  //获取纬度  
	{
		vector<string> vSplitValue;
		split(vSplitValue, strValue, boost::is_any_of("( )"), boost::token_compress_on);  //使用( )拆分字符串  
		if (vSplitValue.size() != 5)
			dLat = 0;
		else
			dLat = boost::lexical_cast<double>(vSplitValue[1]) + boost::lexical_cast<double>(vSplitValue[2]) / 60.0 + boost::lexical_cast<double>(vSplitValue[3]) / 3600.0;
	}
	else if (strName == "EXIF_GPSLongitude")     //获取经度  
	{
		if (strValue == "E")
			bIsEast = true;
		else
			bIsEast = false;
	}
	else if (strName == "EXIF_GPSLatitude")  //获取纬度  
	{
		if (strValue == "N")
			bIsNorth = true;
		else
			bIsNorth = false;
	}
}

dLon = bIsEast ? dLon : -1.0*dLon;
dLat = bIsNorth ? dLat : -1.0*dLat;

return true;
}


void ExifReader::readImageLonLat(vector<string> &filename, vector<double> &longitude, vector<double> &latitude)
{
	double dLon, dLat, dHgt;

	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	for (int i = 0; i < filename.size(); i++)
	{


		GDALDataset * poDataset = (GDALDataset *)GDALOpen(filename[i].c_str(), GA_ReadOnly);
		if (poDataset == NULL)
		{
			longitude.push_back(inf_negative);
			latitude.push_back(inf_negative);
			continue;
		}


		char** papszMetadata = poDataset->GetMetadata(NULL);

		ExtractGPSInfo(papszMetadata, dLon, dLat, dHgt);
		GDALClose(poDataset);
		if (dLat < 0 || dLon < 0)
		{
			dLon = inf_negative;
			dLat = inf_negative;
		}
		longitude.push_back(dLon);
		latitude.push_back(dLat);


	}

}