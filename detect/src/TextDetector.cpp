/*************************************************************************
    > File Name: detect/include/TextDetector.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Tue 17 Jan 2017 09:12:11 AM CST
 ************************************************************************/

#include<iostream>
#include "TextDetector.h"
using namespace std;

		
TextDetector::TextDetector(string path){

}
TextDetector::~TextDetector(){

}
bool TextDetector::detect(Mat& img, vector<Rect>& tlines){
	tlines.clear();
	tlines.push_back(Rect(0,0,img.cols,img.rows));
	return true;
}
