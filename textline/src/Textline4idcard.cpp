/*************************************************************************
    > File Name: Textline4idcard.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Mon 16 Jan 2017 02:06:01 PM CST
 ************************************************************************/

#include "Textline4idcard.h"
#include<iostream>
#include "opencv2/opencv.hpp"
#include "engine_caffe.h"

using namespace singlechar;
using namespace cv;
using namespace std;


IDcardTextlineReader::IDcardTextlineReader(string path){
	m_KeywordEngine = new CharEngineCaffe( (path+"kw_deploy.prototxt").c_str(), (path+"kw.caffemodel").c_str(), (path+"").c_str(), 64, 64*3);
	m_PartEngine = new CharEngineCaffe( (path+"part_deploy.prototxt").c_str(),(path+"part.caffemodel").c_str(), (path+"").c_str(), 64,64);
	m_RecogEngine = new CharEngineCaffe( (path+"recog_deploy.prototxt").c_str(),(path+"recog.caffemodel").c_str(),(path+"word_dict.txt").c_str(),64,64);
	m_IDnumEngine = new CharEngineCaffe( (path+"idnum_deploy.prototxt").c_str(),(path+"idnum.caffemodel").c_str(),(path+"idnum_dict.txt").c_str(),64,64);
	cout<<"TextLineReader read Engine from Path: "<<path<<endl;
	
	m_alphbet_size = 3756;
	m_partcls_size = 5;
}

IDcardTextlineReader::~IDcardTextlineReader(){
	delete m_KeywordEngine;
	delete m_PartEngine;
	delete m_RecogEngine;

}

bool getbt(const Mat& imgt, int &bottom, int &top){
	Mat binaryimg,img = imgt.clone();
	int thre = 45;
	equalizeHist(img,img);
	//imwrite("gray/"+m_Imagename+".jpg",img);
	if(1)
		threshold(img,binaryimg,thre,255,THRESH_BINARY_INV);
	else
		threshold(img,binaryimg,thre,255,THRESH_BINARY);
	//imwrite("bi/"+m_Imagename+".jpg",binaryimg);
	/*
	 * projection in vertical direction
	 */
	vector<float> verproj(binaryimg.rows);
	top = -1;
	bottom = -1;
	float threv = 0.1;
	float maxv = 0.0;
	for(int j=0; j<verproj.size(); j++){
		verproj[j] = 0;
		for(int i=0; i<binaryimg.cols; i++){
			if(binaryimg.at<uchar>(j,i)>1)
				verproj[j]++;
		}
		maxv = maxv>verproj[j]?maxv:verproj[j];
	}
	for(int i=0; i<verproj.size(); i++)
		verproj[i]/=maxv;
	for(int j=1; j<verproj.size()/2; j++){
		if(verproj[j]>threv&&verproj[j-1]<threv)
		{
			top = j;
			break;
		}
	}
	for(int j=verproj.size()-2; j>verproj.size()/2; j--){
		if(verproj[j]>threv&&verproj[j+1]<threv)
		{
			bottom = j;
			break;
		}
	}
	if(-1==top || -1==bottom)
		return false;
	else
		return true;
}

int getpatchs(const Mat& textl_mat, Mat& outpatch){
	Mat him,bimg;
	cvtColor(textl_mat, him, CV_BGR2GRAY);
	equalizeHist(him,him);
	threshold(him,bimg,100,1,THRESH_BINARY_INV);
	int start;
	for(int i=0; i<bimg.cols; i++){
		int cnt=0;
		for(int j=0; j<bimg.rows; j++)
			cnt += bimg.at<uchar>(i,j);
		if(cnt>0){
			start = i;
			break;
		}
	}
	//cout<<"start:"<<start<<endl;
	int b,t;
	if(!getbt(him,b,t))
		return 0;
	//cout<<"bottom:"<<b<<" top:"<<t<<endl;
	int end = (b-t)*3+start;
	uchar bb = textl_mat.at<uchar>(0,0);
	uchar g = textl_mat.at<uchar>(0,1);
	uchar r = textl_mat.at<uchar>(0,2);
	Mat patchim = Mat((b-t),3*(b-t),textl_mat.type(),Scalar(bb,g,r));
	if(end>him.cols){
		//cout<<"end > im cols"<<endl;
		Rect patchrt = Rect(start,t,him.cols-start,b-t);
		textl_mat(patchrt).copyTo( patchim );//(Rect(0,0,him.cols-start,patchim.rows)) );
	}
	else{
		//cout<<"end <= im cols"<<endl;
/*
		for(int i=end; i<bimg.cols; i++){
			int cnt=0;
			for(int j=0; j<bimg.rows; j++)
				cnt += bimg.at<uchar>(i,j);
			if(cnt<=5||i==bimg.cols-1){
				end = i;
				break;
			}
		}
*/
		Rect patchrt = Rect(start,t,end-start,b-t);
		//cout<<patchrt<<','<<textl_mat.size()<<endl;
		//cout<<Rect(0,0,end-start,patchim.rows)<<','<<patchim.size()<<endl;
		textl_mat(patchrt).copyTo( patchim ); //(Rect(0,0,end-start,patchim.rows)) );
	}
	outpatch = patchim;
	//imwrite(savepath,patchim);
}

