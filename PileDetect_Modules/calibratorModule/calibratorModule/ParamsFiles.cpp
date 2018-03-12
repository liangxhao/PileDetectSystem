#include "stdafx.h"
#include "ParamsFiles.h"



void CParamsFiles::writeSetting(FileStorage& fs) const
{
	fs << "Square_Size" << squareSize;
	fs << "Fix_k3" << IsFixK3;
	fs << "Board_Size_Height" << boardSize.height;
	fs << "Board_Size_Width" << boardSize.width;
	fs << "Is_Size_AUto" << isSizeAuto;

}

void CParamsFiles::readSetting(const FileStorage& fs)
{
	fs["Square_Size"] >> squareSize;
	fs["Fix_k3"] >> IsFixK3;
	fs["Board_Size_Height"]>> boardSize.height;
	fs["Board_Size_Width"]>>boardSize.width;
	fs["Is_Size_AUto"]>>isSizeAuto;
}


void CParamsFiles::writeSettingFiles()
{

	for(const string file: SettingsFile)
	{
		FileStorage fs(file, FileStorage::WRITE); // Read the settings
		if (!fs.isOpened())
		{
			continue;
		}
		else
		{
			writeSetting(fs);
			fs.release();
		}
	}
	
}

void CParamsFiles::readSettingFiles()
{
	for (const string file : SettingsFile)
	{
		FileStorage fs(file, FileStorage::READ); // Read the settings
		if (!fs.isOpened())
		{
			continue;
		}
		else 
		{
			readSetting(fs);
			fs.release();
			break;
		}


	}
}

bool CParamsFiles::writeCalibratorFiles(String &filename, Size &imageSize, Size &boardSize,
	Mat &cameraMatrix, Mat &distCoeffs,
	double &rms, double &totalAvgErr, vector<float> &reprojErrs)
{
	FileStorage fs(filename, FileStorage::WRITE); // Read the settings
	if (!fs.isOpened())
	{
		return false;
	}

	time_t tt;
	time(&tt);
	struct tm t2 = { 0 };
	localtime_s(&t2, &tt);
	char buf[1024];
	strftime(buf, sizeof(buf) - 1, "%c", &t2);

	fs << "calibration_time" << buf;

	fs << "image_width" << imageSize.width;
	fs << "image_height" << imageSize.height;
	fs << "board_width" << boardSize.width;
	fs << "board_height" << boardSize.height;
	fs << "square_size" << squareSize;

	fs << "Fix_k3" << IsFixK3;

	fs << "camera_matrix" << cameraMatrix;

	fs << "distortion_coefficients" << distCoeffs;

	fs << "RMS" << rms;

	fs << "avg_reprojection_error" << totalAvgErr;

	if (!reprojErrs.empty())
		fs << "per_view_reprojection_errors" << Mat(reprojErrs);

	fs.release();

	return true;

}