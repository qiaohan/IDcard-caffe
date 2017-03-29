/********************************************************************
	生成时间:	2014/11/20   11:17
	文件名: 	QuadrangleCostCalculator.cpp
	@author:	李鹏 <hzlipeng@corp.netease.com>
				Copyright (c) 2014, NetEase Inc. All rights reserved.
	
	功能:		
*********************************************************************/

#include "Global.h"
#include "QuadrangleCostCalculator.h"
#include "QuadrangleGenerator.h"

using namespace std;

inline Line GetLineFromCollector(const LineSegCollector &collector)
{
	return move(     Line( collector.front().startPoint, collector.back().endPoint )     );
}


/*
 ************************************************************************
 * Function: 
 *
 * @param  leftLine
 * @param  topLine
 * @param  rightLine
 * @param  bottomLine
 * @param  imHeight
 * @param  imWidth
 * @return 
 ************************************************************************
*/
bool CheckLines(const Line &leftLine, const Line &topLine, const Line &rightLine, const Line &bottomLine, int imHeight, int imWidth)
{
	const int widthThresh = imWidth/10.;
	const int heightThresh = imHeight/8;

	if( max(rightLine.startPoint.x, rightLine.endPoint.x) - min(leftLine.startPoint.x, leftLine.endPoint.x) < widthThresh)
		return false;

	if( max(bottomLine.startPoint.y, bottomLine.endPoint.y) - min(topLine.startPoint.y, topLine.endPoint.y) < heightThresh)
		return false;


	return true;
}

/*
 ************************************************************************
 * Function: 
 *
 * @param  curPoint
 * @param  imHeight
 * @param  imWidth
 * @return 
 ************************************************************************
*/
inline bool IsPointInImg(const Point &curPoint, const int imHeight, const int imWidth)
{
	//if(curPoint.x < 0 || curPoint.x >= imWidth)
	//	return false;

	//if(curPoint.y < 0 || curPoint.y >= imHeight)
	//	return false;

	return true;
}


/*
 ************************************************************************
 * Function: 
 *
 * @param  curQuad
 * @param  imHeight
 * @param  imWidth
 * @return 
 ************************************************************************
*/
bool CheckQuadrangle( const Quadrangle &curQuad, int imHeight, int imWidth )
{
	if( !IsPointInImg(curQuad.topLeft, imHeight, imWidth) )
		return false;

	if( !IsPointInImg(curQuad.topRight, imHeight, imWidth) )
		return false;

	if( !IsPointInImg(curQuad.bottomLeft, imHeight, imWidth) )
		return false;

	if( !IsPointInImg(curQuad.bottomRight, imHeight, imWidth) )
		return false;

	if(  (curQuad.topLeft.x>=curQuad.topRight.x)     ||    (curQuad.bottomLeft.x>=curQuad.bottomRight.x)    )
		return false;

	if(  (curQuad.topLeft.y>=curQuad.bottomLeft.y)     ||     (curQuad.topRight.y>=curQuad.bottomRight.y)    )
		return false;

	const int halfHeight = imHeight>>1;
	const int halfWidth = imWidth>>1;
	
	//if(curQuad.topLeft.x>halfWidth && curQuad.bottomLeft.x>halfWidth)
	//	return false;
	//
	//if(curQuad.topLeft.y>halfHeight && curQuad.topRight.y>halfHeight)
	//	return false;
	//
	//if(curQuad.topRight.x<halfWidth && curQuad.bottomRight.x<halfWidth)
	//	return false;
	//
	//if(curQuad.bottomLeft.y<halfHeight && curQuad.bottomRight.y<halfHeight)
	//	return false;

	return true;
}


/*
 ************************************************************************
 * Function: 
 *
 * @param  curQuad
 * @return 
 ************************************************************************
*/
bool CheckAreaAndLength(const Quadrangle &curQuad, int area)
{
	const int leftLen = curQuad.bottomLeft.y - curQuad.topLeft.y;
	const int rightLen = curQuad.bottomRight.y - curQuad.topRight.y;
	const int topLen = curQuad.topRight.x - curQuad.topLeft.x;
	const int bottomLen = curQuad.bottomRight.x - curQuad.bottomLeft.x;

	int verMinLen = rightLen;
	int verMaxLen = leftLen;
	int horMinLen = bottomLen;
	int horMaxLen = topLen;

	if(leftLen < rightLen)
	{
		verMinLen = leftLen;
		verMaxLen = rightLen;
	}

	if(topLen < bottomLen)
	{
		horMinLen = topLen;
		horMaxLen = bottomLen;
	}

	const double ratioMaxMin = 3.;
	const double ratioHorVer_low = 0.3;
	const double ratioHorVer_high = 3.;
	const double ratioArea_low = 1./20;

	if(verMaxLen > ratioMaxMin*verMinLen || horMaxLen > ratioMaxMin*horMinLen)
		return false;

	if(horMaxLen > ratioHorVer_high*verMinLen  ||  horMaxLen < ratioHorVer_low*verMinLen)
		return false;


	if(horMaxLen*verMaxLen < ratioArea_low*area)
		return false;

	return true;
}


