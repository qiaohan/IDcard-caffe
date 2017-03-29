#ifndef _GLOBAL_H_
#define _GLOBAL_H_


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <cfloat>
#include <cstdint>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;

#define DISALLOW_CLASS_COPY_AND_ASSIGN(type) type(const type &); type& operator=(const type &)

//namespace deskew{

struct ImageInfo{
	int kHeight;
	int kWidth;
};


struct Quadrangle{
	Point topLeft;
	Point topRight;
	Point bottomLeft;
	Point bottomRight;
};

struct QuadrangleWithCost{
	QuadrangleWithCost()
		:cost(DBL_MAX)
	{

	}
	Quadrangle myQuad;
	double cost;
	double area;
	int edgeIdx_left;
	int edgeIdx_top;
	int edgeIdx_right;
	int edgeIdx_bottom;
};

static inline bool operator<(const QuadrangleWithCost &lhs, const QuadrangleWithCost &rhs)
{
	return lhs.cost < rhs.cost;
}
//}
#define __DEBUG_MODE__

#endif // _GLOBAL_H_
