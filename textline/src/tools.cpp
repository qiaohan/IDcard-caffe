#include "tools.h"
#include <stack>
Mat AlignRect(Mat im){
	if(im.cols == im.rows)
		return im;
	//cout<<im.cols<<','<<im.rows<<endl;
	int len = im.cols>im.rows?im.cols:im.rows;
	uchar b = im.at<uchar>(0,0);
	uchar g = im.at<uchar>(0,1);
	uchar r = im.at<uchar>(0,2);
	Mat img(len,len,im.type(),Scalar(b,g,r));
	Rect rr;
	if(im.cols>im.rows)
		rr = Rect(0,(len-im.rows)/2,im.cols,im.rows);
	else
		rr = Rect((len-im.cols)/2,0,im.cols,im.rows);
	im.copyTo(img(rr));
	return img;
}

void PreCombine(const Mat& img, vector<Rect>& r){
	int n = r.size();
	if(n<=1)
		return;
	stack<Rect> rr;
	rr.push(r[0]);
	for(int i=1; i<n-1; i++){
		int w = rectw(r[i]);
		if(rectwh(r[i])<=0.3){
			int leftmargin = r[i].tl().x - r[i-1].br().x;
			int rightmargin = r[i+1].tl().x - r[i].br().x;
			int small = leftmargin<rightmargin?leftmargin:rightmargin;
			if(small<=recth(r[i])/5){
				//cout<<small<<','<<recth(r[i])<<endl;
				if(leftmargin == small){
					Rect t = rr.top();
					rr.pop();
					rr.push(combine(t,r[i]));
				}
				else{
					r[i+1] = combine(r[i],r[i+1]);
					r[i] = r[i-1];
				}
				continue;
			}
		}
		rr.push(r[i]);
	}
	rr.push(r[n-1]);
	n = rr.size();
	r.resize(n);
	for(int i=0;i<n;i++)
	{	
		r[n-1-i] = rr.top();
		rr.pop();
	}
}


float get_shape_score(const Rect& r, const float ewh, const int ewidth){
	float score = 100;
	//cout<<"shape ewidth: "<<ewidth<<"\tshape ewh: "<<ewh<<endl;
	float wh = rectwh(r);
	int w = rectw(r);
	float whthred = 0.9;
	bool decay;
	if(ewh<whthred)
		decay = !(wh>ewh&&wh<whthred);
	else
		decay = !(wh<ewh&&wh>whthred);

	if(decay){

		if( ewh - wh > 0 )
			score = score - 10*( ewh-wh );
		else
			score = score - 20*( wh-ewh );
	}
	if(abs(ewidth-w)>ewidth*0.25){
		if( ewidth - w > 0 )
			score = score - 1*( ewidth - w );
		else
			score = score - 2*( w - ewidth );
	}
	//cout<<"shape score: "<<score<<endl;
	return score;
}
/*
float score_shape(const Mat im, const Rect& r, const float ewh, const float ewidth){
	if( rectwh(r)>2 )
		return 0.0;
	float ch_score = get_shape_score(r,ewh,ewidth);
	float nch_score = get_shape_score(r,0.5*ewh,ewidth*0.5);
	//return 1.2*ch_score>nch_score?ch_score:nch_score;
	return ch_score;
}

float score_single_rect(const Mat im, const Rect& r)
{
	return 0;
	float f,score=0;
	word c;
	m_pEn->RecognizeChar(w, h, im, r, bChsSubset, c, f);

	if(f>0.88)
	{
		if(f>0.95)
		score = f*100;
		else if(f>0.92)
		score = f*90;
		else if(f>0.9)
		score = f*75;
		else
		score = f*60;
	}
	else
	{
	float ff[NCANDI];
	word cc[NCANDI];
	m_pCn->RecognizeChar(w, h, im, r, bChsSubset,NCANDI, cc, ff);
	float freq = 0;
	for(int i=0;i<NCANDI;i++)
		//freq += getProb(mdict, cc[i]);
		freq += ff[i];
	
	if(freq>0)
		freq = log(freq/NCANDI);
	else
		freq =0;
	
	score = freq;
	score = score*100;
	}
	//cout<<"char score:"<<score<<endl;
	return score;
	return 0;
}
*/
/*
void DeleteChars(int i, int m, int &n, rect *r)
{
	if(0<m && 0<=i && i+m-1<n)
	{
		n -= m;
		for(int k=i; k<n; k++)
		{
			r[k] = r[k+m];
		}
	}
}
*/
void DecodeRects(int &n, vector<Rect>& r, int * pathcode)
{
	Rect tmp;
	stack<Rect> rr;
	rr.push(r[0]);
	for(int i=1; i<n-1; i++)
	{
		switch (pathcode[i])
		{
		case 0:
		case 2:
			rr.push(r[i]);
			break;
		case 1:	
		case 3:
			tmp = rr.top();
			rr.pop();
			rr.push(combine(tmp,r[i]));
			break;
		default:
			break;
		}
	}

	if(pathcode[n-2]==3||pathcode[n-2]==2)
	{
		tmp = rr.top();
		rr.pop();
		rr.push(combine(tmp,r[n-1]));
	}
	else
	{
		rr.push(r[n-1]);
	}
	n = rr.size();
	r.resize(n);
	for(int i=0;i<n;i++)
	{	
		r[n-1-i] = rr.top();
		rr.pop();
	}
}
