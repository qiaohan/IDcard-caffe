#ifndef _QUADRANGLEFINDER_WRAPPER_H_
#define _QUADRANGLEFINDER_WRAPPER_H_

#include "opencv2/opencv.hpp"

using namespace cv;

bool FindQuadrangle(const Mat &imIn, Quadrangle *out);

#endif // _QUADRANGLEFINDER_WRAPPER_H_
