#ifndef _BCRDESKEW_H_
#define _BCRDESKEW_H_

#include "Global.h"
#include "opencv2/opencv.hpp"
using namespace cv;

/*
 ************************************************************************
 * Function: Deskew the Image so that the ID card is fine
 *
 * @param  imIn: 输入图像
 * @param  len: the length of returned image's larger edge
 * @return  the deskewed ID card image with larger edge's length = len, 
 *			OR an empty image if detect failed
 ************************************************************************
*/
Mat BCRDeskew( const Mat &imIn, int len );



/*
 ************************************************************************
 * Function: 检测图中的四边形边框
 *
 * @param  imIn 输入图像
 * @param  resOut 检测到的边框坐标
 * @return 成功返回true，失败返回false
 ************************************************************************
*/
bool DetectQuadrangle(const Mat &imIn, Quadrangle *resOut);


#endif // _BCRDESKEW_H_
