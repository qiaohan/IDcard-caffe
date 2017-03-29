#ifndef _FILEPATH_H_
#define _FILEPATH_H_

enum PATH_TYPE{
	NO_RECURSIVE,
	RECURSIVE,
};


#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

struct PathPair{
	PathPair(const std::string &fullPathIn, const std::string &NameIn)
		:fullPath(fullPathIn)
		,Name(NameIn)
	{

	}
	std::string fullPath;
	std::string Name;
};

inline bool operator<(const PathPair &lhs, const PathPair &rhs)
{
	return lhs.fullPath < rhs.fullPath;
}

class FilePathClass{
public:
	std::vector<PathPair> getFilePath(const std::string &folder, const std::string &ext, PATH_TYPE mode = NO_RECURSIVE) const;
	std::vector<PathPair> getFolder( const std::string &folder ) const;
private:

};


#endif // _FILEPATH_H_