/*************************************************************************
    > File Name: test/test.cpp
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Tue 13 Dec 2016 09:57:01 PM CST
 ************************************************************************/

#include "engine_caffe.h"
#include "engine.h"
#include "opencv2/opencv.hpp"
#include "FilePathClass.h"
#include <iostream>
using namespace std;
using namespace cv;
using namespace singlechar;

int main(int argc, char ** argv){
	CharEngine * engine = new CharEngineCaffe("/home/hzqiaohan/OCR/danzi_deep/howmanychars/qiaohan/deploy.prototxt","/home/hzqiaohan/OCR/danzi_deep/howmanychars/qiaohan/numch_iter_10000.caffemodel",64,64);
	//CharEngine * engine = new CharEngineCaffe("/home/hzqiaohan/OCR/danzi_deep/IDcardBG/qiaohan/deploy.prototxt","/home/hzqiaohan/OCR/danzi_deep/IDcardBG/qiaohan/numch_iter_10000.caffemodel",64,64);
	//CharEngine * engine = new CharEngineCaffe("deploy.prototxt","recog.caffemodel",64,64);
	vector<PathPair> imgpaths = FilePathClass::getFilePath("../../howmanychars/data/man_qiaohan/test/",".jpg"); 
	cout<<"start test"<<endl;
	for(int i=0; i<imgpaths.size(); i++){
		Mat img = imread(imgpaths[i].fullPath,CV_LOAD_IMAGE_COLOR);
		img.convertTo(img, CV_32FC3);
		int cls=-1;
		float conf=-1;
		engine -> RecognizeChar(img, cls, conf);
		cout<<imgpaths[i].Name<<':'<<cls<<':'<<conf<<endl;
	}
	delete engine;
	return 0;
}
