/********************************************************************
	生成时间:	2014/11/17   10:27
	文件名: 	QuadrangleTrans_Wrapper.cpp
	@author:	李鹏 <hzlipeng@corp.netease.com>
				Copyright (c) 2014, NetEase Inc. All rights reserved.
	
	功能:		
*********************************************************************/

#include "Global.h"
#include "QuadrangleTrans.h"


inline double CalSquare(double val)
{
	return val*val;
}

double getOrigWHRatio( const int * quadCoords );

/*
 ************************************************************************
 * Function: 参看Zhengyou Zhang: Whiteboard Scanning and Image Enhancement
 *
 * @param  quadCoords
 * @param  imHeight
 * @param  imWidth
 * @return 
 ************************************************************************
*/
double CalRectWHRatio(const int *quadCoords, const int imHeight, const int imWidth)
{
	Mat m1(3, 1, CV_64F);
	Mat m2(3, 1, CV_64F);
	Mat m3(3, 1, CV_64F);
	Mat m4(3, 1, CV_64F);
	
	const double u0 = imWidth/2.;
	const double v0 = imHeight/2.;

	m1.at<double>(0) = quadCoords[0];
	m1.at<double>(1) = quadCoords[1];
	m1.at<double>(2) = 1.;

	m2.at<double>(0) = quadCoords[2];
	m2.at<double>(1) = quadCoords[3];
	m2.at<double>(2) = 1.;

	m3.at<double>(0) = quadCoords[4];
	m3.at<double>(1) = quadCoords[5];
	m3.at<double>(2) = 1.;

	m4.at<double>(0) = quadCoords[6];
	m4.at<double>(1) = quadCoords[7];
	m4.at<double>(2) = 1.;
	
	const double k2 = m1.cross(m4).dot(m3) / (  m2.cross(m4).dot(m3)  );
	const double k3 = m1.cross(m4).dot(m2) / (  m3.cross(m4).dot(m2)  );

	Mat n2 = k2*m2 - m1;
	Mat n3 = k3*m3 - m1;

	const double n21 = n2.at<double>(0);
	const double n22 = n2.at<double>(1);
	const double n23 = n2.at<double>(2);
	const double n31 = n3.at<double>(0);
	const double n32 = n3.at<double>(1);
	const double n33 = n3.at<double>(2);

	const double tmpVal = (n21*n31 - (n21*n33 + n23*n31)*u0 + n23*n33*u0*u0) + (n22*n32 - (n22*n33+n23*n32)*v0 + n23*n33*v0*v0);

	if(abs(n23*n33) < 0.000001) //极小
	{
		return getOrigWHRatio(quadCoords);

	}
	const double fsquare = -1.*tmpVal/(n23*n33);
	if (fsquare < 0){
		return CalRectWHRatio(quadCoords,imHeight*2,imWidth-10);
	}

	//tmpMat = inv(AT)*inv(A)
	double buf[9] = {1, 0, -1*u0, 0, 1, -1*v0, -1*u0, -1*v0, u0*u0+v0*v0+fsquare };
	Mat tmpMat(3, 3, CV_64F, buf);

	Mat n2Transpose;
	Mat n3Transpose;
	transpose(n2, n2Transpose);
	transpose(n3, n3Transpose);
	Mat val1 = n2Transpose*tmpMat*n2;
	Mat val2 = n3Transpose*tmpMat*n3;

	double socre1 = val1.at<double>(0);
	double score2 = val2.at<double>(0);
	if (score2 < 0.000001 ){
		return getOrigWHRatio(quadCoords);
	}else{
		const double whRatio = sqrt( abs( val1.at<double>(0)  /  val2.at<double>(0) )  );
		return whRatio;
	}


}

