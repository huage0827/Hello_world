/*****************************************************************
 * 文件名: SimuL_TCmd.cpp
 * 功能: SimuLab与SimuGuard 之间通信命令实现
 *****************************************************************/
#include "stdafx.h"
#include "SimuL_TCmd.h"
#include "Util.h"
#include <stdio.h>
#include <string.h>

using namespace std;

#define MAX_PRINT_BUFFER_LEN 8192


CSimuL_TCmd::CSimuL_TCmd(L_T_CMD sgc)
{
	m_sgc = sgc;
	m_nCmd = sgc;
	m_nFrameBodyLen = 0;
}

CSimuL_TCmd::~CSimuL_TCmd()
{

}

L_T_CMD CSimuL_TCmd::PreParseCmd(unsigned char* szBuffer , unsigned long nBufferLen , unsigned long& nCmdDataLen)
{
	L_T_CMD sgc = L_T_CMD_UNKNOWN;

	if( nBufferLen < L_T_CMD_HEADER_SIZE )
		return L_T_CMD_UNKNOWN;

	memcpy((void*)&nCmdDataLen , szBuffer + L_T_CMD_POS_FRAMEBODYLEN , L_T_CMD_SIZE_FRAMEBODYLEN);
	nCmdDataLen += L_T_CMD_HEADER_SIZE;

	if( nBufferLen < nCmdDataLen )
		return L_T_CMD_UNKNOWN;

	memcpy((void*)&sgc , szBuffer + L_T_CMD_POS_COMMAND , L_T_CMD_SIZE_COMMAND);

	return sgc;
}

L_T_CMD CSimuL_TCmd::GetCmdType() const
{
	return m_sgc;
}


unsigned long CSimuL_TCmd::GetFrameBodyLen()
{
	return m_nFrameBodyLen;
}

/*****************************************************************
 *  函数名：CSimuL_TCmd::Serialize
 *  功能：  序列化命令
 *  入参:   无
 *  出参:	unsigned char* szBuffer -- 序列化成功后的数据存放缓冲区
 *  返回值:	序列化成功后的数据存放缓冲区大小
 *****************************************************************/
unsigned long CSimuL_TCmd::Serialize(unsigned char* szBuffer)
{
	m_nFrameBodyLen = CalcFrameBodyLen();

	m_beginSign[0] = '!';
	m_beginSign[1] = '!';

	m_endSign[0] = '$';
	m_endSign[1] = '$';

	memcpy(szBuffer + L_T_CMD_POS_BEGAINSIGN, m_beginSign, L_T_CMD_SIZE_BEGINSIGN);
	memcpy(szBuffer + L_T_CMD_POS_COMMAND , (void*)&m_nCmd , L_T_CMD_SIZE_COMMAND);
	memcpy(szBuffer + L_T_CMD_POS_FRAMEBODYLEN , (void*)&m_nFrameBodyLen , L_T_CMD_SIZE_FRAMEBODYLEN);
	memcpy(szBuffer + L_T_CMD_POS_ENDSIGN, m_endSign, L_T_CMD_SIZE_ENDSIGN);

	return L_T_CMD_HEADER_SIZE;
}

BOOL CSimuL_TCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( nBufferLen < L_T_CMD_HEADER_SIZE ){
		return FALSE;
	}

	memcpy((void*)&m_nCmd , szBuffer + L_T_CMD_POS_COMMAND , L_T_CMD_SIZE_COMMAND);
	memcpy((void*)&m_nFrameBodyLen , szBuffer + L_T_CMD_POS_FRAMEBODYLEN , L_T_CMD_SIZE_FRAMEBODYLEN);

	return TRUE;
}

