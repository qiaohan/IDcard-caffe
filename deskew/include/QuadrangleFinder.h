#ifndef _QUADRANGLEFINDER_H_
#define _QUADRANGLEFINDER_H_

#include "Line_Utils.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class QuadrangleFinder{
public:
	QuadrangleFinder()
	{

	}

	bool Find(const Mat &imIn, const vector<EdgeCandidate> &candsIn, QuadrangleWithCost *out) const;


private:


	DISALLOW_CLASS_COPY_AND_ASSIGN(QuadrangleFinder);
};



#endif // _QUADRANGLEFINDER_H_
