/*******************************************************************
 * �ļ���: SimuD_SCmd.cpp
 * ����: SimuMonitor �� SimuService֮��ͨ������ʵ��
 ******************************************************************/
#include "stdafx.h"
#include "SimuD_SCmd.h"
#include "Util.h"

#define MAX_PRINT_BUFFER_LEN 8192

CSimuD_SCmd::CSimuD_SCmd(D_S_CMD cmd)
{
	m_dsc = cmd;
	memset(m_szVersion , 0 , sizeof(m_szVersion));
	memset(m_szSimuProjectID, 0, sizeof(m_szSimuProjectID));
	m_szVersion[0] = 0;
	m_szVersion[2] = 1;
	m_szVersion[3] = 0;
	m_szVersion[4] = 0;
	m_nFrameLen = 0;
	m_nCmd = cmd;
	m_nFrameBodyLen = 0;
	memset(m_szSimuModelID, 0x0, sizeof(m_szSimuModelID));
	memset(m_szReserved1 , 0 , sizeof(m_szReserved1));
}

CSimuD_SCmd::~CSimuD_SCmd()
{

}

D_S_CMD CSimuD_SCmd::PreParseCmd(unsigned char* szBuffer , unsigned long nBufferLen , unsigned long& nCmdDataLen)
{
	D_S_CMD sdc = D_S_CMD_UNKNOWN;

	if( nBufferLen < D_S_CMD_HEADER_SIZE )
		return D_S_CMD_UNKNOWN;

	memcpy((void*)&nCmdDataLen , szBuffer + D_S_CMD_POS_FRAMELEN , D_S_CMD_SIZE_FRAMELEN);

	if( nBufferLen < nCmdDataLen )
		return D_S_CMD_UNKNOWN;

	memcpy((void*)&sdc , szBuffer + D_S_CMD_POS_COMMAND , D_S_CMD_SIZE_COMMAND);

	return sdc;
}

D_S_CMD CSimuD_SCmd::GetCmdType() const
{
	return m_dsc;
}

void CSimuD_SCmd::SetSimuProjectID(const unsigned char* szId)
{
	memset(m_szSimuProjectID , 0 , sizeof(m_szSimuProjectID));
	memcpy(m_szSimuProjectID , szId , sizeof(m_szSimuProjectID));
}

void CSimuD_SCmd::GetSimuProjectID(char * szProjectID)
{
	memcpy(szProjectID,m_szSimuProjectID,sizeof(m_szSimuProjectID));	
}

void CSimuD_SCmd::SetSimuModelID(const unsigned char *szModelID)
{
	memset(m_szSimuModelID, 0x0, sizeof(m_szSimuModelID));
	memcpy(m_szSimuModelID, szModelID, sizeof(m_szSimuModelID));
}

void CSimuD_SCmd::GetSimuModelID(unsigned char *szModelID)
{
	memcpy(szModelID, m_szSimuModelID, sizeof(m_szSimuModelID));
}

unsigned long CSimuD_SCmd::Serialize(unsigned char* szBuffer)
{
	m_nFrameBodyLen = CalcFrameBodyLen();
	m_nFrameLen = CalcFrameLen();

	memcpy(szBuffer , m_szVersion , D_S_CMD_SIZE_VERSION);
	memcpy(szBuffer + D_S_CMD_POS_FRAMELEN , (void*)&m_nFrameLen , D_S_CMD_SIZE_FRAMELEN);
	memcpy(szBuffer + D_S_CMD_POS_COMMAND , (void*)&m_nCmd , D_S_CMD_SIZE_COMMAND);
	memcpy(szBuffer + D_S_CMD_POS_FRAMEBODYLEN , (void*)&m_nFrameBodyLen , D_S_CMD_SIZE_FRAMEBODYLEN);
	memcpy(szBuffer + D_S_CMD_POS_SIMUPROJECTID, m_szSimuProjectID, D_S_CMD_SIZE_SIMUPROJECTID);
	memcpy(szBuffer + D_S_CMD_POS_SIMUMODEL, m_szSimuModelID, D_S_CMD_SIZE_SUBMODELID);
	memcpy(szBuffer + D_S_CMD_POS_RESERVED1 , m_szReserved1 , D_S_CMD_SIZE_RESERVED1);
	return D_S_CMD_HEADER_SIZE;
}

BOOL CSimuD_SCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( nBufferLen < D_S_CMD_HEADER_SIZE )
	{
		return FALSE;
	}
	memcpy(m_szVersion , szBuffer + D_S_CMD_POS_VERSION , D_S_CMD_SIZE_VERSION);
	memcpy((void*)&m_nFrameLen , szBuffer + D_S_CMD_POS_FRAMELEN , D_S_CMD_SIZE_FRAMELEN);
	memcpy((void*)&m_nCmd , szBuffer + D_S_CMD_POS_COMMAND , D_S_CMD_SIZE_COMMAND);
	memcpy((void*)&m_nFrameBodyLen , szBuffer + D_S_CMD_POS_FRAMEBODYLEN , D_S_CMD_SIZE_FRAMEBODYLEN);
	memcpy(m_szSimuProjectID, szBuffer + D_S_CMD_POS_SIMUPROJECTID, D_S_CMD_SIZE_SIMUPROJECTID);
	memcpy(m_szSimuModelID, szBuffer + D_S_CMD_POS_SIMUMODEL, D_S_CMD_SIZE_SUBMODELID);
	memcpy(m_szReserved1 , szBuffer + D_S_CMD_POS_RESERVED1 , D_S_CMD_SIZE_RESERVED1);
	return TRUE;
}

