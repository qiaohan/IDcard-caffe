#ifndef __TOOL__
#define __TOOL__

#include <string.h>
#include <stdlib.h>
#include <vector>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;


//-------------------------------------------------------------------------------------------------
#ifndef min2
#define min2(a,b)  (((a)<(b)) ? (a) : (b)) 
#endif

//-------------------------------------------------------------------------------------------------
#ifndef max2
#define max2(a,b)  (((a)>(b)) ? (a) : (b))
#endif



inline int recth(Rect r){return r.height;}
inline int rectw(Rect r){return r.width;}
inline float rectwh(Rect r){return ((float)r.width)/r.height;}

inline Rect combine3(const vector<Rect> &r, int i/* the medium rect in three*/){
	int x0,x1,y0,y1;
	x0 = r[i-1].tl().x;
	x1 = r[i+1].br().x;
	y0 = min2(r[i-1].tl().y,r[i].tl().y);
	y0 = min2(y0,r[i+1].tl().y);
	y1 = max2(r[i-1].br().y,r[i].br().y);
	y1 = max2(y1,r[i+1].br().y);
	return Rect(x0,y0,x1-x0,y1-y0);
}
inline int get_max_index(float *s, vector<int>& idxs){
	int maxidx = 0;
	for(int i=0; i<idxs.size(); i++){
		if(s[idxs[i]]>s[idxs[maxidx]])
			maxidx = i;
	}
	return idxs[maxidx];
}

inline Rect combine(const vector<Rect> &r,int i)
{
	int x0,x1,y0,y1;
	x0 = r[i].tl().x;
	x1 = r[i+1].br().x;
	y0 = min2(r[i].tl().y,r[i+1].tl().y);
	y1 = max2(r[i].br().y,r[i+1].br().y);
	return Rect(x0,y0,x1-x0,y1-y0);
}
inline Rect combine(Rect &r1,Rect &r2)
{
	int x0,x1,y0,y1;
	x0 = min2(r1.tl().x,r2.tl().x);
	x1 = max2(r2.br().x,r1.br().x);
	y0 = min2(r1.tl().y,r2.tl().y);
	y1 = max2(r1.br().y,r2.br().y);
	return Rect(x0,y0,x1-x0,y1-y0);
}

Mat AlignRect(Mat im);

void PreCombine(const Mat& img, vector<Rect>& r);

float get_shape_score(const Rect& r, const float ewh, const int ewidth);

//float score_shape(const Mat im, const Rect& r, const float ewh, const float ewidth);

//float score_single_rect(const Mat im, const Rect& r);

//void DeleteChars(int i, int m, int &n, rect *r)

void DecodeRects(int &n, vector<Rect>& r, int * pathcode);
#endif
