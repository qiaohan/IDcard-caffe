//==========================================================================
// Name         : hashDict.h
// Author       : niuruiyao@meituan.com
// Organization : tne.nlpml@meituan.com
// Version      : v1.0
// Description  : class hashDict gives an interface to build(add) and
					//load a hashDict.
                    //When a hashDict is built, it allows the user to judge
                    //if a key is in this hashDict and get value of the key.
//==========================================================================
#pragma once
#ifdef NGRAMLIBDLL
#define HASHDICTAPI _declspec(dllexport)
#else
#define HASHDICTAPI  _declspec(dllimport)
#endif


#ifndef HASHDICT_H
#define HASHDICT_H

#include "pthread.h"
#include <stdint.h>
#include <map>
#include <sys/types.h>
#include "MMapFile.h"

namespace NLP
{
	class CMMapFile;
	class CChainHashtable;

	typedef std::map<std::string, CMMapFile *> MAPFILEMAP;
	typedef std::map<std::string, CMMapFile *>::iterator MAPFILEMAPIT;

	class  HASHDICTAPI hashDict
	{
	public:
		hashDict();
		virtual ~hashDict();

		bool ifLoaded() {return _bIfLoaded;};

		/**
		 * Load the hashDict from a file
		 * @param filename: filename of hashDict
		 * @return false if load hashDict failed, else return true
		*/
		bool loadDict(const char* filename);

		/**
		 * Match a key in the hashDict
		 * @param key: key to be matched
		 * @param value: value of key
		 * @return false if the key is not exist, else return true
		*/
		bool match(std::string& key, std::string& value);
		bool match(std::string& key);

		//Release file map and write hashDict from memory into disk.
		static void release();
		//Initialize a hashDict when it is been built.
		bool initHash(const char *mapFile, unsigned int hashSize,
				unsigned int fileSize);
		//Add <key,value> into hashDict to build it.
		int8_t add(const std::string &key, std::string &value);

	private:
		//about MMap
		static MAPFILEMAP m_mapFileMap;
		CMMapFile *m_pMmapFile;
		static pthread_mutex_t m_mutex;

		char *m_base;//pointer to the beginning of a hashDict
		CChainHashtable *m_hash;//mount m_base to CChainHashtable
		bool _bIfLoaded;
	};
}

#endif // HASHDICT_H