void CSimuD_SCmd::Print(char* szBuffer)
{
	char szTmp[256];
	char szStr[128];
	int i = 0;

	switch( m_nCmd )
	{
	case D_S_CMD_UNKNOWN	 :
		sprintf_s(szTmp , 64 , "Cmd: D_S_CMD_UNKNOWN");break;
	case D_S_CMD_INITMONITOR	 :
		sprintf_s(szTmp , 64 , "Cmd: D_S_CMD_INITMONITOR");break;
	case D_S_CMD_INITMONITOR_RESP	 :
		sprintf_s(szTmp , 64 , "Cmd: D_S_CMD_INITMONITOR_RESP");break;
	case D_S_CMD_LOADSIMUMODEL_REPORT	 :
		sprintf_s(szTmp , 64 , "Cmd: D_S_CMD_LOADSIMUMODEL_REPORT");break;
	case D_S_CMD_EXECSIMUMODEL	 :
		sprintf_s(szTmp , 64 , "Cmd: D_S_CMD_EXECSIMUMODEL");break;
	case D_S_CMD_EXECSIMUMODEL_RESP	 :
		sprintf_s(szTmp , 64 , "Cmd: D_S_CMD_EXECSIMUMODEL_RESP");break;
	case D_S_CMD_PAUSESIMUMODEL	 :
		sprintf_s(szTmp , 64 , "Cmd: D_S_CMD_PAUSESIMUMODEL");break;
	case D_S_CMD_PAUSESIMUMODEL_RESP	 :
		sprintf_s(szTmp , 64 , "Cmd: D_S_CMD_PAUSESIMUMODEL_RESP");break;
	case D_S_CMD_RESETSIMUMODEL	 :
		sprintf_s(szTmp , 64 , "Cmd: D_S_CMD_RESETSIMUMODEL");break;
	case D_S_CMD_RESETSIMUMODEL_RESP	 :
		sprintf_s(szTmp , 64 , "Cmd: D_S_CMD_RESETSIMUMODEL_RESP");break;
	case D_S_CMD_SETSIMUMODELPARAM	 :
		sprintf_s(szTmp , 64 , "Cmd: D_S_CMD_SETSIMUMODELPARAM");break;
	case D_S_CMD_SETSIMUMODELPARAM_RESP	 :
		sprintf_s(szTmp , 64 , "Cmd: D_S_CMD_SETSIMUMODELPARAM_RESP");break;
	case D_S_CMD_TESTEVENT_REPORT	 :
		sprintf_s(szTmp , 64 , "Cmd: D_S_CMD_TESTEVENT_REPORT");break;
	case D_S_CMD_EXCEPTEVENT_REPORT	 :
		sprintf_s(szTmp , 64 , "Cmd: D_S_CMD_EXCEPTEVENT_REPORT");break;
	case D_S_CMD_ENABLESIMUMONITOR	 :
		sprintf_s(szTmp , 64 , "Cmd: D_S_CMD_ENABLESIMUMONITOR");break;
	case D_S_CMD_ENABLESIMUMONITOR_RESP	 :
		sprintf_s(szTmp , 64 , "Cmd: D_S_CMD_ENABLESIMUMONITOR_RESP");break;
	default :
		break;
	}
	strcat_s(szBuffer, 1024, szTmp);

	sprintf_s(szTmp , sizeof(szTmp) , ",FrameLen: %ld" , m_nFrameLen);
	strcat_s(szBuffer , 1024 , szTmp);

	sprintf_s(szTmp , sizeof(szTmp) , ",FrameBodyLen:%ld" , m_nFrameBodyLen);
	strcat_s(szBuffer , 1024 , szTmp);

	memset(szStr, 0x0, sizeof(szStr));
	memcpy(szStr, m_szSimuProjectID, D_S_CMD_SIZE_SIMUPROJECTID);
	sprintf_s(szTmp , sizeof(szTmp) , ",ProjID=%s" , szStr);
	strcat_s(szBuffer , 1024 , szTmp);

	memset(szStr, 0x0, sizeof(szStr));
	memcpy(szStr, m_szSimuModelID, D_S_CMD_POS_SIMUMODEL);
	sprintf_s(szTmp , sizeof(szTmp) , ",ModelID=%s \n" , szStr);
	strcat_s(szBuffer , 1024 , szTmp);

	return ;
}

unsigned long CSimuD_SCmd::CalcFrameLen()
{
	return D_S_CMD_HEADER_SIZE + CalcFrameBodyLen();
}