int findcutcolumn(const Mat& im){
	return 0;
}

bool IDcardTextlineReader::ReadText(const Mat& img, vector<Rect>& rt, string& str, bool darkonlight){
	vector<int> idxs,part_cls;
	if( !TextLineReader::ReadTextType(img,rt,part_cls,darkonlight) )
		return false;
	str="";
	float pp;
	idxs.resize(rt.size());
	for(int i=0; i<idxs.size(); i++){
		if( part_cls[i] == NUM){
			recog_num(img, rt[i], idxs[i], pp);
			str += m_IDnumEngine -> getchar(idxs[i]); 
		}else{
			recog_char(img, rt[i], idxs[i], pp);
			str += m_RecogEngine -> getchar(idxs[i]);
		}
#ifdef __DEBUG_MODE__
		cout<<idxs[i]<<endl;
#endif
	}
	return true;
}
bool IDcardTextlineReader::ReadTextRect(const Mat& img, vector<Rect>& rt, bool darkonlight){
	Mat patch;
	if(0==getpatchs(img,patch))
		return false;

	int cls;
	float prob;
	Mat rectim = patch.clone();
	rectim.convertTo(rectim, CV_32FC3);
	m_KeywordEngine -> RecognizeChar(rectim, cls, prob);
	cout<<"starting rec"<<endl;
	//if(KW_NO==cls)
	//	return false;
	int k = findcutcolumn(patch);
	cout<< k <<endl;
	TextLineReader::ReadTextRect(img(Rect(k,0,img.cols-k,img.rows)),rt,darkonlight);
}

inline void IDcardTextlineReader::recog_part(const Mat& im, const Rect& r, vector<int> &cls, vector<float> &prob){
	Mat rectim = AlignRect( im(r).clone() );
	rectim.convertTo(rectim, CV_32FC3);
#ifdef __DEBUG_MODE__
	static int i=0;
	char ss[200];
	sprintf(ss,"partim/_%d.jpg",i++);
	m_PartEngine -> setDebugPath(string(ss));
#endif
	m_PartEngine -> GetTopN(rectim, m_partcls_size, cls, prob);
}
inline void IDcardTextlineReader::recog_part(const Mat& im, const Rect& r, int &cls, float &prob){
	Mat rectim = AlignRect( im(r).clone() );
	rectim.convertTo(rectim, CV_32FC3);
#ifdef __DEBUG_MODE__
	static int i=0;
	char ss[200];
	sprintf(ss,"partim/_%d.png",i++);
	m_PartEngine -> setDebugPath(string(ss));
#endif
	m_PartEngine -> RecognizeChar(rectim, cls, prob);
}
inline void IDcardTextlineReader::recog_char(const Mat& im, const Rect& r, vector<int> &cls, vector<float> &prob, int topn){
	Mat rectim = AlignRect( im(r).clone() );
	rectim.convertTo(rectim, CV_32FC3);
#ifdef __DEBUG_MODE__
	static int i=0;
	char ss[200];
	sprintf(ss,"charim/_%d.jpg",i++);
	m_RecogEngine -> setDebugPath(string(ss));
#endif
	m_RecogEngine -> GetTopN(rectim, topn, cls, prob);
}
inline void IDcardTextlineReader::recog_char(const Mat& im, const Rect& r, int &cls, float &prob){
	Mat rectim = AlignRect( im(r).clone() );
	rectim.convertTo(rectim, CV_32FC3);
#ifdef __DEBUG_MODE__
	static int i=0;
	char ss[200];
	sprintf(ss,"charim/_%d.png",i++);
	m_RecogEngine -> setDebugPath(string(ss));
#endif
	m_RecogEngine -> RecognizeChar(rectim, cls, prob);
}
inline void IDcardTextlineReader::recog_num(const Mat& im, const Rect& r, int &cls, float &prob){
	Mat rectim = AlignRect( im(r).clone() );
	rectim.convertTo(rectim, CV_32FC3);
#ifdef __DEBUG_MODE__
	static int i=0;
	char ss[200];
	sprintf(ss,"charim/_%d.png",i++);
	m_IDnumEngine -> setDebugPath(string(ss));
#endif
	m_IDnumEngine -> RecognizeChar(rectim, cls, prob);
}
