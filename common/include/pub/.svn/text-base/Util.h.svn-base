/*****************************************************************
 * 文件名: Util.h
 * 功能: 公共工具类的定义
 *****************************************************************/
#pragma once

#include <list>
#include <vector>
#include <string>

using namespace std;

/****************************************************************
 * 结构体名 : FileTime
 * 说明 : 文件时间参数
 ****************************************************************/
typedef struct tagFileTime
{
	std::string szCreateTime;
	std::string szAccessTime;
	std::string szModifyTime;
} FileTime , *PFileTime;

/****************************************************************
 * 类名 : CUtil
 * 父类 : 无
 * 说明 : 公共工具类
 ****************************************************************/
class CUtil
{
public:

	static void StrToList(const char *src, const char* separateChar,std::list<std::string>& lsDest);	/* 以指定的分隔符把一个字符串分割成一个字符串列表 */
	static void StrToVector(const char *src, const char* separateChar,std::vector<std::string>& vecDest);	/* 以指定的分隔符把一个字符串分割成一个字符串数组 */
	static bool StrIsDigit(const char* src);	/* 把一个字符串转换成数字 */
	static bool IsInteger(const char* src);	/* 判断一个指定的字符串是否是整型 */
	static bool IsFloat(const char* src);	/* 判断一个指定的字符串是否是浮点类型 */
	static bool IsDouble(const char* src);	/* 判断一个指定的字符串是否是双精度类型 */
	static void GetLocalIp(char* szIpAddr);
	static bool IsHexChar(const char ch);

	static bool CreateDir(const char* szDir);
	static bool DeleteDir(const char* szDir);
	static bool CreateFile(const char* szFile);
	static bool CreateFile(const char* szFile , const char* szFileLocation , char* szFullPath);
	static bool DeleteFile(const char* szFullPath);
	static bool RenameFile(const char* szOldFile , const char* szNewFile);
	static bool FileIsExist(const char* szFullPath);
	static unsigned long GetFileSize(const char *szFile);
	static bool IsIpAddr(const char* szSrc);
	static bool IsSourceFile(const char* szFile);	/* 判断一个文件是否是C源程序文件 */
	static bool IsHeaderFile(const char* szFile);	/* 判断一个文件是否是C头文件 */
	static void GetTimeStr(int type, char *szBuf, unsigned long times);
};