/****************************************************************
 * ���� : CSimuD_SInitMonitorCmd
 * ���� : CSimuD_SCmd
 * ˵�� : ��ʼ�����
 ****************************************************************/
CSimuD_SInitMonitorCmd::CSimuD_SInitMonitorCmd()
	: CSimuD_SCmd(D_S_CMD_INITMONITOR)
{
	memset(m_szModelConfPath, 0x0, sizeof(m_szModelConfPath));
}

CSimuD_SInitMonitorCmd::~CSimuD_SInitMonitorCmd()
{
}

unsigned long CSimuD_SInitMonitorCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuD_SCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_usModelConfPathLen, 2);
	nLen += 2;
	memcpy(szBuffer + nLen, m_szModelConfPath, m_usModelConfPathLen);
	nLen += m_usModelConfPathLen;

	return nLen;
}

BOOL CSimuD_SInitMonitorCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuD_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = D_S_CMD_HEADER_SIZE;
	memcpy(&m_usModelConfPathLen , szBuffer + nBodyPos , 2);
	nBodyPos += 2;
	memcpy(m_szModelConfPath , szBuffer + nBodyPos , m_usModelConfPathLen);
	nBodyPos += m_usModelConfPathLen;

	return TRUE;
}

void CSimuD_SInitMonitorCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuD_SCmd::Print(szBuffer);

	sprintf_s(szTmp, sizeof(szTmp), "nLen:%d, confPath:%s \n",m_usModelConfPathLen, m_szModelConfPath);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuD_SInitMonitorCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;
	CommuModelProgFilePath_t *pModelPath = NULL;

	nLen += 2 + m_usModelConfPathLen;

	return nLen;
}

void  CSimuD_SInitMonitorCmd::SetData(const unsigned char *szConfPath, unsigned short nConfPathLen)
{
	memcpy(m_szModelConfPath, szConfPath, nConfPathLen);
	m_usModelConfPathLen = nConfPathLen;
}

void  CSimuD_SInitMonitorCmd::GetData(unsigned char *szConfPath, unsigned short *nConfPathLen)
{
	memcpy(szConfPath, m_szModelConfPath, m_usModelConfPathLen);
	*nConfPathLen = m_usModelConfPathLen;
}

/****************************************************************
 * ���� : CSimuD_SInitMonitorRespCmd
 * ���� : CSimuD_SCmd
 * ˵�� : ��ʼ�������Ӧ
 ****************************************************************/
CSimuD_SInitMonitorRespCmd::CSimuD_SInitMonitorRespCmd()
	: CSimuD_SCmd(D_S_CMD_INITMONITOR_RESP)
{
	m_ulResult = 0;
}

CSimuD_SInitMonitorRespCmd::~CSimuD_SInitMonitorRespCmd()
{

}

unsigned long CSimuD_SInitMonitorRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuD_SCmd::Serialize(szBuffer);
	memcpy(szBuffer + D_S_CMD_HEADER_SIZE , &m_ulResult, sizeof(unsigned long));
	nLen += sizeof(unsigned long);
	return nLen;
}

BOOL CSimuD_SInitMonitorRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuD_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;
	memcpy(&m_ulResult , szBuffer + D_S_CMD_HEADER_SIZE , sizeof(unsigned long));
	return TRUE;
}

void CSimuD_SInitMonitorRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuD_SCmd::Print(szBuffer);

	sprintf_s(szTmp , sizeof(szTmp) , "%d\n" , m_ulResult);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuD_SInitMonitorRespCmd::CalcFrameBodyLen()
{
	return 4;
}

void CSimuD_SInitMonitorRespCmd::SetResult(unsigned long ret)
{
	m_ulResult = ret;
}

unsigned long CSimuD_SInitMonitorRespCmd::GetResult()
{
	return m_ulResult;
}

/****************************************************************
 * ���� : CSimuD_SLoadSimuModelReportCmd
 * ���� : CSimuD_SCmd
 * ˵�� : ģ�ͼ����ϱ�
 ****************************************************************/
CSimuD_SLoadSimuModelReportCmd::CSimuD_SLoadSimuModelReportCmd()
	: CSimuD_SCmd(D_S_CMD_LOADSIMUMODEL_REPORT)
{
	m_ulResult = 0;
}

CSimuD_SLoadSimuModelReportCmd::~CSimuD_SLoadSimuModelReportCmd()
{

}

unsigned long CSimuD_SLoadSimuModelReportCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuD_SCmd::Serialize(szBuffer);
	memcpy(szBuffer + D_S_CMD_HEADER_SIZE , &m_ulResult, sizeof(unsigned long));
	nLen += sizeof(unsigned long);
	return nLen;
}

BOOL CSimuD_SLoadSimuModelReportCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuD_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;
	memcpy(&m_ulResult , szBuffer + D_S_CMD_HEADER_SIZE , sizeof(unsigned long));
	return TRUE;
}

