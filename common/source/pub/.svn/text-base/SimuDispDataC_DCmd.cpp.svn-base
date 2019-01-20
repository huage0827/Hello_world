/*****************************************************************
 * 文件名: SimuDispDataC_DCmd.h
 * 功能: SimuService,SimuGuard与SimuMonitor之间通信命令实现
 *****************************************************************/
#include "stdafx.h"
#include "SimuDispDataC_DCmd.h"
#include "Util.h"
#include <stdio.h>
#include <string.h>

using namespace std;

CSimuDispDataC_DCmd::CSimuDispDataC_DCmd(DispDataC_D_CMD sgc)
{
	m_sgc = sgc;
	m_nCmd = sgc;
	m_nFrameBodyLen = 0;
	m_szSign[0] = '!';
	m_szSign[1] = '!';
	
	m_usModelID = 0;
	m_nTimeStamp = 0;
}

CSimuDispDataC_DCmd::~CSimuDispDataC_DCmd()
{
}

DispDataC_D_CMD CSimuDispDataC_DCmd::PreParseCmd(const unsigned char* szBuffer , unsigned long nBufferLen , unsigned long& nCmdDataLen)
{
	DispDataC_D_CMD sgc = DDATA_C_D_CMD_UNKNOWN;

	if( nBufferLen < DDATA_C_D_CMD_HEADER_SIZE )
		return DDATA_C_D_CMD_UNKNOWN;

	memcpy((void*)&nCmdDataLen , szBuffer + DDATA_C_D_CMD_POS_FRAMEBODYLEN , DDATA_C_D_CMD_SIZE_FRAMEBODYLEN);
	nCmdDataLen += DDATA_C_D_CMD_HEADER_SIZE;

	if( nBufferLen < nCmdDataLen )
		return DDATA_C_D_CMD_UNKNOWN;

	memcpy((void*)&sgc , szBuffer + DDATA_C_D_CMD_POS_COMMAND , DDATA_C_D_CMD_SIZE_COMMAND);

	return sgc;
}

DispDataC_D_CMD CSimuDispDataC_DCmd::GetCmdType() const
{
	return m_sgc;
}

unsigned long CSimuDispDataC_DCmd::GetFrameBodyLen()
{
	return m_nFrameBodyLen;
}

unsigned long CSimuDispDataC_DCmd::Serialize(unsigned char* szBuffer)
{
	m_nFrameBodyLen = CalcFrameBodyLen();

	memcpy(szBuffer , m_szSign , DDATA_C_D_CMD_SIZE_SIGN);
	memcpy(szBuffer + DDATA_C_D_CMD_POS_MODELID , (void*)&m_usModelID , DDATA_C_D_CMD_SIZE_MODELID);
	memcpy(szBuffer + DDATA_C_D_CMD_POS_COMMAND , (void*)&m_nCmd , DDATA_C_D_CMD_SIZE_COMMAND);
	memcpy(szBuffer + DDATA_C_D_CMD_POS_TIMESTAMP , (void*)&m_nTimeStamp , DDATA_C_D_CMD_SIZE_TIMESTAMP);
	memcpy(szBuffer + DDATA_C_D_CMD_POS_FRAMEBODYLEN , &m_nFrameBodyLen , DDATA_C_D_CMD_SIZE_FRAMEBODYLEN);
	
	return DDATA_C_D_CMD_HEADER_SIZE;
}

BOOL CSimuDispDataC_DCmd::Unserialize(const unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( nBufferLen < DDATA_C_D_CMD_HEADER_SIZE ){
		return FALSE;
	}

	memcpy(m_szSign ,	szBuffer + DDATA_C_D_CMD_POS_SIGN , DDATA_C_D_CMD_SIZE_SIGN);
	memcpy(&m_usModelID , szBuffer + DDATA_C_D_CMD_POS_MODELID , DDATA_C_D_CMD_SIZE_MODELID);
	memcpy(&m_nCmd ,	  szBuffer + DDATA_C_D_CMD_POS_COMMAND , DDATA_C_D_CMD_SIZE_COMMAND);
	memcpy(&m_nTimeStamp , szBuffer + DDATA_C_D_CMD_POS_TIMESTAMP , DDATA_C_D_CMD_SIZE_TIMESTAMP);
	memcpy(&m_nFrameBodyLen , szBuffer + DDATA_C_D_CMD_POS_FRAMEBODYLEN , DDATA_C_D_CMD_SIZE_FRAMEBODYLEN);

	return TRUE;
}

/* 设置模型ID */
void CSimuDispDataC_DCmd::SetModelID(unsigned short id)
{
	m_usModelID = id;
}

