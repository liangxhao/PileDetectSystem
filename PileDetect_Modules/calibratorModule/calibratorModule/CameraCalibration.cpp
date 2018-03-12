#include "stdafx.h"
#include "CameraCalibration.h"

//#include <Mmsystem.h>

//Mat类型的图像转换为自定义类型的图像
void CCameraCalibration::matToArray(emxArray_uint8_T* &dst, Mat &src)
{
	int iv0[2] = { src.rows, src.cols };
	dst = emxCreateND_uint8_T(2, *(int(*)[2])&iv0[0]);
	//自定义的arrayUchar类型数据是按列顺序放到一行上面存储
	for (int idx0 = 0; idx0 < dst->size[0U]; idx0++) {
		for (int idx1 = 0; idx1 < dst->size[1U]; idx1++) {
			dst->data[idx0 + dst->size[0] * idx1] = src.ptr<uchar>(idx0)[idx1];
		}
	}	
}
//检测单张图像上棋盘点坐标和棋盘尺寸
void CCameraCalibration::detectCheckerboardPoints(emxArray_uint8_T *img, vector<Point2f> &imagePoints, double *boardSize)
{
	emxArray_real_T *points;

	emxInit_real_T(&points, 2);

	detectCheckerboardPointsOnImage(img, this->sigma,this->minCornerMetric,points, boardSize);

	int rows = points->size[0];
	for (int i = 0; i < rows; i++)
	{
		imagePoints.push_back(Point2f(float(points->data[i]), float(points->data[i + rows])));
	}

	emxFree_real_T(&points);
}

//获取图片数量
int CCameraCalibration::getNumImage()
{
	return (int)imageNameList.size();
}



//检测多张图片上棋盘点坐标和棋盘尺寸
bool CCameraCalibration::PointsOnImagesList()
{
	int numImages = (int)imageNameList.size();

	vector<Point2f> imagePoints;
	double boardSizeXY[2];

	vector<Point2f> imagePointsTmp;
	imagePointsTmp.push_back(Point2f(0.0F, 0.0F));

	imagePointsList.assign(numImages, imagePointsTmp);
	boardSizeList.assign(numImages, Point2i(0, 0));

	detectCheckerboardInitialize();


#pragma omp parallel for 
	for (int i = 0; i < numImages; i++)
	{
		Mat imgMat = imread(imageNameList[i], IMREAD_GRAYSCALE);
		emxArray_uint8_T *arryImg = nullptr;
		matToArray(arryImg, imgMat);
		imgMat.release();

		detectCheckerboardPoints(arryImg, imagePoints, boardSizeXY);
		emxFree_uint8_T(&arryImg);

		imagePointsList[i] = imagePoints;
		boardSizeList[i] = Point2i((int)boardSizeXY[0]-1, (int)boardSizeXY[1]-1);

		imagePoints.clear();


		PostMessage(this->hwnd, WM_PROCESS_STEP, 0, (LPARAM)1);

	}

	CheckerboardPointsTerminate();

	this->verticalSortPoints();
	PostMessage(this->hwnd, WM_PROCESS_STEP, 0, (LPARAM)1);

	return true;
}

//开启新线程进行计算
void CCameraCalibration::runDetect(HWND &hwnd)
{
	this->hwnd = hwnd;
	thread t(&CCameraCalibration::PointsOnImagesList, this);
	t.detach();

}


