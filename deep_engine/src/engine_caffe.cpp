/*************************************************************************
    > File Name: engine_caffe.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Tue 13 Dec 2016 05:22:31 PM CST
 ************************************************************************/

#include <iostream>
#include <opencv2/opencv.hpp>
#include "caffe/blob.hpp"
#include "caffe/net.hpp"
#include "engine_caffe.h"
#include <boost/shared_ptr.hpp>

using namespace cv;
using namespace caffe;

namespace singlechar{

CharEngineCaffe::CharEngineCaffe(const char * prototxt, const char * caffemodel, const char * label_dict, int w, int h):CharEngine(label_dict)
{
	m_model.reset( new Net<float>(prototxt,caffe::TEST) );
	m_input_blob = NULL;
	m_width = -1;
	m_height = -1;
	if(m_model != NULL){
		m_model -> CopyTrainedLayersFrom(caffemodel);
		m_input_blob.reset( m_model -> input_blobs()[0] );
		m_input_blob -> Reshape(1,3,h,w);
		m_width = w;
		m_height = h;
		m_clsnum = m_model -> blob_by_name("prob") -> channels();
	}
}

CharEngineCaffe::~CharEngineCaffe(){

}

bool CharEngineCaffe::GetTopN(Mat im, int n, vector<int>& topn, vector<float>& topnconf){
	if(m_model==NULL)
		return false;
	if(im.type()!=CV_32FC3)
		return false;
	boost::shared_ptr<caffe::Blob<float> > prob_blob=m_model->blob_by_name("prob");
	if(prob_blob->channels()<n)
		return false;
	if(im.cols!=m_width||im.rows!=m_height)
		resize(im,im,Size(m_width,m_height));

	float * input_data = m_input_blob->mutable_cpu_data();
	float * data_ptr = new float[m_input_blob->count()];
	vector<Mat> mat_vec;
	split(im, mat_vec);
	for (int i = 0; i < 3; ++i) {
		float* src_ptr = reinterpret_cast<float *>(mat_vec[i].data);
		caffe_copy(im.total(), src_ptr, data_ptr+im.total()*i);
	}
	caffe_copy(m_input_blob->count(),data_ptr,input_data);
	
	//vector<caffe::Blob<float>* > input_vec;
	//m_model -> Forward(input_vec);
	m_model -> Forward();
	const float* prob_blob_data = prob_blob->cpu_data();
	map<float,int> probs;
	for(int d=0;d<prob_blob->channels();d++){
		probs.insert(pair<float,int>(-prob_blob_data[d],d));
	}
	//sort(probs.begin(),probs.end());
	topn.resize(n);
	topnconf.resize(n);
	auto it = probs.begin();
	for(int d=0; d<n; d++){
		topnconf[d] = -it->first;
		topn[d] = it->second;
		it++;
	}
	delete data_ptr;

#ifdef __DEBUG_MODE__
	cout<<"res: "<<topn[0]<<" saved:"<<m_debugpath<<endl;
	imwrite(m_debugpath,im);
#endif

}

}
