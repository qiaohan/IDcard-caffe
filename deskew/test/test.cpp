/********************************************************************
	生成时间:	2014/11/13   13:12
	文件名: 	main.cpp
	@author:	李鹏 <hzlipeng@corp.netease.com>
				Copyright (c) 2014, NetEase Inc. All rights reserved.
	
	功能:		
*********************************************************************/

#include "Global.h"
#include "FilePathClass.h"
//#include "QuadrangleFinder_Wrapper.h"
#include <boost/timer.hpp>
//#include "QuadrangleTrans_Wrapper.h"
//#include "LSD_Wrapper.h"
//#include "ImgStream.h"
#include "BCRDeskew.h"
//#include "ColorImageEnhancer.h"
//#include "Timer.h"
//#include "Deskew.h"
//#include "YoudaoInfo.h"

//int YoudaoInfo::_whichBorder = 0;


using namespace std;

extern std::string filePathTmp;

int main(int argc, char** argv)
{
	FilePathClass pathEngine;
	const std::string folderPath = argv[1];//"./data/";
	std::vector<PathPair> pathRes = pathEngine.getFilePath(folderPath, ".jpg");
	const int nFiles = (int)pathRes.size();
	//QuadrangleDetectionParam p = QuadrangleDetectionParam::createAccurateDetectParam();
	//QuadrangleFinder_Wrapper myWrapper(p);
	for(int k=0; k<nFiles; ++k)
	{
		std::string filePath = pathRes[k].fullPath;
		filePathTmp = std::string(argv[2]) + pathRes[k].Name;
		fprintf(stdout, "\nProcessing file: %s\n", filePath.c_str());
		fflush(stdout);

		Mat im_tmp = imread(filePath);
		Mat im_org = im_tmp.clone();

		boost::timer curTimer;
		//QuadrangleDetectionParam xxx = p;
		//Quadrangle yyy;
		//if( DetectQuadrangle(cv::Mat(im_org.c_img()), xxx, &yyy) )
		Mat resim = BCRDeskew(im_org,1000);
		if( !resim.empty() )
		{
			string savepath = string(argv[2])+pathRes[k].Name;
			std::cout << "succ! save to:" << savepath << std::endl;
			imwrite(savepath,resim);
			//std::cout << yyy.topLeft.x << yyy.topRight.x << yyy.bottomLeft.x << yyy.bottomRight.x << std::endl;
		}
		else
		{
			std::cout << "unsucc!" <<std::endl;
		}
		std::cout << curTimer.elapsed() << std::endl;
	}

	return EXIT_SUCCESS;
}
