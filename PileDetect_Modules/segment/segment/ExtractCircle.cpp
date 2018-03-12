#include "ExtractCircle.h"
#include <fstream>  


ExtractCircle::ExtractCircle(const char* pCoordsFile, const char* pImageName)
{
	this->pCoordsFile = pCoordsFile;
	this->pImageName = pImageName;

	block=nullptr;//获取块坐标
	blockImage = nullptr;//获取块图像
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
	//通过输入文件获取每个圆的坐标
	/*vector<int> index;*/
	vector<int> xRcoords;
	vector<int> yRcoords;
	vector<int> radius;

	CircleInputFile coordsFile= CircleInputFile(this->pCoordsFile);
	coordsFile.readCoordsFile(this->index, xRcoords, yRcoords, radius);

	//用来获取图像信息和图像块
	blockImage =new ReadImageBlock(this->pImageName);
	blockImage->getImageSize(width, height);

	//去掉超过图像边界的点
	this->filter(xRcoords, yRcoords, radius);

	//计算每个图像块的坐标
	block = new BlockCoords();
	block->setCoords(this->index, xRcoords, yRcoords, radius);
	block->setImageSize(width, height);


}


void ExtractCircle::run()
{

	this->prepare();
	//保存结果至文件
	ofstream outfile(this->pExportFile);
	if (!outfile)
	{
		cout << "save result failed!";
	}
	outfile << "index,rx,ry,radius,succeed" << endl;
	outfile << this->index.size() << endl;


	//用来分割和拟合
	FitSegmentCircle *fit = nullptr;
	//拟合结果
	int objX = 0;
	int objY = 0;
	int objRadius = 0;
	int flag = 0;

	//图像块的左上角坐标、尺寸、圆心相对于图像块的坐标
	int dx, dy, blockWidth, blockHeigh, rx, ry;
	int number=0;//当前处理圆的序号

	cout << "num:" << this->index.size() << endl;

	//处理每一个圆
	for(int i=0;i<this->index.size();i++)
	{
		//当前图像块的序号
		number = block->getNextBlockCoords(dx, dy, blockWidth, blockHeigh, rx, ry);
		int radius = block->getCurrentRadius();

		//获取该图像块
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


		//输出结果
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
//去掉超过图像边界的点
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