/*
 ************************************************************************
 * Function: 
 *
 * @param  curCollector
 * @param  intersectPt1
 * @param  intersectPt2
 * @param  isVertical
 * @return 
 ************************************************************************
*/
double CalLineSegCost(const LineSegCollector &curCollector, const Point &intersectPt1, const Point &intersectPt2, const bool isVertical)
{
	int offset = 0;
	int coord1 = intersectPt1.x;
	int coord2 = intersectPt2.x;

	if(isVertical)
	{
		offset = 1;
		coord1 = intersectPt1.y;
		coord2 = intersectPt2.y;
	}

	int externalCost = 0;
	int internalCost = 0;
	int occupied = 0;
	for(LineSegCollector::size_type k=0; k<curCollector.size(); ++k)
	{
		const Line &curLine = curCollector[k];
		const int curStart  = *( &(curLine.startPoint.x) + offset );
		const int curEnd    = *( &(curLine.endPoint.x) + offset );

		if(coord1 >= curStart && coord1 <= curEnd)
			externalCost += (coord1-curStart);

		if(coord2 >= curStart && coord2 <= curEnd)
			externalCost += (curEnd-coord2);


		if(curStart < coord1)
		{
			if(curEnd > coord1)
			{
				if(curEnd < coord2)
					occupied += (curEnd - coord1);
				else
					occupied += (coord2 - coord1);
			}
		}
		else if(curStart > coord2)
		{

		}
		else
		{
			if(curEnd < coord2)
				occupied += (curEnd - curStart);
			else
				occupied += (coord2 - curStart);
		}
	}

	internalCost = coord2 - coord1 - occupied;

	return internalCost + externalCost;
}



/*
 ************************************************************************
 * Function: 
 *
 * @param  left
 * @param  top
 * @param  right
 * @param  bottom
 * @return 
 ************************************************************************
*/
QuadrangleWithCost QuadrangleCostCalculator::Calculate( const LineSegCollector &left, 
														const LineSegCollector &top, 
														const LineSegCollector &right, 
														const LineSegCollector &bottom ) const
{
	const Line leftLine   = GetLineFromCollector(left);
	const Line topLine    = GetLineFromCollector(top);
	const Line rightLine  = GetLineFromCollector(right);
	const Line bottomLine = GetLineFromCollector(bottom);

	QuadrangleWithCost res;

	if( !CheckLines(leftLine, topLine, rightLine, bottomLine, _imHeight, _imWidth) )
		return move(res);


	QuadrangleGenerator myGenerator;
	const Quadrangle curQuad = myGenerator.Generate(leftLine, topLine, rightLine, bottomLine);

	if( !CheckQuadrangle(curQuad, _imHeight, _imWidth) )
		return move(res);

	if( !CheckAreaAndLength(curQuad, _imHeight*_imWidth) )
		return move(res);


	const double topCost = CalLineSegCost(top, curQuad.topLeft, curQuad.topRight, false);
	const double leftCost = CalLineSegCost(left, curQuad.topLeft, curQuad.bottomLeft, true);
	const double rightCost = CalLineSegCost(right, curQuad.topRight, curQuad.bottomRight, true);
	const double bottomCost = CalLineSegCost(bottom, curQuad.bottomLeft, curQuad.bottomRight, false);
	
	
	const int leftLen   = curQuad.bottomLeft.y - curQuad.topLeft.y;
	const int rightLen  = curQuad.bottomRight.y - curQuad.topRight.y;
	const int topLen    = curQuad.topRight.x - curQuad.topLeft.x;
	const int bottomLen = curQuad.bottomRight.x - curQuad.bottomLeft.x;
	const int perimeter = leftLen + rightLen + topLen + bottomLen;

	const double normCost = (topCost+leftCost+rightCost+bottomCost)/perimeter;
	
	res.myQuad = curQuad;
	res.cost = normCost;

	return move(res);
}
