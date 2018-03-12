#include "GeoCorrection.h"

void GeoCorrection::setImageNames(const vector<string> &imageNames)
{ 
	assert(imageNames.size() > 0);

	this->imageNames = imageNames; 
	//��ȡͼ��ߴ�
	cv::Mat img = cv::imread(imageNames[0]);
	this->imageSize = img.size();
	img.release();
}


void GeoCorrection::setImageLimits(const cv::Mat &limits_x, const cv::Mat &limits_y)
{
	//�������뱣��һ��
	assert(limits_x.rows == limits_y.rows);
	assert(limits_x.rows == this->imageNames.size());


	int num = limits_x.rows;

	double maxValue, minValue;

	//ÿ��ͼ��ͼ��Χ
	for (int i = 0; i < num; i++) {

		//ÿ�е�����ֵ
		cv::Mat points;

		//����x����ķ�Χ
		points = limits_x.row(i);
		cv::minMaxLoc(points, &minValue, &maxValue);
		this->image_limits[i].setLimitX(minValue, maxValue);

		//����y����ķ�Χ
		points = limits_y.row(i);
		cv::minMaxLoc(points, &minValue, &maxValue);
		this->image_limits[i].setLimitY(minValue, maxValue);
	}
}

void GeoCorrection::setCoeff(const cv::Mat &coeff_x, const cv::Mat &coeff_y)
{
	//�������뱣��һ��
	assert(coeff_x.rows == coeff_y.rows);
	assert(coeff_x.rows == this->imageNames.size());
	this->coeff_x = coeff_x;
	this->coeff_y = coeff_y;
}



void GeoCorrection::coordsToMapping(int idx, const LimitCoords &limit, cv::Mat &mapping_x, cv::Mat &mapping_y)
{
	int height = this->imageSize.height;
	int width = this->imageSize.width;

	//ͨ��ͼ��Χ������ÿ�����ص��Ӧ�ĵ�������ֵ
	//����Ҫ��ȡ����б��У��ͼ������ÿһ�е�y������������ͬ�ģ�ÿһ�е�x������������ͬ�ģ���������coordMap�������͹��ˣ������ظ�����

	//x����ĵ�������ֵ
	cv::Mat coordMap_x(1, width, CV_64FC1);
	//y����ĵ�������ֵ
	cv::Mat coordMap_y(1, height, CV_64FC1);

	//��������֮��ľ�����
	double dx = (limit.max_x - limit.min_x) / (width - 1);
	double dy = (limit.max_y - limit.min_y) / (height - 1);

	double *data = nullptr;
	//��ֵ
	data = coordMap_x.ptr<double>(0);
	for (int i = 0; i < width; i++)
	{
		*data++ = limit.min_x + i*dx;
	}

	data = coordMap_y.ptr<double>(0);
	for (int i = 0; i < height; i++)
	{
		*data++ = limit.min_y + i*dy;
	}

	///
	//X^2
	cv::Mat x2 = coordMap_x.mul(coordMap_x);
	//X*Y
	cv::Mat xy = coordMap_x.mul(coordMap_y);
	//Y^2
	cv::Mat y2 = coordMap_y.mul(coordMap_y);

	double *coeff = nullptr;

	//����������������ԭͼ���������꣬�����ڴ�
	//ϵ��
	coeff = this->coeff_x.ptr<double>(idx);
	cv::Mat mapX = coeff[0] + coeff[1] * coordMap_x + coeff[2] * coordMap_y + coeff[3] * x2 + coeff[4] * xy + coeff[5] * y2;

	coeff = this->coeff_y.ptr<double>(idx);
	cv::Mat mapY = coeff[0] + coeff[1] * coordMap_x + coeff[2] * coordMap_y + coeff[3] * x2 + coeff[4] * xy + coeff[5] * y2;


	mapping_x = cv::repeat(mapX, height, 1);
	mapping_y = cv::repeat(mapY.t(), 1, width);


}

void GeoCorrection::geoCorrect()
{
	for (int i = 0; i < this->imageNames.size(); i++)
	{
		cv::Mat image = cv::imread(this->imageNames[i]);
		//У�����ͼ��
		cv::Mat correctImage;
		
		this->correctImage(image, correctImage, i);

		string saveName = this->imageNames[i] + this->suffix;

		cv::imwrite(saveName, correctImage);


#ifdef ENABLE_LOG
		LOGLN(i);
#endif // ENABLE_LOG
	}
}

void GeoCorrection::correctImage(const cv::Mat &image, cv::Mat correctImage, int idx)
{
	//ӳ��ͼ
	cv::Mat map_x, map_y;
	this->coordsToMapping(idx, this->image_limits[idx], map_x, map_y);

	//У��
	cv::remap(image, correctImage, map_x, map_y, cv::INTER_LINEAR);
}