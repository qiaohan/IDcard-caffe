#ifndef _LSD_WRAPPER_H_
#define _LSD_WRAPPER_H_


#include "Line_Utils.h"
#include "opencv2/opencv.hpp"
#include "Global.h"
using namespace cv;
//¼ì²â²ÎÊý
struct LSD_Opts{
	LSD_Opts()
		:scale(0.8)
		,sigma_scale(0.6)
		,quant(2.)
		,ang_th(22.5)
		,log_eps(0.)
		,density_th(0.7)
		,n_bins(1024)
	{
	}
	double scale;
	double sigma_scale;
	double quant;
	double ang_th;
	double log_eps;
	double density_th;
	int n_bins;
};
class LSD_Wrapper{
public:
	explicit LSD_Wrapper(const LSD_Opts &optsIn)
		:_opts(optsIn)
	{
	}
	vector<Line> DetectLines(const Mat& image) const;
private:
	const LSD_Opts _opts;
	vector<double> GetImgPtr(const Mat &curIm) const;
	DISALLOW_CLASS_COPY_AND_ASSIGN(LSD_Wrapper);
};
#endif // _LSD_WRAPPER_H_
