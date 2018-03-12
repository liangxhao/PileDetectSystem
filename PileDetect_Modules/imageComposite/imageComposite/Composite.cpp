#include "Composite.h"

#include "io.h"
#include "stdio.h"
//����ͼ������
void Composite::setImageNames(const vector<string> &names)
{
	//������һ��ͼ��
	assert(names.size()>0);

	this->imageNames = names;
	int num = names.size();
	
	this->imageDesc.resize(num);

	//��ȡͼ��ߴ�
	cv::Mat img = cv::imread(names[0]);
	this->imageSize = img.size();
	img.release();
	 
}

//����ͼ��ߴ�
void Composite::setImageAngles(const vector<double> &angles)
{
	//ͼ��������Ƕȵ�����������ͬ
	assert(angles.size() == this->imageAngles.size());

	this->imageAngles = angles;

	int width = this->imageSize.width;
	int height = this->imageSize.height;

	
	//������ת���ͼ��ߴ�
	for (int i = 0; i < angles.size(); i++)
	{
		double a = sin(angles[i]), b = cos(angles[i]);


		int width_rotate = int(height * fabs(a) + width * fabs(b));
		int height_rotate = int(width * fabs(a) + height * fabs(b));

		this->imageDesc[i].sz = cv::Size(width_rotate, height_rotate);
		this->imageDesc[i].rotateSz= cv::Size(width_rotate, height_rotate);
	}
	
}

//����ͼ��ĵ������귶Χ
//limits_x:  n*4   ÿ����ͼ�����Ͻǿ�ʼ��ʱ���ĸ����x��������
//limits_y:  n*4   ÿ����ͼ�����Ͻǿ�ʼ��ʱ���ĸ����y��������
void Composite::setImageLimits(const cv::Mat &limits_x, const cv::Mat &limits_y)
{
	//�������뱣��һ��
	assert(limits_x.rows == limits_y.rows);
	assert(limits_x.rows == this->imageNames.size());

	int num = limits_x.rows;

	//�洢�ĸ��ǵ����Сֵ�����ֵ
	double x_min=1.7e308, x_max= -1.7e308, y_min= 1.7e308, y_max= -1.7e308;

	double maxValue, minValue;

	//ÿ��ͼ��ͼ��Χ
	for (int i = 0; i < num; i++) {

		//ÿ�е�����ֵ
		cv::Mat points;

		//����x����ķ�Χ
		points = limits_x.row(i);
		cv::minMaxLoc(points, &minValue, &maxValue);
		this->imageDesc[i].limit.setLimitX(minValue, maxValue);

		x_min = x_min < minValue ? x_min : minValue;
		x_max = x_max > maxValue ? x_max : maxValue;

		
		//����y����ķ�Χ
		points = limits_y.row(i);
		cv::minMaxLoc(points, &minValue, &maxValue);
		this->imageDesc[i].limit.setLimitY(minValue, maxValue);

		y_min = y_min < minValue ? y_min : minValue;
		y_max = y_max > maxValue ? y_max : maxValue;


	}

	//��ͼ��ͼ��Χ
	this->imageLimit_min = Point(x_min, y_min);
	this->imageLimit_max = Point(x_max, y_max);

	/*this->imageLimit_min = cv::Point2f(x_min, y_min);
	this->imageLimit_max = cv::Point2f(x_max, y_max);*/

}



//����ͼ��ֱ��ʣ�ʹ���Ƿֱ�����ͬ
void Composite::unifyResolution()
{

	double xRes = 0, yRes = 0;
	//��һ��ͼ��ķֱ�����ΪĬ�Ϸֱ���
	xRes = (this->imageDesc[0].limit.max_x - this->imageDesc[0].limit.min_x) / this->imageDesc[0].sz.width;
	yRes = (this->imageDesc[0].limit.max_y - this->imageDesc[0].limit.min_y) / this->imageDesc[0].sz.height;
	double resolution = (xRes + yRes) / 2;

	for (int i = 1; i < this->imageNames.size(); i++)
	{
		xRes = (this->imageDesc[i].limit.max_x - this->imageDesc[i].limit.min_x) / this->imageDesc[i].sz.width;
		yRes = (this->imageDesc[i].limit.max_y - this->imageDesc[i].limit.min_y) / this->imageDesc[i].sz.height;
		double res = (xRes + yRes) / 2;

		//��Ե����ű���
		//double scale = resolution / res;
		double scale = res / resolution;

		this->imageDesc[i].scale = scale;


		//���¼���ͼ��ĳߴ�
		this->imageDesc[i].sz.width = round(this->imageDesc[i].sz.width*scale);
		this->imageDesc[i].sz.height = round(this->imageDesc[i].sz.height*scale);


	}

}

