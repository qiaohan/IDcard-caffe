/*************************************************************************
    > File Name: detect/include/TextDetector.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Tue 17 Jan 2017 09:12:11 AM CST
 ************************************************************************/

#ifndef __TEXTDETECTOR__
#define __TEXTDETECTOR__

#include<iostream>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
class TextDetector{
	public:
		TextDetector(string path = "engine_bin/");
		~TextDetector();
		bool detect(Mat&,vector<Rect>&);
	private:
};

#endif
