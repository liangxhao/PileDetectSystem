#pragma once
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"
#include <fstream>  


//�ַ����ָ�
std::vector<double> split(std::string str, std::string pattern = ",");

//��ȡͼ���������Ĳ����ļ�
void readCompositeTxt(const std::string fileName, std::vector<std::string> &imageName, std::vector<double> &angles, cv::Mat &limit_x, cv::Mat &limit_y);

//��ȡͼ�񼸺�У������Ĳ����ļ�
void readCorrectTxt(const std::string fileName, std::vector<std::string> &imageName, cv::Mat &coeff_x,cv::Mat &coeff_y, cv::Mat &limit_x, cv::Mat &limit_y);