void CSimuD_SLoadSimuModelReportCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuD_SCmd::Print(szBuffer);

	sprintf_s(szTmp , sizeof(szTmp) , "%d\n" , m_ulResult);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuD_SLoadSimuModelReportCmd::CalcFrameBodyLen()
{
	return 4;
}

void CSimuD_SLoadSimuModelReportCmd::SetResult(unsigned long ret)
{
	m_ulResult = ret;
}

unsigned long CSimuD_SLoadSimuModelReportCmd::GetResult()
{
	return m_ulResult;
}

/****************************************************************
 * ���� : CSimuD_SExecSimuModelCmd
 * ���� : CSimuD_SCmd
 * ˵�� : ģ��ִ������
 ****************************************************************/
CSimuD_SExecSimuModelCmd::CSimuD_SExecSimuModelCmd()
	: CSimuD_SCmd(D_S_CMD_EXECSIMUMODEL)
{

}

CSimuD_SExecSimuModelCmd::~CSimuD_SExecSimuModelCmd()
{
}

unsigned long CSimuD_SExecSimuModelCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuD_SCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_nExecMode, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_nTestTask, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_nTestCase, 4);
	nLen += 4;
	
	return nLen;
}

BOOL CSimuD_SExecSimuModelCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuD_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = D_S_CMD_HEADER_SIZE;
	memcpy(&m_nExecMode , szBuffer + nBodyPos , 4);
	nBodyPos += 4;
	memcpy(&m_nTestTask, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_nTestCase, szBuffer + nBodyPos, 4);
	nBodyPos += 4;

	return TRUE;
}

void CSimuD_SExecSimuModelCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuD_SCmd::Print(szBuffer);

	sprintf_s(szTmp, sizeof(szTmp), "ExecMode:%d, TestTask:%d, TestCase:%d \n", m_nExecMode, m_nTestTask, m_nTestCase);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuD_SExecSimuModelCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen += 4 + 4 + 4;

	return nLen;
}

void CSimuD_SExecSimuModelCmd::SetData(int nExecMode, int TestTask, int TestCase)
{
	m_nExecMode = nExecMode;
	m_nTestTask = TestTask;
	m_nTestCase = TestCase;
}

unsigned long CSimuD_SExecSimuModelCmd::GetExecMode()
{
	return m_nExecMode;
}

unsigned long CSimuD_SExecSimuModelCmd::GetTestTask()
{
	return m_nTestTask;
}

unsigned long CSimuD_SExecSimuModelCmd::GetTestCase()
{
	return m_nTestCase;
}

/****************************************************************
 * ���� : CSimuD_SExecSimuModelRespCmd
 * ���� : CSimuD_SCmd
 * ˵�� : ģ��ִ��������Ӧ
 ****************************************************************/
CSimuD_SExecSimuModelRespCmd::CSimuD_SExecSimuModelRespCmd()
	: CSimuD_SCmd(D_S_CMD_EXECSIMUMODEL_RESP)
{
	m_ulResult = 0;
}

CSimuD_SExecSimuModelRespCmd::~CSimuD_SExecSimuModelRespCmd()
{

}

unsigned long CSimuD_SExecSimuModelRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuD_SCmd::Serialize(szBuffer);
	memcpy(szBuffer + D_S_CMD_HEADER_SIZE , &m_ulResult, sizeof(unsigned long));
	nLen += sizeof(unsigned long);
	return nLen;
}

BOOL CSimuD_SExecSimuModelRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuD_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;
	memcpy(&m_ulResult , szBuffer + D_S_CMD_HEADER_SIZE , sizeof(unsigned long));
	return TRUE;
}

void CSimuD_SExecSimuModelRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuD_SCmd::Print(szBuffer);

	sprintf_s(szTmp , sizeof(szTmp) , "%d\n" , m_ulResult);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuD_SExecSimuModelRespCmd::CalcFrameBodyLen()
{
	return 4;
}

void CSimuD_SExecSimuModelRespCmd::SetResult(unsigned long ret)
{
	m_ulResult = ret;
}

unsigned long CSimuD_SExecSimuModelRespCmd::GetResult()
{
	return m_ulResult;
}

/****************************************************************
 * ���� : CSimuD_SPauseSimuModelCmd
 * ���� : CSimuD_SCmd
 * ˵�� : ģ����ͣ����
 ****************************************************************/
CSimuD_SPauseSimuModelCmd::CSimuD_SPauseSimuModelCmd()
	: CSimuD_SCmd(D_S_CMD_PAUSESIMUMODEL)
{

}

CSimuD_SPauseSimuModelCmd::~CSimuD_SPauseSimuModelCmd()
{
}

unsigned long CSimuD_SPauseSimuModelCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuD_SCmd::Serialize(szBuffer);

	return nLen;
}

BOOL CSimuD_SPauseSimuModelCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuD_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	return TRUE;
}

void CSimuD_SPauseSimuModelCmd::Print(char* szBuffer)
{
//	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuD_SCmd::Print(szBuffer);
}

