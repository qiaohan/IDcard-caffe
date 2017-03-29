#ifndef SEGMENTSGROUP_H_
#define SEGMENTSGROUP_H_
#include "Global.h"
#include "Line_Utils.h"
#include <list>

class ExtendedLine : public Line{
public:
	//ExtendedLine()
	//{
	//}
	//ExtendedLine(const Point &startPointIn, const Point &endPointIn) : Line(startPointIn,endPointIn){};
	ExtendedLine( const Line& line);
	bool isConnnected( const Line& line ) const;
	bool isInRange(const Point& point) const;
	//vector<Point> endPoints;
private:
	float startR;
	float endR;
	
};

class SegmentsGroup{
public:
	SegmentsGroup(Line fristLine,ImageInfo _info);
public:
	const ImageInfo info;
	bool isVertical;
	double angle ;
	double len ;
	double radius;
	vector<ExtendedLine> segments;
	bool updateRadius();
	bool add(const Line& line);
	Point project(int x ,int y );
	bool mergeSegments();

	void projectSegments();

	bool isAcceptable( const Line& line ) const;
	//bool isSameLine(const Line& line) const;
	bool isSameLine( const Line& line ,float& dist) const;
	bool isInRange( const SegmentsGroup& other ) const;
	bool merge(const SegmentsGroup& other);
	//double calDistance( const Line& line ) const;

	bool pickSegments( vector<Line>& candidates );
private:
	bool isOverLap(const Line& line);
	//const double kRadiusTolRough;
	//const double kRadiusTol;
	int start;
	int end;
	Point startPoint;
	Point endPoint;
};

#endif // SEGMENTSGROUP_H_