void CSimuL_TCmd::Print(char* szBuffer)
{
	char szTmp[256];

	switch( m_nCmd )
	{
	case L_T_CMD_UNKNOWN	 :
		sprintf(szTmp , "Cmd: L_S_CMD_UNKNOWN");break;
	case L_T_CMD_TARGET_HEARTBEAT_REQ	 :
		sprintf(szTmp , "Cmd: L_T_CMD_TARGET_HEARTBEAT_REQ");break;
	case L_T_CMD_TARGET_HEARTBEAT_RESP	 :
		sprintf(szTmp , "Cmd: L_T_CMD_TARGET_HEARTBEAT_RESP");break;
	case L_T_CMD_GET_TARGET_INFO_REQ	 :
		sprintf(szTmp , "Cmd: L_T_CMD_GET_TARGET_INFO_REQ");break;
	case L_T_CMD_GET_TARGET_INFO_RESP	 :
		sprintf(szTmp , "Cmd: L_T_CMD_GET_TARGET_INFO_RESP");break;

	default :
		break;
	}
	strcat(szBuffer, szTmp);

	sprintf(szTmp , ", FrameBodyLen:%ld \n" , m_nFrameBodyLen);
	strcat(szBuffer, szTmp);

	return ;
}

/****************************************************************
* 类名 : CSimuL_THeartBeatCmd
* 父类 : CSimuL_TCmd
* 说明 : 心跳检测请求命令
 ****************************************************************/
CSimuL_THeartBeatCmd::CSimuL_THeartBeatCmd()
	: CSimuL_TCmd(L_T_CMD_TARGET_HEARTBEAT_REQ)
{
	m_ulSerial = 0;
}

CSimuL_THeartBeatCmd::~CSimuL_THeartBeatCmd()
{

}

unsigned long CSimuL_THeartBeatCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuL_TCmd::Serialize(szBuffer);

	memcpy(szBuffer + L_T_CMD_HEADER_SIZE , &m_ulSerial, sizeof(unsigned long));
	nLen += sizeof(unsigned long);
	return nLen;
}

BOOL CSimuL_THeartBeatCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuL_TCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameBodyLen + L_T_CMD_HEADER_SIZE)
		return FALSE;

	memcpy(&m_ulSerial , szBuffer + L_T_CMD_HEADER_SIZE , sizeof(unsigned long));
	return TRUE;
}

void CSimuL_THeartBeatCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuL_TCmd::Print(szBuffer);

	sprintf(szTmp , "Serial %ld\n" , m_ulSerial);
	strcat(szBuffer, szTmp);
}

unsigned long CSimuL_THeartBeatCmd::CalcFrameBodyLen()
{
	return 4;
}

void CSimuL_THeartBeatCmd::SetSerial(unsigned long ret)
{
	m_ulSerial = ret;
}

unsigned long CSimuL_THeartBeatCmd::GetSerial()
{
	return m_ulSerial;
}

/****************************************************************
* 类名 : CSimuL_THeartBeatRespCmd
* 父类 : CSimuL_TCmd
* 说明 : 心跳检测响应命令
 ****************************************************************/
CSimuL_THeartBeatRespCmd::CSimuL_THeartBeatRespCmd()
	: CSimuL_TCmd(L_T_CMD_TARGET_HEARTBEAT_RESP)
{
	m_ulSerial = 0;
}

CSimuL_THeartBeatRespCmd::~CSimuL_THeartBeatRespCmd()
{
}

unsigned long CSimuL_THeartBeatRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuL_TCmd::Serialize(szBuffer);

	memcpy(szBuffer + L_T_CMD_HEADER_SIZE , &m_ulSerial, sizeof(unsigned long));
	nLen += sizeof(unsigned long);
	return nLen;
}

BOOL CSimuL_THeartBeatRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuL_TCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameBodyLen + L_T_CMD_HEADER_SIZE)
		return FALSE;

	memcpy(&m_ulSerial , szBuffer + L_T_CMD_HEADER_SIZE , sizeof(unsigned long));
	return TRUE;
}

void CSimuL_THeartBeatRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuL_TCmd::Print(szBuffer);

	sprintf(szTmp , "Serial %ld\n" , m_ulSerial);
	strcat(szBuffer, szTmp);
}

unsigned long CSimuL_THeartBeatRespCmd::CalcFrameBodyLen()
{
	return 4;
}

void CSimuL_THeartBeatRespCmd::SetSerial(unsigned long ret)
{
	m_ulSerial = ret;
}