unsigned long CSimuD_SPauseSimuModelCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	return nLen;
}
/****************************************************************
 * ���� : CSimuD_SPauseSimuModelRespCmd
 * ���� : CSimuD_SCmd
 * ˵�� : ģ����ͣ������Ӧ
 ****************************************************************/
CSimuD_SPauseSimuModelRespCmd::CSimuD_SPauseSimuModelRespCmd()
	: CSimuD_SCmd(D_S_CMD_PAUSESIMUMODEL_RESP)
{
	m_ulResult = 0;
}

CSimuD_SPauseSimuModelRespCmd::~CSimuD_SPauseSimuModelRespCmd()
{

}

unsigned long CSimuD_SPauseSimuModelRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuD_SCmd::Serialize(szBuffer);
	memcpy(szBuffer + D_S_CMD_HEADER_SIZE , &m_ulResult, sizeof(unsigned long));
	nLen += sizeof(unsigned long);
	return nLen;
}

BOOL CSimuD_SPauseSimuModelRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuD_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;
	memcpy(&m_ulResult , szBuffer + D_S_CMD_HEADER_SIZE , sizeof(unsigned long));
	return TRUE;
}

void CSimuD_SPauseSimuModelRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuD_SCmd::Print(szBuffer);

	sprintf_s(szTmp , sizeof(szTmp) , "%d\n" , m_ulResult);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuD_SPauseSimuModelRespCmd::CalcFrameBodyLen()
{
	return 4;
}

void CSimuD_SPauseSimuModelRespCmd::SetResult(unsigned long ret)
{
	m_ulResult = ret;
}

unsigned long CSimuD_SPauseSimuModelRespCmd::GetResult()
{
	return m_ulResult;
}

/****************************************************************
 * ���� : CSimuD_SResetSimuModelCmd
 * ���� : CSimuD_SCmd
 * ˵�� : ����ģ������
 ****************************************************************/
CSimuD_SResetSimuModelCmd::CSimuD_SResetSimuModelCmd()
	: CSimuD_SCmd(D_S_CMD_RESETSIMUMODEL)
{

}

CSimuD_SResetSimuModelCmd::~CSimuD_SResetSimuModelCmd()
{
}

unsigned long CSimuD_SResetSimuModelCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuD_SCmd::Serialize(szBuffer);

	return nLen;
}

BOOL CSimuD_SResetSimuModelCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuD_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	return TRUE;
}

void CSimuD_SResetSimuModelCmd::Print(char* szBuffer)
{
//	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuD_SCmd::Print(szBuffer);
}

unsigned long CSimuD_SResetSimuModelCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	return nLen;
}

/****************************************************************
 * ���� : CSimuD_SResetSimuModelRespCmd
 * ���� : CSimuD_SCmd
 * ˵�� : ����ģ��������Ӧ
 ****************************************************************/
CSimuD_SResetSimuModelRespCmd::CSimuD_SResetSimuModelRespCmd()
	: CSimuD_SCmd(D_S_CMD_RESETSIMUMODEL_RESP)
{
	m_ulResult = 0;
}

CSimuD_SResetSimuModelRespCmd::~CSimuD_SResetSimuModelRespCmd()
{

}

unsigned long CSimuD_SResetSimuModelRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuD_SCmd::Serialize(szBuffer);
	memcpy(szBuffer + D_S_CMD_HEADER_SIZE , &m_ulResult, sizeof(unsigned long));
	nLen += sizeof(unsigned long);
	return nLen;
}

BOOL CSimuD_SResetSimuModelRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuD_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;
	memcpy(&m_ulResult , szBuffer + D_S_CMD_HEADER_SIZE , sizeof(unsigned long));
	return TRUE;
}

void CSimuD_SResetSimuModelRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuD_SCmd::Print(szBuffer);

	sprintf_s(szTmp , sizeof(szTmp) , "%d\n" , m_ulResult);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuD_SResetSimuModelRespCmd::CalcFrameBodyLen()
{
	return 4;
}

void CSimuD_SResetSimuModelRespCmd::SetResult(unsigned long ret)
{
	m_ulResult = ret;
}

unsigned long CSimuD_SResetSimuModelRespCmd::GetResult()
{
	return m_ulResult;
}

/****************************************************************
 * ���� : CSimuD_SSetSimuModelParamCmd
 * ���� : CSimuD_SCmd
 * ˵�� : ����ģ�Ͳ���
 ****************************************************************/
CSimuD_SSetSimuModelParamCmd::CSimuD_SSetSimuModelParamCmd()
	: CSimuD_SCmd(D_S_CMD_SETSIMUMODELPARAM)
{
}

CSimuD_SSetSimuModelParamCmd::~CSimuD_SSetSimuModelParamCmd()
{
	for(unsigned int i = 0; i < m_vecModelParam.size(); i++)
		delete m_vecModelParam.at(i);
	m_vecModelParam.clear();
}

