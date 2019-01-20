/*****************************************************************
 * 文件名: Util.cpp
 * 功能: 公共工具类的定义
 *
 *****************************************************************/
#include "stdafx.h"
#include "Util.h"
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#ifdef QNX_PLATFORM
#include <unistd.h>
#endif


using namespace std;


bool CUtil::IsIpAddr(const char* szSrc)
{
	if( szSrc == NULL || strcmp(szSrc , "") == 0 )
		return false;
	vector<string> vec;
	vec.clear();
	CUtil::StrToVector(szSrc , "." , vec);
	if( vec.size() != 4 )
		return false;
	if( !CUtil::IsInteger(vec[0].c_str()) ||
		!CUtil::IsInteger(vec[1].c_str()) ||
		!CUtil::IsInteger(vec[2].c_str()) ||
		!CUtil::IsInteger(vec[3].c_str()) )
		return false;
	if( atoi(vec[0].c_str()) <= 0 || atoi(vec[0].c_str()) > 255 ||
		atoi(vec[1].c_str()) < 0 || atoi(vec[1].c_str()) > 255 ||
		atoi(vec[2].c_str()) < 0 || atoi(vec[2].c_str()) > 255 ||
		atoi(vec[3].c_str()) < 0 || atoi(vec[3].c_str()) > 255 )
		return false;

	return true;
}


void CUtil::StrToList(const char *src, const char* separateChar,std::list<std::string>& lsDest)
{
	lsDest.clear();
	char* str = new char[strlen(src)+1];
	memset(str , 0 , strlen(src)+1);
	memcpy(str , src , strlen(src));
	char* token = strtok(str, separateChar);
	while( token != NULL )
	{
		lsDest.push_back(token);
		token = strtok( NULL , separateChar);
	}
	delete []str;
}

void CUtil::StrToVector(const char *src, const char* separateChar,std::vector<std::string>& vecDest)
{
	vecDest.clear();
	char* str = new char[strlen(src)+1];
	memset(str , 0 , strlen(src)+1);
	memcpy(str , src , strlen(src));
	char* token = strtok(str, separateChar);
	while( token != NULL )
	{
		vecDest.push_back(token);
		token = strtok(NULL,separateChar);
	}
	delete []str;
}


