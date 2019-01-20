/*****************************************************************
 * 文件名: FixedBuffer.cpp
 * 功能: 缓冲区类的实现
 *****************************************************************/
#include "stdafx.h"
#include "FixedBuffer.h"

CFixedBuffer::CFixedBuffer()
	: m_pBuffer(NULL) , m_lWritePos(0) ,
	m_lReadPos(0) , m_lMaxBufferSize(0)
{

}

CFixedBuffer::~CFixedBuffer()
{
	if( m_pBuffer )
	{
		delete []m_pBuffer;
		m_pBuffer = NULL;
	}
}

void CFixedBuffer::Init(unsigned long lOriginSize)
{
	m_lMaxBufferSize = lOriginSize;
	m_pBuffer = new unsigned char[lOriginSize];
}

void CFixedBuffer::Reset()
{
	m_lWritePos = 0;
	m_lReadPos = 0;
}

unsigned long CFixedBuffer::PreRead(unsigned char* szBuf , unsigned long lLen)
{
	unsigned long nCurCount = Count();

	if( lLen >= nCurCount )
		lLen = nCurCount;

	if( lLen == 0 ) 
	{
		return 0;
	}

	unsigned long pos = Position(m_lReadPos);
	unsigned long distance = m_lMaxBufferSize - pos;

	if( distance >= lLen )
	{
		memcpy(szBuf , &m_pBuffer[pos] , lLen);
	}
	else
	{
		memcpy(szBuf , &m_pBuffer[pos] , distance);
		memcpy(&szBuf[distance] , m_pBuffer , lLen - distance);
	}

	return lLen;
}

unsigned long CFixedBuffer::Read(unsigned char* szBuf , unsigned long lLen)
{
	unsigned long nCurCount = Count();

	if( lLen >= nCurCount )
		lLen = nCurCount;

	if( lLen == 0 ) 
	{
		return 0;
	}
	
	unsigned long pos = Position(m_lReadPos);
	unsigned long distance = m_lMaxBufferSize - pos;

	if( distance >= lLen )
	{
		memcpy(szBuf , &m_pBuffer[pos] , lLen);
	}
	else
	{
		memcpy(szBuf , &m_pBuffer[pos] , distance);
		memcpy(&szBuf[distance] , m_pBuffer , lLen - distance);
	}

	m_lReadPos += lLen;

	return lLen;
}

unsigned long CFixedBuffer::MoveReadPos(unsigned long nLen)
{
	m_lReadPos += nLen;

	return m_lReadPos;
}

unsigned long CFixedBuffer::GetWritePosition()
{
	return Position(m_lWritePos);
}

long CFixedBuffer::Write(const unsigned char* szBuf , unsigned long lLen , bool bIgnoreRead)
{
	if( bIgnoreRead )
	{
		if( lLen > 0 && lLen < m_lMaxBufferSize )
		{
			unsigned long pos = Position(m_lWritePos);
			unsigned long distance = m_lMaxBufferSize - pos;

			if( distance >= lLen )
			{
				memcpy(&m_pBuffer[pos] , szBuf , lLen);
			}
			else
			{
				memcpy(&m_pBuffer[pos],szBuf,distance);
				memcpy(m_pBuffer,&szBuf[distance],lLen-distance);
			}
			m_lWritePos += lLen;
			return lLen;
		}
	}
	else
	{
		if( lLen > 0 && lLen < m_lMaxBufferSize )
		{
			if(((m_lMaxBufferSize + m_lReadPos) >= (m_lWritePos + lLen))
				||(m_lWritePos == 0 && m_lReadPos == 0))
			{
				unsigned long pos = Position(m_lWritePos);
				unsigned long distance = m_lMaxBufferSize - pos;

				if( distance >= lLen )
				{
					memcpy(&m_pBuffer[pos] , szBuf , lLen);
				}
				else
				{
					memcpy(&m_pBuffer[pos],szBuf,distance);
					memcpy(m_pBuffer,&szBuf[distance],lLen-distance);
				}
				m_lWritePos += lLen;
				return lLen;
			}else{
				return -1;
			}
		}
	}
	return 0;
}

unsigned long CFixedBuffer::Count()
{
	return m_lWritePos - m_lReadPos;
}

inline unsigned long CFixedBuffer::Position(unsigned long pos)
{
	return pos % m_lMaxBufferSize;
}

CCycleBuffer::CCycleBuffer(int size)
{
	m_nBufSize = size;
	m_nReadPos = 0;
	m_nWritePos = 0;
	m_pBuf = new char[m_nBufSize];
	m_bEmpty = TRUE;
	m_bFull = FALSE;
}

CCycleBuffer::~CCycleBuffer()
{
	delete[] m_pBuf;
}




