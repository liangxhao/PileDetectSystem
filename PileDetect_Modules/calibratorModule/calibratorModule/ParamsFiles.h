#pragma once

#include <opencv2/core.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class CParamsFiles
{

private:
	vector<String> SettingsFile = { "./config/camera.xml","../config/camera.xml" };
public:
	float squareSize=30.00F;
	int IsFixK3=0;

	Size boardSize = Size(15, 15);
	int isSizeAuto = 1;

private:
	void writeSetting(FileStorage& fs) const;
	void readSetting(const FileStorage& fs);

public:
	void readSettingFiles();
	void writeSettingFiles();
	bool writeCalibratorFiles(String &filename, Size &imageSize, Size &boardSize, Mat &cameraMatrix, Mat &distCoeffs,
		double &rms, double &totalAvgErr, vector<float> &reprojErrs);

};

