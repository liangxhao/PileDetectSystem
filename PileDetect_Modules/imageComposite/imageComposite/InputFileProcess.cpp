#include "InputFileProcess.h"


std::vector<double> split(std::string str, std::string pattern )
{
	std::string::size_type pos;
	std::vector<double> result;
	str += pattern;//��չ�ַ����Է������
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(atof(s.c_str()));
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}


void readCompositeTxt(const std::string fileName, std::vector<std::string> &imageName, std::vector<double> &angles, cv::Mat &limit_x, cv::Mat &limit_y)
{
	std::ifstream infile(fileName);
	if (!infile.is_open())
	{
		return;
	}


	int num = 0;//ͼƬ����

	char tmp[300];
	infile >> tmp;
	num = atoi(tmp);

	//��ȡͼƬ��
	for (int i = 0; i < num; i++) {
		infile >> tmp;
		imageName.push_back(std::string(tmp));
	}

	//��ȡ�Ƕ�
	for (int i = 0; i < num; i++) {
		infile >> tmp;
		angles.push_back(atof(tmp));
	}
	

	//��ȡͼ��Χ��x����
	std::vector<double> limitX;
	limit_x.create(cv::Size(4, num), CV_64F);

	for (int i = 0; i < num; i++) {
		infile >> tmp;

		limitX = split(std::string(tmp));

		double* row = limit_x.ptr<double>(i);
		for (int j = 0; j < 4; j++)
		{
			row[j] = limitX[j];
			//cout << row[j] << "	";
		}
		//cout << endl;

	}

	//��ȡͼ��Χ��y����
	std::vector<double> limitY;
	limit_y.create(cv::Size(4, num), CV_64F);

	for (int i = 0; i < num; i++) {
		infile >> tmp;


		limitY = split(std::string(tmp));

		double* row = limit_y.ptr<double>(i);
		for (int j = 0; j < 4; j++)
		{
			row[j] = limitY[j];
			//cout << row[j] << "	"; 
		}
		//cout << endl;
	}

}



//��ȡͼ�񼸺�У������Ĳ����ļ�
void readCorrectTxt(const std::string fileName, std::vector<std::string> &imageName, cv::Mat &coeff_x, cv::Mat &coeff_y, cv::Mat &limit_x, cv::Mat &limit_y)
{
	std::ifstream infile(fileName);
	if (!infile.is_open())
	{
		return;
	}


	int num = 0;//ͼƬ����

	char tmp[300];
	infile >> tmp;
	num = atoi(tmp);

	//��ȡͼƬ��
	for (int i = 0; i < num; i++) {
		infile >> tmp;
		imageName.push_back(std::string(tmp));
	}


	//��ȡϵ����X
	std::vector<double> coeffX;
	coeff_x.create(cv::Size(6, num), CV_64F);

	for (int i = 0; i < num; i++)
	{
		infile >> tmp;

		coeffX = split(std::string(tmp));

		double* row = coeff_x.ptr<double>(i);
		for (int j = 0; j < 6; j++)
		{
			row[j] = coeffX[j];
			//cout << row[j] << "	";
		}
	}


	//��ȡϵ����y
	std::vector<double> coeffY;
	coeff_y.create(cv::Size(6, num), CV_64F);

	for (int i = 0; i < num; i++)
	{
		infile >> tmp;

		coeffY = split(std::string(tmp));

		double* row = coeff_y.ptr<double>(i);
		for (int j = 0; j < 6; j++)
		{
			row[j] = coeffY[j];
			//cout << row[j] << "	";
		}
	}



	//��ȡͼ��Χ��x����
	std::vector<double> limitX;
	limit_x.create(cv::Size(4, num), CV_64F);

	for (int i = 0; i < num; i++) 
	{
		infile >> tmp;

		limitX = split(std::string(tmp));

		double* row = limit_x.ptr<double>(i);
		for (int j = 0; j < 4; j++)
		{
			row[j] = limitX[j];
			//cout << row[j] << "	";
		}
		//cout << endl;

	}

	//��ȡͼ��Χ��y����
	std::vector<double> limitY;
	limit_y.create(cv::Size(4, num), CV_64F);

	for (int i = 0; i < num; i++) 
	{
		infile >> tmp;


		limitY = split(std::string(tmp));

		double* row = limit_y.ptr<double>(i);
		for (int j = 0; j < 4; j++)
		{
			row[j] = limitY[j];
			//cout << row[j] << "	"; 
		}
		//cout << endl;
	}
}