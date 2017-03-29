/*************************************************************************
    > File Name: engine_caffe.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Tue 13 Dec 2016 05:22:31 PM CST
 ************************************************************************/
#ifndef __ENGINE_CAFFE__
#define __ENGINE_CAFFE__

#include <iostream>
#include <opencv2/opencv.hpp>
#include "caffe/blob.hpp"
#include "caffe/net.hpp"
#include "engine.h"

using namespace cv;
using namespace caffe;

namespace singlechar{
class CharEngineCaffe:public CharEngine{
	public:
		CharEngineCaffe(const char * prototxt, const char * caffemodel, const char * label_dict, int w, int h);
		~CharEngineCaffe();
		bool GetTopN(Mat im, int n, vector<int>& topn, vector<float>& topnconf);
	private:
		boost::shared_ptr< Net<float> > m_model;
		boost::shared_ptr< Blob<float> > m_input_blob;
		int m_width,m_height;
};
}
#endif
