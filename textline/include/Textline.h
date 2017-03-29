/*************************************************************************
    > File Name: ../Textline_opencvWarp.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Fri 30 Sep 2016 11:34:15 AM CST
 ************************************************************************/
#ifndef __TEXTLINE_READER__
#define __TEXTLINE_READER__

#include <iostream>
#include "opencv2/opencv.hpp"
#include "tools.h"
//#include "engine_caffe.h"
#include "4debug.h"

using namespace std;
using namespace cv;
//using namespace singlechar;
//using namespace tline;

#define BG 0
#define CN 1
#define NUM 2
#define PART 3
#define TWO 4

class TextLineReader{
	public:
		TextLineReader(string path = "engine_bin/");
		~TextLineReader();
		bool ReadText(const Mat& img, vector<Rect>& rt, vector<int>& char_cls, bool darkonlight=true);
		virtual bool ReadTextRect(const Mat& img,vector<Rect>& rt, bool darkonlight=true);
		bool ReadTextType(const Mat& img,vector<Rect>& rt,vector<int>& part_cls, bool darkonlight=true);
		void setImgName(string t){ m_Imagename=t; }
		void setDebugPath(string p){ m_DebugPath = p; }
		void setInMatMode(int m){ m_inmat_mode=m; }
	
	protected:
		bool CombineRects(const Mat& img,vector<Rect>& rt);
		void CombineRect(const Mat& img, vector<Rect>& r);

		virtual float score_shape(const Mat im, const Rect& r, const float ewh, const float ewidth);
		virtual float score_single_rect(const Mat im, const Rect& r);
		virtual void recog_part(const Mat& im, const Rect& r, int &cls, float &prob)=0;
		virtual void recog_part(const Mat& im, const Rect& r, vector<int> &cls, vector<float> &prob)=0;
		virtual void recog_char(const Mat& im, const Rect& r, int &cls, float &prob)=0;
		virtual void recog_char(const Mat& im, const Rect& r, vector<int> &cls, vector<float> &prob, int topn=8)=0;
		
		int m_inmat_mode;

		string m_Imagename="test";
		string m_DebugPath = "res";
		void DrawRects(string path,const Mat& img,vector<Rect>& rt);
};
#endif
