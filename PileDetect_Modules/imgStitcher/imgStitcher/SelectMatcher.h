#pragma once
#include <iostream>
#include <vector>
#include "opencv2/highgui.hpp"


using namespace std;
using namespace cv;
struct node
{
	float value;
	int index;
};

class SelectMatcher
{
public:


	void selectMatching(vector<double> &x, vector<double> &y,Mat& flag_maks);

private:
	const int pairNum = 25;
};

