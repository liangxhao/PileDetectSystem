#include "Composite.h"

#include "io.h"
#include "stdio.h"
//设置图像名称
void Composite::setImageNames(const vector<string> &names)
{
	//至少有一张图像
	assert(names.size()>0);

	this->imageNames = names;
	int num = names.size();
	
	this->imageDesc.resize(num);

	//获取图像尺寸
	cv::Mat img = cv::imread(names[0]);
	this->imageSize = img.size();
	img.release();
	 
}

//设置图像尺寸
void Composite::setImageAngles(const vector<double> &angles)
{
	//图像数量与角度的数量必须相同
	assert(angles.size() == this->imageAngles.size());

	this->imageAngles = angles;

	int width = this->imageSize.width;
	int height = this->imageSize.height;

	
	//计算旋转后的图像尺寸
	for (int i = 0; i < angles.size(); i++)
	{
		double a = sin(angles[i]), b = cos(angles[i]);


		int width_rotate = int(height * fabs(a) + width * fabs(b));
		int height_rotate = int(width * fabs(a) + height * fabs(b));

		this->imageDesc[i].sz = cv::Size(width_rotate, height_rotate);
		this->imageDesc[i].rotateSz= cv::Size(width_rotate, height_rotate);
	}
	
}

//设置图像的地理坐标范围
//limits_x:  n*4   每行是图像左上角开始逆时针四个点的x地理坐标
//limits_y:  n*4   每行是图像左上角开始逆时针四个点的y地理坐标
void Composite::setImageLimits(const cv::Mat &limits_x, const cv::Mat &limits_y)
{
	//数量必须保持一致
	assert(limits_x.rows == limits_y.rows);
	assert(limits_x.rows == this->imageNames.size());

	int num = limits_x.rows;

	//存储四个角点的最小值与最大值
	double x_min=1.7e308, x_max= -1.7e308, y_min= 1.7e308, y_max= -1.7e308;

	double maxValue, minValue;

	//每张图的图像范围
	for (int i = 0; i < num; i++) {

		//每行的坐标值
		cv::Mat points;

		//计算x坐标的范围
		points = limits_x.row(i);
		cv::minMaxLoc(points, &minValue, &maxValue);
		this->imageDesc[i].limit.setLimitX(minValue, maxValue);

		x_min = x_min < minValue ? x_min : minValue;
		x_max = x_max > maxValue ? x_max : maxValue;

		
		//计算y坐标的范围
		points = limits_y.row(i);
		cv::minMaxLoc(points, &minValue, &maxValue);
		this->imageDesc[i].limit.setLimitY(minValue, maxValue);

		y_min = y_min < minValue ? y_min : minValue;
		y_max = y_max > maxValue ? y_max : maxValue;


	}

	//整图的图像范围
	this->imageLimit_min = Point(x_min, y_min);
	this->imageLimit_max = Point(x_max, y_max);

	/*this->imageLimit_min = cv::Point2f(x_min, y_min);
	this->imageLimit_max = cv::Point2f(x_max, y_max);*/

}



//调整图像分辨率，使他们分辨率相同
void Composite::unifyResolution()
{

	double xRes = 0, yRes = 0;
	//第一张图像的分辨率作为默认分辨率
	xRes = (this->imageDesc[0].limit.max_x - this->imageDesc[0].limit.min_x) / this->imageDesc[0].sz.width;
	yRes = (this->imageDesc[0].limit.max_y - this->imageDesc[0].limit.min_y) / this->imageDesc[0].sz.height;
	double resolution = (xRes + yRes) / 2;

	for (int i = 1; i < this->imageNames.size(); i++)
	{
		xRes = (this->imageDesc[i].limit.max_x - this->imageDesc[i].limit.min_x) / this->imageDesc[i].sz.width;
		yRes = (this->imageDesc[i].limit.max_y - this->imageDesc[i].limit.min_y) / this->imageDesc[i].sz.height;
		double res = (xRes + yRes) / 2;

		//相对的缩放比率
		//double scale = resolution / res;
		double scale = res / resolution;

		this->imageDesc[i].scale = scale;


		//重新计算图像的尺寸
		this->imageDesc[i].sz.width = round(this->imageDesc[i].sz.width*scale);
		this->imageDesc[i].sz.height = round(this->imageDesc[i].sz.height*scale);


	}

}

