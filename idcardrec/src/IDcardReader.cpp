/*************************************************************************
    > File Name: include/IDcardReader.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Mon 16 Jan 2017 05:20:18 PM CST
 ************************************************************************/

#include "Textline4idcard.h"
#include "TextDetector.h"
#include<iostream>
#include "BCRDeskew.h"
#include "opencv2/opencv.hpp"
#include "IDcardReader.h"

using namespace cv;
using namespace std;

IDcardReader::IDcardReader(string path){
		m_tlinereader = new IDcardTextlineReader(path);
		m_detector = new TextDetector(path);
}

IDcardReader::~IDcardReader(){
	delete m_detector;
	delete m_tlinereader;
}
bool IDcardReader::ReadImg(const Mat& img, string& res){
	//Mat im = BCRDeskew(img,1000);
	Mat im = img.clone();
	if(im.empty())
		return false;
	vector<Rect> tlinerects;
	if(!m_detector->detect(im,tlinerects))
		return false;
	cout<<"start textline:"<<tlinerects.size()<<endl;
	res.clear();
	for(int i=0; i<tlinerects.size(); i++){
		//vector<int> char_idx;
		string text;
		vector<Rect> char_rect;
		cout<<tlinerects[i]<<endl;
		//m_tlinereader -> set
		if( m_tlinereader->ReadText(img(tlinerects[i]),char_rect,text,true) ){
			res+=text;
		}
	}
	return true;
}