unsigned long CSimuD_SSetSimuModelParamCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuD_SCmd::Serialize(szBuffer);
	CommuSetModelParam_t *pParam = NULL;

	memcpy(szBuffer + nLen, &m_nModelParamCount, 4);
	nLen += 4;

	for(unsigned int i = 0; i < m_vecModelParam.size(); i++){
		pParam = m_vecModelParam.at(i);

		memcpy(szBuffer + nLen, &pParam->nModelID, 4);
		nLen += 4;
		memcpy(szBuffer + nLen, pParam->szParamName, COMMU_MODEL_PARAM_NAME_LEN);
		nLen += COMMU_MODEL_PARAM_NAME_LEN;
		memcpy(szBuffer + nLen, &pParam->nParamValueLen, 4);
		nLen += 4;
		memcpy(szBuffer + nLen, pParam->szParamValue, pParam->nParamValueLen);
		nLen += pParam->nParamValueLen;
	}
	return nLen;
}

BOOL CSimuD_SSetSimuModelParamCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuD_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	CommuSetModelParam_t *pParam = NULL;

	int nBodyPos = D_S_CMD_HEADER_SIZE;
	memcpy(&m_nModelParamCount , szBuffer + nBodyPos , 4);
	nBodyPos += 4;

	for(unsigned int i = 0; i < m_nModelParamCount; i++){
		pParam = new CommuSetModelParam_t;
		memset(pParam->szParamName, 0x0, sizeof(pParam->szParamName));
		memset(pParam->szParamValue, 0x0, sizeof(pParam->szParamValue));

		memcpy(&pParam->nModelID, szBuffer + nBodyPos, 4);
		nBodyPos += 4;
		memcpy(pParam->szParamName, szBuffer + nBodyPos, COMMU_MODEL_PARAM_NAME_LEN);
		nBodyPos += COMMU_MODEL_PARAM_NAME_LEN;
		memcpy(&pParam->nParamValueLen, szBuffer + nBodyPos, 4);
		nBodyPos += 4;
		memcpy(pParam->szParamValue, szBuffer + nBodyPos, pParam->nParamValueLen);
		nBodyPos += pParam->nParamValueLen;

		m_vecModelParam.push_back(pParam);
	}
	return TRUE;
}

void CSimuD_SSetSimuModelParamCmd::Print(char* szBuffer)
{
	char szTmp[1024];
	CommuSetModelParam_t *pParam = NULL;

	szBuffer[0] = '\0';
	CSimuD_SCmd::Print(szBuffer);

	sprintf_s(szTmp, sizeof(szTmp), "ModelCount:%d \n",m_nModelParamCount);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);

	for(unsigned int i = 0; i < m_vecModelParam.size(); i++){
		pParam = m_vecModelParam.at(i);

		sprintf_s(szTmp, sizeof(szTmp), "modelId:%d, paramName:%s,nValueLen:%d, paramValue:%s \n",pParam->nModelID,
			pParam->szParamName, pParam->nParamValueLen, pParam->szParamValue);
		strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
	}
}

unsigned long CSimuD_SSetSimuModelParamCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;
	CommuSetModelParam_t *pParam = NULL;

	nLen += 4;
	for(unsigned int i = 0; i < m_vecModelParam.size(); i++){
		pParam = m_vecModelParam.at(i);
		nLen += 4 + COMMU_MODEL_PARAM_NAME_LEN + 4 + pParam->nParamValueLen;
	}
	return nLen;
}

void CSimuD_SSetSimuModelParamCmd::SetData(vector<CommuSetModelParam_t *> &vecModelParam)
{
	CommuSetModelParam_t *pParam = NULL;
	CommuSetModelParam_t *pNewParam = NULL;

	m_nModelParamCount = vecModelParam.size();
	for( unsigned int i = 0; i < m_nModelParamCount; i++){
		pParam = vecModelParam.at(i);
		pNewParam = new CommuSetModelParam_t;
		memset(pNewParam->szParamName, 0x0, sizeof(pNewParam->szParamName));
		memset(pNewParam->szParamValue, 0x0, sizeof(pNewParam->szParamValue));
		pNewParam->nModelID = pParam->nModelID;
		pNewParam->nParamId = pParam->nParamId;
		pNewParam->nParamValueLen = pParam->nParamValueLen;
		memcpy(pNewParam->szParamName, pParam->szParamName, strlen((char *)pParam->szParamName));
		memcpy(pNewParam->szParamValue, pParam->szParamValue, pParam->nParamValueLen);
		m_vecModelParam.push_back(pNewParam);
	}
}

const vector<CommuSetModelParam_t *> *CSimuD_SSetSimuModelParamCmd::GetData()
{
	return &m_vecModelParam;
}

/****************************************************************
 * ���� : CSimuD_SSetSimuModelParamRespCmd
 * ���� : CSimuD_SCmd
 * ˵�� : ����ģ�Ͳ���������Ӧ
 ****************************************************************/
CSimuD_SSetSimuModelParamRespCmd::CSimuD_SSetSimuModelParamRespCmd()
	: CSimuD_SCmd(D_S_CMD_SETSIMUMODELPARAM_RESP)
{
	m_ulResult = 0;
}

