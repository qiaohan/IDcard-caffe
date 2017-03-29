/********************************************************************
	生成时间:	2014/11/21   16:53
	文件名: 	BCRDeskew.cpp
	@author:	李鹏 <hzlipeng@corp.netease.com>
				Copyright (c) 2014, NetEase Inc. All rights reserved.
	
	功能:		
*********************************************************************/

#include "Global.h"
#include "BCRDeskew.h"
#include "QuadrangleFinder_Wrapper.h"
#include "QuadrangleTrans.h"

using namespace std;
using namespace cv;
//using namespace deskew;

bool DetectQuadrangle(const Mat &imIn, Quadrangle *resOut)
{
	Quadrangle resQuad;
	const size_t mySize = 600;
	Mat imStd;
	resize(imIn, imStd, Size(mySize, mySize) );
	const bool suc = FindQuadrangle(imStd, &resQuad);

	if(!suc)
		return false;

	const double xRatio = imIn.cols/(double)mySize;
	const double yRatio = imIn.rows/(double)mySize;

	resQuad.topLeft.x *= xRatio;
	resQuad.topRight.x *= xRatio;
	resQuad.bottomLeft.x *= xRatio;
	resQuad.bottomRight.x *= xRatio;

	resQuad.topLeft.y *= yRatio;
	resQuad.topRight.y *= yRatio;
	resQuad.bottomLeft.y *= yRatio;
	resQuad.bottomRight.y *= yRatio;

	(*resOut) = resQuad;

	return true;
}



/*
 ************************************************************************
 * Function: 
 *
 * @param  imIn
 * @param  finalRes
 * @return 
 ************************************************************************
*/
Mat BCRDeskew( const Mat &imIn, int len )
{
	Quadrangle resQuad;
	const size_t mySize = 800;

	const int shorterDim = imIn.cols < imIn.rows ? imIn.cols : imIn.rows;

	double myRatio = (double)mySize/(double)shorterDim;

	Mat imStd;
	resize(imIn,imStd,Size(myRatio*imIn.cols,myRatio*imIn.rows) );
		
	const bool suc = FindQuadrangle(imStd, &resQuad);

	if(!suc)
		return Mat();

	const double xRatio = 1/myRatio;
	const double yRatio = 1/myRatio;


	resQuad.topLeft.x *= xRatio;
	resQuad.topRight.x *= xRatio;
	resQuad.bottomLeft.x *= xRatio;
	resQuad.bottomRight.x *= xRatio;

	resQuad.topLeft.y *= yRatio;
	resQuad.topRight.y *= yRatio;
	resQuad.bottomLeft.y *= yRatio;
	resQuad.bottomRight.y *= yRatio;
/*
	printf("topleft: %d,%d\n", resQuad.topLeft.x, resQuad.topLeft.y);
	printf("topright: %d,%d\n", resQuad.topRight.x, resQuad.topRight.y);
	printf("bottomleft: %d,%d\n", resQuad.bottomLeft.x, resQuad.bottomLeft.y);
	printf("bottomright: %d,%d\n", resQuad.bottomRight.x, resQuad.bottomRight.y);

	(*finalRes) = resQuad;


	Mat finalImg(imIn);
	Mat image(finalImg.c_img());
	Scalar color = cvScalar(255, 0, 0);
	const int lineWidth = 10;
	line(image,Point(resQuad.topLeft.x,resQuad.topLeft.y),Point(resQuad.topRight.x,resQuad.topRight.y), color, lineWidth);
	line(image,Point(resQuad.topLeft.x,resQuad.topLeft.y),Point(resQuad.bottomLeft.x,resQuad.bottomLeft.y), color, lineWidth);
	line(image,Point(resQuad.bottomLeft.x,resQuad.bottomLeft.y),Point(resQuad.bottomRight.x,resQuad.bottomRight.y), color, lineWidth);
	line(image,Point(resQuad.topRight.x,resQuad.topRight.y),Point(resQuad.bottomRight.x,resQuad.bottomRight.y), color, lineWidth);
	//namedWindow("xx");
	//imshow("xx", image);
	//waitKey(0);
	imwrite("D:/res.png", image);
*/
	int quadCoords[8];
	quadCoords[0] = resQuad.topLeft.x;
	quadCoords[1] = resQuad.topLeft.y;
	quadCoords[2] = resQuad.topRight.x;
	quadCoords[3] = resQuad.topRight.y;
	quadCoords[4] = resQuad.bottomLeft.x;
	quadCoords[5] = resQuad.bottomLeft.y;
	quadCoords[6] = resQuad.bottomRight.x;
	quadCoords[7] = resQuad.bottomRight.y;
	Mat finalIm = DeskewTransform(imIn, quadCoords, len);
	return move(finalIm);

}