//计算叠加后的整图尺寸
void Composite::clcCompositeSize()
{
	vector<ImageDesc> _imageDesc(this->imageDesc);

	this->clcCompositeSizeX(_imageDesc);
	this->clcCompositeSizeY(_imageDesc);

	_imageDesc.clear();


}
//计算x方向的尺寸
void Composite::clcCompositeSizeX(vector<ImageDesc> &_imageDesc)
{
	//计算整幅图像的宽度
	///////////////////////////////////////////////

	//按照图像的min_x进行排序
	sort(_imageDesc.begin(), _imageDesc.end(), compareX);

	//整图的图像宽度
	//像素坐标的宽度
	double width_pixels = 0;
	//当前所有已经叠加图像的最右端（x_min）坐标
	double xMin_geo = 0;
	//当前所有已经叠加图像的最右端（x_max）坐标
	double xMax_geo = 0;

	//x从小到大，依次增加每个图像的宽度，去掉重叠的部分

	//先初始化为第一张图像的宽度
	width_pixels = _imageDesc[0].sz.width;
	xMin_geo = _imageDesc[0].limit.min_x;
	xMax_geo = _imageDesc[0].limit.max_x;

	for (int i = 1; i < _imageDesc.size(); i++) {

		//第i张图像x的范围
		double _xMin = _imageDesc[i].limit.min_x;
		double _xMax = _imageDesc[i].limit.max_x;

		//之前的图像与第i张图像相交
		if (xMax_geo >= _xMin && xMax_geo <= _xMax)
		{

			//重叠的像素宽度
			double overlap = (xMax_geo - _xMin) / (xMax_geo - xMin_geo) * width_pixels;
			//更新
			width_pixels = width_pixels + _imageDesc[i].sz.width - overlap;
			xMax_geo = _xMax;

		}
		//之前的图像包含第i张图像
		else if (xMax_geo > _xMin && xMax_geo > _xMax)
		{
			continue;
		}
		//之前的图像与第i张图像相离
		else
		{
			//分离相隔的像素宽度
			double separate = (_xMin - xMax_geo) / (xMax_geo - xMin_geo) * width_pixels;
			//更新
			width_pixels = width_pixels + _imageDesc[i].sz.width + separate;
			xMax_geo = _xMax;

		}

	}

	//得到宽度后，需要取整
	int width = ceil(width_pixels);

	this->compositeSize.width = width;
	//重新计算图像范围
	//因为width_pixels是精确的对应图像，取整后需要考虑取整的偏差
	//保持x_min不变，只更新x_max
	this->imageLimit_max.x = this->imageLimit_min.x + (this->imageLimit_max.x - this->imageLimit_min.x) / width_pixels*width;


}



//计算y方向的尺寸
void Composite::clcCompositeSizeY(vector<ImageDesc> &_imageDesc)
{

	//计算整幅图像的高度
	///////////////////////////////////////////////

	//按照图像的min_y进行排序
	sort(_imageDesc.begin(), _imageDesc.end(), compareY);

	//整图的图像宽度
	//像素坐标的宽度
	double height_pixels = 0;
	//当前所有已经叠加图像的最右端（y_min）坐标
	double yMin_geo = 0;
	//当前所有已经叠加图像的最右端（y_max）坐标
	double yMax_geo = 0;

	//y从小到大，依次增加每个图像的高度，去掉重叠的部分

	//先初始化为第一张图像的高度
	height_pixels = _imageDesc[0].sz.height;
	yMin_geo = _imageDesc[0].limit.min_y;
	yMax_geo = _imageDesc[0].limit.max_y;

	for (int i = 1; i < _imageDesc.size(); i++) {

		//第i张图像x的范围
		double _yMin = _imageDesc[i].limit.min_y;
		double _yMax = _imageDesc[i].limit.max_y;

		//之前的图像与第i张图像相交
		if (yMax_geo >= _yMin && yMax_geo <= _yMax)
		{

			//重叠的像素高度
			double overlap = (yMax_geo - _yMin) / (yMax_geo - yMin_geo) * height_pixels;
			//更新
			height_pixels = height_pixels + _imageDesc[i].sz.height - overlap;
			yMax_geo = _yMax;

		}
		//之前的图像包含第i张图像
		else if (yMax_geo > _yMin && yMax_geo > _yMax)
		{
			continue;
		}
		//之前的图像与第i张图像相离
		else
		{
			//分离相隔的像素高度
			double separate = (_yMin - yMax_geo) / (yMax_geo - yMin_geo) * height_pixels;
			//更新
			height_pixels = height_pixels + _imageDesc[i].sz.height + separate;
			yMax_geo = _yMax;

		}

	}

	//得到高度后，需要取整
	int height = ceil(height_pixels);

	this->compositeSize.height = height;
	//重新计算图像范围
	//因为height_pixels是精确的对应图像，取整后需要考虑取整的偏差
	//保持y_min不变，只更新y_max
	this->imageLimit_max.y = this->imageLimit_min.y + (this->imageLimit_max.y - this->imageLimit_min.y) / height_pixels*height;

}


