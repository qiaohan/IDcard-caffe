#pragma once
#ifdef NGRAMLIBDLL
#define NGRAMAPI _declspec(dllexport)
#else
#define NGRAMAPI  _declspec(dllimport)
#endif


#ifndef NGRAM_H
#define NGRAM_H

#include <string>
#include <vector>
#include "hashDict.h"

using namespace std;

namespace NLP
{
    class NGRAMAPI NgramBase
    {
        public:
            NgramBase();
            virtual ~NgramBase();

            bool loadModule(const char* filename);
            virtual bool parse(const char* filename) = 0;
            virtual void printConf() = 0;
            virtual double getProb(string &s) = 0;
            virtual void setBareFactor(unsigned int *factors);

        protected:
            unsigned int getSpaceIdx(string &s, vector<int> &vecIdx);
            unsigned int corpusN;
            unsigned int gramN;
            NLP::hashDict module;
    };

    class NGRAMAPI AddOne: public NgramBase
    {
        public:
            AddOne();
            virtual ~AddOne();
            bool parse(const char* filename);
            void printConf();
            double getProb(string &s);

        private:
            unsigned int corpusV;
    };

    class NGRAMAPI Katz : public NgramBase
    {
        public:
            Katz();
			virtual ~Katz();
            bool parse(const char* filename);
            void printConf();
            double getProb(string &s);

        private:
            double getKatzProb(string &gram);
            unsigned int katz_k;
    };

    class NGRAMAPI BareBackoff : public NgramBase
    {
        public:
            BareBackoff();
            virtual ~BareBackoff();
            bool parse(const char* filename);
            void printConf();
            void setBareFactor(unsigned int *factors);
            double getProb(string &s);

        private:
            double getBareBackoffProb(string &gram);
            unsigned int *bare;
    };
}
#endif //NGRAM_H