int CCycleBuffer::Write(char* buf, int count)
{
	if(count <= 0) return 0;

	m_bEmpty = FALSE;

	// 缓冲区已满，不能继续写入
	if(m_bFull) 
	{
		return 0;
	}
	else if(m_nReadPos == m_nWritePos) // 缓冲区为空时
	{

		int leftcount = m_nBufSize - m_nWritePos;
		if(leftcount > count)
		{
			memcpy(m_pBuf + m_nWritePos, buf, count);
			m_nWritePos += count;
			m_bFull = (m_nWritePos == m_nReadPos);
			return count;
		}
		else
		{
			memcpy(m_pBuf + m_nWritePos, buf, leftcount);
			m_nWritePos = (m_nReadPos > count - leftcount) ? count - leftcount : m_nWritePos;
			memcpy(m_pBuf, buf + leftcount, m_nWritePos);
			m_bFull = (m_nWritePos == m_nReadPos);
			return leftcount + m_nWritePos;
		}
	} 
	else if(m_nReadPos < m_nWritePos) // 有剩余空间可写入
	{

		// 剩余缓冲区大小(从写入位置到缓冲区尾)
		int leftcount = m_nBufSize - m_nWritePos;  
		if(leftcount > count)   // 有足够的剩余空间存放
		{
			memcpy(m_pBuf + m_nWritePos, buf, count);
			m_nWritePos += count;
			m_bFull = (m_nReadPos == m_nWritePos);
			//ASSERT(m_nReadPos <= m_nBufSize);
			//ASSERT(m_nWritePos <= m_nBufSize);
			return count;
		}
		else       // 剩余空间不足
		{
			// 先填充满剩余空间，再回头找空间存放
			memcpy(m_pBuf + m_nWritePos, buf, leftcount);
			int m_nWritePos = (m_nReadPos >= count - leftcount) ? count - leftcount : m_nReadPos;
			memcpy(m_pBuf, buf + leftcount, m_nWritePos);
			m_bFull = (m_nReadPos == m_nWritePos);   
			//ASSERT(m_nReadPos <= m_nBufSize);
			//ASSERT(m_nWritePos <= m_nBufSize);
			return leftcount + m_nWritePos;
		}
	}
	else
	{

		int leftcount = m_nReadPos - m_nWritePos;
		if(leftcount > count)
		{
			// 有足够的剩余空间存放
			memcpy(m_pBuf + m_nWritePos, buf, count);
			m_nWritePos += count;
			m_bFull = (m_nReadPos == m_nWritePos);
			//ASSERT(m_nReadPos <= m_nBufSize);
			//ASSERT(m_nWritePos <= m_nBufSize);
			return count;
		}
		else
		{
			// 剩余空间不足时要丢弃后面的数据
			memcpy(m_pBuf + m_nWritePos, buf, leftcount);
			m_nWritePos += leftcount;
			m_bFull = (m_nReadPos == m_nWritePos);
			//ASSERT(m_bFull);
			//ASSERT(m_nReadPos <= m_nBufSize);
			//ASSERT(m_nWritePos <= m_nBufSize);
			return leftcount;
		}
	}
}




int CCycleBuffer::Read(char* buf, int count)
{
	if(count <= 0) return 0;

	m_bFull = FALSE;

	if(m_bEmpty)       // 缓冲区空，不能继续读取数据
	{
		return 0;
	}
	else if(m_nReadPos == m_nWritePos)   // 缓冲区满时
	{

		int leftcount = m_nBufSize - m_nReadPos;
		if(leftcount > count)
		{
			memcpy(buf, m_pBuf + m_nReadPos, count);
			m_nReadPos += count;
			m_bEmpty = (m_nReadPos == m_nWritePos);
			return count;
		}
		else
		{
			memcpy(buf, m_pBuf + m_nReadPos, leftcount);
			m_nReadPos = (m_nWritePos > count - leftcount) ? count - leftcount : m_nWritePos;
			memcpy(buf + leftcount, m_pBuf, m_nReadPos);
			m_bEmpty = (m_nReadPos == m_nWritePos);
			return leftcount + m_nReadPos;
		}
	}
	else if(m_nReadPos < m_nWritePos)   // 写指针在前(未读数据是连接的)
	{

		int leftcount = m_nWritePos - m_nReadPos;
		int c = (leftcount > count) ? count : leftcount;
		memcpy(buf, m_pBuf + m_nReadPos, c);
		m_nReadPos += c;
		m_bEmpty = (m_nReadPos == m_nWritePos);
		//ASSERT(m_nReadPos <= m_nBufSize);
		//ASSERT(m_nWritePos <= m_nBufSize);
		return c;
	}
	else          // 读指针在前(未读数据可能是不连接的)
	{

		int leftcount = m_nBufSize - m_nReadPos;
		if(leftcount > count)   // 未读缓冲区够大，直接读取数据
		{
			memcpy(buf, m_pBuf + m_nReadPos, count);
			m_nReadPos += count;
			m_bEmpty = (m_nReadPos == m_nWritePos);
			//ASSERT(m_nReadPos <= m_nBufSize);
			//ASSERT(m_nWritePos <= m_nBufSize);
			return count;
		}
		else       // 未读缓冲区不足，需回到缓冲区头开始读
		{
			memcpy(buf, m_pBuf + m_nReadPos, leftcount);
			m_nReadPos = (m_nWritePos >= count - leftcount) ? count - leftcount : m_nWritePos;
			memcpy(buf, m_pBuf, m_nReadPos);
			m_bEmpty = (m_nReadPos == m_nWritePos);
			//ASSERT(m_nReadPos <= m_nBufSize);
			//ASSERT(m_nWritePos <= m_nBufSize);
			return leftcount + m_nReadPos;
		}  
	}
}


