/********************************************************************
	生成时间:	2014/11/13   10:55
	文件名: 	LSD_Wrapper.cpp
	@author:	李鹏 <hzlipeng@corp.netease.com>
				Copyright (c) 2014, NetEase Inc. All rights reserved.
	
	功能:		
*********************************************************************/
#include "LSD_Wrapper.h"
#include "lsd.h"
#include "Line_Utils.h"
#include "ImgExceptions.h"
using namespace std;
using namespace cv;
/*
 ************************************************************************
 * Function: 
 *
 * @param  imData
 * @param  imHeight
 * @param  imWidth
 * @return 
 ************************************************************************
*/
vector<Line> LSD_Wrapper::DetectLines(const Mat& image) const
{
	const int imHeight = image.rows;
	const int imWidth  = image.cols;
	if( image.empty() )
		throw ImgBad("image cannot be empty!", __FILE__, __LINE__);

	if(! (imHeight > 0 && imWidth > 0) )
		throw ImgBad("image's height and width must be greater than 0!", __FILE__, __LINE__);

	if(image.channels()!=3 )
		throw ImgBad("the number of image's channel must be 3!", __FILE__, __LINE__);
	
	int nDetectedLines = 0;

	vector<double*> rawData;
	vector<Mat> images;
	split(image,images);//BGR
	vector<double> imageR = GetImgPtr(images[2]);
	vector<double> imageG = GetImgPtr(images[1]);
	vector<double> imageB = GetImgPtr(images[0]);
	rawData.push_back(imageR.data());
	rawData.push_back(imageG.data());
	rawData.push_back(imageB.data());

	double* linesRes = LineSegmentDetection( &nDetectedLines, rawData.data(),rawData.size(), imWidth, imHeight,
							_opts.scale,
							_opts.sigma_scale,
							_opts.quant,
							_opts.ang_th,
							_opts.log_eps,
							_opts.density_th,
							_opts.n_bins,
							NULL,
							NULL,
							NULL);
	
	vector<Line> finalRes;
	static const int nElemPerLine = 7;
	for(int k=0; k<nDetectedLines; ++k)
	{
		
		int xStart    = static_cast<int>(linesRes[k*nElemPerLine]);
		xStart = xStart<0 ? 0 : xStart;
		xStart = xStart>=imWidth ? imWidth-1 : xStart;
		int yStart    = static_cast<int>(linesRes[k*nElemPerLine+1]);
		yStart = yStart<0 ? 0 : yStart;
		yStart = yStart>=imHeight ? imHeight-1 : yStart;
		int xEnd      = static_cast<int>(linesRes[k*nElemPerLine+2]);
		xEnd = xEnd<0 ? 0 : xEnd;
		xEnd = xEnd>=imWidth ? imWidth-1 : xEnd;
		int yEnd      = static_cast<int>(linesRes[k*nElemPerLine+3]);
		yEnd = yEnd<0 ? 0 : yEnd;
		yEnd = yEnd>=imHeight ? imHeight-1 : yEnd;
		Line curLine(Point(xStart,yStart),Point(xEnd,yEnd));
		curLine.lineWidth = static_cast<int>(linesRes[k*nElemPerLine+4]+0.5);
		curLine.score = static_cast<double>(linesRes[k*nElemPerLine+6]);
		//curLine.startPoint.x    = static_cast<int>(linesRes[k*nElemPerLine]);
		//curLine.startPoint.y    = static_cast<int>(linesRes[k*nElemPerLine+1]);
		//curLine.endPoint.x      = static_cast<int>(linesRes[k*nElemPerLine+2]);
		//curLine.endPoint.y      = static_cast<int>(linesRes[k*nElemPerLine+3]);

		finalRes.push_back( move(curLine) );
	}


	free(linesRes); linesRes = NULL;

	return move(finalRes);
}

vector<double> LSD_Wrapper::GetImgPtr( const Mat &grayIm ) const
{
	if( grayIm.empty() )
		throw ImgBad("image cannot be empty!", __FILE__, __LINE__);
	if( grayIm.type() != CV_8UC1 )
		throw ImgBad("LSD input image's componet must be CV_8UC1 type!", __FILE__, __LINE__);
	
	vector<double> res;

	for(int j=0; j<grayIm.rows; ++j)
	{
		for(int i=0; i<grayIm.cols; ++i)
		{
			res.push_back(  grayIm.at<uchar>(j,i) );
		}
	}
	return move(res);
}
