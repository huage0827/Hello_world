/*****************************************************************
 * 文件名: SimuM_CCmd.cpp
 * 功能: SimuService、SimuGuard与SimuModel之间通信命令实现
 *****************************************************************/
#include "stdafx.h"
#include "SimuM_CCmd.h"
#include "Util.h"
#include <stdio.h>
#include <string.h>

using namespace std;

#if 0

#define MAX_PRINT_BUFFER_LEN 8192

CSimuM_CCmd::CSimuM_CCmd(M_C_CMD sgc)
{
	m_sgc = sgc;
	m_nFrameLen = 0;
	m_nCmd = sgc;
	m_nModelId = 0;
}

CSimuM_CCmd::~CSimuM_CCmd()
{

}

M_C_CMD CSimuM_CCmd::PreParseCmd(unsigned char* szBuffer , unsigned long nBufferLen , unsigned long& nCmdDataLen)
{
	M_C_CMD sgc = M_C_CMD_Unknown;

	if( nBufferLen < M_C_CMD_HEADER_SIZE )
		return M_C_CMD_Unknown;

	memcpy((void*)&nCmdDataLen , szBuffer + M_C_CMD_POS_FRAMELEN , M_C_CMD_SIZE_FRAMELEN);

	if( nBufferLen < nCmdDataLen )
		return M_C_CMD_Unknown;

	memcpy((void*)&sgc , szBuffer + M_C_CMD_POS_COMMAND , M_C_CMD_SIZE_COMMAND);

	return sgc;
}

M_C_CMD CSimuM_CCmd::GetCmdType() const
{
	return m_sgc;
}

unsigned long CSimuM_CCmd::GetFrameLen()
{
	return m_nFrameLen;
}

void CSimuM_CCmd::SetSimuModelID(unsigned long nModelId)
{
	m_nModelId = nModelId;
}

unsigned long CSimuM_CCmd::GetSimuModelID()
{
	return m_nModelId;
}

unsigned long CSimuM_CCmd::Serialize(unsigned char* szBuffer)
{
	m_nFrameLen = CalcFrameBodyLen() + M_C_CMD_HEADER_SIZE;

	memcpy(szBuffer + M_C_CMD_POS_FRAMELEN , (void*)&m_nFrameLen , M_C_CMD_SIZE_FRAMELEN);
	memcpy(szBuffer + M_C_CMD_POS_COMMAND , (void*)&m_nCmd , M_C_CMD_SIZE_COMMAND);
	memcpy(szBuffer + M_C_CMD_POS_SIMUMODELID , &m_nModelId , M_C_CMD_SIZE_SIMUMODELID);

	return M_C_CMD_HEADER_SIZE;
}

BOOL CSimuM_CCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( nBufferLen < M_C_CMD_HEADER_SIZE ){
		return FALSE;
	}
	memcpy((void*)&m_nFrameLen , szBuffer + M_C_CMD_POS_FRAMELEN , M_C_CMD_SIZE_FRAMELEN);
	memcpy((void*)&m_nCmd , szBuffer + M_C_CMD_POS_COMMAND , M_C_CMD_SIZE_COMMAND);
	memcpy((void*)&m_nModelId , szBuffer + M_C_CMD_POS_SIMUMODELID , M_C_CMD_SIZE_SIMUMODELID);

	return TRUE;
}

void CSimuM_CCmd::Print(char* szBuffer)
{
	char szTmp[256];

	switch( m_nCmd )
	{
	case M_C_CMD_Unknown	 :
		sprintf(szTmp , "Cmd: L_S_CMD_UNKNOWN");break;
	
	default :
		break;
	}
	strcat(szBuffer, szTmp);

	sprintf(szTmp , ", FrameLen: %ld, ModelId:%d" , m_nFrameLen, m_nModelId);
	strcat(szBuffer, szTmp);

	return ;
}

/****************************************************************
 * 类名 : CSimuM_CExecSimuModelCmd
 * 父类 : CSimuM_CCmd
 * 说明 : 1,执行仿真模型
 ****************************************************************/
CSimuM_CExecSimuModelCmd::CSimuM_CExecSimuModelCmd()
	: CSimuM_CCmd(M_C_CMD_ExecSimuModel)
{
	m_nExecMode = 0;
}

CSimuM_CExecSimuModelCmd::~CSimuM_CExecSimuModelCmd()
{
}

unsigned long CSimuM_CExecSimuModelCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuM_CCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_nExecMode, 4);
	nLen += 4;

	return nLen;
}

BOOL CSimuM_CExecSimuModelCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuM_CCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = M_C_CMD_HEADER_SIZE;
	memcpy(&m_nExecMode , szBuffer + nBodyPos , 4);
	nBodyPos += 4;
	
	return TRUE;
}

void CSimuM_CExecSimuModelCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuM_CCmd::Print(szBuffer);

	sprintf(szTmp, "nExecMode:%d \n", m_nExecMode);
	strcat(szBuffer, szTmp);
}

