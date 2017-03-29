#ifndef __ENGINE__
#define __ENGINE__
#include "4debug.h"
#include "opencv2/opencv.hpp"
#include <map>

using namespace std;
using namespace cv;

namespace singlechar{
	
const unsigned char kFirstBitMask = 128; // 1000000
const unsigned char kSecondBitMask = 64; // 0100000
const unsigned char kThirdBitMask = 32; // 0010000
const unsigned char kFourthBitMask = 16; // 0001000
const unsigned char kFifthBitMask = 8; // 0000100
  
class CharEngine
{
public:
	CharEngine(const char * path){
		m_idx2char.clear();
		m_char2idx.clear();
		ifstream dictfile(path);
		string line;
		for(int i=0; getline(dictfile, line); i++){
			int pos = line.find_first_of(" ");
			string idx_str, label_line;

			idx_str = line.substr(0, pos);
			label_line = line.substr(pos+1);

			replace_all(label_line, " ", "");
			replace_all(label_line, "　", "");
			replace_all(label_line, "\t", "");
			
			cout<<"index str:"<<idx_str<<" index number:"<<i<<" char:"<<label_line<<endl;
			assert(1==char_length(label_line));
			m_idx2char[i] = label_line;
			m_char2idx[label_line] = i;
		}
	}
	~CharEngine(){}

	static int utf8_char_len(char firstByte)
	{
		std::string::difference_type offset = 1;
		if(firstByte & kFirstBitMask) // This means the first byte has a value greater than 127, and so is beyond the ASCII range.
		{  
			if(firstByte & kThirdBitMask) // This means that the first byte has a value greater than 224, and so it must be at least a three-octet code point.
			{  
				if(firstByte & kFourthBitMask) // This means that the first byte has a value greater than 240, and so it must be a four-octet code point.
					offset = 4;
				else
					offset = 3;
			}  
			else
			{  
				offset = 2;
			}  
		}  
		return offset;
	}

	static string&   replace_all(string&   str,const   string&   old_value,const   string&   new_value)     
	{     
		while(true)   {     
			string::size_type   pos(0);     
			if(   (pos=str.find(old_value))!=string::npos   )     
				str.replace(pos,old_value.length(),new_value);     
			else   break;     
		}     
		return   str;     
	}    

	static int char_length(string line)
	{
		int index = 0;
		int length = 0;
		while(index < line.size())
		{
			int char_len = utf8_char_len(line[index]);
			length++;
			index += char_len;
		}
		return length;
	}
	//bool  LoadEngine(char *ldafile, char *profile, char *svmfile);
	inline bool RecognizeChar(Mat im, int &cls, float & conf){
		vector<int> clsvec;
		vector<float> confvec;
		if( GetTopN(im, 1, clsvec, confvec) )
		{
			cls = clsvec[0];
			conf = confvec[0];
			return true;
		}
		else
			return false;
	};
	virtual bool GetTopN(Mat im, int n, vector<int>& topn, vector<float>& topnconf) = 0;

	int get_clsnum(){ return m_clsnum; } //class number
	inline int getidx(string thechar){
		if(m_char2idx.find(thechar)!=m_char2idx.end())
			return m_char2idx[thechar];
		else
			return -1;
	}
	inline string getchar(int idx){
		if(m_idx2char.find(idx)!=m_idx2char.end())
			return m_idx2char[idx];
		else
			return "";
	}
	void setDebugPath(string p){ m_debugpath = p; }
protected:
	int	m_clsnum;
	map<int,string> m_idx2char;
	map<string,int> m_char2idx;
	string m_debugpath="t.png";
};
}
#endif
