#include "SegmentsGroup.h"
#include "Line_Utils.h"
#include <cassert>
#include <numeric>
#include <utility>
#include <algorithm>
#include <cmath>
#include <cfloat>
#include <boost/tuple/tuple.hpp>

using namespace std;

namespace{
	static const int kProbWidth = 600;
	static const double kAngleTolRough = 10 * M_PI / 180.;
	static const double kAngleTol = 5 * M_PI / 180.;
	static const double kRadiusTolRough = 0.005 * kProbWidth;
	static const double kRadiusTolLine = 1;
	static const double kRadiusTolGroup = 0.005 * kProbWidth;
	static const double kAngleLearningRate = 1;
	static const int kMaxLineGap = 5;
	static const float kOverlapRatio = 0.5;

	static const float kLineWidth = 2;
	static const float kLineExtended = 30;
	typedef boost::tuples::tuple<float,float> Dist;
	typedef boost::tuples::tuple<int,float> Idx;
}

SegmentsGroup::SegmentsGroup(Line fristLine,ImageInfo _info)
	: info(_info)
	//, kRadiusTolRough(max(info.kHeight,info.kWidth)*kRadiusTolRatioRough)
	//, kRadiusTol(max(info.kHeight,info.kWidth)*kRadiusTolRatio)
{
	angle = fristLine.angle;
	isVertical = abs(angle) < M_PI_4 ;
	len = fristLine.len;
	radius = fristLine.radius;
	fristLine.fixStartEnd(isVertical);
	start = fristLine.start;
	end = fristLine.end;
	segments.push_back(fristLine);
}


bool SegmentsGroup::updateRadius()
{
	double totalLen =0;
	double radius = 0;
	for (int i = 0; i != segments.size() ; ++i){
		radius += (calRadius(segments[i].startPoint.x,segments[i].startPoint.y,angle) + calRadius(segments[i].endPoint.x,segments[i].endPoint.y,angle))*segments[i].len;
		totalLen += segments[i].len;
	}
	this->radius = radius/2./totalLen;
	return true;
}

//bool SegmentsGroup::update(){
//	for (int i = 0; i != segment ; ++i){
//		
//	}
//	
//}
bool SegmentsGroup::add( const Line& line )
{
	//if (!isOverLap(line) ){

	//}

	len += line.len;
	angle = angleAdd(angle,kAngleLearningRate * line.len*calAngleDiff(line.angle,angle)  / (double)(len));
	updateRadius();
	ExtendedLine newSeg(line);
	newSeg.fixStartEnd(isVertical);
	if (newSeg.start < start){
		start = newSeg.start;
	}
	if (newSeg.start > end){
		end = newSeg.end;
	}
	segments.push_back(newSeg);

	return true;
}

Point SegmentsGroup::project( int x ,int y )
{
	if (isVertical){
		return Point((radius - y*sin(angle))/cos(angle),y);
	}else{
		return Point(x,(radius - x*cos(angle))/sin(angle));
	}

}

