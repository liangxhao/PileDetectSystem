#include "CircleInputFile.h"

std::vector<std::string> splitString(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//��չ�ַ����Է������
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos;
		}
	}
	return result;
}

CircleInputFile::CircleInputFile(const char *filename)
{
	this->filename = filename;
}


CircleInputFile::~CircleInputFile()
{
}

int CircleInputFile::readCoordsFile(vector<int>&index, vector<int> &xRcoords, vector<int> &yRcoords, vector<int> &radius)
{
	//��ȡtxt�ļ�
	ifstream infile(this->filename);
	if (!infile.is_open())
	{
		cout << "�޷��������ļ�";
		return 0;
	}
	index.clear();
	xRcoords.clear();
	yRcoords.clear();
	radius.clear();


	char tmp[100];
	vector<string> tmpStr;
	int numPoints = 0;

	//��ȡ��һ��
	infile >> tmp;
	numPoints = atoi(tmp);


	for (int i = 0; i < numPoints; i++)
	{
		infile >> tmp;
		tmpStr = splitString(string(tmp), ",");

		index.push_back(atoi(tmpStr[0].c_str()));
		xRcoords.push_back(atoi(tmpStr[1].c_str()));
		yRcoords.push_back(atoi(tmpStr[2].c_str()));
		radius.push_back(atoi(tmpStr[3].c_str()));

	}

	infile.close();

	return 1;
}
