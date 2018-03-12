#include "stdafx.h"
#include "SelectMatcher.h"
#include <algorithm>
bool cmp(node &x, node &y) { return x.value < y.value ? true : false; }
void SelectMatcher::selectMatching(vector<double> &x, vector<double> &y , Mat& flag_maks)
{
	int num_all = (int)x.size();
	if (num_all < pairNum)
	{
		flag_maks.create(num_all, num_all,CV_8U);
		flag_maks.setTo(1);
		return;

	}

	//任意两个坐标之间的距离
	node **dist_all = new node*[num_all];


	for (int i = 0; i < num_all; i++)
	{
		dist_all[i] = new node[num_all];
	}
	//calculate the distance matrix
	for (int i = 0; i < num_all; i++)
	{
		for (int j = i; j < num_all; j++)
		{
			dist_all[i][j].value = float(1e10)*float((x[i] - x[j])*(x[i] - x[j]) + (y[i] - y[j])*(y[i] - y[j]));//sqrt is not needed
			dist_all[i][j].index = j;
		}
	}
	for (int i = 0; i < num_all; i++)
	{
		for (int j = i-1; j >-1; j--)
		{
			dist_all[i][j].value = dist_all[j][i].value;
			dist_all[i][j].index = j;
		}
	}

	for (int i = 0; i < num_all; i++) {
		sort(dist_all[i], dist_all[i] + num_all, cmp);
	}

	flag_maks.create(num_all, num_all, CV_8U);
	flag_maks.setTo(0);

	//找出最近的
	for (int i = 0; i < num_all; i++)
	{
		uchar *mask_row = flag_maks.ptr<uchar>(i);
		for (int k = 0; k <pairNum; k++)
		{
			mask_row[dist_all[i][k].index] = 1;
			
		}
	}


	for (int i = 0; i < num_all; i++)
	{
		if (x[i] < 0 || y[i] < 0)
		{
			uchar *mask_row = flag_maks.ptr<uchar>(i);
			for (int j = 0; j < num_all; j++)
			{
				mask_row[j] = 1;
			}
		}
	}


	for (int i = 0; i < num_all; i++)
	{
		uchar *mask_row = flag_maks.ptr<uchar>(i);
		mask_row[i] = 0;
	}


	for (int i = 0; i < num_all; i++)
	{
		delete[] dist_all[i];
		dist_all[i] = NULL;
	}
	delete[] dist_all;
	dist_all = NULL;

}


