/********************************************************************
	生成时间:	2014/11/20   10:41
	文件名: 	QuadrangleFinder.cpp
	@author:	李鹏 <hzlipeng@corp.netease.com>
				Copyright (c) 2014, NetEase Inc. All rights reserved.
	
	功能:		
*********************************************************************/

#include "Global.h"
#include "QuadrangleFinder.h"
#include "QuadrangleCostCalculator.h"
#include "IntersectionFinder.h"

namespace {
	const double kCostThresh = 0.35;
}//End namespace 


static double quadArea(const QuadrangleWithCost quad){
	return (quad.myQuad.topRight.x - quad.myQuad.topLeft.x + quad.myQuad.bottomRight.x - quad.myQuad.bottomLeft.x) 
		* (quad.myQuad.bottomLeft.y - quad.myQuad.topLeft.y + quad.myQuad.bottomRight.y - quad.myQuad.topRight.y) / 4.;
}




/*
 ************************************************************************
 * Function: 
 *
 * @param  imIn
 * @param  candsIn
 * @param  out
 * @return 
 ************************************************************************
*/
bool QuadrangleFinder::Find( const Mat &imIn, const vector<EdgeCandidate> &candsIn, QuadrangleWithCost *out ) const
{
	const int imHeight = imIn.rows;
	const int imWidth = imIn.cols;

	vector<LineSegCollector> verCands;
	vector<LineSegCollector> horCands;

	vector<int> verCandsEdgeIdx;
	vector<int> horCandsEdgeIdx;
	for(auto k=0; k<candsIn.size(); ++k)
	{
		if(candsIn[k].isVertical)
		{
			verCands.push_back( candsIn[k].segments );
			verCandsEdgeIdx.push_back(k);
		}
		else
		{
			horCands.push_back( candsIn[k].segments );
			horCandsEdgeIdx.push_back(k);
		}
	}


	QuadrangleCostCalculator myCalculator(imHeight, imWidth);

	vector<QuadrangleWithCost> resList;

	for(auto k=0; k<horCands.size(); ++k)
	{
		const LineSegCollector &top = horCands[k];
		for(auto u=0; u<horCands.size(); ++u)
		{
			const LineSegCollector &bottom = horCands[u];
			for(auto m=0; m<verCands.size(); ++m)
			{
				const LineSegCollector &left = verCands[m];
				for(auto n=0; n<verCands.size(); ++n)
				{
					const LineSegCollector &right = verCands[n];

					QuadrangleWithCost curQuad = myCalculator.Calculate(left, top, right, bottom);
					if(curQuad.cost < kCostThresh)
					{
						curQuad.edgeIdx_left   = verCandsEdgeIdx[m];
						curQuad.edgeIdx_top    = horCandsEdgeIdx[k];
						curQuad.edgeIdx_right  = verCandsEdgeIdx[n];
						curQuad.edgeIdx_bottom = horCandsEdgeIdx[u];
						curQuad.area = quadArea(curQuad);
						resList.push_back( move(curQuad) );
					}
				}
			}
			
		}
	}


	if(resList.empty())
		return false;

	(*out) = *min_element(resList.begin(), resList.end());

	return true;
}