/* 获取模型ID */
unsigned short CSimuDispDataC_DCmd::GetModelID()
{
	return m_usModelID;
}

void CSimuDispDataC_DCmd::SetTimeStamp(unsigned long ts)
{
	m_nTimeStamp = ts;
}

unsigned long CSimuDispDataC_DCmd::GetTimeStamp()
{
	return m_nTimeStamp;
}

void CSimuDispDataC_DCmd::Print(char* szBuffer)
{
	char szTmp[256];
//	char szStr[128];

	switch( m_nCmd )
	{
	case DDATA_C_D_CMD_UNKNOWN:
		sprintf(szTmp , "Cmd: DDATA_C_D_CMD_UNKNOWN");break;
	case DDATA_C_D_CMD_DISP_SLPRINT:
		sprintf(szTmp , "Cmd: DDATA_C_D_CMD_DISP_SLPRINT");break;
	case DDATA_C_D_CMD_DISP_SIMUDATA:
		sprintf(szTmp , "Cmd: DDATA_C_D_CMD_DISP_SIMUDATA");break;
	case DDATA_C_D_CMD_DISP_SIMUSTATUS:
		sprintf(szTmp , "Cmd: DDATA_C_D_CMD_DISP_SIMUSTATUS");break;
	default :
		break;
	}
	strcat(szBuffer, szTmp);

	sprintf(szTmp , ",FrameBodyLen:%ld" , m_nFrameBodyLen);
	strcat(szBuffer, szTmp);

	sprintf(szTmp ,",modelId: %ld, timestamp:%d\n", m_usModelID, m_nTimeStamp);
	strcat(szBuffer , szTmp);

	return ;
}

/****************************************************************
 * 类名 : CSimuDispDataC_DSLPrintCmd
 * 父类 : CSimuDispDataC_DCmd
 * 说明 : SLPrint打印数据
 ****************************************************************/
CSimuDispDataC_DSLPrintCmd::CSimuDispDataC_DSLPrintCmd()
	: CSimuDispDataC_DCmd(DDATA_C_D_CMD_DISP_SLPRINT)
{
	memset(m_szData, 0x0, sizeof(m_szData));
}

CSimuDispDataC_DSLPrintCmd::~CSimuDispDataC_DSLPrintCmd()
{
}

unsigned long CSimuDispDataC_DSLPrintCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuDispDataC_DCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_nStep, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_nDataLen, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, m_szData, m_nDataLen);
	nLen += m_nDataLen;

	return nLen;
}

BOOL CSimuDispDataC_DSLPrintCmd::Unserialize(const unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuDispDataC_DCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameBodyLen + DDATA_C_D_CMD_HEADER_SIZE )
		return FALSE;

	int nBodyPos = DDATA_C_D_CMD_HEADER_SIZE;

	memcpy(&m_nStep, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_nDataLen, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(m_szData, szBuffer + nBodyPos, m_nDataLen);
	nBodyPos += m_nDataLen;

	return TRUE;
}

void CSimuDispDataC_DSLPrintCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuDispDataC_DCmd::Print(szBuffer);

	
	sprintf(szTmp, ",step:%ld, dataLen:%ld, data:%s \n", m_nStep, m_nDataLen, m_szData);
	strcat(szBuffer ,szTmp);
}

unsigned long CSimuDispDataC_DSLPrintCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen = 4 + 4 + m_nDataLen;
	return nLen;
}

void CSimuDispDataC_DSLPrintCmd::SetStep(unsigned long nStep)
{
	m_nStep = nStep;
}

unsigned long CSimuDispDataC_DSLPrintCmd::GetStep()
{
	return m_nStep;
}

void CSimuDispDataC_DSLPrintCmd::SetData(const unsigned char *szData, unsigned long nDataLen)
{
	memcpy(m_szData, szData, nDataLen);
	m_nDataLen = nDataLen;
}

unsigned long CSimuDispDataC_DSLPrintCmd::GetData(unsigned char *szBuff, unsigned long nBuffLen)
{
	memset(szBuff, 0x0, nBuffLen);
	memcpy(szBuff , m_szData, m_nDataLen);
	return m_nDataLen;
}
unsigned long CSimuDispDataC_DSLPrintCmd::GetDataLen()
{
	return m_nDataLen;
}
/****************************************************************
 * 类名 : CSimuDispDataC_DSimuDataCmd
 * 父类 : CSimuDispDataC_DCmd
 * 说明 : 显示的仿真模型数据
 ****************************************************************/
