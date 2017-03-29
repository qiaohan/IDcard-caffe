/*************************************************************************
    > File Name: ../test/test.cpp
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Tue 17 Jan 2017 11:27:57 AM CST
 ************************************************************************/

#include<iostream>
#include "opencv2/opencv.hpp"
#include "IDcardReader.h"

#include<stdio.h>
#include<time.h>
#include<sys/time.h>
#include<string.h>
#include<stdlib.h>

using namespace std;
using namespace cv;

int main(int argc, char ** argv){
	IDcardReader idreader("engine_bin/");
	string r;
	Mat im = imread(argv[1]);
	cout<<im.size()<<endl;
	idreader.ReadImg(im,r);
	struct timeval starttime,endtime;
	double timeuse;
	gettimeofday(&starttime,NULL);
	idreader.ReadImg(im,r);
	cout<<r<<endl;
	gettimeofday(&endtime,NULL);
	timeuse=1000000*(endtime.tv_sec-starttime.tv_sec)+endtime.tv_usec-starttime.tv_usec;
	timeuse/=1000000;/*转换成秒输出*/
	printf("timeuse=%f",timeuse);
	return 0;
}
