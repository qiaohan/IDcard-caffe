/*************************************************************************
    > File Name: include/IDcardReader.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Mon 16 Jan 2017 05:20:18 PM CST
 ************************************************************************/

#include "Textline4idcard.h"
#include "TextDetector.h"
#include<iostream>

using namespace std;

class IDcardReader{
	public:
		IDcardReader(string path = "engine_bin/");
		~IDcardReader();
		bool ReadImg(const Mat& img, string& res);
	private:
		IDcardTextlineReader * m_tlinereader;
		TextDetector * m_detector;
		map<int,string> m_dict;
};
