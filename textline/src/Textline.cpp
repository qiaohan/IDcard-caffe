/*************************************************************************
    > File Name: ../Textline_opencvWarp.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Fri 30 Sep 2016 11:34:15 AM CST
 ************************************************************************/

#include <iostream>
#include "Textline.h"
#include "opencv2/opencv.hpp"
#include "string.h"
using namespace std;
using namespace cv;

TextLineReader::TextLineReader(string path)
{
	m_inmat_mode = CV_8UC3;
	
}
TextLineReader::~TextLineReader()
{
	//delete m_KeywordEngine;
}
/*
 * char_cls here is the char classifier's output
 * */
bool TextLineReader::ReadText(const Mat& img, vector<Rect>& r, vector<int>& char_cls, bool darkonlight)
{
	vector<int> part_cls;
	vector<Rect> rs;
	if(!ReadTextType(img, rs, part_cls, darkonlight))
		return false;
	char_cls.clear();
	r.clear();
	for(int i = 0; i < rs.size(); i++){
		float prob;
		int c;
		Rect rrr;
		/*
		if(TWO == part_cls[i]){
			rrr = Rect(rs[i].tl(),Size(rs[i].width/2,rs[i].height) );
			recog_char(img, rrr, c, prob);	
			char_cls.push_back(c);
			r.push_back(rrr);

			rrr = Rect(rs[i].tl().x+rs[i].width/2,rs[i].tl().y,rs[i].width/2,rs[i].height);
			recog_char(img, rrr, c, prob);	
			char_cls.push_back(c);
			r.push_back(rrr);
		}else
		*/
		{
			r.push_back(rs[i]);
			recog_char(img, rs[i], c, prob);	
			char_cls.push_back(c);
		}
	}
	return true;
}
/*
 * part_cls is the part classifier's output instead of char(cn ,en, ...)'s
 * */
bool TextLineReader::ReadTextType(const Mat& img, vector<Rect>& rs, vector<int>& part_cls, bool darkonlight)
{
		
	if(!ReadTextRect(img, rs, darkonlight))
		return false;
	part_cls.resize(rs.size());
	for(int i = 0; i < rs.size(); i++){
		float prob;
		int c;
		recog_part(img, rs[i], c, prob);
		part_cls[i] = c;
	}
	return true;
}

bool TextLineReader::ReadTextRect(const Mat& textl_mat, vector<Rect>& rs, bool darkonlight)
{
	//img.channel() == 3
	/*
	Mat textl,him;
	cvtColor(textl_mat, textl, CV_BGR2HSV);
	vector<Mat> matvec;
	split(textl,matvec);
	him = matvec[0];
	equalizeHist(matvec[2],matvec[2]);
	merge(matvec,textl);
	*/
	Mat img;
	if(m_inmat_mode!=textl_mat.type())
		return false;
	if(m_inmat_mode == CV_8UC3)
		cvtColor(textl_mat, img, CV_BGR2GRAY);
	else
		img = textl_mat.clone();

	Mat binaryimg;
	int thre = 45;
	equalizeHist(img,img);
	//imwrite("gray/"+m_Imagename+".jpg",img);
	
	if(darkonlight)
		threshold(img,binaryimg,thre,255,THRESH_BINARY_INV);
	else
		threshold(img,binaryimg,thre,255,THRESH_BINARY);
	//imwrite("bi/"+m_Imagename+".jpg",binaryimg);
	/*
	 * projection in vertical direction
	 */
	vector<float> verproj(binaryimg.rows);
	int top = -1;
	int bottom = -1;
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
	/*
	 * projection in horizontal direction
	 */
	vector<int> horproj(binaryimg.cols);
	for(int i=0; i<horproj.size(); i++)
	{
		horproj[i] = 0;
		for(int j=0; j<binaryimg.rows; j++){
			if(binaryimg.at<uchar>(j,i)>1)
			horproj[i]++;		
		}
	}
	vector<int> leftp,rightp,topp,bottomp;
	int thredcnt = 0;
	bool forleft=true;
	if(horproj[0]>thredcnt){
		leftp.push_back(0);
		forleft = false;
	}
	for(int i=1; i<horproj.size(); i++){
		//cout<<i<<':'<<horproj[i]<<':'<<forleft<<endl;
		if(forleft){
			//if( (horproj[i]>5&&horproj[i-1]<=5)
			//	||(horproj[i]>5&&1==i) )
			if(horproj[i]>thredcnt&&horproj[i-1]<=thredcnt)
			{
				leftp.push_back(i);
				forleft=!forleft;
			}
		}
		else{
			//if( (horproj[i+1]>5&&horproj[i]<=5)
			//	||(horproj.size()-2==i&&horproj[i]>5) )
			if(horproj[i-1]>thredcnt&&horproj[i]<=thredcnt)
			{
				rightp.push_back(i);
				forleft=!forleft;
			}
		}
	}
	if(!forleft&&horproj[horproj.size()-1]>thredcnt)
		rightp.push_back(horproj.size()-1);
	if(leftp.size()==0||rightp.size()==0)
		return false;
	//vector<Rect> rs;
	rs.clear();
	for(int i=0; i<rightp.size(); i++){
		if(rightp[i]-leftp[i]<2)
			continue;
		//cout<<leftp[i]<<'-'<<rightp[i]<<endl;
		//if(rightp[i]-leftp[i]>1.2*(bottom-top))
		//	rs.push_back(Rect(rightp[i]-(bottom-top),top,bottom-top,bottom-top));
		//else
			rs.push_back(Rect(leftp[i],top,rightp[i]-leftp[i],bottom-top));
		//cout<<bottom<<','<<top<<','<<rs[i]<<endl;
	}
	return CombineRects(textl_mat,rs);
}

