#ifndef _QUADRANGLETRANS_WRAPPER_H_
#define _QUADRANGLETRANS_WRAPPER_H_

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

Mat IDcardsplit(Mat);
Mat IDcardtranspose(Mat);
bool isgood(Mat &);
Mat DeskewTransform(const Mat &orgIm, const int *quadCoords,int len);



#endif // _QUADRANGLETRANS_WRAPPER_H_
