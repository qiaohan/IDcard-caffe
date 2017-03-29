#include "bigram.h"
#define BIGRAMFREQENCY 1


#if BIGRAMFREQENCY
static float getTransitionProb(hashDict * dict, word a, word b);
static float getProb(hashDict * dict, word a);
static float dp2gram(word ** str, float ** conf, word * result, int length, int* ncandidate, hashDict * dict);
#else
static float getTransitionProb(NgramBase * dict, word a, word b);
static float getProb(NgramBase * dict, word a);
static float dp2gram(word ** str, float ** conf, word * result, int length, int* ncandidate, NgramBase * dict);
#endif
#if 0
static float enum2gram(word ** str, float ** conf, word * result, int length, int* ncandidate, int endindex, hashDict * dict);
#endif
static void word2string(word * output, int nchar, string &a);

#if BIGRAMFREQENCY
static float getTransitionProb(hashDict * dict, word a, word b)
{
	char s[6];
	memcpy(s,&a,2);
	s[2]=' ';
	memcpy(s+3,&b,2);
	s[5]=0;
	string key = s;
	string v;
	dict->match(key,v);
	//cout<<key<<":"<<atoi(v.c_str())<<endl;
    return atoi(v.c_str());
}
static float getProb(hashDict * dict, word a)
{
	return 1.0;	

	//char s[3];
	//memcpy(s,&a,2);
	//s[2]=0;

	//string key = s;
	//string v;
	//dict->match(key,v);
	////cout<<key<<":"<<atoi(v.c_str())<<endl;
	//float p = atoi(v.c_str());
	//return (0.01 * p);

	//if (p > 0)
	//	return log(p);
	//else
	//	return 0;
	//return log((float)));
}

static float dp2gram(word ** str, float ** conf, word * result, int length, int* ncandidate, hashDict * dict)
{
	int ** pre_node = new int*[length];
	float ** score = new float*[length];
	float max_score = -1;
	int max_cand = -1;
	if(length<1)
	{
		return -1;
	}
	score[0] = new float[ncandidate[0]];
	//first
	for(int i=0; i<ncandidate[0]; i++)
	{
		if( (score[0][i]=getProb(dict, str[0][i])) > max_score )
		{
			max_cand = i;
			max_score = score[0][i];
		}
	}

	if(1==length)
	{
		result[0]=score[0][max_cand];
		return max_score;
	}

	//rest
	for(int i=1; i<length; i++)
	{
		int ncand = ncandidate[i];
		pre_node[i] = new int [ncand];
		score[i] = new float[ ncand ];
		for(int j=0; j<ncand; j++)
		{
			int max_pre_index=-1;
			float max_this_score=-1;
			for(int k=0; k< ncandidate[i-1]; k++)
			{
				float score_t = score[i-1][k] + getTransitionProb(dict,str[i-1][k],str[i][j]) + getProb(dict,str[i][j]);
				if(score_t > max_this_score)
				{
					max_this_score = score_t;
					max_pre_index = k;
				}
			}
			score[i][j] = max_this_score;
			pre_node[i][j] = max_pre_index;		
		}
	}
	//find max score
	max_score = -1;
	max_cand = -1;
	for(int i=0; i<ncandidate[length-1]; i++)
	{
		if( max_score<score[length-1][i] )
		{
			max_cand = i;
			max_score = score[length-1][i];
		}
	}
	result[length-1]=str[length-1][max_cand];
	int cand = max_cand;
	//backtracking
	for(int i = length-2; i>=0; i--)
	{
		cand = pre_node[i+1][cand];
		result[i] = str[i][cand];
	}
	return max_score;
}

#else
static float getTransitionProb(NgramBase * ngram, word a, word b)
{
	char s[6];
	memcpy(s,&a,2);
	s[2]=' ';
	memcpy(s+3,&b,2);
	s[5]=0;
	string key = s;
	string v;
	//dict->match(key,v);
	float poss1 = -ngram->getProb(key);
	//poss1 = -log(poss1);
	
	return poss1;
}
static float getProb(NgramBase * ngram, word a)
{
	return 0;
	char s[3];
	memcpy(s,&a,2);
	s[2]=0;
	string key = s;
	//dict->match(key,v);
	float poss1 = ngram->getProb(key);
	//poss1 = -log(poss1) * 0.5;
	poss1 = 0;
	return poss1;
}