float TextLineReader::score_single_rect(const Mat im, const Rect& r){

}

float TextLineReader::score_shape(const Mat im, const Rect& r, const float ewh, const float ewidth){
	if( rectwh(r)>2 )
		return 0.0;
	/*
	vector<float> prob;
	int cls;
	recog_part(im, r, cls, prob);
	float ch_score = get_shape_score(r,ewh,ewidth)*prob[CN];
	float nch_score = get_shape_score(r,0.5*ewh,ewidth*0.5)*prob[NUM];
	return ch_score>nch_score?ch_score:nch_score;
	return ch_score;
	*/
	return get_shape_score(r,ewh,ewidth);
}

bool TextLineReader::CombineRects(const Mat& img, vector<Rect>& rt){
#ifdef __DEBUG_MODE__
	cout<<m_Imagename<<endl;
	cout<<"before:"<<rt.size()<<endl;
	DrawRects("res/before/"+m_Imagename+".png",img,rt);
#endif
	CombineRect(img,rt);
	vector<Rect> rs = rt;
	//char_cls.clear();
	rt.clear();
	for(int i = 0; i < rs.size(); i++){
		float prob;
		int c;
		Rect rrr;
		int part_cls;
		recog_part(img,rs[i],part_cls,prob);
		//cout<<"cls:"<<part_cls<<" prob:"<<prob<<endl;
		if(rectwh(rs[i])>1.0&&TWO == part_cls){
			rrr = Rect(rs[i].tl(),Size(rs[i].width/2,rs[i].height) );
			//recog_char(img, rrr, c, prob);	
			//char_cls.push_back(c);
			rt.push_back(rrr);

			rrr = Rect(rs[i].tl().x+rs[i].width/2,rs[i].tl().y,rs[i].width/2,rs[i].height);
			//recog_char(img, rrr, c, prob);	
			//char_cls.push_back(c);
			rt.push_back(rrr);
		}else{
			//recog_char(img, rs[i], c, prob);	
			//char_cls.push_back(c);
			rt.push_back(rs[i]);
		}
	}
#ifdef __DEBUG_MODE__
	DrawRects("res/after/"+m_Imagename+".png",img,rt);
	cout<<"after:"<<rt.size()<<endl;
#endif
	return true;
}
void TextLineReader::CombineRect(const Mat& img, vector<Rect>& r)
{
	PreCombine(img, r);
	int n = r.size();
	//return;
	//n = 3;
	if(n<3)
		return;
	//rect ** rr = new rect*[n];
	//float ** conf = new float*[n];
	//word ** cc = new word*[n];

	//get desired width
	float ewh=1.1,ewidth=0;
	ewidth = recth(r[n/2]);
	if(n>8)
	{
		vector<float> heights; 
		for(int i=0;i<n;i++)
		{
			heights.push_back( rectw(r[i]) );
		}
		sort( heights.begin(), heights.end() );
		int t = n/3;
		for(int i=t; i<t*2; i++)
		ewidth += heights[i];
		ewidth = ewidth/t;
	}
	ewidth *= 1.1;
	//cout<<ewidth<<endl;
	
	/*
	*
	* define variables used in DP
	*
	*/
	const float wordweight = 0.0;
	const float shapeweight = 1.0;
	const float marginweight = 1.0;
	float ** score = new float*[n];
	int ** pre = new int*[n];
	int statenum = 6;
	int i,k;
	float s;
	float *scoretmp = new float[statenum];
	Rect rr;
	
	/*
	*
	* compute the first position
	*
	*/
		i = 0;
		score[i] = new float[statenum];
		pre[i] = new int[statenum];
		for(int j=0; j<statenum; j++)
		{	
			switch(j)
			{
			case 0:
				s = shapeweight*( score_shape(img, r[i], ewh, ewidth) ) + wordweight*score_single_rect(img,r[i]);
				score[i][j] = s;
				break;
			case 1:
			case 2:
			case 3:
				score[i][j] = 0;
				break;
			case 4:
				rr = combine(r,i);
				s = shapeweight*( score_shape(img, rr, ewh, ewidth) ) +wordweight*score_single_rect(img,rr);
				score[i][j] = s;
				break;		
			case 5:
				rr = combine3(r,i+1);
				score[i][j] = shapeweight*( score_shape(img, rr, ewh, ewidth) ) +wordweight*score_single_rect(img,rr);
				break;	
			default:
				break;
			}
			//cout<<j<<':'<<score[i][j]<<',';
		}
		//cout<<endl;
	/*
	*
	* compute the medium positions
	*
	*/
	vector<int> idxs;
	idxs.push_back(0);idxs.push_back(2);idxs.push_back(3);
	for(int i=1;i<n-2;i++)
	{
		score[i] = new float[statenum];
		pre[i] = new int[statenum];
		for(int j=0; j<statenum; j++)
		{
			switch(j)
			{
			case 0:
				s = shapeweight*( score_shape(img, r[i], ewh, ewidth) ) + wordweight*score_single_rect(img,r[i]);
				//pre 0
				scoretmp[0] = s + score[i-1][0];
				//pre 2
				scoretmp[2] = s + score[i-1][2];
				//pre 3
				scoretmp[3] = s + score[i-1][3];
				
				k = get_max_index(scoretmp,idxs);
				score[i][j] = scoretmp[k];
				pre[i][j]=k;
				break;
			case 1:
				//pre 5
				rr = combine3(r,i);
				s = shapeweight*( score_shape(img, rr, ewh, ewidth) ) + wordweight*score_single_rect(img,rr);
				score[i][j] = s + score[i-1][5];				
				pre[i][j] = 5;
				break;
			case 2:
				//pre 4
				rr = combine(r,i-1);
				s = shapeweight*( score_shape(img, rr, ewh, ewidth) ) + wordweight*score_single_rect(img,rr);			
				score[i][j] = s + score[i-1][4];				
				pre[i][j] = 4;
				break;		
			case 3:
				//pre 1
				rr = combine3(r,i-1);
				s = shapeweight*( score_shape(img, rr, ewh, ewidth) ) + wordweight*score_single_rect(img,rr);			
				score[i][j] = s + score[i-1][1];				
				pre[i][j] = 1;
				break;
			case 4:
				rr = combine(r,i);
				s = shapeweight*( score_shape(img, rr, ewh, ewidth) ) + wordweight*score_single_rect(img,rr);
				//pre 0
				scoretmp[0] = s + score[i-1][0];
				//pre 2
				scoretmp[2] = s + score[i-1][2];
				//pre 3
				scoretmp[3] = s + score[i-1][3];
				
				k = get_max_index(scoretmp,idxs);
				score[i][j] = scoretmp[k];
				pre[i][j]=k;
				break;
			case 5:
				rr = combine3(r,i+1);
				s = shapeweight*( score_shape(img, rr, ewh, ewidth) ) + wordweight*score_single_rect(img,rr);
				//pre 0
				scoretmp[0] = s + score[i-1][0];
				//pre 2
				scoretmp[2] = s + score[i-1][2];
				//pre 3
				scoretmp[3] = s + score[i-1][3];
				
				k = get_max_index(scoretmp,idxs);
				score[i][j] = scoretmp[k];
				pre[i][j]=k;
				break;
			default:
				break;
			}
		}		
	}

	/*
	*
	* compute the second last position
	*
	*/
		i = n-2;
		score[i] = new float[statenum];
		pre[i] = new int[statenum];
		for(int j=0; j<statenum; j++)
		{	
			switch(j)
			{
			case 0:
				s = shapeweight*( score_shape(img, r[i], ewh, ewidth) ) + wordweight*score_single_rect(img,r[i]);
				//pre 0
				scoretmp[0] = s + score[i-1][0];
				//pre 2
				scoretmp[2] = s + score[i-1][2];
				//pre 3
				scoretmp[3] = s + score[i-1][3];
				
				k = get_max_index(scoretmp,idxs);
				score[i][j] = scoretmp[k];
				pre[i][j]=k;
				break;
			case 1:
				//pre 5
				rr = combine3(r,i);
				s = shapeweight*( score_shape(img, rr, ewh, ewidth) ) + wordweight*score_single_rect(img,rr);
				score[i][j] = s + score[i-1][5];				
				pre[i][j] = 5;
				break;
			case 2:
				//pre 4
				rr = combine(r,i-1);
				s = shapeweight*( score_shape(img, rr, ewh, ewidth) ) + wordweight*score_single_rect(img,rr);			
				score[i][j] = s + score[i-1][4];				
				pre[i][j] = 4;
				break;		
			case 3:
				//pre 1
				rr = combine3(r,i-1);
				s = shapeweight*( score_shape(img, rr, ewh, ewidth) ) + wordweight*score_single_rect(img,rr);			
				score[i][j] = s + score[i-1][1];				
				pre[i][j] = 1;
				break;
			case 4:
				rr = combine(r,i);
				s = shapeweight*( score_shape(img, rr, ewh, ewidth) ) + wordweight*score_single_rect(img,rr);
				//pre 0
				scoretmp[0] = s + score[i-1][0];
				//pre 2
				scoretmp[2] = s + score[i-1][2];
				//pre 3
				scoretmp[3] = s + score[i-1][3];
				
				k = get_max_index(scoretmp,idxs);
				score[i][j] = scoretmp[k];
				pre[i][j]=k;
				break;
			case 5:
				score[i][j] = 0;
				break;				
			default:
				break;
			}
			//cout<<j<<':'<<score[i][j]<<',';
		}
		//cout<<endl;
	/*
	*
	* compute the last position
	*
	*/
		i = n-1;
		score[i] = new float[statenum];
		pre[i] = new int[statenum];
		for(int j=0; j<statenum; j++)
		{	
			switch(j)
			{
			case 0:
				s = shapeweight*( score_shape(img, r[i], ewh, ewidth) ) + wordweight*score_single_rect(img,r[i]);
				//pre 0
				scoretmp[0] = s + score[i-1][0];
				//pre 2
				scoretmp[2] = s + score[i-1][2];
				//pre 3
				scoretmp[3] = s + score[i-1][3];
				
				k = get_max_index(scoretmp,idxs);
				score[i][j] = scoretmp[k];
				pre[i][j]=k;
				break;
			case 1:
			case 4:
			case 5:
				score[i][j] = 0;
				break;
			case 2:
				//pre 4
				rr = combine(r,i-1);
				s = shapeweight*( score_shape(img, rr, ewh, ewidth) ) + wordweight*score_single_rect(img,rr);			
				score[i][j] = s + score[i-1][4];				
				pre[i][j] = 4;
				break;		
			case 3:
				//pre 1
				rr = combine3(r,i-1);
				s = shapeweight*( score_shape(img, rr, ewh, ewidth) ) + wordweight*score_single_rect(img,rr);			
				score[i][j] = s + score[i-1][1];				
				pre[i][j] = 1;
				break;	
			default:
				break;
			}
			//cout<<j<<':'<<score[i][j]<<',';
		}
		//cout<<endl;
	
	/*
	*
	* find max score
	*
	*/
	float maxscore=-1;
	int maxindex = -1;
	int *bestpath = new int[n];
	for(int j=0;j<statenum;j++)
	{
		if(maxscore<score[n-1][j])
		{
			maxscore = score[n-1][j];
			maxindex = j;
		}
	}
	bestpath[n-1] = maxindex;
	for(int i=n-2; i>=0; i--)
	{
		bestpath[i] = pre[i+1][bestpath[i+1]];
	}
	//print score
	/*
	for(int i=0; i<n; i++)
	{
		cout<<i<<" final code->"<<bestpath[i]<<" each code score:"<<endl;
		for(int j=0; j<statenum; j++)
		{
			cout<<j<<':'<<score[i][j]<<" pre: "<<pre[i][j]<<endl;
		}
		cout<<endl;
	}
	*/
	for(int i=0;i<n;i++)
	{
		switch(bestpath[i]){
			case 1:
				bestpath[i] = 3;
				break;
			case 2:
			case 3:
				bestpath[i] = 1;
				break;
			case 4:
			case 5:
				bestpath[i] = 2;
				break;
			default:
				break;
		}
	}
	int nn = n;
	DecodeRects(nn,r,bestpath);
	for(int i=0;i<n;i++)
	{		
		delete [] score[i];
		delete [] pre[i];
	}
	delete [] score;
	delete [] pre;
	delete bestpath;
	delete scoretmp;
}

void TextLineReader::DrawRects(string path,const Mat& img,vector<Rect>& rt)
{
	RNG rng;
	Mat colorImage;
	if(img.channels()!=3)
		cvtColor(img, colorImage, CV_GRAY2BGR);
	else
		colorImage = img.clone();
	for(size_t i = 0; i < rt.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
		rectangle( colorImage, rt[i].tl(), rt[i].br(), color, 1, 8, 0 );
	}
	imwrite(path,colorImage);
}

