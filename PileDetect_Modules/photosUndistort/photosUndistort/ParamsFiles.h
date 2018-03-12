#pragma once
#include <opencv2/core.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class CParamsFiles
{

private:
	String SettingsFile = ".\\data\\camera\\setting.xml";
public:
	float squareSize = 30.00F;
	int IsFixMoreK = 0;

private:
	void writeSetting(FileStorage& fs) const;
	void readSetting(const FileStorage& fs);

public:
	void readSettingFiles();
	void writeSettingFiles();
	bool writeCalibratorFiles(String &filename, Size &imageSize, Size &boardSize, Mat &cameraMatrix, Mat &distCoeffs,
		double &rms, double &totalAvgErr, vector<float> &reprojErrs);
	bool readCalibratorFiles(String &filename, Size &imageSize, Mat &cameraMatrix, Mat &distCoeffs);
};