int CCycleBuffer::PreRead(char* buf, int count)
{
	if(count <= 0) return 0;

	int nNewReadPos = 0;

	m_bFull = FALSE;

	if(m_bEmpty)       // 缓冲区空，不能继续读取数据
	{
		return 0;
	}
	else if(m_nReadPos == m_nWritePos)   // 缓冲区满时
	{
		printf("buffer full %ld , %ld\n" , m_nReadPos , m_nWritePos);
		int leftcount = m_nBufSize - m_nReadPos;
		if(leftcount > count)
		{
			memcpy(buf, m_pBuf + m_nReadPos, count);
			//m_nReadPos += count;
			nNewReadPos = m_nReadPos + count;
			m_bEmpty = (nNewReadPos == m_nWritePos);
			return count;
		}
		else
		{
			memcpy(buf, m_pBuf + m_nReadPos, leftcount);
			//m_nReadPos = (m_nWritePos > count - leftcount) ? count - leftcount : m_nWritePos;
			nNewReadPos = (m_nWritePos > count - leftcount) ? count - leftcount : m_nWritePos;
			memcpy(buf + leftcount, m_pBuf, nNewReadPos);
			m_bEmpty = (nNewReadPos == m_nWritePos);
			return leftcount + nNewReadPos;
		}
	}
	else if(m_nReadPos < m_nWritePos)   // 写指针在前(未读数据是连接的)
	{

		int leftcount = m_nWritePos - m_nReadPos;
		int c = (leftcount > count) ? count : leftcount;
		memcpy(buf, m_pBuf + m_nReadPos, c);
		//m_nReadPos += c;
		nNewReadPos += c;
		m_bEmpty = (nNewReadPos == m_nWritePos);
		//ASSERT(nNewReadPos <= m_nBufSize);
		//ASSERT(m_nWritePos <= m_nBufSize);
		return c;
	}
	else          // 读指针在前(未读数据可能是不连接的)
	{

		int leftcount = m_nBufSize - m_nReadPos;
		if(leftcount > count)   // 未读缓冲区够大，直接读取数据
		{
			memcpy(buf, m_pBuf + m_nReadPos, count);
			//m_nReadPos += count;
			nNewReadPos += count;
			m_bEmpty = (nNewReadPos == m_nWritePos);
			//ASSERT(nNewReadPos <= m_nBufSize);
			//ASSERT(m_nWritePos <= m_nBufSize);
			return count;
		}
		else       // 未读缓冲区不足，需回到缓冲区头开始读
		{
			memcpy(buf, m_pBuf + m_nReadPos, leftcount);
			//m_nReadPos = (m_nWritePos >= count - leftcount) ? count - leftcount : m_nWritePos;
			nNewReadPos = (m_nWritePos >= count - leftcount) ? count - leftcount : m_nWritePos;
			memcpy(buf, m_pBuf, nNewReadPos);
			m_bEmpty = (nNewReadPos == m_nWritePos);
			//ASSERT(nNewReadPos <= m_nBufSize);
			//ASSERT(m_nWritePos <= m_nBufSize);
			return leftcount + nNewReadPos;
		}  
	}
}

int CCycleBuffer::GetLength()
{
	if(m_bEmpty)
	{
		return 0;
	}
	else if(m_bFull)
	{
		return m_nBufSize;
	}
	else if(m_nReadPos < m_nWritePos)
	{
		return m_nWritePos - m_nReadPos;
	}
	else
	{
		return m_nBufSize - m_nReadPos + m_nWritePos;
	}
}

void CCycleBuffer::Empty()
{
	m_nReadPos = 0;
	m_nWritePos = 0;
	m_bEmpty = TRUE;
	m_bFull = FALSE;
}

BOOL CCycleBuffer::isEmpty()
{
	return m_bEmpty;
}

BOOL CCycleBuffer::isFull()
{
	return m_bFull;
}
