#ifndef _QUADRANGLECOSTCALCULATOR_H_
#define _QUADRANGLECOSTCALCULATOR_H_


#include "Line_Utils.h"

typedef vector<Line> LineSegCollector;

class QuadrangleCostCalculator{
public:
	QuadrangleCostCalculator(int imHeight, int imWidth)
		:_imHeight(imHeight)
		,_imWidth(imWidth)
	{

	}

	QuadrangleWithCost Calculate(const LineSegCollector &left, const LineSegCollector &top, const LineSegCollector &right, const LineSegCollector &bottom) const;


private:
	const int _imHeight;
	const int _imWidth;


	DISALLOW_CLASS_COPY_AND_ASSIGN(QuadrangleCostCalculator);

};




#endif // _QUADRANGLECOSTCALCULATOR_H_
