/********************************************************************
	生成时间:	2014/11/13   15:46
	文件名: 	Line_Utils.cpp
	@author:	李鹏 <hzlipeng@corp.netease.com>
				Copyright (c) 2014, NetEase Inc. All rights reserved.
	
	功能:		
*********************************************************************/

#include "Global.h"
#include "Line_Utils.h"


double calAngleDiff( double to ,double from )
{
	if (std::abs(to-from) <= M_PI_2){
		return to-from;
	}
	else if(from > to){
		return to+M_PI-from;
	}else{
		return to-M_PI-from;
	}
}

double calRadius( double x ,double y,double angle )
{
	return x*std::cos(angle) + y*std::sin(angle);
}

double calAngle( double xStart,double yStart,double xEnd , double yEnd )
{
	return std::atan(-1.0*(xEnd-xStart+DBL_EPSILON)/(yEnd-yStart+DBL_EPSILON));
}

double calLen( double xStart,double yStart,double xEnd , double yEnd )
{
	return std::abs(xEnd-xStart) + std::abs(yEnd - yStart);
}

double angleAdd( double angle ,double increamental )
{
	angle += increamental;
	if (angle < -M_PI_2){
		angle += M_PI;
	}else if (angle > M_PI_2){
		angle -= M_PI;
	}
	return angle;
}


Line::Line( const Point &startPointIn, const Point &endPointIn ) 
	:BaseLine(startPointIn,endPointIn)
	,isUsed(false)
{
	angle = calAngle(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
	radius = calRadius(startPoint.x, startPoint.y, angle);
	len = calLen(startPoint.x,startPoint.y,endPoint.x,endPoint.y);
	origLSDLines.push_back(BaseLine(startPointIn,endPointIn));

}

void BaseLine::fixStartEnd( bool isVertical )
{
	if((isVertical && endPoint.y < startPoint.y) 
		|| (!isVertical && endPoint.x < startPoint.x)){
			int x = endPoint.x;
			int y = endPoint.y;
			endPoint.x = startPoint.x;
			endPoint.y = startPoint.y;
			startPoint.x = x;
			startPoint.y = y;
	}
	if (isVertical){
		start = startPoint.y;end = endPoint.y;
	}else{
		start = startPoint.x;end = endPoint.x;
	}
}