CSimuD_SSetSimuModelParamRespCmd::~CSimuD_SSetSimuModelParamRespCmd()
{

}

unsigned long CSimuD_SSetSimuModelParamRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuD_SCmd::Serialize(szBuffer);
	memcpy(szBuffer + D_S_CMD_HEADER_SIZE , &m_ulResult, sizeof(unsigned long));
	nLen += sizeof(unsigned long);
	return nLen;
}

BOOL CSimuD_SSetSimuModelParamRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuD_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;
	memcpy(&m_ulResult , szBuffer + D_S_CMD_HEADER_SIZE , sizeof(unsigned long));
	return TRUE;
}

void CSimuD_SSetSimuModelParamRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuD_SCmd::Print(szBuffer);

	sprintf_s(szTmp , sizeof(szTmp) , "%d\n" , m_ulResult);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuD_SSetSimuModelParamRespCmd::CalcFrameBodyLen()
{
	return 4;
}

void CSimuD_SSetSimuModelParamRespCmd::SetResult(unsigned long ret)
{
	m_ulResult = ret;
}

unsigned long CSimuD_SSetSimuModelParamRespCmd::GetResult()
{
	return m_ulResult;
}

/****************************************************************
 * ���� : CSimuD_STestEventReportCmd
 * ���� : CSimuD_SCmd
 * ˵�� : �����¼��ϱ�
 ****************************************************************/
CSimuD_STestEventReportCmd::CSimuD_STestEventReportCmd()
	: CSimuD_SCmd(D_S_CMD_TESTEVENT_REPORT)
{
	memset(m_Report.szEventValue, 0x0 , sizeof(m_Report.szEventValue));
}

CSimuD_STestEventReportCmd::~CSimuD_STestEventReportCmd()
{

}

unsigned long CSimuD_STestEventReportCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuD_SCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_Report.nReportType, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_Report.nTestTaskId, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_Report.nTestCaseId, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_Report.nEventValueLen, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, m_Report.szEventValue, m_Report.nEventValueLen);
	nLen += m_Report.nEventValueLen;

	return nLen;
}

BOOL CSimuD_STestEventReportCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuD_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = D_S_CMD_HEADER_SIZE;
	memcpy(&m_Report.nReportType, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_Report.nTestTaskId, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_Report.nTestCaseId, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_Report.nEventValueLen, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(m_Report.szEventValue, szBuffer + nBodyPos, m_Report.nEventValueLen);
	nBodyPos += m_Report.nEventValueLen;

	return TRUE;
}

void CSimuD_STestEventReportCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuD_SCmd::Print(szBuffer);

	sprintf_s(szTmp, sizeof(szTmp), "Type:%d , TaskId:%d, caseId:%d nLen:%d, value:%s\n",
		m_Report.nReportType,m_Report.nTestTaskId, m_Report.nTestCaseId,
		m_Report.nEventValueLen, m_Report.szEventValue);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuD_STestEventReportCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen += 4 + 4 + 4;
	nLen += 4 + m_Report.nEventValueLen;

	return nLen;
}

void CSimuD_STestEventReportCmd::SetReport(const CommuTestEventReport_t *pReport)
{
	m_Report.nReportType = pReport->nReportType;
	m_Report.nTestTaskId = pReport->nTestTaskId;
	m_Report.nTestCaseId = pReport->nTestCaseId;
	m_Report.nEventValueLen = pReport->nEventValueLen;
	memcpy(m_Report.szEventValue, pReport->szEventValue, pReport->nEventValueLen);
}

const CommuTestEventReport_t *CSimuD_STestEventReportCmd::GetReport()
{
	return &m_Report;
}

/****************************************************************
 * ���� : CSimuD_SExceptEventReportCmd
 * ���� : CSimuD_SCmd
 * ˵�� : �쳣�¼��ϱ�
 ****************************************************************/
CSimuD_SExceptEventReportCmd::CSimuD_SExceptEventReportCmd()
	: CSimuD_SCmd(D_S_CMD_EXCEPTEVENT_REPORT)
{
	memset(m_Report.szReportValue, 0x0 , sizeof(m_Report.szReportValue));
}

CSimuD_SExceptEventReportCmd::~CSimuD_SExceptEventReportCmd()
{

}

unsigned long CSimuD_SExceptEventReportCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuD_SCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_Report.nReportType, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_Report.nReportLen, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, m_Report.szReportValue, m_Report.nReportLen);
	nLen += m_Report.nReportLen;

	return nLen;
}

BOOL CSimuD_SExceptEventReportCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuD_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = D_S_CMD_HEADER_SIZE;
	memcpy(&m_Report.nReportType, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_Report.nReportLen, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(m_Report.szReportValue, szBuffer + nBodyPos, m_Report.nReportLen);
	nBodyPos += m_Report.nReportLen;

	return TRUE;
}

