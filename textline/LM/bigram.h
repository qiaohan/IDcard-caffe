#include <iostream>
#include <fstream>
//#include "hashDict.h"
#include "ngram.h"
#include <string>
#include <stdlib.h>
using namespace std;
using namespace NLP;

typedef unsigned short word;

bool bigram(word ** str, float ** conf, int nchar, int * ncandidate, word * finalout);