unsigned long CSimuL_THeartBeatRespCmd::GetSerial()
{
	return m_ulSerial;
}

/****************************************************************
* 类名 : CSimuL_TGetTargetInfoCmd
* 父类 : CSimuL_TCmd
* 说明 : 获取下位机信息请求命令
 ****************************************************************/
CSimuL_TGetTargetInfoCmd::CSimuL_TGetTargetInfoCmd()
	: CSimuL_TCmd(L_T_CMD_GET_TARGET_INFO_REQ)
{
}

CSimuL_TGetTargetInfoCmd::~CSimuL_TGetTargetInfoCmd()
{

}

unsigned long CSimuL_TGetTargetInfoCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuL_TCmd::Serialize(szBuffer);

	return nLen;
}

BOOL CSimuL_TGetTargetInfoCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuL_TCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameBodyLen + L_T_CMD_HEADER_SIZE )
		return FALSE;

	return TRUE;
}

void CSimuL_TGetTargetInfoCmd::Print(char* szBuffer)
{
	szBuffer[0] = '\0';
	CSimuL_TCmd::Print(szBuffer);
}

unsigned long CSimuL_TGetTargetInfoCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	return nLen;
}

/****************************************************************
* 类名 : CSimuL_TGetTargetInfoRespCmd
* 父类 : CSimuL_TCmd
* 说明 : 获取下位机信息响应命令
 ****************************************************************/
CSimuL_TGetTargetInfoRespCmd::CSimuL_TGetTargetInfoRespCmd()
	: CSimuL_TCmd(L_T_CMD_GET_TARGET_INFO_RESP)
{
	memset(m_TargetInfo.szMac, 0x0, sizeof(m_TargetInfo.szMac));
	memset(m_TargetInfo.szOsVersion, 0x0, sizeof(m_TargetInfo.szOsVersion));
	memset(m_TargetInfo.szPlatform, 0x0, sizeof(m_TargetInfo.szPlatform));
	memset(m_TargetInfo.szSimuLabVer, 0x0, sizeof(m_TargetInfo.szSimuLabVer));
	memset(m_TargetInfo.szSysArch, 0x0, sizeof(m_TargetInfo.szSysArch));
	memset(m_TargetInfo.szTargetName, 0x0, sizeof(m_TargetInfo.szTargetName));
}

CSimuL_TGetTargetInfoRespCmd::~CSimuL_TGetTargetInfoRespCmd()
{
	CommuTargetIOCardInfo_t *pIOCard = NULL;
	for( unsigned int i = 0; i < m_TargetInfo.vecIOCard.size(); i++){
		pIOCard = m_TargetInfo.vecIOCard.at(i);
		delete pIOCard;
	}
	m_TargetInfo.vecIOCard.clear();
}

unsigned long CSimuL_TGetTargetInfoRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuL_TCmd::Serialize(szBuffer);
	CommuTargetIOCardInfo_t *pIOCard = NULL;

	memcpy(szBuffer + nLen, m_TargetInfo.szTargetName, 32);
	nLen += 32;
	memcpy(szBuffer + nLen, m_TargetInfo.szMac, 32);
	nLen += 32;
	memcpy(szBuffer + nLen, m_TargetInfo.szPlatform, 32);
	nLen += 32;
	memcpy(szBuffer + nLen, m_TargetInfo.szOsVersion, 64);
	nLen += 64;
	memcpy(szBuffer + nLen, m_TargetInfo.szSimuLabVer, 64);
	nLen += 64;
	memcpy(szBuffer + nLen, m_TargetInfo.szSysArch, 32);
	nLen += 32;
	memcpy(szBuffer + nLen, &m_TargetInfo.nCpuNum, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_TargetInfo.nCpuSpeed, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_TargetInfo.nFreeDisk, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_TargetInfo.nMemSize, 4);
	nLen += 4;

	//
	memcpy(szBuffer + nLen, &m_TargetInfo.nIOCardCount, 4);
	nLen += 4;
	for( unsigned i = 0; i < m_TargetInfo.vecIOCard.size(); i++)
	{
		pIOCard = m_TargetInfo.vecIOCard.at(i);

		memcpy(szBuffer + nLen, pIOCard->szVendorID, 16);
		nLen += 16;
		memcpy(szBuffer + nLen, pIOCard->szDeviceID, 16);
		nLen += 16;
		memcpy(szBuffer + nLen, pIOCard->szClassCode, 16);
		nLen += 16;

		/*memcpy(szBuffer + nLen, &pIOCard->usVendorID, 2);
		nLen += 2;
		memcpy(szBuffer + nLen, &pIOCard->usDeviceID, 2);
		nLen += 2;
		memcpy(szBuffer + nLen, &pIOCard->ulClassCode, 4);
		nLen += 4;*/

		memcpy(szBuffer + nLen, &pIOCard->ulCardNum, 4);
		nLen += 4;
		memcpy(szBuffer + nLen, pIOCard->szVendor, 64);
		nLen += 64;
		memcpy(szBuffer + nLen, pIOCard->szDevice, 64);
		nLen += 64;
		memcpy(szBuffer + nLen, pIOCard->szClass, 64);
		nLen += 64;
	}
	return nLen;
}

