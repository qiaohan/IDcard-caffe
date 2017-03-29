/** 
* @file          RawSegmentProcessor.cpp 
* @Synopsis       
* @author        NieYuan,hznieyuan@corp.netease.com,qiushuiwuhong@gmail.com 
* @version       000 
* @date          2014\11\19
*/
#include "RawSegmentProcessor.h"
#include "SegmentsGroup.h"
#include <algorithm>
#include "ImgExceptions.h"
#include <list>
#include <iterator>
#include <boost/tuple/tuple.hpp>
#include <boost/algorithm/minmax_element.hpp>
//#include <boost/algorithm/string/detail/predicate.hpp>
using namespace std;
namespace{
	
	static const int kMaxLineNum = 10;
	static const double kMinLenRatio = 0.1;
	static const double kRadiusTolGroup = 1;
	//static const double kRadiusTolaranceRatio = 0.02;
	typedef boost::tuples::tuple<int,double> LineDistance;
	struct LineInfo{
		int idx;
		bool isUsed;
	};
}


RawSegmentProcessor::RawSegmentProcessor(ImageInfo _info)
	:info(_info)
	//,kRadiusTolarance(max(info.kHeight,info.kWidth)*kRadiusTolaranceRatio)
{

}



RawSegmentProcessor::~RawSegmentProcessor( void )
{
	clear();
}



vector<EdgeCandidate> RawSegmentProcessor::process(vector<Line>& rawSegments )
{
	if(rawSegments.empty()){
		throw ImgBad("rawsegments cannot be empty!", __FILE__, __LINE__);
	}
	clear();

	//int remainSegs = rawSegments.size();
	mergeSegments(rawSegments);
	connectSegments();
	getLines();
	return getEdgeCandidates();

}


//bool RawSegmentProcessor::mergeSegments()
//{
//	for (auto itGroup = groups.begin(); itGroup != groups.end() ; ++itGroup){
//		(*itGroup)->mergeSegments();
//	}
//	return true;
//}

void RawSegmentProcessor::clear()
{
	for (int i=0 ; i != groups.size() ; i++){
		if (groups[i] != nullptr){
			delete groups[i];
			groups[i] = nullptr;
		}
	}
	groups.clear();
}

void RawSegmentProcessor::mergeSegments( vector<Line> &rawSegments )
{
	sort(rawSegments.begin(),rawSegments.end(),[](const Line& lval , const Line& rval){
		return lval.len > rval.len;
	});
	groups.push_back(new SegmentsGroup(rawSegments.front(),info));
	for (int i=1 ; i != rawSegments.size() ; i++){
		vector<LineDistance> lineDistances;
		float diff= 0;
		for (int j = 0; j != groups.size() ; ++j){
			if (groups[j]->isSameLine(rawSegments[i],diff)){
				lineDistances.push_back(boost::tuples::make_tuple(j,diff));
			}		
		}
		auto bestMatched = min_element(lineDistances.begin(),lineDistances.end(),[](const LineDistance& lval,const LineDistance& rval){
			return lval.get<1>() < rval.get<1>();
		});

		if (!lineDistances.empty() && bestMatched->get<1>() < kRadiusTolGroup){
			groups[bestMatched->get<0>()]->add(rawSegments[i]);
		}else{
			groups.push_back(new SegmentsGroup(rawSegments[i],info));
		}
	}
}

void RawSegmentProcessor::connectSegments()
{
	vector<Line> tmpSegments;
	for (auto itGroup = groups.begin(); itGroup != groups.end() ; ++itGroup){
		(*itGroup)->mergeSegments();
		//(*itGroup)->projectSegments();
		copy((*itGroup)->segments.begin(),(*itGroup)->segments.end(),back_inserter(tmpSegments));
	}
	sort(tmpSegments.begin(),tmpSegments.end(),[](const Line& lval , const Line& rval){
		return lval.len > rval.len;
	});
	clear();
	while(!tmpSegments.empty()){
		SegmentsGroup* currentGroup = new SegmentsGroup(tmpSegments.front(),info);
		tmpSegments.erase(tmpSegments.begin(),tmpSegments.begin()+1);
		currentGroup->pickSegments(tmpSegments);
		groups.push_back(currentGroup);
	}

	for (auto itGroup = groups.begin(); itGroup != groups.end() ; ++itGroup){
		(*itGroup)->mergeSegments();
		(*itGroup)->projectSegments();
	}
}

void RawSegmentProcessor::getLines()
{
	sort(groups.begin(),groups.end(),[](const SegmentsGroup* lval , const SegmentsGroup* rval){
		return lval->len > rval->len;
	});

	for (int i = 0; i < groups.size()-1 ; ++i){
		if (groups[i] == nullptr){
			continue;
		}
		for (int j = i+1; j < groups.size() ; ++j){
			if (groups[j] == nullptr){
				continue;
			}
			if (groups[i]->isInRange(*groups[j])){
				groups[i]->merge(*groups[j]);
				delete groups[j];
				groups[j] = nullptr;
			}
		}
	}

	groups.erase(remove(groups.begin(),groups.end(),nullptr),groups.end());
	for (auto itGroup = groups.begin(); itGroup != groups.end() ; ++itGroup){
		(*itGroup)->mergeSegments();
		(*itGroup)->projectSegments();
	}
}

vector<EdgeCandidate> RawSegmentProcessor::getEdgeCandidates()
{
	vector<EdgeCandidate> res;
	int verticalNum = 0 ,horizentalNum = 0;
	for (int i = 0; i != groups.size() ; ++i){
		if (groups[i]->len > kMinLenRatio * max(info.kHeight,info.kWidth)){
			
			//candidate.segments = groups[i]->segments;

			//copy(groups[i]->segments.begin(),groups[i]->segments.end(),back_inserter(candidate.segments));
			//candidate.segments.resize(groups[i]->segments.size());
			//transform(groups[i]->segments.begin(),groups[i]->segments.end(),candidate.segments.begin(),[](const ExtendedLine& val)->Line{
			//	Line res;
			//	res.startPoint = val.startPoint;
			//	res.endPoint = val.endPoint;
			//	return res;
			//});

			
			if(abs(groups[i]->angle) < 50./180.*M_PI) //vertical
			{
				EdgeCandidate candidate;
				candidate.isVertical = true;
				candidate.angle = groups[i]->angle;
				for (int j = 0; j != groups[i]->segments.size() ; ++j){
					Line res = groups[i]->segments[j];
					candidate.origLSDLines.splice(candidate.origLSDLines.end(),res.origLSDLines);

					candidate.segments.push_back(res);
				}

				for_each(candidate.origLSDLines.begin(),candidate.origLSDLines.end(),[candidate](BaseLine & line){
					line.fixStartEnd(candidate.isVertical);
				});

				res.push_back(candidate);
			}
			

			if(abs(groups[i]->angle) > 40./180.*M_PI) //hor
			{
				EdgeCandidate candidate;
				candidate.isVertical = false;
				candidate.angle = groups[i]->angle;
				for (int j = 0; j != groups[i]->segments.size() ; ++j){
					Line res = groups[i]->segments[j];
					candidate.origLSDLines.splice(candidate.origLSDLines.end(),res.origLSDLines);

					candidate.segments.push_back(res);
				}

				for_each(candidate.origLSDLines.begin(),candidate.origLSDLines.end(),[candidate](BaseLine & line){
					line.fixStartEnd(candidate.isVertical);
				});

				res.push_back(candidate);
			}

			if (groups[i]->isVertical){
				verticalNum++;
			}else{
				horizentalNum++;
			}
		}
	}
	return res;
}

