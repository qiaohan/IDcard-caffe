/********************************************************************
	生成时间:	2014/11/20   10:16
	文件名: 	IntersectionFinder.cpp
	@author:	李鹏 <hzlipeng@corp.netease.com>
				Copyright (c) 2014, NetEase Inc. All rights reserved.
	
	功能:		
*********************************************************************/

#include "Global.h"
#include "IntersectionFinder.h"

using namespace std;

inline double CalTheta(const Line &curLine)
{
	return atan(  -1.0*(curLine.startPoint.x-curLine.endPoint.x+DBL_EPSILON)/(curLine.startPoint.y-curLine.endPoint.y+DBL_EPSILON)   );
}

/*
 ************************************************************************
 * Function: 
 *
 * @param  sinTheta_lhs
 * @param  cosTheta_lhs
 * @param  r_lhs
 * @param  sinTheta_rhs
 * @param  cosTheta_rhs
 * @param  r_rhs
 * @return 
 ************************************************************************
*/
static Point CalIntersection(double sinTheta_lhs, 
								double cosTheta_lhs,
								double r_lhs,
								double sinTheta_rhs,
								double cosTheta_rhs,
								double r_rhs)
{
	const double a = r_lhs + r_rhs;
	const double b = r_lhs - r_rhs;
	const double c = cosTheta_lhs + cosTheta_rhs;
	const double d = cosTheta_lhs - cosTheta_rhs;
	const double e = sinTheta_lhs + sinTheta_rhs;
	const double f = sinTheta_lhs - sinTheta_rhs;

	Point res;

	res.x = static_cast<int>( (a*f - b*e) / (c*f - d*e) + 0.5);
	res.y = static_cast<int>( (a*d - b*c) / (e*d - f*c) + 0.5);

	return move(res);
}

/*
 ************************************************************************
 * Function: 
 *
 * @param  theta_lhs
 * @param  r_lhs
 * @param  theta_rhs
 * @param  r_rhs
 * @return 
 ************************************************************************
*/
Point IntersectionFinder::FindIntersection( const double theta_lhs, const double r_lhs, const double theta_rhs, const double r_rhs ) const
{
	const double sinTheta_lhs = sin(theta_lhs);
	const double cosTheta_lhs = cos(theta_lhs);
	const double sinTheta_rhs = sin(theta_rhs);
	const double cosTheta_rhs = cos(theta_rhs);

	return CalIntersection(sinTheta_lhs, cosTheta_lhs, r_lhs, sinTheta_rhs, cosTheta_rhs, r_rhs);
}



/*
 ************************************************************************
 * Function: 
 *
 * @param  lhs
 * @param  rhs
 * @return 
 ************************************************************************
*/
Point IntersectionFinder::FindIntersection( const Line &lhs, const Line &rhs ) const
{
	const double theta_lhs = CalTheta(lhs);
	const double theta_rhs = CalTheta(rhs);

	const double sinTheta_lhs = sin(theta_lhs);
	const double cosTheta_lhs = cos(theta_lhs);
	const double sinTheta_rhs = sin(theta_rhs);
	const double cosTheta_rhs = cos(theta_rhs);

	const double r_lhs = lhs.startPoint.x*cosTheta_lhs + lhs.startPoint.y*sinTheta_lhs;
	const double r_rhs = rhs.startPoint.x*cosTheta_rhs + rhs.startPoint.y*sinTheta_rhs;

	return CalIntersection(sinTheta_lhs, cosTheta_lhs, r_lhs, sinTheta_rhs, cosTheta_rhs, r_rhs);
}