bool CUtil::StrIsDigit(const char* src)
{
	if( src == NULL || strlen(src) == 0 )
		return false;
	for( size_t i = 0 ; i < strlen(src) ; i++ )
	{
		if( src[i] >= '0' && src[i] <= '9' )
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}


bool CUtil::IsInteger(const char* src)
{
	if( src == NULL || strlen(src) <= 0 )
		return false;

	unsigned int iStart = 0;

	if( src[0] == '+' || src[0] == '-' )
		iStart = 1;

	if( src[iStart] < '1' || src[iStart] > '9' )
	{
		if( src[iStart] == '0' && strlen(src) == (iStart + 1) )
			return true;
		else
			return false;
	}

	for(size_t i = iStart + 1 ; i < strlen(src) ; i++ )
	{
		if( src[i] < '0' || src[i] > '9' )
			return false;
	}

	return true;
}

bool CUtil::IsFloat(const char* src)
{
	int nDotCount = 0;

	if( src == NULL || strlen(src) <= 0 )
		return false;

	unsigned int iStart = 0;

	if( src[0] == '+' || src[0] == '-' )
		iStart = 1;

	if( src[iStart] < '1' || src[iStart] > '9' )
	{
		if( src[iStart] == '0' )
		{
			if( src[iStart] == '0' && strlen(src) == (iStart + 1) )
			{
				return true;
			}
			else
			{
				if( src[iStart + 1] != '.' )
				{
					return false;
				}
			}

		}
		else
		{
			return false;
		}
	}

	for(size_t i = iStart + 1 ; i < strlen(src) ; i++)
	{
		if( !((src[i] >= '0' && src[i] <= '9') || src[i]=='.') )
			return false;

		if( src[i] == '.' )
		{
			nDotCount++;
			if( nDotCount > 1 )
				return false;
		}
	}

	return true;
}

bool CUtil::IsDouble(const char* src)
{
	return IsFloat(src);
}


void CUtil::GetLocalIp(char* szIpAddr)
{
#ifdef WIN_PLATFORM
	hostent*	localHost;
	char*		localIP = NULL;
	u_long		ul = 1;

	localHost = gethostbyname("");
	localIP = inet_ntoa(*(struct in_addr *)*localHost->h_addr_list);
	strcpy(szIpAddr, localIP);
#else

#endif
}

bool CUtil::IsHexChar(const char ch)
{
	if( isdigit(ch))return true;
	if( ch == 'a' || ch == 'b' || ch == 'c' ||
		ch == 'd' || ch == 'e' || ch == 'f')
		return true;
	if( ch == 'A' || ch == 'B' || ch == 'C' ||
		ch == 'D' || ch == 'E' || ch == 'F')
		return true;
	return false;
}


bool CUtil::CreateDir(const char* szDir)
{
#if defined(QNX_PLATFORM) || defined(SIMUMODEL_API)
	return false;
#endif

#ifdef WIN_PLATFORM
	bool bRet = ::CreateDirectory(CA2CT(szDir),NULL);
	if( !bRet){
		if(GetLastError() == ERROR_ALREADY_EXISTS){
			return true;
		}else{
			return false;
		}
	}
	return bRet;
#endif
	return false;
}

bool CUtil::DeleteDir(const char* szDir)
{
#if defined(QNX_PLATFORM) || defined(SIMUMODEL_API)
	return false;
#endif
#ifdef WIN_PLATFORM
	return ::RemoveDirectory(CA2CT(szDir));
#endif
	return false;
}

bool CUtil::CreateFile(const char* szFile)
{
	FILE *fp = NULL;
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	fp = fopen(szFile , "w+");
#else

#ifndef SIMUMODEL_API
	fopen_s(&fp , szFile , "w+");
#endif

#endif	
	if( fp == NULL )
		return false;
	fclose(fp);
	return true;
}

bool CUtil::CreateFile(const char* szFile , const char* szFileLocation , char* szFullPath)
{
	if( szFile == NULL || strlen(szFile) == 0 )
		return false;
	if( szFileLocation == NULL || strlen(szFileLocation) == 0 )
		return false;
	if( szFileLocation[strlen(szFileLocation)-1] == _T('\\') || 
		szFileLocation[strlen(szFileLocation)-1] == _T('/') )
	{
#if defined(QNX_PLATFORM)
		sprintf(szFullPath , "%s%s" , szFileLocation , szFile);
#elif defined(WIN_PLATFORM)

#ifndef SIMUMODEL_API
		sprintf_s(szFullPath , 256 , "%s%s" , szFileLocation , szFile);
#endif
#else

#endif
	}
	else
	{
#if defined(QNX_PLATFORM)
		sprintf(szFullPath , "%s//%s" , szFileLocation , szFile);
#elif defined(WIN_PLATFORM)
#ifndef SIMUMODEL_API
		sprintf_s(szFullPath , 256 , "%s//%s" , szFileLocation , szFile);
#endif
#else

#endif
	}

	return CUtil::CreateFile(szFullPath);
}

bool CUtil::DeleteFile(const char* szFullPath)
{
#ifdef WIN_PLATFORM

#ifndef SIMUMODEL_API
	return ::DeleteFile(CA2CT(szFullPath));
#else
	return false;
#endif

#else
	return false;
#endif
}

bool CUtil::RenameFile(const char* szOldFile , const char* szNewFile)
{
#ifdef WIN_PLATFORM
	return rename(szOldFile , szNewFile);
#else 
	return false;
#endif
}

bool CUtil::FileIsExist(const char* szFullPath)
{
#ifdef WIN_PLATFORM

#ifndef SIMUMODEL_API
	CFileFind   p;
	return p.FindFile(CA2CT(szFullPath));
#else
	return false;
#endif

#else
	return false;
#endif
}

unsigned long CUtil::GetFileSize(const char *szFile)
{
#ifdef WIN_PLATFORM
	WIN32_FIND_DATA fileInfo;
	HANDLE hFind;
	DWORD fileSize = 0;

	hFind = FindFirstFile(szFile, &fileInfo);
	if( hFind != INVALID_HANDLE_VALUE){
		fileSize = fileInfo.nFileSizeLow;
		FindClose(hFind);
	}
	return fileSize;
#else
	return 0;
#endif
}

void CUtil::GetTimeStr(int type, char *szBuf, unsigned long times)
{
#ifdef WIN_PLATFORM

#ifndef SIMUMODEL_API
	time_t totalSec = times;
	struct tm  Tm;
	localtime_s(&Tm, &totalSec);

	if( type == 1){
		sprintf(szBuf,"%04d-%02d-%02d",Tm.tm_year+1900, Tm.tm_mon+1, Tm.tm_mday);
	}else if( type == 2){
		sprintf(szBuf,"%04d-%02d-%02d %02d:%02d:%02d",Tm.tm_year+1900, Tm.tm_mon+1,
			Tm.tm_mday, Tm.tm_hour, Tm.tm_min, Tm.tm_sec);
	}
#endif

#else
	return;
#endif
}

