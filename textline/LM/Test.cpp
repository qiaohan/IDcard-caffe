#include <iostream>
#include <fstream>
#include "hashDict.h"
#include "string.h"
#include <stdlib.h>
using namespace std;
using namespace NLP;

int main()
{
	//init the hash dictionary
	string hashFile("C:\\corpus.index");
	string input("input.txt");
	string cfd("confidence.txt");

	hashDict dict;
	if(!dict.loadDict( hashFile.c_str() ) )
	{
		cerr<<"cannot load dict"<<endl;
		return false;
	}
	string key = "Î÷ ³Ç";
	string value;

	dict.match(key,value);
	double numerator = atoi(value.c_str())+1;
}