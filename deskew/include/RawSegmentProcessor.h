/** 
* @file          RawSegmentProcessor.h 
* @Synopsis       
* @author        NieYuan,hznieyuan@corp.netease.com,qiushuiwuhong@gmail.com 
* @version       000 
* @date          2014\11\19
*/


#ifndef RAWSEGMENTPROCESSOR_H_
#define RAWSEGMENTPROCESSOR_H_
#include <vector>
#include "SegmentsGroup.h"


class RawSegmentProcessor
{
public:
	RawSegmentProcessor(ImageInfo _info);
	~RawSegmentProcessor(void);
	vector<EdgeCandidate> process(vector<Line>& rawSegments);

	vector<EdgeCandidate> getEdgeCandidates();

private:
	void clear();
	void getLines();
	void connectSegments();
	void mergeSegments( vector<Line> &rawSegments );
	const ImageInfo info;
	vector<SegmentsGroup*> groups;
	//const double kRadiusTolarance;
};
#endif // RAWSEGMENTPROCESSOR_H_