unsigned long CSimuM_CExecSimuModelCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 4;

	return nLen;
}

void CSimuM_CExecSimuModelCmd::SetParam(int nExecMode)
{
	m_nExecMode = nExecMode;
}

unsigned long CSimuM_CExecSimuModelCmd::GetParam()
{
	return m_nExecMode;
}

/****************************************************************
 * 类名 : CSimuM_CPauseSimuModelCmd
 * 父类 : CSimuM_CCmd
 * 说明 : 2,暂停仿真模型
 ****************************************************************/
CSimuM_CPauseSimuModelCmd::CSimuM_CPauseSimuModelCmd()
	: CSimuM_CCmd(M_C_CMD_PauseSimuModel)
{
}

CSimuM_CPauseSimuModelCmd::~CSimuM_CPauseSimuModelCmd()
{
}

unsigned long CSimuM_CPauseSimuModelCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuM_CCmd::Serialize(szBuffer);

	return nLen;
}

BOOL CSimuM_CPauseSimuModelCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuM_CCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	return TRUE;
}

void CSimuM_CPauseSimuModelCmd::Print(char* szBuffer)
{
	szBuffer[0] = '\0';
	CSimuM_CCmd::Print(szBuffer);
}

unsigned long CSimuM_CPauseSimuModelCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;
	return nLen;
}

/****************************************************************
 * 类名 : CSimuM_CResetSimuModelCmd
 * 父类 : CSimuM_CCmd
 * 说明 : 3,重置仿真模型
 ****************************************************************/
CSimuM_CResetSimuModelCmd::CSimuM_CResetSimuModelCmd()
	: CSimuM_CCmd(M_C_CMD_ResetSimuModel)
{
}

CSimuM_CResetSimuModelCmd::~CSimuM_CResetSimuModelCmd()
{
}

unsigned long CSimuM_CResetSimuModelCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuM_CCmd::Serialize(szBuffer);
	return nLen;
}

BOOL CSimuM_CResetSimuModelCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuM_CCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	return TRUE;
}

void CSimuM_CResetSimuModelCmd::Print(char* szBuffer)
{
	szBuffer[0] = '\0';
	CSimuM_CCmd::Print(szBuffer);
}

unsigned long CSimuM_CResetSimuModelCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;
	return nLen;
}

/****************************************************************
 * 类名 : CSimuM_CSetSimuModelParamCmd
 * 父类 : CSimuM_CCmd
 * 说明 : 4,设置仿真模型参数
 ****************************************************************/
CSimuM_CSetSimuModelParamCmd::CSimuM_CSetSimuModelParamCmd()
	: CSimuM_CCmd(M_C_CMD_SetModelParam)
{
}

CSimuM_CSetSimuModelParamCmd::~CSimuM_CSetSimuModelParamCmd()
{
	for(unsigned int i = 0; i < m_vecModelParam.size(); i++)
		delete m_vecModelParam.at(i);
	m_vecModelParam.clear();
}

unsigned long CSimuM_CSetSimuModelParamCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuM_CCmd::Serialize(szBuffer);
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

BOOL CSimuM_CSetSimuModelParamCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuM_CCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	CommuSetModelParam_t *pParam = NULL;

	int nBodyPos = M_C_CMD_HEADER_SIZE;
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

void CSimuM_CSetSimuModelParamCmd::Print(char* szBuffer)
{
	char szTmp[1024];
	CommuSetModelParam_t *pParam = NULL;

	szBuffer[0] = '\0';
	CSimuM_CCmd::Print(szBuffer);

	sprintf(szTmp, "ModelCount:%d \n",m_nModelParamCount);
	strcat(szBuffer, szTmp);

	for(unsigned int i = 0; i < m_vecModelParam.size(); i++){
		pParam = m_vecModelParam.at(i);

		sprintf(szTmp, "modelId:%d, paramName:%s,nValueLen:%d, paramValue:%s \n",pParam->nModelID,
			pParam->szParamName, pParam->nParamValueLen, pParam->szParamValue);
		strcat(szBuffer, szTmp);
	}
}

unsigned long CSimuM_CSetSimuModelParamCmd::CalcFrameBodyLen()
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

void CSimuM_CSetSimuModelParamCmd::SetModelParam(vector<CommuSetModelParam_t *> &vecModelParam)
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
		pNewParam->nParamValueLen = pParam->nParamValueLen;
		memcpy(pNewParam->szParamName, pParam->szParamName, strlen((char *)pParam->szParamName));
		memcpy(pNewParam->szParamValue, pParam->szParamValue, pParam->nParamValueLen);
		m_vecModelParam.push_back(pNewParam);
	}
}

const vector<CommuSetModelParam_t *> *CSimuM_CSetSimuModelParamCmd::GetModelParam()
{
	return &m_vecModelParam;
}

////////////////////////////////////////////////////////////////////////////////////////////

