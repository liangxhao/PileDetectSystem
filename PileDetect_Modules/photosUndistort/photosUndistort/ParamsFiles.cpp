#include "stdafx.h"
#include "ParamsFiles.h"


void CParamsFiles::writeSetting(FileStorage& fs) const
{
	fs << "Square_Size" << squareSize;
	fs << "Fix_k4_k5_k6" << IsFixMoreK;

}

void CParamsFiles::readSetting(const FileStorage& fs)
{
	fs["Square_Size"] >> squareSize;
	fs["Fix_k4_k5_k6"] >> IsFixMoreK;
}


void CParamsFiles::writeSettingFiles()
{
	FileStorage fs(SettingsFile, FileStorage::WRITE); // Read the settings
	if (!fs.isOpened())
	{
		return;
	}

	writeSetting(fs);
	fs.release();

}

void CParamsFiles::readSettingFiles()
{
	FileStorage fs(SettingsFile, FileStorage::READ); // Read the settings
	if (!fs.isOpened())
	{
		return;
	}

	readSetting(fs);
	fs.release();
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

	fs << "Fix_k4_k5_k6" << IsFixMoreK;

	fs << "camera_matrix" << cameraMatrix;

	fs << "distortion_coefficients" << distCoeffs;

	fs << "RMS" << rms;

	fs << "avg_reprojection_error" << totalAvgErr;

	if (!reprojErrs.empty())
		fs << "per_view_reprojection_errors" << Mat(reprojErrs);

	fs.release();

	return true;

}


bool CParamsFiles::readCalibratorFiles(String &filename, Size &imageSize, Mat &cameraMatrix, Mat &distCoeffs)
{
	FileStorage fs(filename, FileStorage::READ); // Read the settings
	if (!fs.isOpened())
	{
		return false;
	}

	fs["image_width"] >> imageSize.width;
	fs["image_height"] >> imageSize.height;


	fs["camera_matrix"]>> cameraMatrix;

	fs["distortion_coefficients"] >> distCoeffs;

	fs.release();
	return true;
}