//计算每张图像相对于整图的偏移
void Composite::clcOffsets()
{
	double xRate = this->compositeSize.width / (this->imageLimit_max.x - this->imageLimit_min.x);
	double yRate = this->compositeSize.height / (this->imageLimit_max.y - this->imageLimit_min.y);

	//图像左上角的坐标
	double xCor = this->imageLimit_min.x;
	double yCor = this->imageLimit_max.y;

	//需要检查一下叠加后的图像是否会超过边界
	int width = this->compositeSize.width;
	int height = this->compositeSize.height;

	for (int i = 0; i < this->imageDesc.size(); i++)
	{
		double dx = (this->imageDesc[i].limit.min_x - xCor)*xRate;
		double dy = (yCor - this->imageDesc[i].limit.max_y)*yRate;

		this->imageDesc[i].dx = round(dx) > 0 ? round(dx) : 0;
		this->imageDesc[i].dy = round(dy) > 0 ? round(dy) : 0;

		int xEnd = this->imageDesc[i].dx + this->imageDesc[i].sz.width;
		int yEnd= this->imageDesc[i].dy + this->imageDesc[i].sz.height;

		width = width > xEnd ? width : xEnd;
		height = height > yEnd ? height : yEnd;
	}

	//如果超过原来计算的图像尺寸，则重新计算
	if (width > this->compositeSize.width)
	{
		this->imageLimit_max.x = this->imageLimit_min.x + (this->imageLimit_max.x - this->imageLimit_min.x) / this->compositeSize.width*width;
		this->compositeSize.width = width;
		
	}

	if (height > this->compositeSize.height)
	{
		this->imageLimit_min.y = this->imageLimit_max.y - (this->imageLimit_max.y - this->imageLimit_min.y) / this->compositeSize.height*height;
		this->compositeSize.height = height;
	}
}


//计算畸变矫正后的mask(因为畸变校正后会导致图像边缘产生黑色背景像素）
void Composite::getUndistMask(cv::Mat &mask, cv::Mat &mask_undist)
{
	//没有设置矫正参数
	if (this->cameraFile == "")
	{
		mask_undist = mask;
		return;
	}

	CameraParams camera;
	cv::Size size;
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;

	camera.readCalibratorFiles(this->cameraFile, size, cameraMatrix, distCoeffs);
	//矫正参数与图片不匹配
	if (size != this->imageSize)
	{
		mask_undist = mask;
		return;
	}

	undistort(mask, mask_undist, cameraMatrix, distCoeffs);
}

