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
	static std::vector<PathPair> getFilePath( std::string folder, std::string ext, PATH_TYPE mode = NO_RECURSIVE);
	static std::vector<PathPair> getFolder( std::string &folder );
private:
};
#endif // _FILEPATH_H_