//角点排列顺序旋转到正确位置
void CCameraCalibration::verticalSortPoints()
{
	
	for (int k = 0; k < 4; k++) {
		for (int n = 0; n < boardSizeList.size(); n++)
		{
			if (imagePointsList[n].size()== 0)
				continue;

			Point2f a1 = imagePointsList[n][0];
			Point2f a2 = imagePointsList[n][boardSizeList[n].x - 1];
			float ang = atan2(a2.y - a1.y, a2.x - a1.x);
			ang = (ang < 0.0F ? ang + 2 * (float)CV_PI : ang) * 180 / (float)CV_PI;

			if (ang  > 135.0F || ang<45.0F)
			{
				vector<Point2f> imagePoints; 

				for (int i = boardSizeList[n].x - 1; i >= 0; i--)
					for (int j = 0; j < boardSizeList[n].y; j++)
					{
						imagePoints.push_back(imagePointsList[n][i + j*(boardSizeList[n].x)]);
					}
				imagePointsList[n].swap(imagePoints);
				boardSizeList[n] = Point2i(boardSizeList[n].y, boardSizeList[n].x);
			}

		}
	}
	
}


//检查各个棋盘图像是否检测到相同尺寸的棋盘
bool CCameraCalibration::checkPointsSizeIsSame()
{
	
	int minSize = 5;

	int num =(int) boardSizeList.size();

	int curIdx = 0, count = 1;
	for (int i = 1; i < num; ++i)
	{
		boardSizeList[i] == boardSizeList[curIdx] ? ++count : --count;
		if (!count)
		{
			curIdx = i;
			count = 1;
		}
	}

	boardSize = boardSizeList[curIdx];//众数

	if (boardSize.width < minSize || boardSize.height < minSize)
		return false;

	else
	{
		count = 0;
		for (int i = 0; i < num; ++i)
		{
			(boardSizeList[i].x >= boardSize.width&&boardSizeList[i].y >= boardSize.height) ? ++count : 0;
		}

		if (count >= num / 2)
		{
			boardSize = Size(boardSize.width , boardSize.height );
			return true;
		}
		else
		{
			return false;
		}		
	}
	
}



void CCameraCalibration::autoGetChessSize()
{
	int num = (int)boardSizeList.size();
	imageIsVaild.clear();
	if (IsSizeAuto)
	{
		if (!checkPointsSizeIsSame())
		{

			int minSize = 10;
			int maxVal = 1000;

			int minRowSize = maxVal;
			int minColSize = maxVal;
			for (int i = 0; i < num; i++)
			{
				if (boardSizeList[i].x >= minSize)
					minRowSize = minRowSize <= boardSizeList[i].x ? minRowSize : boardSizeList[i].x;

				if (boardSizeList[i].y >= minSize)
					minColSize = minColSize <= boardSizeList[i].y ? minColSize : boardSizeList[i].y;
			}

			if (minRowSize == maxVal || minColSize == maxVal)
			{
				imageIsVaild.assign(num, false);
				boardSize = Size(0, 0);
				return;

			}
			else
			{
				boardSize = Size(minRowSize, minColSize);

			}
		}
	}
	

	vector<Point2f> imagePoints;
	for (int n = 0; n < num; n++)
	{
		if (boardSize.width <=boardSizeList[n].x&&boardSize.height <= boardSizeList[n].y)
		{
			this->imageIsVaild.push_back(true);
			imagePoints.clear();
			int jStart = (boardSizeList[n].y - boardSize.height) / 2;
			int iStart = (boardSizeList[n].x - boardSize.width) / 2;

			for (int j = jStart; j < jStart+boardSize.height; j++)
				for (int i = iStart; i < iStart+boardSize.width; i++)
				{
					imagePoints.push_back(imagePointsList[n][i + j*(boardSizeList[n].x)]);
				}

			imagePointsList[n].swap(imagePoints);
		}
		else
		{
			this->imageIsVaild.push_back(false);
		}
	}
	
}


void CCameraCalibration::drawPointsOnImage(Mat &img, string &imageName,int flag)
{
	if (imagePointsList.size() == 0)
		return;

	if (boardSizeList.size() == 0)
		return;

	vector<string>::iterator pos = find(imageNameList.begin(), imageNameList.end(), imageName);

	int idx =(int) distance(imageNameList.begin(), pos);
	if (flag == 0)
	{
		this->drawChessboardCorners(img, this->boardSizeList[idx], Mat(imagePointsList[idx]), false);
	}
	else
	{
		this->drawChessboardCorners(img, boardSize, Mat(imagePointsList[idx]), imageIsVaild[idx]);
	}
	
}