CSimuDispDataC_DSimuDataCmd::CSimuDispDataC_DSimuDataCmd()
	: CSimuDispDataC_DCmd(DDATA_C_D_CMD_DISP_SIMUDATA)
{
	memset(m_szData, 0x0, sizeof(m_szData));
}

CSimuDispDataC_DSimuDataCmd::~CSimuDispDataC_DSimuDataCmd()
{
}

unsigned long CSimuDispDataC_DSimuDataCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuDispDataC_DCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_nStep, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_nDataLen, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, m_szData, m_nDataLen);
	nLen += m_nDataLen;

	return nLen;
}

BOOL CSimuDispDataC_DSimuDataCmd::Unserialize(const unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuDispDataC_DCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameBodyLen + DDATA_C_D_CMD_HEADER_SIZE )
		return FALSE;

	int nBodyPos = DDATA_C_D_CMD_HEADER_SIZE;

	memcpy(&m_nStep, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_nDataLen, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(m_szData, szBuffer + nBodyPos, m_nDataLen);
	nBodyPos += m_nDataLen;

	return TRUE;
}

void CSimuDispDataC_DSimuDataCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuDispDataC_DCmd::Print(szBuffer);

	sprintf(szTmp, ",step:%ld, dataLen:%ld\n", m_nStep, m_nDataLen);
	strcat(szBuffer , szTmp);
}

unsigned long CSimuDispDataC_DSimuDataCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen = 4 + 4 + m_nDataLen;
	return nLen;
}

void CSimuDispDataC_DSimuDataCmd::SetStep(unsigned long nStep)
{
	m_nStep = nStep;
}

unsigned long CSimuDispDataC_DSimuDataCmd::GetStep()
{
	return m_nStep;
}

void CSimuDispDataC_DSimuDataCmd::SetData(const unsigned char *szData, unsigned long nDataLen)
{
	memcpy(m_szData, szData, nDataLen);
	m_nDataLen = nDataLen;
}

unsigned long CSimuDispDataC_DSimuDataCmd::GetData(unsigned char *szBuff, unsigned long nBuffLen)
{
	memset(szBuff, 0x0, nBuffLen);
	memcpy(szBuff , m_szData, m_nDataLen);
	return m_nDataLen;
}
unsigned long CSimuDispDataC_DSimuDataCmd::GetDataLen()
{
	return m_nDataLen;
}

unsigned char *CSimuDispDataC_DSimuDataCmd::GetData()
{
	return m_szData;
}
/****************************************************************
 * 类名 : CSimuDispDataC_DSimuStatusCmd
 * 父类 : CSimuDispDataC_DCmd
 * 说明 : 显示的仿真状态
 ****************************************************************/
CSimuDispDataC_DSimuStatusCmd::CSimuDispDataC_DSimuStatusCmd()
	: CSimuDispDataC_DCmd(DDATA_C_D_CMD_DISP_SIMUSTATUS)
{
	m_nStep = 0;
}

CSimuDispDataC_DSimuStatusCmd::~CSimuDispDataC_DSimuStatusCmd()
{
}

unsigned long CSimuDispDataC_DSimuStatusCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuDispDataC_DCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_nStep, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_nOverRun, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_ExecTime, 8);
	nLen += 8;

	return nLen;
}

BOOL CSimuDispDataC_DSimuStatusCmd::Unserialize(const unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuDispDataC_DCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameBodyLen + DDATA_C_D_CMD_HEADER_SIZE )
		return FALSE;

	int nBodyPos = DDATA_C_D_CMD_HEADER_SIZE;

	memcpy(&m_nStep, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_nOverRun, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_ExecTime, szBuffer + nBodyPos, 8);
	nBodyPos += 8;

	return TRUE;
}

void CSimuDispDataC_DSimuStatusCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuDispDataC_DCmd::Print(szBuffer);

	sprintf(szTmp, ",step:%ld, overRun:%ld, execTime:%lf\n", m_nStep, m_nOverRun, m_ExecTime);
	strcat(szBuffer , szTmp);
}

unsigned long CSimuDispDataC_DSimuStatusCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen = 4 + 4 + 8;
	return nLen;
}

void CSimuDispDataC_DSimuStatusCmd::SetData(unsigned long nStep, unsigned long nOverRun, double execTime)
{
	m_nStep = nStep;
	m_ExecTime = execTime;
	m_nOverRun = nOverRun;
}

unsigned long CSimuDispDataC_DSimuStatusCmd::GetStep()
{
	return m_nStep;
}

unsigned long CSimuDispDataC_DSimuStatusCmd::GetOverRunCount()
{
	return m_nOverRun;
}

double CSimuDispDataC_DSimuStatusCmd::GetExecTime()
{
	return m_ExecTime;
}