bool SegmentsGroup::mergeSegments()
{
	assert(!segments.empty());
	//vector<Line>& segments = group.segments;
	for (int i = 0; i != segments.size() ; ++i){
		segments[i].fixStartEnd(isVertical);
	}
	sort(segments.begin(),segments.end(),[](const Line& lval , const Line& rval){
		return lval.start < rval.start;
	});
	int lastSeg = 0;

	for (int i = lastSeg+1; i != segments.size() ; ++i){
		if(segments[i].end < segments[lastSeg].end){
			segments[lastSeg].origLSDLines.splice(segments[lastSeg].origLSDLines.end(),segments[i].origLSDLines);
			//segments[lastSeg].baseLines ;push_back(segments[i].startPoint);
			//segments[lastSeg].endPoints.push_back(segments[i].endPoint);
			//endPoints.push_back(line.endPoint);
			continue;
		}
		if (segments[i].start - segments[lastSeg].end < kMaxLineGap){
			segments[lastSeg].end = segments[i].end;
			segments[lastSeg].endPoint.x = segments[i].endPoint.x;
			segments[lastSeg].endPoint.y = segments[i].endPoint.y;
			segments[lastSeg].len = calLen(segments[lastSeg].startPoint.x,segments[lastSeg].startPoint.y,segments[lastSeg].endPoint.x,segments[lastSeg].endPoint.y);
			segments[lastSeg].angle = calAngle(segments[lastSeg].startPoint.x,segments[lastSeg].startPoint.y,segments[lastSeg].endPoint.x,segments[lastSeg].endPoint.y);
			segments[lastSeg].radius = calRadius(segments[lastSeg].startPoint.x,segments[lastSeg].startPoint.y,segments[lastSeg].angle);

			//segments[lastSeg].endPoints.push_back(segments[i].startPoint);
			//segments[lastSeg].endPoints.push_back(segments[i].endPoint);
			segments[lastSeg].origLSDLines.splice(segments[lastSeg].origLSDLines.end(),segments[i].origLSDLines);
		}else{
			lastSeg++;
			segments[lastSeg] = segments[i];
		}
	}
	segments.erase(segments.begin()+lastSeg+1,segments.end());
	//projectSegments();

	return true;
}

Dist calDistance(const SegmentsGroup& group, const Line& line )
{	
	//int tol = max(abs(0.8 * line.len * sin(angleDiff)), R_TOL);
	Dist dist;
	dist.get<0>() = abs(group.radius -calRadius(line.startPoint.x,line.startPoint.y,group.angle));
	dist.get<1>() = abs(group.radius -calRadius(line.endPoint.x,line.endPoint.y,group.angle));
	return dist;
	//if (distance <= kRadiusTolarance){
	//	return distance;
	//}else{
	//	return DBL_MAX;
	//}
	
	//if (abs(angleDiff) < kAngleTolarance &&
	//	radius - kRadiusTolarance <= startRadius && startRadius <= radius + kRadiusTolarance 
	//	&& radius - kRadiusTolarance <= endRadius && endRadius <= radius + kRadiusTolarance ){
	//		return true;
	//}else{
	//	return false;
	//}
}

bool SegmentsGroup::isAcceptable( const Line& line ) const
{
	const double angleDiff = calAngleDiff(line.angle , angle);
	if (abs(angleDiff) >= kAngleTolRough){
		return false;
	}
	Dist dist = calDistance(*this,line);
	return min(dist.get<0>(),dist.get<1>()) < kRadiusTolRough;
}



bool SegmentsGroup::pickSegments( vector<Line>& candidates )
{
	list<Idx > possibleIdx;
	for (int i=0 ; i != candidates.size() ; i++){
		if (isAcceptable(candidates[i])){
			possibleIdx.push_back(boost::tuples::make_tuple(i,abs(calAngleDiff(candidates[i].angle , angle))));
		}
	}
	//possibleIdx.sort([&candidates,this](const Idx& idx1,const Idx idx2){
	//	return idx1.get<1>() < idx2.get<1>();
	//});
	for (int i = 0; i != segments.size() ; ++i){
		for (auto itIdx = possibleIdx.begin(); itIdx != possibleIdx.end() ;){
			int idx = itIdx->get<0>();
			//candidates[idx].fixStartEnd(isVertical);
			//if (isOverLap(candidates[idx])){
			//	itIdx++;
			//	continue;
			//}
			//if (itIdx->get<0>() > kAngleTol){
			//	itIdx++;
			//	continue;
			//}
			if (segments[i].isConnnected(candidates[idx])){
				add(candidates[idx]);
				candidates[idx].isUsed = true;
				itIdx = possibleIdx.erase(itIdx);
			}else{
				itIdx++;
			}	
		}	
	}

	//for (int i = 0; i != segments.size() ; ++i){
	//	for (auto itIdx = possibleIdx.begin(); itIdx != possibleIdx.end() ;){
	//		int idx = itIdx->get<0>();
	//		if (segments[i].isConnnected(candidates[idx])){
	//			add(candidates[idx]);
	//			candidates[idx].isUsed = true;
	//			itIdx = possibleIdx.erase(itIdx);
	//		}else{
	//			itIdx++;
	//		}	
	//	}	
	//}

	candidates.erase(remove_if(candidates.begin(),candidates.end(),[](const Line& val){
		return val.isUsed;
	}),candidates.end());
	return true;
}

