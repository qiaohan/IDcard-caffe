/*************************************************************************
    > File Name: Textline4idcard.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Mon 16 Jan 2017 02:06:01 PM CST
 ************************************************************************/

#ifndef __IDCARDTEXTLINEREADER__
#define __IDCARDTEXTLINEREADER__

#include<iostream>
#include "Textline.h"
#include "opencv2/opencv.hpp"
#include "engine_caffe.h"

using namespace singlechar;
using namespace cv;
using namespace std;

#define KW_NO 0
#define KW_NAME 1
#define KW_ADDR 2
#define KW_NUM 3

class IDcardTextlineReader: public TextLineReader{
	public:
		IDcardTextlineReader(string path="engine_bin/");
		~IDcardTextlineReader();
		bool ReadTextRect(const Mat& img, vector<Rect>& rt, bool darkonlight=true);
		bool ReadText(const Mat& img, vector<Rect>& rt, string& str, bool darkonlight);
	protected:
		inline void recog_part(const Mat& im, const Rect& r, int &cls, float &prob);
		inline void recog_part(const Mat& im, const Rect& r, vector<int> &cls, vector<float> &prob);
		inline void recog_char(const Mat& im, const Rect& r, vector<int> &cls, vector<float> &prob, int topn=8);
		inline void recog_char(const Mat& im, const Rect& r, int &cls, float &prob);
		inline void recog_num(const Mat& im, const Rect& r, int &cls, float &prob);

	private:
		
		CharEngine *m_IDnumEngine;
		CharEngine *m_KeywordEngine;
		CharEngine *m_PartEngine;
		CharEngine *m_RecogEngine;
	
		int m_alphbet_size;
		int m_partcls_size;
};

#endif
