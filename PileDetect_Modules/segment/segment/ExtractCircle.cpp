#include "ExtractCircle.h"
#include <fstream>  


ExtractCircle::ExtractCircle(const char* pCoordsFile, const char* pImageName)
{
	this->pCoordsFile = pCoordsFile;
	this->pImageName = pImageName;

	block=nullptr;//��ȡ������
	blockImage = nullptr;//��ȡ��ͼ��
}


ExtractCircle::~ExtractCircle()
{
	if (block != nullptr)
	{
		delete block;
		block = nullptr;
	}

	if (blockImage != nullptr)
	{
		delete blockImage;
		blockImage = nullptr;
	}
}

void ExtractCircle::setExportFileName(const char* pExportFile)
{
	this->pExportFile = pExportFile;
}

void ExtractCircle::prepare()
{
	//ͨ�������ļ���ȡÿ��Բ������
	/*vector<int> index;*/
	vector<int> xRcoords;
	vector<int> yRcoords;
	vector<int> radius;

	CircleInputFile coordsFile= CircleInputFile(this->pCoordsFile);
	coordsFile.readCoordsFile(this->index, xRcoords, yRcoords, radius);

	//������ȡͼ����Ϣ��ͼ���
	blockImage =new ReadImageBlock(this->pImageName);
	blockImage->getImageSize(width, height);

	//ȥ������ͼ��߽�ĵ�
	this->filter(xRcoords, yRcoords, radius);

	//����ÿ��ͼ��������
	block = new BlockCoords();
	block->setCoords(this->index, xRcoords, yRcoords, radius);
	block->setImageSize(width, height);


}


void ExtractCircle::run()
{

	this->prepare();
	//���������ļ�
	ofstream outfile(this->pExportFile);
	if (!outfile)
	{
		cout << "save result failed!";
	}
	outfile << "index,rx,ry,radius,succeed" << endl;
	outfile << this->index.size() << endl;


	//�����ָ�����
	FitSegmentCircle *fit = nullptr;
	//��Ͻ��
	int objX = 0;
	int objY = 0;
	int objRadius = 0;
	int flag = 0;

	//ͼ�������Ͻ����ꡢ�ߴ硢Բ�������ͼ��������
	int dx, dy, blockWidth, blockHeigh, rx, ry;
	int number=0;//��ǰ����Բ�����

	cout << "num:" << this->index.size() << endl;

	//����ÿһ��Բ
	for(int i=0;i<this->index.size();i++)
	{
		//��ǰͼ�������
		number = block->getNextBlockCoords(dx, dy, blockWidth, blockHeigh, rx, ry);
		int radius = block->getCurrentRadius();

		//��ȡ��ͼ���
		cv::Mat img;
		blockImage->prepare();
		blockImage->read(dx, dy, blockWidth, blockHeigh, img);
		blockImage->end();

		//imshow("sss", img);
		//waitKey(0);

		cv::Mat gray;
		cv::cvtColor(img, gray, CV_BGR2GRAY);
		img.release();

		fit = new FitSegmentCircle(gray);
		fit->setPriorParms(rx, ry, radius);
		flag = fit->getCircle(objX, objY, objRadius);

		gray.release();


		//������
		cout << this->index[number] << "," << dx+objX << "," << dy+objY << "," << objRadius << "," << flag << endl;
		outfile << this->index[number] << "," << dx+objX << "," << dy+objY << "," << objRadius << "," << flag << endl;

		if (fit != nullptr)
		{
			delete fit;
			fit = nullptr;
		}

	}

	outfile.close();
	

}
//ȥ������ͼ��߽�ĵ�
void ExtractCircle::filter(vector<int> &xRcoords, vector<int> &yRcoords, vector<int> &radius)
{
	vector<int> id;
	vector<int> x;
	vector<int> y;
	vector<int> r;

	for (int i = 0; i < xRcoords.size(); i++)
	{
		if (xRcoords[i] >= 0 && xRcoords[i] < this->width &&
			yRcoords[i] >= 0 && yRcoords[i] < this->height)
		{
			id.push_back(this->index[i]);
			x.push_back(xRcoords[i]);
			y.push_back(yRcoords[i]);
			r.push_back(radius[i]);

		}
	}
	this->index.clear();
	xRcoords.clear();
	yRcoords.clear();
	radius.clear();

	this->index = id;
	xRcoords = x;
	yRcoords = y;
	radius = r;


}

