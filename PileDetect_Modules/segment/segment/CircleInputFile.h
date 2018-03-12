#pragma once
#include <iostream>
#include <vector>
#include <fstream>  

using namespace std;

class CircleInputFile
{
public:
	CircleInputFile(const char *filename);
	~CircleInputFile();
public:
	int readCoordsFile(vector<int> &index, vector<int> &xRcoords, vector<int> &yRcoords, vector<int> &radius);

private:
	const char* filename;
};

