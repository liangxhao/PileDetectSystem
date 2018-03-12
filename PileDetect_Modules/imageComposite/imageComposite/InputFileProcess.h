#pragma once
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"
#include <fstream>  


//字符串分割
std::vector<double> split(std::string str, std::string pattern = ",");

//读取图像叠加所需的参数文件
void readCompositeTxt(const std::string fileName, std::vector<std::string> &imageName, std::vector<double> &angles, cv::Mat &limit_x, cv::Mat &limit_y);

//读取图像几何校正所需的参数文件
void readCorrectTxt(const std::string fileName, std::vector<std::string> &imageName, cv::Mat &coeff_x,cv::Mat &coeff_y, cv::Mat &limit_x, cv::Mat &limit_y);