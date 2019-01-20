/*****************************************************************
 * 文件名: FixedBuffer.h
 * 功能: 缓冲区类
 *****************************************************************/
#pragma once

#include <list>
#include <vector>
#include <string>

using namespace std;

/****************************************************************
 * 类名 : CFixedBuffer
 * 父类 : 无
 * 说明 : 缓冲区类
 ****************************************************************/
class CFixedBuffer
{
public:
	CFixedBuffer();
	virtual ~CFixedBuffer();
	
	void Init(unsigned long lOriginSize);
	void Reset();
	long Write(const unsigned char* szBuf , unsigned long lLen , bool bIgnoreRead = false);
	unsigned long PreRead(unsigned char* szBuf , unsigned long lLen);
	unsigned long Read(unsigned char* szBuf , unsigned long lLen);
	unsigned long MoveReadPos(unsigned long nLen);
	unsigned long Count();
	unsigned long GetWritePosition();
	
private :
	inline unsigned long Position(unsigned long pos);
	
private :
	unsigned char* m_pBuffer;
	unsigned long  m_lWritePos;
	unsigned long  m_lReadPos;
	unsigned long  m_lMaxBufferSize;
};

class CCycleBuffer 
{
public:
	BOOL isFull();
	BOOL isEmpty();
	void Empty();
	int GetLength();
	CCycleBuffer(int size);
	virtual ~CCycleBuffer();
	int Write(char* buf, int count);
	int Read(char* buf, int count);
	int PreRead(char* buf, int count);
private:
	BOOL m_bEmpty, m_bFull;
	char * m_pBuf;
	int m_nBufSize;
	int m_nReadPos;
	int m_nWritePos;
};