/****************************************************************
* 类名 : CSimuM_CRegisterSimuModelCmd
* 父类 : CSimuM_CCmd
* 说明 : 1,注册仿真模型请求命令
 ****************************************************************/
CSimuM_CRegisterSimuModelCmd::CSimuM_CRegisterSimuModelCmd()
	: CSimuM_CCmd(M_C_CMD_RegisterModel)
{
}

CSimuM_CRegisterSimuModelCmd::~CSimuM_CRegisterSimuModelCmd()
{
}

unsigned long CSimuM_CRegisterSimuModelCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuM_CCmd::Serialize(szBuffer);
	return nLen;
}

BOOL CSimuM_CRegisterSimuModelCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuM_CCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;
	
	return TRUE;
}

void CSimuM_CRegisterSimuModelCmd::Print(char* szBuffer)
{
	szBuffer[0] = '\0';
	CSimuM_CCmd::Print(szBuffer);
}

unsigned long CSimuM_CRegisterSimuModelCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;
	return nLen;
}

/****************************************************************
 * 类名 : CSimuM_CTestEventReportCmd
 * 父类 : CSimuM_CCmd
 * 说明 : 2,测试事件上报
 ****************************************************************/
CSimuM_CTestEventReportCmd::CSimuM_CTestEventReportCmd()
	: CSimuM_CCmd(M_C_CMD_TestEventReport)
{
	memset(m_Report.szEventValue, 0x0 , sizeof(m_Report.szEventValue));
}

CSimuM_CTestEventReportCmd::~CSimuM_CTestEventReportCmd()
{
}

unsigned long CSimuM_CTestEventReportCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuM_CCmd::Serialize(szBuffer);

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

BOOL CSimuM_CTestEventReportCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuM_CCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = M_C_CMD_HEADER_SIZE;
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

void CSimuM_CTestEventReportCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuM_CCmd::Print(szBuffer);

	sprintf(szTmp, "Type:%d , TaskId:%d, caseId:%d nLen:%d, value:%s\n",
		m_Report.nReportType,m_Report.nTestTaskId, m_Report.nTestCaseId,
		m_Report.nEventValueLen, m_Report.szEventValue);
	strcat(szBuffer, szTmp);
}

unsigned long CSimuM_CTestEventReportCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;
	
	nLen += 4 + 4 + 4;
	nLen += 4 + m_Report.nEventValueLen;

	return nLen;
}

void CSimuM_CTestEventReportCmd::SetReport(CommuTestEventReport_t *pReport)
{
	m_Report.nReportType = pReport->nReportType;
	m_Report.nTestTaskId = pReport->nTestTaskId;
	m_Report.nTestCaseId = pReport->nTestCaseId;
	m_Report.nEventValueLen = pReport->nEventValueLen;
	memcpy(m_Report.szEventValue, pReport->szEventValue, pReport->nEventValueLen);
}

const CommuTestEventReport_t *CSimuM_CTestEventReportCmd::GetReport()
{
	return &m_Report;
}

/****************************************************************
 * 类名 : CSimuM_CExceptEventReportCmd
 * 父类 : CSimuM_CCmd
 * 说明 : 3,异常事件上报
 ****************************************************************/
CSimuM_CExceptEventReportCmd::CSimuM_CExceptEventReportCmd()
	: CSimuM_CCmd(M_C_CMD_ExceptEventReport)
{
	memset(m_Report.szReportValue, 0x0 , sizeof(m_Report.szReportValue));
}

CSimuM_CExceptEventReportCmd::~CSimuM_CExceptEventReportCmd()
{

}

unsigned long CSimuM_CExceptEventReportCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuM_CCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_Report.nReportType, 4);
	nLen += 4;

	return nLen;
}

BOOL CSimuM_CExceptEventReportCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuM_CCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = M_C_CMD_HEADER_SIZE;
	memcpy(&m_Report.nReportType, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_Report.nReportLen, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(m_Report.szReportValue, szBuffer + nBodyPos, m_Report.nReportLen);
	nBodyPos += m_Report.nReportLen;

	return TRUE;
}

void CSimuM_CExceptEventReportCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuM_CCmd::Print(szBuffer);

	sprintf(szTmp, "Type:%d , nLen:%d, value:%s\n",
		m_Report.nReportType, m_Report.nReportLen, m_Report.szReportValue);
	strcat(szBuffer, szTmp);
}

unsigned long CSimuM_CExceptEventReportCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen += 4 + 4 + m_Report.nReportLen;

	return nLen;
}

void CSimuM_CExceptEventReportCmd::SetReport(CommuExceptReport_t *pReport)
{
	m_Report.nReportType = pReport->nReportType;
	m_Report.nReportLen = pReport->nReportLen;
	memcpy(m_Report.szReportValue, pReport->szReportValue, pReport->nReportLen);
}

const CommuExceptReport_t *CSimuM_CExceptEventReportCmd::GetReport()
{
	return &m_Report;
}
#endif
