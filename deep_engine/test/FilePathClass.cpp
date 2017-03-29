/********************************************************************
	生成时间:	2013/03/07   11:52
	文件名: 	FilePath.cpp
	@author:	李鹏(hzlipeng@corp.netease.com)
	
	功能:		使用boost库封装的文件路径类
*********************************************************************/
#include "FilePathClass.h"
/*
 ************************************************************************
 * Function: 
 *
 * @param  folder
 * @param  mode
 * @param  ext
 * @return 
 ************************************************************************
*/
std::vector<PathPair> FilePathClass::getFilePath( std::string folder, std::string ext, PATH_TYPE mode /*= NO_RECURSIVE*/ )
{
	using namespace boost::filesystem;
	if (!exists(folder) || !is_directory(folder))
	{
		return std::vector<PathPair>();
	}

	std::vector<PathPair> res;

	if(mode == NO_RECURSIVE)
	{
		boost::filesystem::directory_iterator it(folder),itend;

		if(ext == "")
		{
			for (; it != itend ; ++it)
			{
				path file = it->path();
				if(is_regular_file(file))
					res.push_back(          PathPair(  file.string(),   file.filename().string()   )        );
			}
		}
		else
		{
			const std::string ext_no_caps = boost::algorithm::to_lower_copy(ext);
			const int extLen = ext_no_caps.size();
			for (; it != itend ; ++it)
			{
				path file = it->path();

				const std::string fileString = file.string();
				const std::string fileExt = (fileString.size()>=extLen) ? fileString.substr(fileString.size()-extLen) : ""; 
				
				if (is_regular_file(file) && boost::algorithm::to_lower_copy(fileExt) == ext_no_caps)
					res.push_back(          PathPair(  file.string(),   file.filename().string()   )        );
			}
		}
	}
	else
	{
		boost::filesystem::recursive_directory_iterator it(folder),itend;

		if(ext == "")
		{
			for (; it != itend ; ++it)
			{
				path file = it->path();
				if(is_regular_file(file))
					res.push_back(          PathPair(  file.string(),   file.filename().string()   )        );
			}
		}
		else
		{
			const std::string ext_no_caps = boost::algorithm::to_lower_copy(ext);
			const int extLen = ext_no_caps.size();
			for (; it != itend ; ++it)
			{
				path file = it->path();

				const std::string fileString = file.string();
				const std::string fileExt = (fileString.size()>=extLen) ? fileString.substr(fileString.size()-extLen) : ""; 

				if (is_regular_file(file) && boost::algorithm::to_lower_copy(fileExt) == ext_no_caps)
					res.push_back(          PathPair(  file.string(),   file.filename().string()   )        );
			}
		}
	}

	std::sort(res.begin(), res.end());

	return std::move(res);
}

/*
 ************************************************************************
 * Function: 
 *
 * @param  folder
 * @return 
 ************************************************************************
*/
std::vector<PathPair> FilePathClass::getFolder( std::string &folder )
{
	using namespace boost::filesystem;
	if (!exists(folder) || !is_directory(folder))
	{
		return std::vector<PathPair>();
	}

	std::vector<PathPair> res;

	boost::filesystem::directory_iterator it(folder),itend;
	for (; it != itend ; ++it)
	{
		path file = it->path();
		if(is_directory(file))
			res.push_back(      PathPair(  file.string(),   file.filename().string()   )       );
	}

	std::sort(res.begin(), res.end());
	return std::move(res);
}
