/******************************************************************************
 *
 * File     :MMapFile.h
 * Version  :$Id: MMapFile.h,v 1.9 2005/09/19 09:23:39 kwq Exp $
 * Desc     :硬盘文件影射文件类及一些对硬盘文件影射的一些操作
 * Log		:Created by victor, 2004-12-24
 *
 ******************************************************************************/
#ifndef MMAPFILE_H
#define MMAPFILE_H

//#include "AFX.h"
#include <iostream>
#include <assert.h>
#ifdef _WIN32
#	include <windows.h>
#else
#	include <sys/mman.h>
#	include <unistd.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <fcntl.h>
#endif

#define FILE_OPEN_READ      1
#define FILE_OPEN_WRITE     2
#define FILE_OPEN_APPEND    4
#define FILE_OPEN_UPDATE    5

#define FILE_TYPE_MEMREAD   1
#define FILE_TYPE_SHM       2
#define FILE_TYPE_MMAP      3
#define FILE_TYPE_FS        4
#define FILE_TYPE_BFS       5

namespace NLP
{
	class CMMapFile
{
public:
	//构造函数
	CMMapFile();
	//析构函数
	virtual ~CMMapFile();
	/*
	 * 打开文件
	 * 
	 */ 
	bool open(const char *szFileName, int flag = FILE_OPEN_READ, size_t length = 0);
	/*
	 * 重新映射文件
	 */
	bool reopen(off_t nOffset, size_t nLength);
	/* 
	 * 关闭内存映射文件
	 * 说明：
	 *		把内存映射文件保存到硬盘文件。
	 *		释放和该此操作有关的一切数据和资源。
	 */
	bool close();
	/* 把偏移量转换为地址 */
	inline char *offset2Addr(size_t nOffset) const
	{
		assert(m_pBase!=NULL);
		return (m_pBase + nOffset);
	}
	/* 返回文件大小 */
	size_t getSize()
	{
		return m_nLength;
	}
	/* 内存映像文件内容被改变后,调用此函数可以同步数据 */
	bool flush();
	// 设置文件位置
	bool setPosition(size_t pos, int whence = SEEK_SET);
	// 得到文件当前位置
	size_t getPosition();
	// 读文件, 返回是读到字节数
	size_t read(void *src, size_t len);
	// 写文件, 返回是写的字节数
	size_t write(void *src, size_t len);
    int getFileType() { return FILE_TYPE_MMAP; }
	char *getFileName() const {
		return m_szFileName;
	}
private:
	/* 
	* 将某一个文件映射到内存,bReadOnly指明被打开的文件是否只用于读,否则可以进行读写
	* 打开文件成功返回映射的内存基地址，失败返回NULL。
	*/
	bool openInternal(const char* pszName, bool bReadOnly = false);
	/*  
	* 创建一个指定大小的内存映像文件,所有的数据都保存在内存中,并和硬盘上的数据相映射
	* 返回：
	*		映射的内存基地址，失败返回NULL。
	*/
	bool create(const char* pszName, size_t nLength);

private:
	size_t m_nLength;				//文件大小
	size_t m_nPos;					//位置
	char* m_pBase;					//数据基指针

	struct SFileHandler				//实现类声明
	{
#ifdef _WIN32
		HANDLE m_hFile;
		HANDLE m_hMapFile;
#else
		int m_fd;		//文件描述符
#endif
	};
	struct SFileHandler m_pHander;		    //实现对象指针
	char *m_szFileName;
};
}



#endif //MMAPFILE_H