void CCameraCalibration::drawChessboardCorners(InputOutputArray _image, Size patternSize, InputArray _corners,
	bool patternWasFound)
{
	Mat corners = _corners.getMat();
	if (corners.empty())
		return;
	Mat image = _image.getMat(); CvMat c_image = _image.getMat();
	int nelems = corners.checkVector(2, CV_32F, true);

	this->cvDrawChessboardCorners(&c_image, patternSize, corners.ptr<CvPoint2D32f>(),
		nelems, patternWasFound);
}



void CCameraCalibration::cvDrawChessboardCorners(CvArr* _image, CvSize pattern_size,
	CvPoint2D32f* corners, int count, int found)
{
	const int shift = 0;
	const int radius = 4;
	
	int i;
	CvMat stub, *image;
	double scale = 1;
	int type, cn, line_type;

	image = cvGetMat(_image, &stub);

	type = CV_MAT_TYPE(image->type);
	cn = CV_MAT_CN(type);

	int thickness =(int) ceil((image->height<image->width ? image->height: image->width)/1000.0);
	int r = radius*(1 << shift)*thickness;
	scale = 1;


	line_type = type == CV_8UC1 || type == CV_8UC3 ? CV_AA : 8;

	if (!found)
	{
		CvScalar color(0, 0, 255, 0);
		if (cn == 1)
			color = cvScalarAll(200);
		color.val[0] *= scale;
		color.val[1] *= scale;
		color.val[2] *= scale;
		color.val[3] *= scale;

		for (i = 0; i < count; i++)
		{
			CvPoint pt;
			pt.x = cvRound(corners[i].x*(1 << shift));
			pt.y = cvRound(corners[i].y*(1 << shift));
			cvLine(image, cvPoint(pt.x - r, pt.y - r),
				cvPoint(pt.x + r, pt.y + r), color, thickness, line_type, shift);
			cvLine(image, cvPoint(pt.x - r, pt.y + r),
				cvPoint(pt.x + r, pt.y - r), color, thickness, line_type, shift);
			cvCircle(image, pt, r + (1 << shift), color, thickness, line_type, shift);
		}
	}
	else
	{
		int x, y;
		CvPoint prev_pt;
		const int line_max = 7;
		static const CvScalar line_colors[line_max] =
		{
			CvScalar(0,0,255),
			CvScalar(0,128,255),
			CvScalar(0,200,200),
			CvScalar(0,255,0),
			CvScalar(200,200,0),
			CvScalar(255,0,0),
			CvScalar(255,0,255)
		};

		for (y = 0, i = 0; y < pattern_size.height; y++)
		{
			CvScalar color = line_colors[y % line_max];
			if (cn == 1)
				color = cvScalarAll(200);
			color.val[0] *= scale;
			color.val[1] *= scale;
			color.val[2] *= scale;
			color.val[3] *= scale;

			for (x = 0; x < pattern_size.width; x++, i++)
			{
				CvPoint pt;
				pt.x = cvRound(corners[i].x*(1 << shift));
				pt.y = cvRound(corners[i].y*(1 << shift));

				if (i != 0)
					cvLine(image, prev_pt, pt, color, thickness, line_type, shift);

				cvLine(image, cvPoint(pt.x - r, pt.y - r),
					cvPoint(pt.x + r, pt.y + r), color, thickness, line_type, shift);
				cvLine(image, cvPoint(pt.x - r, pt.y + r),
					cvPoint(pt.x + r, pt.y - r), color, thickness, line_type, shift);
				cvCircle(image, pt, r + (1 << shift), color, thickness, line_type, shift);
				prev_pt = pt;
			}
			//原点
			CvPoint pt;
			pt.x = cvRound(corners[0].x);
			pt.y = cvRound(corners[0].y);
			
			cvCircle(image, pt, r * 3, CvScalar(0, 0, 200), thickness*4, line_type, shift);
		}
	}
}