//������Ӻ����ͼ�ߴ�
void Composite::clcCompositeSize()
{
	vector<ImageDesc> _imageDesc(this->imageDesc);

	this->clcCompositeSizeX(_imageDesc);
	this->clcCompositeSizeY(_imageDesc);

	_imageDesc.clear();


}
//����x����ĳߴ�
void Composite::clcCompositeSizeX(vector<ImageDesc> &_imageDesc)
{
	//��������ͼ��Ŀ��
	///////////////////////////////////////////////

	//����ͼ���min_x��������
	sort(_imageDesc.begin(), _imageDesc.end(), compareX);

	//��ͼ��ͼ����
	//��������Ŀ��
	double width_pixels = 0;
	//��ǰ�����Ѿ�����ͼ������Ҷˣ�x_min������
	double xMin_geo = 0;
	//��ǰ�����Ѿ�����ͼ������Ҷˣ�x_max������
	double xMax_geo = 0;

	//x��С������������ÿ��ͼ��Ŀ�ȣ�ȥ���ص��Ĳ���

	//�ȳ�ʼ��Ϊ��һ��ͼ��Ŀ��
	width_pixels = _imageDesc[0].sz.width;
	xMin_geo = _imageDesc[0].limit.min_x;
	xMax_geo = _imageDesc[0].limit.max_x;

	for (int i = 1; i < _imageDesc.size(); i++) {

		//��i��ͼ��x�ķ�Χ
		double _xMin = _imageDesc[i].limit.min_x;
		double _xMax = _imageDesc[i].limit.max_x;

		//֮ǰ��ͼ�����i��ͼ���ཻ
		if (xMax_geo >= _xMin && xMax_geo <= _xMax)
		{

			//�ص������ؿ��
			double overlap = (xMax_geo - _xMin) / (xMax_geo - xMin_geo) * width_pixels;
			//����
			width_pixels = width_pixels + _imageDesc[i].sz.width - overlap;
			xMax_geo = _xMax;

		}
		//֮ǰ��ͼ�������i��ͼ��
		else if (xMax_geo > _xMin && xMax_geo > _xMax)
		{
			continue;
		}
		//֮ǰ��ͼ�����i��ͼ������
		else
		{
			//������������ؿ��
			double separate = (_xMin - xMax_geo) / (xMax_geo - xMin_geo) * width_pixels;
			//����
			width_pixels = width_pixels + _imageDesc[i].sz.width + separate;
			xMax_geo = _xMax;

		}

	}

	//�õ���Ⱥ���Ҫȡ��
	int width = ceil(width_pixels);

	this->compositeSize.width = width;
	//���¼���ͼ��Χ
	//��Ϊwidth_pixels�Ǿ�ȷ�Ķ�Ӧͼ��ȡ������Ҫ����ȡ����ƫ��
	//����x_min���䣬ֻ����x_max
	this->imageLimit_max.x = this->imageLimit_min.x + (this->imageLimit_max.x - this->imageLimit_min.x) / width_pixels*width;


}



//����y����ĳߴ�
void Composite::clcCompositeSizeY(vector<ImageDesc> &_imageDesc)
{

	//��������ͼ��ĸ߶�
	///////////////////////////////////////////////

	//����ͼ���min_y��������
	sort(_imageDesc.begin(), _imageDesc.end(), compareY);

	//��ͼ��ͼ����
	//��������Ŀ��
	double height_pixels = 0;
	//��ǰ�����Ѿ�����ͼ������Ҷˣ�y_min������
	double yMin_geo = 0;
	//��ǰ�����Ѿ�����ͼ������Ҷˣ�y_max������
	double yMax_geo = 0;

	//y��С������������ÿ��ͼ��ĸ߶ȣ�ȥ���ص��Ĳ���

	//�ȳ�ʼ��Ϊ��һ��ͼ��ĸ߶�
	height_pixels = _imageDesc[0].sz.height;
	yMin_geo = _imageDesc[0].limit.min_y;
	yMax_geo = _imageDesc[0].limit.max_y;

	for (int i = 1; i < _imageDesc.size(); i++) {

		//��i��ͼ��x�ķ�Χ
		double _yMin = _imageDesc[i].limit.min_y;
		double _yMax = _imageDesc[i].limit.max_y;

		//֮ǰ��ͼ�����i��ͼ���ཻ
		if (yMax_geo >= _yMin && yMax_geo <= _yMax)
		{

			//�ص������ظ߶�
			double overlap = (yMax_geo - _yMin) / (yMax_geo - yMin_geo) * height_pixels;
			//����
			height_pixels = height_pixels + _imageDesc[i].sz.height - overlap;
			yMax_geo = _yMax;

		}
		//֮ǰ��ͼ�������i��ͼ��
		else if (yMax_geo > _yMin && yMax_geo > _yMax)
		{
			continue;
		}
		//֮ǰ��ͼ�����i��ͼ������
		else
		{
			//������������ظ߶�
			double separate = (_yMin - yMax_geo) / (yMax_geo - yMin_geo) * height_pixels;
			//����
			height_pixels = height_pixels + _imageDesc[i].sz.height + separate;
			yMax_geo = _yMax;

		}

	}

	//�õ��߶Ⱥ���Ҫȡ��
	int height = ceil(height_pixels);

	this->compositeSize.height = height;
	//���¼���ͼ��Χ
	//��Ϊheight_pixels�Ǿ�ȷ�Ķ�Ӧͼ��ȡ������Ҫ����ȡ����ƫ��
	//����y_min���䣬ֻ����y_max
	this->imageLimit_max.y = this->imageLimit_min.y + (this->imageLimit_max.y - this->imageLimit_min.y) / height_pixels*height;

}