bool SegmentsGroup::isInRange( const SegmentsGroup& other ) const
{
	const Line& maxLine = *max_element(other.segments.begin(),other.segments.end(),[](const Line& lval,const Line& rval){
		return lval.len < rval.len;
	});
	const double angleDiff = calAngleDiff(maxLine.angle , angle);
	if (abs(angleDiff) >= kAngleTol){
		return false;
	}
	Dist dist = calDistance(*this,maxLine);
	return max(dist.get<0>(),dist.get<1>()) < kRadiusTolGroup;
}

bool SegmentsGroup::isSameLine( const Line& line ,float& diff) const
{
	const double angleDiff = calAngleDiff(line.angle , angle);
	if (abs(angleDiff) >= kAngleTol){
		return false;
	}
	Dist dist = calDistance(*this,line);
	diff = max(dist.get<0>(),dist.get<1>());
	return diff < kRadiusTolLine;
}

bool SegmentsGroup::merge( const SegmentsGroup& other )
{
	for (int i = 0; i != other.segments.size() ; ++i){
		add(other.segments[i]);
	}
	return true;
}

bool SegmentsGroup::isOverLap( const Line& line )
{
	float overlap = min(line.end -start , end - line.start);
	float score = overlap / min(end -start ,line.end - line.start );
	return score > kOverlapRatio;

}

void SegmentsGroup::projectSegments()
{
	len = 0;
	for (int i = 0; i != segments.size() ; ++i){
		Point point = project(segments[i].startPoint.x,segments[i].startPoint.y);
		segments[i].startPoint.x = point.x;
		segments[i].startPoint.y = point.y;
		point = project(segments[i].endPoint.x,segments[i].endPoint.y);
		segments[i].endPoint.x = point.x;
		segments[i].endPoint.y = point.y;
		segments[i].len = calLen(segments[i].startPoint.x,segments[i].startPoint.y,segments[i].endPoint.x,segments[i].endPoint.y);
		segments[i].angle = angle;
		segments[i].radius = radius;
		len += segments[i].len;
	}
}


float PointDistance(const Point& lval, const Point& rval){
	return sqrt((float)(lval.x - rval.x) * (lval.x - rval.x) + (lval.y - rval.y) * (lval.y - rval.y));
}

bool ExtendedLine::isConnnected( const Line& line ) const
{
	return isInRange(line.startPoint) || isInRange(line.endPoint);
	//return true;
}

ExtendedLine::ExtendedLine( const Line& line ) : Line(line)
{
	float start = calRadius(startPoint.x,startPoint.y,angle- M_PI_2);
	float end = calRadius(endPoint.x,endPoint.y,angle- M_PI_2);
	if (end > start){
		startR = start;endR = end;
	}else{
		startR = end;endR = start;
	}
	//endPoints.push_back(line.startPoint);
	//endPoints.push_back(line.endPoint);
	
}

bool ExtendedLine::isInRange( const Point& point ) const
{
	float parallelR = calRadius(point.x,point.y,angle);
	float prependicularR = calRadius(point.x,point.y,angle - M_PI_2);
	if(radius - kLineWidth < parallelR && radius + kLineWidth > parallelR ){
		if( (startR - kLineExtended < prependicularR && startR + kLineExtended > prependicularR)
		|| (endR + kLineExtended > prependicularR && endR - kLineExtended < prependicularR) ){
			return true;
		}
	}
	return false;
}