/*
 ************************************************************************
 * Function: 
 *
 * @param  quadCoords
 * @return 
 ************************************************************************
*/
vector<int> GetRectCoords(const int *quadCoords, const int imHeight, const int imWidth, int len)
{
	const double quad_topleftx     = quadCoords[0];
	const double quad_toplefty     = quadCoords[1];
	const double quad_toprightx    = quadCoords[2];
	const double quad_toprighty    = quadCoords[3];
	const double quad_bottomleftx  = quadCoords[4];
	const double quad_bottomlefty  = quadCoords[5];
	const double quad_bottomrightx = quadCoords[6];
	const double quad_bottomrighty = quadCoords[7];


	const double topLen    = sqrt( CalSquare(quad_topleftx-quad_toprightx) + CalSquare(quad_toplefty-quad_toprighty) );
	const double bottomLen = sqrt( CalSquare(quad_bottomleftx-quad_bottomrightx) + CalSquare(quad_bottomlefty-quad_bottomrighty) );
	const double leftLen   = sqrt( CalSquare(quad_topleftx-quad_bottomleftx) + CalSquare(quad_toplefty-quad_bottomlefty) );
	const double rightLen  = sqrt( CalSquare(quad_toprightx-quad_bottomrightx) + CalSquare(quad_toprighty-quad_bottomrighty) );



	//const double whRatio = CalRectWHRatio(quadCoords, imHeight, imWidth);
	double whRatio = getOrigWHRatio(quadCoords);
	int minwidth = 0;
	int longedge = len;
	if( whRatio > 1 ){
		whRatio = 85.6/54.0;
		minwidth = longedge;
	}else{
		whRatio = 54.0/85.6;
		minwidth = (int)longedge*whRatio;
	}

	int rect_width = static_cast<int>( min(topLen, bottomLen) );
	if(rect_width < minwidth)
		rect_width = minwidth;
	int rect_height = rect_width/whRatio;


	vector<int> res;

	res.push_back(0);
	res.push_back(0);

	res.push_back(rect_width-1);
	res.push_back(0);

	res.push_back(0);
	res.push_back(rect_height-1);

	res.push_back(rect_width-1);
	res.push_back(rect_height-1);

	return move(res);
}

/*
 ************************************************************************
 * Function: 
 *
 * @param  quadCoords
 * @param  imHeight
 * @param  imWidth
 * @return 
 ************************************************************************
*/
/*
Size GetRectHeightWidth( const int *quadCoords, const int imHeight, const int imWidth )
{
	vector<int> rectCoords = GetRectCoords(quadCoords, imHeight, imWidth, length);
	Size res;
	res.height = rectCoords[7] - rectCoords[1] + 1;
	res.width = rectCoords[6] - rectCoords[0] + 1;
	return res;
}
*/
double calWarpRatio(double x , double y){
	return y*y /(y*y - x*x);
}
double getOrigWHRatio( const int * quadCoords )
{
	const double quad_topleftx     = quadCoords[0];
	const double quad_toplefty     = quadCoords[1];
	const double quad_toprightx    = quadCoords[2];
	const double quad_toprighty    = quadCoords[3];
	const double quad_bottomleftx  = quadCoords[4];
	const double quad_bottomlefty  = quadCoords[5];
	const double quad_bottomrightx = quadCoords[6];
	const double quad_bottomrighty = quadCoords[7];

	const double topLen    = sqrt( CalSquare(quad_topleftx-quad_toprightx) + CalSquare(quad_toplefty-quad_toprighty) );
	const double bottomLen = sqrt( CalSquare(quad_bottomleftx-quad_bottomrightx) + CalSquare(quad_bottomlefty-quad_bottomrighty) );
	const double leftLen   = sqrt( CalSquare(quad_topleftx-quad_bottomleftx) + CalSquare(quad_toplefty-quad_bottomlefty) );
	const double rightLen  = sqrt( CalSquare(quad_toprightx-quad_bottomrightx) + CalSquare(quad_toprighty-quad_bottomrighty) );

	const double width = (topLen+bottomLen) * calWarpRatio(abs(leftLen-rightLen),max(leftLen,rightLen));
	const double height = (leftLen+rightLen) * calWarpRatio(abs(topLen-bottomLen),max	(topLen,bottomLen));

	return width/height;
	//return (topLen+bottomLen)/(leftLen+rightLen);
}

