#ifndef _IMGEXCEPTION_H_
#define _IMGEXCEPTION_H_
#include <stdexcept>
#include "opencv2/opencv.hpp"
#include <stdlib.h>

using namespace cv;
using namespace std;

class ImgBad: public std::logic_error  
{  
public:  
    explicit ImgBad(const std::string &s) : std::logic_error(s),linenum(-1),img(NULL),filename("miss") {}  
    ImgBad(const std::string &s, const std::string &fname, const int lnum, Mat * im = NULL):
        std::logic_error(s), filename(fname), linenum(lnum), img(im) {}  
	
	const std::string filename;
	const int linenum;
	const Mat * img;
	
	const char* what() const throw(){
		string s = std::logic_error::what();
		char linenum_a[20];
		sprintf(linenum_a, "%d", linenum);
		s += "\n in file: "+filename
			+"\n line: "+string(linenum_a);
		return s.c_str();
	}
};  

#endif