BOOL CSimuL_TGetTargetInfoRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuL_TCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameBodyLen + L_T_CMD_HEADER_SIZE )
		return FALSE;

	int nBodyPos = L_T_CMD_HEADER_SIZE;
	memcpy(m_TargetInfo.szTargetName, szBuffer + nBodyPos, 32);
	nBodyPos += 32;
	memcpy(m_TargetInfo.szMac, szBuffer + nBodyPos, 32);
	nBodyPos += 32;
	memcpy(m_TargetInfo.szPlatform, szBuffer + nBodyPos, 32);
	nBodyPos += 32;
	memcpy(m_TargetInfo.szOsVersion, szBuffer + nBodyPos, 64);
	nBodyPos += 64;
	memcpy(m_TargetInfo.szSimuLabVer, szBuffer + nBodyPos, 64);
	nBodyPos += 64;
	memcpy(m_TargetInfo.szSysArch, szBuffer + nBodyPos, 32);
	nBodyPos += 32;
	memcpy(&m_TargetInfo.nCpuNum, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_TargetInfo.nCpuSpeed, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_TargetInfo.nFreeDisk, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_TargetInfo.nMemSize, szBuffer + nBodyPos, 4);
	nBodyPos += 4;

	memcpy(&m_TargetInfo.nIOCardCount, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	CommuTargetIOCardInfo_t *pIOCard = NULL;
	for( unsigned i = 0; i < m_TargetInfo.nIOCardCount; i++){
		pIOCard = new CommuTargetIOCardInfo_t;
		memcpy(&pIOCard->usVendorID, szBuffer + nBodyPos, 2);
		nBodyPos += 2;
		memcpy(&pIOCard->usDeviceID, szBuffer + nBodyPos, 2);
		nBodyPos += 2;
		memcpy(&pIOCard->ulClassCode, szBuffer + nBodyPos, 4);
		nBodyPos += 4;
		memcpy(&pIOCard->ulCardNum, szBuffer + nBodyPos, 4);
		nBodyPos += 4;
		memcpy(pIOCard->szVendor, szBuffer + nBodyPos, 64);
		nBodyPos += 64;
		memcpy(pIOCard->szDevice, szBuffer + nBodyPos, 64);
		nBodyPos += 64;
		memcpy(pIOCard->szClass, szBuffer + nBodyPos, 64);
		nBodyPos += 64;
		m_TargetInfo.vecIOCard.push_back(pIOCard);
	}
	return TRUE;
}

void CSimuL_TGetTargetInfoRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuL_TCmd::Print(szBuffer);

	sprintf(szTmp, "taget:%s,mac:%s,paltform:%s, osVer:%s, simulabVer:%s, arch:%s\n",
		m_TargetInfo.szTargetName, m_TargetInfo.szMac, m_TargetInfo.szPlatform, 
		m_TargetInfo.szOsVersion, m_TargetInfo.szSimuLabVer, m_TargetInfo.szSysArch);
	strcat(szBuffer, szTmp);

	sprintf(szTmp, "cpuNum:%d, cpuSpeed:%d, freeDisk:%d, memSize:%d\n",
		m_TargetInfo.nCpuNum, m_TargetInfo.nCpuSpeed, m_TargetInfo.nFreeDisk, m_TargetInfo.nMemSize);
	strcat(szBuffer, szTmp);
}

