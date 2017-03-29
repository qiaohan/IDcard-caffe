#ifndef _LINE_UTILS_H_
#define _LINE_UTILS_H_

#include "Global.h"
#include <vector>
#include <list>

double calAngleDiff(double to ,double from);
double calRadius(double x ,double y,double angle);
double calAngle(double xStart,double yStart,double xEnd , double yEnd);
double calLen(double xStart,double yStart,double xEnd , double yEnd);
double angleAdd(double angle ,double increamental);

struct BaseLine{
	BaseLine(){};
	BaseLine(const Point &_startPoint, const Point &_endPoint)
		:startPoint(_startPoint)
		,endPoint(_endPoint){};
	Point startPoint;
	Point endPoint;
	int start;
	int end;
	void fixStartEnd(bool isVertical);
};

struct Line : public BaseLine{
	Line()
	{

	}
	Line(const Point &startPointIn, const Point &endPointIn);
	
	//Point startPoint;
	//Point endPoint;
	//int startPoint.x;
	//int startPoint.y;
	//int endPoint.x;
	//int endPoint.y;
	int lineWidth;
	//bool isVertical;
	double len;
	double score;
	double angle;
	double radius;

	bool isUsed;

	std::list<BaseLine> origLSDLines;
	//float minRadius;
	//float maxRadius;
};

struct EdgeCandidate{
	bool isVertical;
	std::vector<Line> segments;
	std::list<BaseLine> origLSDLines;
	double angle;
};
//typedef std::vector<Line> EdgeCadindate;




#endif // _LINE_UTILS_H_