/*
 ************************************************************************
 * Function: 
 *
 * @param  orgIm
 * @param  quadCoords
 * @return 
 ************************************************************************
*/
Mat DeskewTransform( const Mat &orgIm, const int *quadCoords, int len )
{
	const vector<int> rectCoords = GetRectCoords(quadCoords, orgIm.rows, orgIm.cols, len);
	const int rectHeight              = rectCoords[7] + 1;
	const int rectWidth               = rectCoords[6] + 1;

	const int leftHeight = quadCoords[5] - quadCoords[1];
	const int rightHeight = quadCoords[7] - quadCoords[3];

	vector<Point2f> rectPoint2f;
	vector<Point2f> quadPoint2f;

	rectPoint2f.push_back( Point2f(rectCoords[0], rectCoords[1]));
	rectPoint2f.push_back( Point2f(rectCoords[2], rectCoords[3]));
	rectPoint2f.push_back( Point2f(rectCoords[4], rectCoords[5]));
	rectPoint2f.push_back( Point2f(rectCoords[6], rectCoords[7]));

	quadPoint2f.push_back( Point2f(quadCoords[0], quadCoords[1]));
	quadPoint2f.push_back( Point2f(quadCoords[2], quadCoords[3]));
	quadPoint2f.push_back( Point2f(quadCoords[4], quadCoords[5]));
	quadPoint2f.push_back( Point2f(quadCoords[6], quadCoords[7]));
	
	Mat transMat = getPerspectiveTransform(quadPoint2f.data(), rectPoint2f.data());
	if(transMat.empty())
	{
		return Mat();
	}

	Mat resMat = Mat::zeros(rectHeight, rectWidth, CV_8UC3);
	warpPerspective(orgIm, resMat, transMat, resMat.size(), INTER_LINEAR, BORDER_REPLICATE);
	
	return IDcardtranspose(resMat);
}
void processpatch(Mat idim){
	static int cnt = 0;
	/*
	Mat hsvimg,grayimg;
	cvtColor(idim,hsvimg,CV_BGR2HSV);
	cvtColor(idim,grayimg,CV_BGR2GRAY);
	vector<Mat> mats;
	split(hsvimg,mats);
	Mat bimg;
	threshold(grayimg,bimg,160,255,THRESH_BINARY_INV);
	*/
	char path[100];
	sprintf(path,"tmp/%d.jpg",cnt++);
	imwrite(path,idim);

}

Mat IDcardsplit(Mat idim){
	vector<Rect> r;
	//name
	r.push_back(Rect(idim.cols*0.18,idim.rows*0.1,idim.cols*0.3,idim.rows*0.15));
	//address
	r.push_back(Rect(idim.cols*0.18,idim.rows*0.5,idim.cols*0.45,idim.rows*0.25));
	//number
	r.push_back(Rect(idim.cols*0.33,idim.rows*0.8,idim.cols*0.6,idim.rows*0.15));
	int h = 0.55*idim.rows;
	int w = 0.9*idim.cols;
	cout<<h<<w<<endl;
	Mat combine(h,w,idim.type(),Scalar(255,255,255));
	
	//Mat combine(h,w,idim.type(),Scalar(255,255,255));
	//Mat combine(h,w,idim.type(),idim.at<uchar>(idim.cols*0.1,idim.rows*0.1));
	int y=0;
	for(int i = 0; i<r.size(); i++)
		try{
			//processpatch(idim(r[i]));
			//combine(Rect(0,y,r[i].width,r[i].height)) = idim(r[i])+1;
			idim(r[i]).copyTo( combine(Rect(0,y,r[i].width,r[i].height)) );
			/*
			for(int hh = y; hh < y+r[i].height; hh++){
				Vec3b t = combine.at<Vec3b>(hh,r[i].width-1);
				for(int ww=r[i].width;ww<combine.cols; ww++){
					combine.at<Vec3b>(hh,ww) = t;//combine.at<Vec3b>(ww,hh)*0;
					//combine.at<Vec3b>(ww,hh) = Scalar(0,0,0);
				}
			}
			*/
			y+=r[i].height;
		}
		catch(...)
		{
		}
	return combine;
}

bool isgood(Mat &img){
	Mat binaryimg;
	int thre = 20;
	Mat img_g;
	cvtColor(img,img_g,CV_RGB2GRAY);
	equalizeHist(img_g,img_g);
	//imwrite("tmp/g.jpg",img_g);
	threshold(img_g,binaryimg,thre,1,THRESH_BINARY_INV);
	//imwrite("tmp/b.jpg",binaryimg*255);
	int left = sum(binaryimg(Rect(0,0.25*img.rows,img.cols/2,img.rows*0.5)))[0];
	int right = sum(binaryimg(Rect(img.cols/2,0.25*img.rows,img.cols/2,img.rows*0.5)))[0];
	/*
	cout<<left<<':'<<right<<endl;
	vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

	findContours( binaryimg, contours, hierarchy,
		      CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
	*/
	if(left<right)
		return true;
	else
		return false;
}
/*
Mat IDcardtranspose(Mat im){
	if(im.rows < im.cols && isgood(im)){
		return im;
	}
	else{
		Mat dst1,dst2;
		//if(turnright(im)){
		if(isgood(im)){
			flip(im, dst1,1);
			transpose(dst1,dst2);
		}else{
			transpose(im,dst1);
			flip(dst1, dst2,1);
		}
		return dst2;
	}
}
*/
Mat IDcardtranspose(Mat im){
	Mat dst2 = im;
	if(im.rows > im.cols){
		Mat dst1;
		transpose(im,dst1);
		flip(dst1, dst2,1);
	}
	if(!isgood(dst2)){
		Mat t,tt;
		flip(dst2,t,1);
		flip(t,tt,0);
		return tt;
	}
	else
		return dst2;
}