//����ÿ��ͼ���������ͼ��ƫ��
void Composite::clcOffsets()
{
	double xRate = this->compositeSize.width / (this->imageLimit_max.x - this->imageLimit_min.x);
	double yRate = this->compositeSize.height / (this->imageLimit_max.y - this->imageLimit_min.y);

	//ͼ�����Ͻǵ�����
	double xCor = this->imageLimit_min.x;
	double yCor = this->imageLimit_max.y;

	//��Ҫ���һ�µ��Ӻ��ͼ���Ƿ�ᳬ���߽�
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

	//�������ԭ�������ͼ��ߴ磬�����¼���
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


//�������������mask(��Ϊ����У����ᵼ��ͼ���Ե������ɫ�������أ�
void Composite::getUndistMask(cv::Mat &mask, cv::Mat &mask_undist)
{
	//û�����ý�������
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
	//����������ͼƬ��ƥ��
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
	//������������ļ����൱���Ѿ��������¹��ܡ�Ϊ������������ﲻ���޸ġ���������һЩ��mask�йص�
	this->getUndistMask(_mask_undist, mask_undist);
	_mask_undist.release();


	//cv::Point2f center = cv::Point2f(this->imageSize.width / 2, this->imageSize.height / 2);  // ��ת���� 

	for (int i = 0; i < this->imageNames.size(); i++)
	{
		cv::Mat image = cv::imread(this->imageNames[i]);
		

		double angle = this->imageAngles[i] * 180 / CV_PI;
		
		int width = this->imageDesc[i].sz.width;
		int height = this->imageDesc[i].sz.height;
		cv::Mat image_warp;
		cv::Mat mask_warp;


		//ͼ��Ŵ�
		if (this->imageDesc[i].scale >= 1)
		{

			//����߽������ת��Ŵ󣬷���ᱻ�ü�
			//�ϱ߽�
			int top = (height - this->imageSize.height) / 2;
			//�±߽�
			int bottom = height - top - this->imageSize.height;
			//��߽�
			int left = (width - this->imageSize.width) / 2;
			//��߽�
			int right = width - left - this->imageSize.width;



			cv::Point2f center = cv::Point2f(left + this->imageSize.width / 2.0, top + this->imageSize.height / 2.0);  // ��ת����

			cv::Mat rotateMat = cv::getRotationMatrix2D(center, angle, this->imageDesc[i].scale);

			cv::Mat mask;
			mask = mask_undist.clone();

			cv::Mat image_border;
			cv::Mat mask_border;


			//������ͼ��߽�
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
		else //��С
		{
			cv::Point2f center = cv::Point2f(this->imageSize.width / 2.0, this->imageSize.height / 2.0);  // ��ת����
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


	//x��������طֱ���
	double A = (this->imageLimit_max.x - this->imageLimit_min.x) / this->compositeSize.width;
	//y��������طֱ���
	double E = (this->imageLimit_max.y - this->imageLimit_min.y) / this->compositeSize.height;

	//���Ͻ�����x����
	double C = this->imageLimit_min.x;

	//���Ͻ�����y����
	double F = this->imageLimit_max.y;

	if (_access(fileName.c_str(), 0))
	{
		remove(fileName.c_str());
	}

	ofstream outfile(fileName);
	
	if (outfile)
	{
		
		char tmp[30];
		
		//x��������طֱ���
		sprintf(tmp, "%.10f", A);
		outfile << tmp << endl;

		//��תϵ��
		outfile << "0.0" << endl;

		//��תϵͳ
		outfile << "0.0" << endl;

		//y��������طֱ���
		sprintf(tmp, "%.10f", E);
		outfile << tmp << endl;

		//���Ͻ�����x����
		sprintf(tmp, "%.10f", C);
		outfile << tmp << endl;

		//���Ͻ�����y����
		sprintf(tmp, "%.10f", F);
		outfile << tmp << endl;

		
		outfile.close();
	}


}