//计算棋盘点的物理坐标
void CCameraCalibration::chessboardWorldCorners(vector<Point3f>& corners)
{
	corners.resize(0);

	for (int i = 0; i < boardSize.height; i++)
		for (int j = 0; j < boardSize.width; j++)
			corners.push_back(Point3f(float(i*this->squareSize), float(j*this->squareSize), 0));

}

bool CCameraCalibration::clcCalibration()
{
	cameraMatrix = Mat::eye(3, 3, CV_64F);
	distCoeffs = Mat::zeros(8, 1, CV_64F);

	//参与计算的相片
	vector<vector<Point2f>> vaildImagePointsList;
	for (int i = 0; i < imagePointsList.size(); i++)
	{
		if (imageIsVaild[i])
		{
			vaildImagePointsList.push_back(imagePointsList[i]);
		}
	}

	vector<vector<Point3f> > objectPoints(1);
	chessboardWorldCorners(objectPoints[0]);
	objectPoints.resize(vaildImagePointsList.size(), objectPoints[0]);

	//ofstream file("points.txt");
	//file << "NumImages: " << vaildImagePointsList.size() << endl;
	//file << "NumPoints: " << vaildImagePointsList[1].size() << endl;

	//for (int i = 0; i < vaildImagePointsList.size(); i++)
	//{
	//	for (int j = 0; j < vaildImagePointsList[i].size(); j++)
	//	{
	//		file << vaildImagePointsList[i][j].x << "," << vaildImagePointsList[i][j].y << endl;
	//	}

	//}

	//for (int j = 0; j < objectPoints[0].size(); j++)
	//{
	//	file << objectPoints[0][j].x << "," << objectPoints[0][j].y << endl;
	//}



	//获取图片尺寸
	Mat img = imread(imageNameList[0], IMREAD_GRAYSCALE);
	imageSize = img.size();
	img.release();

	vector<Mat> rvecs, tvecs;


	int flag = 0;
	if(!IsFixK3)
	  flag = 0 | CV_CALIB_FIX_K3;
	//开始标定计算
	rms = calibrateCamera(objectPoints, vaildImagePointsList, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs,flag);

	totalAvgErr = computeReprojectionErrors(objectPoints, vaildImagePointsList,
		rvecs, tvecs, cameraMatrix, distCoeffs, reprojErrs);

	this->calibrationSucess = checkRange(cameraMatrix) && checkRange(distCoeffs);

	PostMessage(this->hwnd, WM_PROCESS_STEP, 0, (LPARAM)2);
	return true;

}



void CCameraCalibration::runCalibration(HWND &hwnd)
{
	this->hwnd = hwnd;
	thread t(&CCameraCalibration::clcCalibration, this);
	t.detach();
}



//计算标定后的投影误差
double CCameraCalibration::computeReprojectionErrors(const vector<vector<Point3f>>& objectPoints,
	const vector<vector<Point2f> >& imagePoints,
	const vector<Mat>& rvecs, const vector<Mat>& tvecs,
	const Mat& cameraMatrix, const Mat& distCoeffs,
	vector<float>& perViewErrors)

{
	vector<Point2f> imagePoints2;
	size_t totalPoints = 0;
	double totalErr = 0, err;
	perViewErrors.resize(objectPoints.size());

	for (size_t i = 0; i < objectPoints.size(); ++i)
	{

		projectPoints(objectPoints[i], rvecs[i], tvecs[i], cameraMatrix, distCoeffs, imagePoints2);

		err = norm(imagePoints[i], imagePoints2, NORM_L2);

		size_t n = objectPoints[i].size();
		perViewErrors[i] = (float)std::sqrt(err*err / n);
		totalErr += err*err;
		totalPoints += n;
	}

	return std::sqrt(totalErr / totalPoints);
}