static float dp2gram(word ** str, float ** conf, word * result, int length, int* ncandidate, NgramBase * dict)
{
	int ** pre_node = new int*[length];
	float ** score = new float*[length];
	float max_score = -10000;
	int max_cand = -1;
	if(length<1)
	{
		return -1;
	}
	score[0] = new float[ncandidate[0]];
	//first
	for(int i=0; i<ncandidate[0]; i++)
	{
		if( (score[0][i]=getProb(dict, str[0][i])) > max_score )
		{
			max_cand = i;
			max_score = score[0][i];
		}
	}

	if(1==length)
	{
		result[0]=score[0][max_cand];
		return max_score;
	}

	//rest
	for(int i=1; i<length; i++)
	{
		int ncand = ncandidate[i];
		pre_node[i] = new int [ncand];
		score[i] = new float[ ncand ];
		for(int j=0; j<ncand; j++)
		{
			int max_pre_index=-1;
			float max_this_score=-10000;
			for(int k=0; k< ncandidate[i-1]; k++)
			{
				float score_t = score[i-1][k] + getTransitionProb(dict,str[i-1][k],str[i][j]) + getProb(dict,str[i][j]);
				if(score_t > max_this_score)
				{
					max_this_score = score_t;
					max_pre_index = k;
				}
			}
			score[i][j] = max_this_score;
			pre_node[i][j] = max_pre_index;		
		}
	}
	//find max score
	max_score = -10000;
	max_cand = -1;
	for(int i=0; i<ncandidate[length-1]; i++)
	{
		if( max_score<score[length-1][i] )
		{
			max_cand = i;
			max_score = score[length-1][i];
		}
	}
	result[length-1]=str[length-1][max_cand];
	int cand = max_cand;
	//backtracking
	for(int i = length-2; i>=0; i--)
	{
		cand = pre_node[i+1][cand];
		result[i] = str[i][cand];
	}
	return max_score;
}

#endif

#if 0
static float enum2gram(word ** str, float ** conf, word * result, int length, int* ncandidate, int endindex, hashDict * dict)
{
	float score;
	if(1==length)
	{
		result[0] = str[0][endindex];
		score = getProb(dict, result[0]);
		return score;
	}
	float * score_pre = new float[ncandidate[length-2]];
	word ** result_t = new word*[ncandidate[length-2]];
	int max_pre=0;
	float max_score=-1.0;
	result[length-1] = str[length-1][endindex];
	for(int i=0; i<ncandidate[length-2]; i++)
	{
		result_t[i] = new word[length-1];
		score_pre[i] = getProb(dict, str[length-2][i])+ enum2gram(str,conf,result_t[i],length-1,ncandidate,i,dict);
		float score_t = score_pre[i]+getTransitionProb(dict, str[length-2][i],str[length-1][endindex]);
		if(score_t>max_score)
		{
			max_pre = i;
			max_score = score_t;
		}
	}
	delete [] score_pre;
	memcpy( result,result_t[max_pre],sizeof(word)*(length-1) );
	
	for(int i=0; i<ncandidate[length-2]; i++)
	{
		delete [] result_t[i];
	}
	delete [] result_t;
	cout<<length<<endl;
	return max_score;
}
#endif

static void word2string(word * output, int nchar, string &a)
{
	char cc[255];
	int k = 0;
	for(int i=0; i<nchar; i++)
	{
		//cout<<hex<<output[i]<<" ";
		if(output[i]>127)
		{
			memcpy(cc+k, &output[i], 2);
			k+=2;
		}
		else
		{
			memcpy(cc+k, &output[i], 1);
			k++;
		}
	}
	cc[k]=0;
	a = cc;
}

#if BIGRAMFREQENCY
//bool bigram(word ** str, float ** conf, int nchar, int * ncandidate, word * finalout)
//{
//	//init the hash dictionary
//    string hashFile("C:\\corpus.index");
//    string input("input.txt");
//    string cfd("confidence.txt");
//
//	hashDict dict;
//	if(!dict.loadDict( hashFile.c_str() ) )
//	{
//		cerr<<"cannot load dict"<<endl;
//		return false;
//	}
//
//	//float score = DP2gram(str,conf,output,nchar,ncandidate,0,&dict);
//	float score = dp2gram(str,conf,finalout,nchar,ncandidate,&dict);
//}

bool bigram(word ** str, float ** conf, int nchar, int * ncandidate, word * finalout)
{
	//init the hash dictionary
	string hashFile("C:\\addone_6_26.index");
	//string hashFile("C:\\corpus.index");

	hashDict dict;
	if(!dict.loadDict( hashFile.c_str() ) )
	{
		cerr<<"cannot load dict"<<endl;
		return false;
	}

	//float score = DP2gram(str,conf,output,nchar,ncandidate,0,&dict);
	float score = dp2gram(str,conf,finalout,nchar,ncandidate,&dict);
}

#else
bool bigram(word ** str, float ** conf, int nchar, int * ncandidate, word * finalout)
{
	//string confName("C:\\confPrint_6_29.txt");
	string confName("C:\\corpus.txt");
	NgramBase *ngram = NULL;
	
	//string hashFile("C:\\katzs.index");
	//ngram = new Katz;
	//ngram->parse(confName.c_str());
	//ngram->printConf();
	//ngram->loadModule(hashFile.c_str());

	//string hashFile("C:\\addone_6_29.index");
	string hashFile("C:\\corpus.index");
	ngram = new AddOne;
	ngram->parse(confName.c_str());
	ngram->printConf();
	ngram->loadModule(hashFile.c_str());


	string s1 = "西";
	string s2 = "西 城 区";
	string s3 = "口 城 区";
	double poss1 = 0.0;
	double poss2 = 0.0;
	double poss3 = 0.0;
	poss1 = ngram->getProb(s1);
	poss2 = ngram->getProb(s2);
	poss3 = ngram->getProb(s3);
	//float score = DP2gram(str,conf,output,nchar,ncandidate,0,&dict);
	float score = dp2gram(str,conf,finalout,nchar,ncandidate,ngram);
	
	return 0;
}
#endif