unsigned long CSimuL_TGetTargetInfoRespCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen += 32 + 32 + 32;
	nLen += 64 + 64 + 32;
	nLen += 4 + 4 + 4 + 4;

	nLen += 4;
	for(unsigned i = 0; i < m_TargetInfo.vecIOCard.size(); i++){
		nLen += 16*3 + 4 + 64 * 3;
	}
	return nLen;
}

void CSimuL_TGetTargetInfoRespCmd::SetInfo(CommuTargetInfo_t *pInfo)
{
	CommuTargetIOCardInfo_t *pIOCard = NULL;
	CommuTargetIOCardInfo_t *pNewIOCard = NULL;

	memcpy(m_TargetInfo.szTargetName, pInfo->szTargetName, strlen(pInfo->szTargetName));
	memcpy(m_TargetInfo.szMac, pInfo->szMac, strlen(pInfo->szMac));
	memcpy(m_TargetInfo.szPlatform, pInfo->szPlatform, strlen(pInfo->szPlatform));
	memcpy(m_TargetInfo.szOsVersion, pInfo->szOsVersion, strlen(pInfo->szOsVersion));
	memcpy(m_TargetInfo.szSimuLabVer, pInfo->szSimuLabVer, strlen(pInfo->szSimuLabVer));
	memcpy(m_TargetInfo.szSysArch, pInfo->szSysArch, strlen(pInfo->szSysArch));

	m_TargetInfo.nCpuNum = pInfo->nCpuNum;
	m_TargetInfo.nCpuSpeed = pInfo->nCpuSpeed;
	m_TargetInfo.nFreeDisk = pInfo->nFreeDisk;
	m_TargetInfo.nMemSize = pInfo->nMemSize;

	m_TargetInfo.nIOCardCount = pInfo->vecIOCard.size();
	for( unsigned i = 0; i < pInfo->vecIOCard.size(); i++){
		pIOCard = pInfo->vecIOCard.at(i);
		pNewIOCard = new CommuTargetIOCardInfo_t;

		memset(pNewIOCard->szVendorID, 0x0, sizeof(pNewIOCard->szVendorID));
		memset(pNewIOCard->szDeviceID, 0x0, sizeof(pNewIOCard->szDeviceID));
		memset(pNewIOCard->szClassCode, 0x0, sizeof(pNewIOCard->szClassCode));
		memset(pNewIOCard->szVendor, 0x0, sizeof(pNewIOCard->szVendor));
		memset(pNewIOCard->szDevice, 0x0, sizeof(pNewIOCard->szDevice));
		memset(pNewIOCard->szClass, 0x0, sizeof(pNewIOCard->szDevice));

		/*
		pNewIOCard->usVendorID = pIOCard->usVendorID;
		pNewIOCard->usDeviceID = pIOCard->usDeviceID;
		pNewIOCard->ulClassCode = pIOCard->ulClassCode;*/

		strcpy(pNewIOCard->szVendorID, pIOCard->szVendorID);
		strcpy(pNewIOCard->szDeviceID, pIOCard->szDeviceID);
		strcpy(pNewIOCard->szClassCode, pIOCard->szClassCode);
		pNewIOCard->ulCardNum = pIOCard->ulCardNum;
		strcpy(pNewIOCard->szVendor, pIOCard->szVendor);
		strcpy(pNewIOCard->szDevice, pIOCard->szDevice);
		strcpy(pNewIOCard->szClass, pIOCard->szClass);
		m_TargetInfo.vecIOCard.push_back(pNewIOCard);
	}
}

const CommuTargetInfo_t *CSimuL_TGetTargetInfoRespCmd::GetInfo()
{
	return &m_TargetInfo;
}