void CSimuD_SExceptEventReportCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuD_SCmd::Print(szBuffer);

	sprintf_s(szTmp, sizeof(szTmp), "Type:%d , nLen:%d, value:%s\n",
		m_Report.nReportType, m_Report.nReportLen, m_Report.szReportValue);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuD_SExceptEventReportCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen += 4 + 4 + m_Report.nReportLen;

	return nLen;
}

void CSimuD_SExceptEventReportCmd::SetReport(const CommuExceptReport_t *pReport)
{
	m_Report.nReportType = pReport->nReportType;
	m_Report.nReportLen = pReport->nReportLen;
	memcpy(m_Report.szReportValue, pReport->szReportValue, pReport->nReportLen);
}

const CommuExceptReport_t *CSimuD_SExceptEventReportCmd::GetReport()
{
	return &m_Report;
}

/****************************************************************
 * ���� : CSimuD_SEnableSimuMonitorCmd
 * ���� : CSimuD_SCmd
 * ˵�� : ʹ�ܼ������
 ****************************************************************/
CSimuD_SEnableSimuMonitorCmd::CSimuD_SEnableSimuMonitorCmd()
	: CSimuD_SCmd(D_S_CMD_ENABLESIMUMONITOR)
{
	m_ulEnableFlag = 0;
}

CSimuD_SEnableSimuMonitorCmd::~CSimuD_SEnableSimuMonitorCmd()
{

}

unsigned long CSimuD_SEnableSimuMonitorCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuD_SCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_ulEnableFlag, 4);
	nLen += 4;

	return nLen;
}

BOOL CSimuD_SEnableSimuMonitorCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuD_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = D_S_CMD_HEADER_SIZE;

	memcpy(&m_ulEnableFlag, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	
	return TRUE;
}

void CSimuD_SEnableSimuMonitorCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuD_SCmd::Print(szBuffer);

	sprintf_s(szTmp, sizeof(szTmp), "EnableFlag:%d \n", m_ulEnableFlag);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuD_SEnableSimuMonitorCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen += 4;

	return nLen;
}

void CSimuD_SEnableSimuMonitorCmd::SetFlag(unsigned long ulFlag)
{
	m_ulEnableFlag = ulFlag;
}

unsigned long CSimuD_SEnableSimuMonitorCmd::GetFlag()
{
	return m_ulEnableFlag;
}

/****************************************************************
 * ���� : CSimuD_SEnableSimuMonitorRespCmd
 * ���� : CSimuD_SCmd
 * ˵�� : ʹ�ܼ��������Ӧ
 ****************************************************************/
CSimuD_SEnableSimuMonitorRespCmd::CSimuD_SEnableSimuMonitorRespCmd()
	: CSimuD_SCmd(D_S_CMD_ENABLESIMUMONITOR_RESP)
{
	m_ulErrInfoLen = 0;
	m_ulResult = 0;
}

CSimuD_SEnableSimuMonitorRespCmd::~CSimuD_SEnableSimuMonitorRespCmd()
{

}

unsigned long CSimuD_SEnableSimuMonitorRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuD_SCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_ulResult, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_ulErrInfoLen, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, m_szErrInfo, m_ulErrInfoLen);
	nLen += m_ulErrInfoLen;

	return nLen;
}

BOOL CSimuD_SEnableSimuMonitorRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuD_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = D_S_CMD_HEADER_SIZE;
	memset(m_szErrInfo, 0x0, sizeof(m_szErrInfo));

	memcpy(&m_ulResult, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_ulErrInfoLen, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(m_szErrInfo, szBuffer + nBodyPos, m_ulErrInfoLen);
	nBodyPos += m_ulErrInfoLen;

	return TRUE;
}

void CSimuD_SEnableSimuMonitorRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuD_SCmd::Print(szBuffer);

	sprintf_s(szTmp, sizeof(szTmp), "result:%d, errInfo:%s, errLen:%d \n", m_ulResult, m_szErrInfo, m_ulErrInfoLen);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuD_SEnableSimuMonitorRespCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen += 4 + 4 + m_ulErrInfoLen;

	return nLen;
}

void CSimuD_SEnableSimuMonitorRespCmd::SetResult(unsigned long nResult)
{
	m_ulResult = nResult;
}

unsigned long CSimuD_SEnableSimuMonitorRespCmd::GetResult()
{
	return m_ulResult;
}

void CSimuD_SEnableSimuMonitorRespCmd::SetErrInfo(const char *szErrInfo, unsigned long ulErrLen)
{
	memset(m_szErrInfo, 0x0, sizeof(m_szErrInfo));
	memcpy(m_szErrInfo, szErrInfo, ulErrLen);
	m_ulErrInfoLen = ulErrLen;
}

unsigned long CSimuD_SEnableSimuMonitorRespCmd::GetErrInfo(char *pszErrInfo, unsigned long nBuffLen)
{
	if( nBuffLen < m_ulErrInfoLen){
		return 0;
	}
	memset(pszErrInfo, 0x0, nBuffLen);
	memcpy(pszErrInfo, m_szErrInfo, m_ulErrInfoLen);
	return m_ulErrInfoLen;
}