void Composite::compositeImage()
{
	
	this->unifyResolution();
	this->clcCompositeSize();
	this->clcOffsets();

	this->outputCoords();

	//this->compositeSize.height += 200;


	Blenders blender;
	blender.setBlendersName(this->compositeName.c_str());
	blender.prepare(this->compositeSize);

	cv::Mat _mask_undist = cv::Mat(this->imageSize, CV_8U);
	_mask_undist.setTo(cv::Scalar::all(255));
	cv::Mat mask_undist;
	//不再设置相机文件，相当于已经弃用以下功能。为方便起见，这里不再修改。。启用了一些和mask有关的
	this->getUndistMask(_mask_undist, mask_undist);
	_mask_undist.release();


	//cv::Point2f center = cv::Point2f(this->imageSize.width / 2, this->imageSize.height / 2);  // 旋转中心 

	for (int i = 0; i < this->imageNames.size(); i++)
	{
		cv::Mat image = cv::imread(this->imageNames[i]);
		

		double angle = this->imageAngles[i] * 180 / CV_PI;
		
		int width = this->imageDesc[i].sz.width;
		int height = this->imageDesc[i].sz.height;
		cv::Mat image_warp;
		cv::Mat mask_warp;


		//图像放大
		if (this->imageDesc[i].scale >= 1)
		{

			//扩充边界后再旋转与放大，否则会被裁剪
			//上边界
			int top = (height - this->imageSize.height) / 2;
			//下边界
			int bottom = height - top - this->imageSize.height;
			//左边界
			int left = (width - this->imageSize.width) / 2;
			//左边界
			int right = width - left - this->imageSize.width;



			cv::Point2f center = cv::Point2f(left + this->imageSize.width / 2.0, top + this->imageSize.height / 2.0);  // 旋转中心

			cv::Mat rotateMat = cv::getRotationMatrix2D(center, angle, this->imageDesc[i].scale);

			cv::Mat mask;
			mask = mask_undist.clone();

			cv::Mat image_border;
			cv::Mat mask_border;


			//先扩充图像边界
			cv::copyMakeBorder(image, image_border, top, bottom, left, right, cv::BORDER_CONSTANT, cv::Scalar(0));
			cv::copyMakeBorder(mask, mask_border, top, bottom, left, right, cv::BORDER_CONSTANT, cv::Scalar(0));
			image.release();
			mask.release();

			cv::warpAffine(image_border, image_warp, rotateMat, this->imageDesc[i].sz);
			/*	cv::imshow("sss", image);
				cv::waitKey(0);*/

			cv::warpAffine(mask_border, mask_warp, rotateMat, this->imageDesc[i].sz, cv::INTER_NEAREST);
			rotateMat.release();
			image_border.release();
			mask_border.release();

		}
		else //缩小
		{
			cv::Point2f center = cv::Point2f(this->imageSize.width / 2.0, this->imageSize.height / 2.0);  // 旋转中心
			cv::Mat rotateMat = cv::getRotationMatrix2D(center, angle, 1.0);

			cv::Mat mask;
			mask = mask_undist.clone();
			cv::Mat image_rotate;
			cv::Mat mask_rotate;
			
			cv::warpAffine(image, image_rotate, rotateMat, this->imageDesc[i].rotateSz);
			cv::warpAffine(mask, mask_rotate, rotateMat, this->imageDesc[i].rotateSz);

			cv::resize(image_rotate, image_warp, this->imageDesc[i].sz);
			cv::resize(mask_rotate, mask_warp, this->imageDesc[i].sz);



			rotateMat.release();
			image_rotate.release();
			mask_rotate.release();
		}
		//cv::imwrite("../" + to_string(i) + "rotate.jpg", image_warp);
		//cv::imwrite("../" + to_string(i) + "mask.jpg", mask_warp);


		blender.feed(image_warp, mask_warp, cv::Point2i(this->imageDesc[i].dx, this->imageDesc[i].dy));


		image_warp.release();
		mask_warp.release();

#ifdef ENABLE_LOG
		LOGLN(i);
#endif // ENABLE_LOG

	}

	blender.blend();

#ifdef ENABLE_LOG
	LOGLN("#"<<this->compositeSize.width<<","<<this->compositeSize.height);
#endif // ENABLE_LOG
	


}


void Composite::outputCoords()
{
	string fileName = this->compositeName.substr(0, this->compositeName.find_last_of(".")) + ".tfw";


	//x方向的像素分辨率
	double A = (this->imageLimit_max.x - this->imageLimit_min.x) / this->compositeSize.width;
	//y方向的像素分辨率
	double E = (this->imageLimit_max.y - this->imageLimit_min.y) / this->compositeSize.height;

	//左上角中心x坐标
	double C = this->imageLimit_min.x;

	//左上角中心y坐标
	double F = this->imageLimit_max.y;

	if (_access(fileName.c_str(), 0))
	{
		remove(fileName.c_str());
	}

	ofstream outfile(fileName);
	
	if (outfile)
	{
		
		char tmp[30];
		
		//x方向的像素分辨率
		sprintf(tmp, "%.10f", A);
		outfile << tmp << endl;

		//旋转系数
		outfile << "0.0" << endl;

		//旋转系统
		outfile << "0.0" << endl;

		//y方向的像素分辨率
		sprintf(tmp, "%.10f", E);
		outfile << tmp << endl;

		//左上角中心x坐标
		sprintf(tmp, "%.10f", C);
		outfile << tmp << endl;

		//左上角中心y坐标
		sprintf(tmp, "%.10f", F);
		outfile << tmp << endl;

		
		outfile.close();
	}


}