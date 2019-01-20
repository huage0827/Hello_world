/*****************************************************************
 * �ļ���: SimuS_GCmd.cpp
 * ����: SimuService��SimuGuard֮��ͨ������ʵ��
 *
 *****************************************************************/
#include "stdafx.h"
#include "SimuS_GCmd.h"
#include "Util.h"
#include <string.h>

using namespace std;

#define MAX_PRINT_BUFFER_LEN 8192

/*****************************************************************
 *  ��������	CSimuS_GCmd::CSimuS_GCmd
 *  ���ܣ�		SimuService��SimuGuard֮��ͨ�ŵ���������ĸ��๹�캯��
 *  ���:		S_G_CMD sgc -- ������
 *  ����:		��
 *  ����ֵ:		��
 *****************************************************************/
CSimuS_GCmd::CSimuS_GCmd(S_G_CMD sgc)
{
	m_sgc = sgc;
	memset(m_szVersion , 0 , sizeof(m_szVersion));
	m_szVersion[0] = 0;
	m_szVersion[2] = 1;
	m_szVersion[3] = 0;
	m_szVersion[4] = 0;
	m_nFrameLen = 0;
	m_nCmd = sgc;
	m_nFrameBodyLen = 0;
	memset(m_szSimuProjectID , 0 , sizeof(m_szSimuProjectID));
	memset(m_szSimuModelID , 0 , sizeof(m_szSimuModelID));
	memset(m_szReserved2 , 0 , sizeof(m_szReserved2));
	memset(m_szReserved3 , 0 , sizeof(m_szReserved3));
	memset(m_szReserved4 , 0 , sizeof(m_szReserved4));
}

/*****************************************************************
 *  ��������	CSimuS_GCmd::~CSimuS_GCmd
 *  ���ܣ�		SimuService��SimuGuard֮��ͨ�ŵ���������ĸ�����������
 *  ���:		S_G_CMD sgc -- ������
 *  ����:		��
 *  ����ֵ:		��
 *****************************************************************/
CSimuS_GCmd::~CSimuS_GCmd()
{

}

/*****************************************************************
 *  ��������CSimuS_GCmd::PreParseCmd
 *  ���ܣ�  Ԥ�������� , �����Ӧ������
 *  ���:   unsigned char* szBuffer -- ���ݻ�����
 *			unsigned long nBufferLen -- ���ݻ���������
 *  ����:	unsigned long& nCmdDataLen -- ����ʵ�ʵĳ���
 *  ����ֵ:	Ԥ������õ�������
 *****************************************************************/
S_G_CMD CSimuS_GCmd::PreParseCmd(unsigned char* szBuffer , unsigned long nBufferLen , unsigned long& nCmdDataLen)
{
	S_G_CMD sgc = S_G_CMD_UNKNOWN;

	if( nBufferLen < S_G_CMD_HEADER_SIZE )
		return S_G_CMD_UNKNOWN;

	memcpy((void*)&nCmdDataLen , szBuffer + S_G_CMD_POS_FRAMELEN , S_G_CMD_SIZE_FRAMELEN);

	if( nBufferLen < nCmdDataLen )
		return S_G_CMD_UNKNOWN;

	memcpy((void*)&sgc , szBuffer + S_G_CMD_POS_COMMAND , S_G_CMD_SIZE_COMMAND);

	return sgc;
}

/*****************************************************************
 *  ��������CSimuS_GCmd::GetCmdType
 *  ���ܣ�  ��ȡ�������������
 *  ���:   ��
 *  ����ֵ:	������
 *****************************************************************/
S_G_CMD CSimuS_GCmd::GetCmdType() const
{
	return m_sgc;
}

/*****************************************************************
 *  ��������CSimuS_GCmd::SetSimuProjectID
 *  ���ܣ�  ���ø����������ķ��湤��ID��
 *  ���:   const char* szId -- ���湤��ID��
 *  ����:	��
 *  ����ֵ:	��
 *****************************************************************/
void CSimuS_GCmd::SetSimuProjectID(const unsigned char* szId)
{
	memset(m_szSimuProjectID , 0 , sizeof(m_szSimuProjectID));
	memcpy(m_szSimuProjectID , szId , sizeof(m_szSimuProjectID));
}

/*****************************************************************
 *  ��������CSimuS_GCmd::GetFrameLen
 *  ���ܣ�  ��ȡ����֡����
 *  ���:   ��
 *  ����:	��
 *  ����ֵ:	����֡����
 *****************************************************************/
unsigned long CSimuS_GCmd::GetFrameLen()
{
	return m_nFrameLen;
}

/*****************************************************************
 *  ��������CSimuS_GCmd::GetFrameBodyLen
 *  ���ܣ�  ��ȡ����֡�峤��
 *  ���:   ��
 *  ����:	��
 *  ����ֵ:	����֡�峤��
 *****************************************************************/
unsigned long CSimuS_GCmd::GetFrameBodyLen()
{
	return m_nFrameBodyLen;
}

/*****************************************************************
 *  ��������CSimuS_GCmd::GetSimuProjectID
 *  ���ܣ�  ��ȡ�����������ķ��湤��ID��
 *  ���:   ��
 *  ����:	char* szId -- ���������ķ��湤��ID��
 *  ����ֵ:	��
 *****************************************************************/
void CSimuS_GCmd::GetSimuProjectID(unsigned char* szId)
{
	if( szId == NULL )
		return ;
	memcpy(szId , m_szSimuProjectID , sizeof(m_szSimuProjectID));
}


void CSimuS_GCmd::SetSimuModelID(const unsigned char *szModelID)
{
	if( szModelID == NULL )
		return ;
	memcpy(m_szSimuModelID , szModelID , sizeof(m_szSimuModelID));
}

void CSimuS_GCmd::GetSimuModelID(unsigned char *szModelID)
{
	if( szModelID == NULL )
		return ;
	memcpy(szModelID , m_szSimuModelID , sizeof(m_szSimuModelID));
}
/*****************************************************************
 *  ��������CSimuS_GCmd::SerializeHeader
 *  ���ܣ�  ���л�����֡ͷ
 *  ���:   ��
 *  ����:	unsigned char* szBuffer -- ���л��ɹ�������ݴ�Ż�����
 *  ����ֵ:	���л��ɹ�������ݴ�Ż�������С
 *****************************************************************/
unsigned long CSimuS_GCmd::SerializeHeader(unsigned char* szBuffer)
{
	memcpy(szBuffer , m_szVersion , S_G_CMD_SIZE_VERSION);
	memcpy(szBuffer + S_G_CMD_POS_FRAMELEN , (void*)&m_nFrameLen , S_G_CMD_SIZE_FRAMELEN);
	memcpy(szBuffer + S_G_CMD_POS_COMMAND , (void*)&m_nCmd , S_G_CMD_SIZE_COMMAND);
	memcpy(szBuffer + S_G_CMD_POS_FRAMEBODYLEN , (void*)&m_nFrameBodyLen , S_G_CMD_SIZE_FRAMEBODYLEN);
	memcpy(szBuffer + S_G_CMD_POS_SIMUPROJECTID , m_szSimuProjectID , S_G_CMD_SIZE_SIMUPROJECTID);
	memcpy(szBuffer + S_G_CMD_POS_SIMUMODELID , m_szSimuModelID , S_G_CMD_SIZE_SIMUMODELID);
	memcpy(szBuffer + S_G_CMD_POS_RESERVED2 , m_szReserved2 , S_G_CMD_SIZE_RESERVED2);
	memcpy(szBuffer + S_G_CMD_POS_RESERVED3 , m_szReserved3 , S_G_CMD_SIZE_RESERVED3);
	memcpy(szBuffer + S_G_CMD_POS_RESERVED4 , m_szReserved4 , S_G_CMD_SIZE_RESERVED4);
	return S_G_CMD_HEADER_SIZE;
}

/*****************************************************************
 *  ��������CSimuS_GCmd::Serialize
 *  ���ܣ�  ���л�����
 *  ���:   ��
 *  ����:	unsigned char* szBuffer -- ���л��ɹ�������ݴ�Ż�����
 *  ����ֵ:	���л��ɹ�������ݴ�Ż�������С
 *****************************************************************/
unsigned long CSimuS_GCmd::Serialize(unsigned char* szBuffer)
{
	m_nFrameBodyLen = CalcFrameBodyLen();
	m_nFrameLen = CalcFrameLen();

	return SerializeHeader(szBuffer);
}

/*****************************************************************
 *  ��������CSimuS_GCmd::Unserialize
 *  ���ܣ�  �����л����������
 *  ���:   unsigned char* szBuffer -- ��Ҫ�����л������ݻ�����
 *			unsigned long nBufferLen -- ��Ҫ�����л������ݻ�������С
 *  ����:	��
 *  ����ֵ:	TRUE -- �ɹ� , FALSE -- ʧ��
 *****************************************************************/
BOOL CSimuS_GCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( nBufferLen < S_G_CMD_HEADER_SIZE )
	{
		return FALSE;
	}
	memcpy(m_szVersion , szBuffer + S_G_CMD_POS_VERSION , S_G_CMD_SIZE_VERSION);
	memcpy((void*)&m_nFrameLen , szBuffer + S_G_CMD_POS_FRAMELEN , S_G_CMD_SIZE_FRAMELEN);
	memcpy((void*)&m_nCmd , szBuffer + S_G_CMD_POS_COMMAND , S_G_CMD_SIZE_COMMAND);
	memcpy((void*)&m_nFrameBodyLen , szBuffer + S_G_CMD_POS_FRAMEBODYLEN , S_G_CMD_SIZE_FRAMEBODYLEN);
	memcpy(m_szSimuProjectID , szBuffer + S_G_CMD_POS_SIMUPROJECTID , S_G_CMD_SIZE_SIMUPROJECTID);
	memcpy(m_szSimuModelID , szBuffer + S_G_CMD_POS_SIMUMODELID , S_G_CMD_SIZE_SIMUMODELID);
	memcpy(m_szReserved2 , szBuffer + S_G_CMD_POS_RESERVED2 , S_G_CMD_SIZE_RESERVED2);
	memcpy(m_szReserved3 , szBuffer + S_G_CMD_POS_RESERVED3 , S_G_CMD_SIZE_RESERVED3);
	memcpy(m_szReserved4 , szBuffer + S_G_CMD_POS_RESERVED4 , S_G_CMD_SIZE_RESERVED4);
	return TRUE;
}

void CSimuS_GCmd::Print(char* szBuffer)
{
	char szTmp[256];
	char szStr[128];

	switch( m_nCmd )
	{
	case S_G_CMD_UNKNOWN	 :
		sprintf(szTmp ,  "Cmd: S_G_CMD_UNKNOWN");break;
	case S_G_CMD_LOADSIMUMODEL	 :
		sprintf(szTmp ,  "Cmd: S_G_CMD_LOADSIMUMODEL");break;
	case S_G_CMD_LOADSIMUMODEL_RESP	 :
		sprintf(szTmp ,  "Cmd: S_G_CMD_LOADSIMUMODEL_RESP");break;
	case S_G_CMD_EXECSIMUMODEL	 :
		sprintf(szTmp ,  "Cmd: S_G_CMD_EXECSIMUMODEL");break;
	case S_G_CMD_EXECSIMUMODEL_RESP	 :
		sprintf(szTmp ,  "Cmd: S_G_CMD_EXECSIMUMODEL_RESP");break;
	case S_G_CMD_PAUSESIMUMODEL	 :
		sprintf(szTmp ,  "Cmd: S_G_CMD_PAUSESIMUMODEL");break;
	case S_G_CMD_PAUSESIMUMODEL_RESP	 :
		sprintf(szTmp ,  "Cmd: S_G_CMD_PAUSESIMUMODEL_RESP");break;
	case S_G_CMD_RESETSIMUMODEL	 :
		sprintf(szTmp ,  "Cmd: S_G_CMD_RESETSIMUMODEL");break;
	case S_G_CMD_RESETSIMUMODEL_RESP	 :
		sprintf(szTmp ,  "Cmd: S_G_CMD_RESETSIMUMODEL_RESP");break;
	case S_G_CMD_SETMODELPARAM	 :
		sprintf(szTmp ,  "Cmd: S_G_CMD_SETMODELPARAM");break;
	case S_G_CMD_SETMODELPARAM_RESP	 :
		sprintf(szTmp ,  "Cmd: S_G_CMD_SETMODELPARAM_RESP");break;
	case S_G_CMD_TESTEVENT_REPORT	 :
		sprintf(szTmp ,  "Cmd: S_G_CMD_TESTEVENT_REPORT");break;
	case S_G_CMD_EXCEPTEVENT_REPORT	 :
		sprintf(szTmp ,  "Cmd: S_G_CMD_EXCEPTEVENT_REPORT");break;
	case S_G_CMD_COMPILESIMUMODEL	 :
		sprintf(szTmp ,  "Cmd: S_G_CMD_COMPILESIMUMODEL");break;
	case S_G_CMD_COMPILESIMUMODEL_RESP	 :
		sprintf(szTmp ,  "Cmd: S_G_CMD_COMPILESIMUMODEL_RESP");break;
	case S_G_CMD_COMPILEINFO_REPORT	 :
		sprintf(szTmp ,  "Cmd: S_G_CMD_COMPILEINFO_REPORT");break;
	default :
		break;
	}
	strcat(szBuffer, szTmp);

	sprintf(szTmp, ", FrameLen: %ld" , m_nFrameLen);
	strcat(szBuffer, szTmp);

	sprintf(szTmp, ", FrameBodyLen:%ld \n" , m_nFrameBodyLen);
	strcat(szBuffer, szTmp);

	memset(szStr, 0x0, sizeof(szStr));
	memcpy(szStr, m_szSimuProjectID, S_G_CMD_SIZE_SIMUPROJECTID);
	sprintf(szTmp, "ProjID=%s," , szStr);
	strcat(szBuffer, szTmp);

	memset(szStr, 0x0, sizeof(szStr));
	memcpy(szStr, m_szSimuModelID, S_G_CMD_SIZE_SIMUMODELID);
	sprintf(szTmp, "ModelID=%s \n" , szStr);
	strcat(szBuffer, szTmp);

	return ;
}

unsigned long CSimuS_GCmd::CalcFrameLen()
{
	return S_G_CMD_HEADER_SIZE + CalcFrameBodyLen();
}


/****************************************************************
 * ���� : 1.CSimuS_GLoadSimuModelCmd
 * ���� : CSimuS_GCmd
 * ˵�� : ���ط���ģ����������
 ****************************************************************/
CSimuS_GLoadSimuModelCmd::CSimuS_GLoadSimuModelCmd()
	: CSimuS_GCmd(S_G_CMD_LOADSIMUMODEL)
{
	memset(m_CommuParam.szDebugParam, 0x0, sizeof(m_CommuParam.szDebugParam));
	memset(m_CommuParam.szDriverLibPath, 0x0, sizeof(m_CommuParam.szDriverLibPath));
	memset(m_CommuParam.szModelConfPath, 0x0, sizeof(m_CommuParam.szModelConfPath));
	memset(m_CommuParam.szMonitorIp, 0x0, sizeof(m_CommuParam.szMonitorIp));
}

CSimuS_GLoadSimuModelCmd::~CSimuS_GLoadSimuModelCmd()
{
	for(unsigned int i = 0; i < m_CommuParam.vecModelFilePath.size(); i++)
		delete m_CommuParam.vecModelFilePath.at(i);
	m_CommuParam.vecModelFilePath.clear();
}

unsigned long CSimuS_GLoadSimuModelCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuS_GCmd::Serialize(szBuffer);
	CommuModelProgFilePath_t *pModelPath = NULL;

	memcpy(szBuffer + nLen, m_CommuParam.szMonitorIp, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_CommuParam.usPort, 2);
	nLen += 2;
	memcpy(szBuffer + nLen, &m_CommuParam.bDebugMode, 1);
	nLen += 1;
	memcpy(szBuffer + nLen, &m_CommuParam.usDebugParamLen, 2);
	nLen += 2;
	memcpy(szBuffer + nLen, m_CommuParam.szDebugParam, m_CommuParam.usDebugParamLen);
	nLen += m_CommuParam.usDebugParamLen;
	memcpy(szBuffer + nLen, &m_CommuParam.usModelConfPathLen, 2);
	nLen += 2;
	memcpy(szBuffer + nLen, m_CommuParam.szModelConfPath, m_CommuParam.usModelConfPathLen);
	nLen += m_CommuParam.usModelConfPathLen;
	memcpy(szBuffer + nLen, &m_CommuParam.usDriverLibPathLen, 2);
	nLen += 2;
	memcpy(szBuffer + nLen, m_CommuParam.szDriverLibPath, m_CommuParam.usDriverLibPathLen);
	nLen += m_CommuParam.usDriverLibPathLen;
	memcpy(szBuffer + nLen, &m_CommuParam.nModelCount, 4);
	nLen += 4;
	for(unsigned int i = 0; i < m_CommuParam.vecModelFilePath.size(); i++){
		pModelPath = m_CommuParam.vecModelFilePath.at(i);
		memcpy(szBuffer + nLen, &pModelPath->nModelId, 4);
		nLen += 4;
		memcpy(szBuffer + nLen, &pModelPath->nFilePathLen, 2);
		nLen += 2;
		memcpy(szBuffer + nLen, pModelPath->szFilePath, pModelPath->nFilePathLen);
		nLen += pModelPath->nFilePathLen;
	}
	return nLen;
}

BOOL CSimuS_GLoadSimuModelCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuS_GCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	CommuModelProgFilePath_t *pModelPath = NULL;

	int nBodyPos = S_G_CMD_HEADER_SIZE;
	memcpy(m_CommuParam.szMonitorIp , szBuffer + nBodyPos , 4);
	nBodyPos += 4;
	memcpy(&m_CommuParam.usPort , szBuffer + nBodyPos , 2);
	nBodyPos += 2;
	memcpy(&m_CommuParam.bDebugMode, szBuffer + nBodyPos, 1);
	nBodyPos += 1;
	memcpy(&m_CommuParam.usDebugParamLen, szBuffer + nBodyPos, 2);
	nBodyPos += 2;
	memcpy(m_CommuParam.szDebugParam, szBuffer + nBodyPos, m_CommuParam.usDebugParamLen);
	nBodyPos += m_CommuParam.usDebugParamLen;
	memcpy(&m_CommuParam.usModelConfPathLen, szBuffer + nBodyPos, 2);
	nBodyPos += 2;
	memcpy(m_CommuParam.szModelConfPath, szBuffer + nBodyPos, m_CommuParam.usModelConfPathLen);
	nBodyPos += m_CommuParam.usModelConfPathLen;
	memcpy(&m_CommuParam.usDriverLibPathLen, szBuffer + nBodyPos, 2);
	nBodyPos += 2;
	memcpy(m_CommuParam.szDriverLibPath, szBuffer + nBodyPos, m_CommuParam.usDriverLibPathLen);
	nBodyPos += m_CommuParam.usDriverLibPathLen;
	memcpy(&m_CommuParam.nModelCount, szBuffer + nBodyPos, 4);
	nBodyPos += 4;

	for(unsigned int i = 0; i < m_CommuParam.nModelCount; i++){
		pModelPath = new CommuModelProgFilePath_t;
		memset(pModelPath->szFilePath, 0x0, sizeof(pModelPath->szFilePath));

		memcpy(&pModelPath->nModelId, szBuffer + nBodyPos, 4);
		nBodyPos += 4;
		memcpy(&pModelPath->nFilePathLen, szBuffer + nBodyPos, 2);
		nBodyPos += 2;
		memcpy(&pModelPath->szFilePath, szBuffer + nBodyPos, pModelPath->nFilePathLen);
		nBodyPos += pModelPath->nFilePathLen;
		m_CommuParam.vecModelFilePath.push_back(pModelPath);
	}
	return TRUE;
}

void CSimuS_GLoadSimuModelCmd::Print(char* szBuffer)
{
	char szTmp[1024];
	CommuModelProgFilePath_t *pModelPath = NULL;

	szBuffer[0] = '\0';
	CSimuS_GCmd::Print(szBuffer);

	sprintf(szTmp,   "Monitor:%d.%d.%d.%d, port:%d \n", (unsigned char )m_CommuParam.szMonitorIp[0],
		(unsigned char )m_CommuParam.szMonitorIp[1],(unsigned char )m_CommuParam.szMonitorIp[2],
		(unsigned char )m_CommuParam.szMonitorIp[3],m_CommuParam.usPort);
	strcat(szBuffer, szTmp);

	sprintf(szTmp,   "Debug: model->%d, nLen:%d, param:%s \n", m_CommuParam.bDebugMode,
		m_CommuParam.usDebugParamLen, m_CommuParam.szDebugParam);
	strcat(szBuffer, szTmp);

	sprintf(szTmp,   "Conf:%s, nLen:%d, Dirver:%s, nLen:%d \n",m_CommuParam.szModelConfPath,
		m_CommuParam.usModelConfPathLen, m_CommuParam.szDriverLibPath, m_CommuParam.usDriverLibPathLen);
	strcat(szBuffer, szTmp);

	sprintf(szTmp,   "ModelCount:%ld \n", m_CommuParam.nModelCount);
	strcat(szBuffer, szTmp);

	for(unsigned int i = 0; i < m_CommuParam.vecModelFilePath.size(); i++){
		pModelPath = m_CommuParam.vecModelFilePath.at(i);

		sprintf(szTmp,   "modelId:%ld, nLen:%lu, path:%s \n", pModelPath->nModelId,
			pModelPath->nFilePathLen, pModelPath->szFilePath);
		strcat(szBuffer, szTmp);
	}
}

unsigned long CSimuS_GLoadSimuModelCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;
	CommuModelProgFilePath_t *pModelPath = NULL;

	nLen += 4 + 2;
	nLen += 1 + 2 + m_CommuParam.usDebugParamLen;
	nLen += 2 + m_CommuParam.usModelConfPathLen;
	nLen += 2 + m_CommuParam.usDriverLibPathLen;
	nLen += 4;
	for(unsigned int i = 0; i < m_CommuParam.vecModelFilePath.size(); i++){
		pModelPath = m_CommuParam.vecModelFilePath.at(i);
		nLen += 4 + 2 + pModelPath->nFilePathLen;
	}
	return nLen;
}

void CSimuS_GLoadSimuModelCmd::SetParam(const CommuLoadSimuModelParam_t *pParam)
{
	CommuModelProgFilePath_t *pModelPath = NULL;
	CommuModelProgFilePath_t *pNewModelPath = NULL;

	memcpy(m_CommuParam.szMonitorIp, pParam->szMonitorIp, 4);
	m_CommuParam.usPort = pParam->usPort;
	m_CommuParam.bDebugMode = pParam->bDebugMode;
	m_CommuParam.usDebugParamLen = pParam->usDebugParamLen;
	memcpy(m_CommuParam.szDebugParam, pParam->szDebugParam, pParam->usDebugParamLen);
	m_CommuParam.usModelConfPathLen = pParam->usModelConfPathLen;
	memcpy(m_CommuParam.szModelConfPath, pParam->szModelConfPath, pParam->usModelConfPathLen);
	m_CommuParam.usDriverLibPathLen = pParam->usDriverLibPathLen;
	memcpy(m_CommuParam.szDriverLibPath, pParam->szDriverLibPath, pParam->usDriverLibPathLen);
	m_CommuParam.nModelCount = pParam->vecModelFilePath.size();
	for(unsigned int i = 0; i < pParam->vecModelFilePath.size(); i++){
		pNewModelPath = new CommuModelProgFilePath_t;
		memset(pNewModelPath->szFilePath, 0x0, sizeof(pNewModelPath->szFilePath));

		pModelPath = pParam->vecModelFilePath.at(i);
		pNewModelPath->nModelId = pModelPath->nModelId;
		pNewModelPath->nFilePathLen = pModelPath->nFilePathLen;
		memcpy(pNewModelPath->szFilePath, pModelPath->szFilePath, pModelPath->nFilePathLen);
		m_CommuParam.vecModelFilePath.push_back(pNewModelPath);
	}
}

const CommuLoadSimuModelParam_t *CSimuS_GLoadSimuModelCmd::GetParam()
{
	return &m_CommuParam;
}

/****************************************************************
 * ���� : 2.CSimuS_GLoadSimuModelRespCmd
 * ���� : CSimuS_GCmd
 * ˵�� : ���ط���ģ����Ӧ����
 ****************************************************************/
CSimuS_GLoadSimuModelRespCmd::CSimuS_GLoadSimuModelRespCmd()
	: CSimuS_GCmd(S_G_CMD_LOADSIMUMODEL_RESP)
{
	m_ulResult = 0;
	m_ulErrInfoLen = 0;
	memset(m_szErrInfo, 0x0, sizeof(m_szErrInfo));
}

CSimuS_GLoadSimuModelRespCmd::~CSimuS_GLoadSimuModelRespCmd()
{

}

unsigned long CSimuS_GLoadSimuModelRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuS_GCmd::Serialize(szBuffer);

	memcpy(szBuffer +  nLen, &m_ulResult, sizeof(unsigned long));
	nLen += sizeof(unsigned long);
	memcpy(szBuffer + nLen, &m_ulErrInfoLen, sizeof(unsigned long));
	nLen += 4;
	memcpy(szBuffer + nLen, m_szErrInfo, m_ulErrInfoLen);
	nLen += m_ulErrInfoLen;

	return nLen;
}

BOOL CSimuS_GLoadSimuModelRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuS_GCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	unsigned nPos = S_G_CMD_HEADER_SIZE;
	memcpy(&m_ulResult , szBuffer + nPos , sizeof(unsigned long));
	nPos += 4;
	memcpy(&m_ulErrInfoLen, szBuffer + nPos, 4);
	nPos += 4;
	memcpy(m_szErrInfo, szBuffer + nPos, m_ulErrInfoLen);
	nPos += m_ulErrInfoLen;

	return TRUE;
}

void CSimuS_GLoadSimuModelRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuS_GCmd::Print(szBuffer);

	sprintf(szTmp ,"%d,%d,info:%s\n" , m_ulResult, m_ulErrInfoLen, m_szErrInfo);
	strcat(szBuffer, szTmp);
}

unsigned long CSimuS_GLoadSimuModelRespCmd::CalcFrameBodyLen()
{
	return 4 + 4 + m_ulErrInfoLen;
}

void CSimuS_GLoadSimuModelRespCmd::SetResult(unsigned long ret)
{
	m_ulResult = ret;
}

unsigned long CSimuS_GLoadSimuModelRespCmd::GetResult()
{
	return m_ulResult;
}

void CSimuS_GLoadSimuModelRespCmd::SetErrInfo(unsigned char *szErrInfo, unsigned long nErrLen)
{
	memcpy(m_szErrInfo, szErrInfo, nErrLen);
	m_ulErrInfoLen = nErrLen;
}

int  CSimuS_GLoadSimuModelRespCmd::GetErrInfo(unsigned char *szBuffer)
{
	strcpy((char *)szBuffer, (char *)m_szErrInfo);
	return strlen((char *)m_szErrInfo);
}

/****************************************************************
 * ���� : 3.CSimuS_GExecSimuModelCmd
 * ���� : CSimuS_GCmd
 * ˵�� : ִ�з���ģ��
 ****************************************************************/
CSimuS_GExecSimuModelCmd::CSimuS_GExecSimuModelCmd()
	: CSimuS_GCmd(S_G_CMD_EXECSIMUMODEL)
{
	m_nExecMode = 0;
}

CSimuS_GExecSimuModelCmd::~CSimuS_GExecSimuModelCmd()
{

}

unsigned long CSimuS_GExecSimuModelCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuS_GCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_nExecMode, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_nTestTask, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_nTestCase, 4);
	nLen += 4;

	return nLen;
}

BOOL CSimuS_GExecSimuModelCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuS_GCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = S_G_CMD_HEADER_SIZE;
	memcpy(&m_nExecMode , szBuffer + nBodyPos , 4);
	nBodyPos += 4;
	memcpy(&m_nTestTask, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_nTestCase, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	
	return TRUE;
}

void CSimuS_GExecSimuModelCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuS_GCmd::Print(szBuffer);

	sprintf(szTmp,   "nExecMode:%ld , TestTask:%ld, TestCase:%ld\n", m_nExecMode, m_nTestTask, m_nTestCase);
	strcat(szBuffer, szTmp);
}

unsigned long CSimuS_GExecSimuModelCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 4 * 3;

	return nLen;
}

void CSimuS_GExecSimuModelCmd::SetData(int nExecMode, int TestTask, int TestCase)
{
	m_nExecMode = nExecMode;
	m_nTestTask = TestTask;
	m_nTestCase = TestCase;
}

unsigned long CSimuS_GExecSimuModelCmd::GetExecMode()
{
	return m_nExecMode;
}

unsigned long CSimuS_GExecSimuModelCmd::GetTestTask()
{
	return m_nTestTask;
}

unsigned long CSimuS_GExecSimuModelCmd::GetTestCase()
{
	return m_nTestCase;
}

/****************************************************************
 * ���� : 4.CSimuS_GExecSimuModelRespCmd
 * ���� : CSimuS_GCmd
 * ˵�� : ִ�з���ģ����Ӧ����
 ****************************************************************/
CSimuS_GExecSimuModelRespCmd::CSimuS_GExecSimuModelRespCmd()
	: CSimuS_GCmd(S_G_CMD_EXECSIMUMODEL_RESP)
{
	m_ulResult = 0;
}

CSimuS_GExecSimuModelRespCmd::~CSimuS_GExecSimuModelRespCmd()
{

}

unsigned long CSimuS_GExecSimuModelRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuS_GCmd::Serialize(szBuffer);
	memcpy(szBuffer + S_G_CMD_HEADER_SIZE , &m_ulResult, sizeof(unsigned long));
	nLen += sizeof(unsigned long);
	return nLen;
}

BOOL CSimuS_GExecSimuModelRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuS_GCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;
	memcpy(&m_ulResult , szBuffer + S_G_CMD_HEADER_SIZE , sizeof(unsigned long));
	return TRUE;
}

void CSimuS_GExecSimuModelRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuS_GCmd::Print(szBuffer);

	sprintf(szTmp, "%ld\n" , m_ulResult);
	strcat(szBuffer, szTmp);
}

unsigned long CSimuS_GExecSimuModelRespCmd::CalcFrameBodyLen()
{
	return 4;
}

void CSimuS_GExecSimuModelRespCmd::SetResult(unsigned long ret)
{
	m_ulResult = ret;
}

unsigned long CSimuS_GExecSimuModelRespCmd::GetResult()
{
	return m_ulResult;
}

/****************************************************************
 * ���� : 5.CSimuS_GPauseSimuModelCmd
 * ���� : CSimuS_GCmd
 * ˵�� : ��ͣ����ģ��
 ****************************************************************/
CSimuS_GPauseSimuModelCmd::CSimuS_GPauseSimuModelCmd()
	: CSimuS_GCmd(S_G_CMD_PAUSESIMUMODEL)
{
}

CSimuS_GPauseSimuModelCmd::~CSimuS_GPauseSimuModelCmd()
{

}

unsigned long CSimuS_GPauseSimuModelCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuS_GCmd::Serialize(szBuffer);

	return nLen;
}

BOOL CSimuS_GPauseSimuModelCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuS_GCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	return TRUE;
}

void CSimuS_GPauseSimuModelCmd::Print(char* szBuffer)
{
	szBuffer[0] = '\0';
	CSimuS_GCmd::Print(szBuffer);
}

unsigned long CSimuS_GPauseSimuModelCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	return nLen;
}

/****************************************************************
 * ���� : 6.CSimuS_GPauseSimuModelRespCmd
 * ���� : CSimuS_GCmd
 * ˵�� : ��ͣ����ģ����Ӧ����
 ****************************************************************/
CSimuS_GPauseSimuModelRespCmd::CSimuS_GPauseSimuModelRespCmd()
	: CSimuS_GCmd(S_G_CMD_PAUSESIMUMODEL_RESP)
{
	m_ulResult = 0;
}

CSimuS_GPauseSimuModelRespCmd::~CSimuS_GPauseSimuModelRespCmd()
{

}

unsigned long CSimuS_GPauseSimuModelRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuS_GCmd::Serialize(szBuffer);
	memcpy(szBuffer + S_G_CMD_HEADER_SIZE , &m_ulResult, sizeof(unsigned long));
	nLen += sizeof(unsigned long);
	return nLen;
}

BOOL CSimuS_GPauseSimuModelRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuS_GCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;
	memcpy(&m_ulResult , szBuffer + S_G_CMD_HEADER_SIZE , sizeof(unsigned long));
	return TRUE;
}

void CSimuS_GPauseSimuModelRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuS_GCmd::Print(szBuffer);

	sprintf(szTmp, "%ld\n" , m_ulResult);
	strcat(szBuffer, szTmp);
}

unsigned long CSimuS_GPauseSimuModelRespCmd::CalcFrameBodyLen()
{
	return 4;
}

void CSimuS_GPauseSimuModelRespCmd::SetResult(unsigned long ret)
{
	m_ulResult = ret;
}

unsigned long CSimuS_GPauseSimuModelRespCmd::GetResult()
{
	return m_ulResult;
}

/****************************************************************
 * ���� : 7.CSimuS_GResetSimuModelCmd
 * ���� : CSimuS_GCmd
 * ˵�� : ���÷���ģ��
 ****************************************************************/
CSimuS_GResetSimuModelCmd::CSimuS_GResetSimuModelCmd()
	: CSimuS_GCmd(S_G_CMD_RESETSIMUMODEL)
{
}

CSimuS_GResetSimuModelCmd::~CSimuS_GResetSimuModelCmd()
{

}

unsigned long CSimuS_GResetSimuModelCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuS_GCmd::Serialize(szBuffer);

	return nLen;
}

BOOL CSimuS_GResetSimuModelCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuS_GCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	return TRUE;
}

void CSimuS_GResetSimuModelCmd::Print(char* szBuffer)
{
	szBuffer[0] = '\0';
	CSimuS_GCmd::Print(szBuffer);
}

unsigned long CSimuS_GResetSimuModelCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	return nLen;
}
/****************************************************************
 * ���� : 8.CSimuS_GResetSimuModelRespCmd
 * ���� : CSimuS_GCmd
 * ˵�� : ���÷���ģ����Ӧ����
 ****************************************************************/
CSimuS_GResetSimuModelRespCmd::CSimuS_GResetSimuModelRespCmd()
	: CSimuS_GCmd(S_G_CMD_RESETSIMUMODEL_RESP)
{
	m_ulResult = 0;
}

CSimuS_GResetSimuModelRespCmd::~CSimuS_GResetSimuModelRespCmd()
{

}

unsigned long CSimuS_GResetSimuModelRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuS_GCmd::Serialize(szBuffer);
	memcpy(szBuffer + S_G_CMD_HEADER_SIZE , &m_ulResult, sizeof(unsigned long));
	nLen += sizeof(unsigned long);
	return nLen;
}

BOOL CSimuS_GResetSimuModelRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuS_GCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;
	memcpy(&m_ulResult , szBuffer + S_G_CMD_HEADER_SIZE , sizeof(unsigned long));
	return TRUE;
}

void CSimuS_GResetSimuModelRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuS_GCmd::Print(szBuffer);

	sprintf(szTmp, "%ld\n" , m_ulResult);
	strcat(szBuffer, szTmp);
}

unsigned long CSimuS_GResetSimuModelRespCmd::CalcFrameBodyLen()
{
	return 4;
}

void CSimuS_GResetSimuModelRespCmd::SetResult(unsigned long ret)
{
	m_ulResult = ret;
}

unsigned long CSimuS_GResetSimuModelRespCmd::GetResult()
{
	return m_ulResult;
}

/****************************************************************
 * ���� : 9.CSimuS_GSetSimuModelParamCmd
 * ���� : CSimuS_GCmd
 * ˵�� : ���÷���ģ�Ͳ���
 ****************************************************************/
CSimuS_GSetSimuModelParamCmd::CSimuS_GSetSimuModelParamCmd()
	: CSimuS_GCmd(S_G_CMD_SETMODELPARAM)
{
	m_nModelParamCount = 0;
}

CSimuS_GSetSimuModelParamCmd::~CSimuS_GSetSimuModelParamCmd()
{
	for(unsigned int i = 0; i < m_vecModelParam.size(); i++)
		delete m_vecModelParam.at(i);
	m_vecModelParam.clear();
}

unsigned long CSimuS_GSetSimuModelParamCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuS_GCmd::Serialize(szBuffer);
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

BOOL CSimuS_GSetSimuModelParamCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuS_GCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	CommuSetModelParam_t *pParam = NULL;

	int nBodyPos = S_G_CMD_HEADER_SIZE;
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

void CSimuS_GSetSimuModelParamCmd::Print(char* szBuffer)
{
	char szTmp[1024];
	CommuSetModelParam_t *pParam = NULL;

	szBuffer[0] = '\0';
	CSimuS_GCmd::Print(szBuffer);

	sprintf(szTmp,   "ModelCount:%ld \n",m_nModelParamCount);
	strcat(szBuffer, szTmp);

	for(unsigned int i = 0; i < m_vecModelParam.size(); i++){
		pParam = m_vecModelParam.at(i);

		sprintf(szTmp,   "modelId:%ld,paramId:%d, paramName:%s,nValueLen:%ld, paramValue:%s \n",pParam->nModelID,
			pParam->nParamId, pParam->szParamName, pParam->nParamValueLen, pParam->szParamValue);
		strcat(szBuffer, szTmp);
	}
}

unsigned long CSimuS_GSetSimuModelParamCmd::CalcFrameBodyLen()
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

void CSimuS_GSetSimuModelParamCmd::SetModelParam(const vector<CommuSetModelParam_t *> &vecModelParam)
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

const vector<CommuSetModelParam_t *> *CSimuS_GSetSimuModelParamCmd::GetModelParam()
{
	return &m_vecModelParam;
}

/****************************************************************
 * ���� : 10.CSimuS_GSetSimuModelParamRespCmd
 * ���� : CSimuS_GCmd
 * ˵�� : ���÷���ģ�Ͳ�����Ӧ����
 ****************************************************************/
CSimuS_GSetSimuModelParamRespCmd::CSimuS_GSetSimuModelParamRespCmd()
	: CSimuS_GCmd(S_G_CMD_SETMODELPARAM_RESP)
{
	m_ulResult = 0;
}

CSimuS_GSetSimuModelParamRespCmd::~CSimuS_GSetSimuModelParamRespCmd()
{

}

unsigned long CSimuS_GSetSimuModelParamRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuS_GCmd::Serialize(szBuffer);
	memcpy(szBuffer + S_G_CMD_HEADER_SIZE , &m_ulResult, sizeof(unsigned long));
	nLen += sizeof(unsigned long);
	return nLen;
}

BOOL CSimuS_GSetSimuModelParamRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuS_GCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;
	memcpy(&m_ulResult , szBuffer + S_G_CMD_HEADER_SIZE , sizeof(unsigned long));
	return TRUE;
}

void CSimuS_GSetSimuModelParamRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuS_GCmd::Print(szBuffer);

	sprintf(szTmp, "%d\n" , m_ulResult);
	strcat(szBuffer, szTmp);
}

unsigned long CSimuS_GSetSimuModelParamRespCmd::CalcFrameBodyLen()
{
	return 4;
}

void CSimuS_GSetSimuModelParamRespCmd::SetResult(unsigned long ret)
{
	m_ulResult = ret;
}

unsigned long CSimuS_GSetSimuModelParamRespCmd::GetResult()
{
	return m_ulResult;
}

/****************************************************************
 * ���� : 11.CSimuS_GTestEventReportCmd
 * ���� : CSimuS_GCmd
 * ˵�� : �����¼��ϱ�
 ****************************************************************/
CSimuS_GTestEventReportCmd::CSimuS_GTestEventReportCmd()
	: CSimuS_GCmd(S_G_CMD_TESTEVENT_REPORT)
{
	memset(m_Report.szEventValue, 0x0 , sizeof(m_Report.szEventValue));
}

CSimuS_GTestEventReportCmd::~CSimuS_GTestEventReportCmd()
{
	
}

unsigned long CSimuS_GTestEventReportCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuS_GCmd::Serialize(szBuffer);

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

BOOL CSimuS_GTestEventReportCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuS_GCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = S_G_CMD_HEADER_SIZE;
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

void CSimuS_GTestEventReportCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuS_GCmd::Print(szBuffer);

	sprintf(szTmp, "Type:%d , TaskId:%d, caseId:%d nLen:%d, value:%s\n",
		m_Report.nReportType,m_Report.nTestTaskId, m_Report.nTestCaseId,
		m_Report.nEventValueLen, m_Report.szEventValue);
	strcat(szBuffer, szTmp);
}

unsigned long CSimuS_GTestEventReportCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;
	
	nLen += 4 + 4 + 4;
	nLen += 4 + m_Report.nEventValueLen;

	return nLen;
}

void CSimuS_GTestEventReportCmd::SetReport(const CommuTestEventReport_t *pReport)
{
	m_Report.nReportType = pReport->nReportType;
	m_Report.nTestTaskId = pReport->nTestTaskId;
	m_Report.nTestCaseId = pReport->nTestCaseId;
	m_Report.nEventValueLen = pReport->nEventValueLen;
	memcpy(m_Report.szEventValue, pReport->szEventValue, pReport->nEventValueLen);
}

const CommuTestEventReport_t *CSimuS_GTestEventReportCmd::GetReport()
{
	return &m_Report;
}

/****************************************************************
 * ���� : 12.CSimuS_GExceptEventReportCmd
 * ���� : CSimuS_GCmd
 * ˵�� : �쳣�¼��ϱ�
 ****************************************************************/
CSimuS_GExceptEventReportCmd::CSimuS_GExceptEventReportCmd()
	: CSimuS_GCmd(S_G_CMD_EXCEPTEVENT_REPORT)
{
	memset(m_Report.szReportValue, 0x0 , sizeof(m_Report.szReportValue));
}

CSimuS_GExceptEventReportCmd::~CSimuS_GExceptEventReportCmd()
{

}

unsigned long CSimuS_GExceptEventReportCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuS_GCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_Report.nReportType, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_Report.nReportLen, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, m_Report.szReportValue, m_Report.nReportLen);
	nLen += m_Report.nReportLen;

	return nLen;
}

BOOL CSimuS_GExceptEventReportCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuS_GCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = S_G_CMD_HEADER_SIZE;
	memcpy(&m_Report.nReportType, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_Report.nReportLen, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(m_Report.szReportValue, szBuffer + nBodyPos, m_Report.nReportLen);
	nBodyPos += m_Report.nReportLen;

	return TRUE;
}

void CSimuS_GExceptEventReportCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuS_GCmd::Print(szBuffer);

	sprintf(szTmp,   "Type:%d , nLen:%d, value:%s\n",
		m_Report.nReportType, m_Report.nReportLen, m_Report.szReportValue);
	strcat(szBuffer, szTmp);
}

unsigned long CSimuS_GExceptEventReportCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen += 4 + 4 + m_Report.nReportLen;

	return nLen;
}

void CSimuS_GExceptEventReportCmd::SetReport(const CommuExceptReport_t *pReport)
{
	m_Report.nReportType = pReport->nReportType;
	m_Report.nReportLen = pReport->nReportLen;
	memcpy(m_Report.szReportValue, pReport->szReportValue, pReport->nReportLen);
}

const CommuExceptReport_t *CSimuS_GExceptEventReportCmd::GetReport()
{
	return &m_Report;
}

/****************************************************************
 * ���� : CSimuS_GCompileSimuModelCmd
 * ���� : CSimuS_GCmd
 * ˵�� : ����ģ������
 ****************************************************************/
CSimuS_GCompileSimuModelCmd::CSimuS_GCompileSimuModelCmd()
	: CSimuS_GCmd(S_G_CMD_COMPILESIMUMODEL)
{

}

CSimuS_GCompileSimuModelCmd::~CSimuS_GCompileSimuModelCmd()
{
	for( unsigned i = 0; i < m_CompileParam.vecModelDir.size();i++){
		delete m_CompileParam.vecModelDir.at(i);
	}
	m_CompileParam.vecModelDir.clear();
}

unsigned long CSimuS_GCompileSimuModelCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuS_GCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_CompileParam.usModelCfgPathLen, 2);
	nLen += 2;
	memcpy(szBuffer + nLen, m_CompileParam.szModelCfgPath, m_CompileParam.usModelCfgPathLen);
	nLen += m_CompileParam.usModelCfgPathLen;

	int nModelCount = 0;
	nModelCount = m_CompileParam.vecModelDir.size();
	memcpy(szBuffer + nLen, &nModelCount, 4);
	nLen += 4;

	for(unsigned i = 0; i < nModelCount;i++){
		CommuModelTargetDir_t *pMdlDir = m_CompileParam.vecModelDir.at(i);
		memcpy(szBuffer + nLen, &pMdlDir->nModelId, 4);
		nLen += 4;
		memcpy(szBuffer + nLen, &pMdlDir->usModelDirLen, 2);
		nLen += 2;
		memcpy(szBuffer + nLen, pMdlDir->szModelDir, pMdlDir->usModelDirLen);
		nLen += pMdlDir->usModelDirLen;
	}
	return nLen;
}

BOOL CSimuS_GCompileSimuModelCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuS_GCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = S_G_CMD_HEADER_SIZE;
	int nModelCount = 0;
	CommuModelTargetDir_t *pMdlDir = NULL;

	memcpy(&m_CompileParam.usModelCfgPathLen, szBuffer + nBodyPos, 2);
	nBodyPos += 2;
	memset(m_CompileParam.szModelCfgPath, 0x0, sizeof(m_CompileParam.szModelCfgPath));
	memcpy(m_CompileParam.szModelCfgPath, szBuffer + nBodyPos, m_CompileParam.usModelCfgPathLen);
	nBodyPos += m_CompileParam.usModelCfgPathLen;
	memcpy(&nModelCount, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	for(unsigned i = 0; i < nModelCount; i++){
		pMdlDir = new CommuModelTargetDir_t;
		
		memcpy(&pMdlDir->nModelId, szBuffer + nBodyPos, 4);
		nBodyPos += 4;
		memcpy(&pMdlDir->usModelDirLen, szBuffer + nBodyPos, 2);
		nBodyPos += 2;

		memset(pMdlDir->szModelDir, 0x0, sizeof(pMdlDir->szModelDir));
		memcpy(pMdlDir->szModelDir, szBuffer + nBodyPos, pMdlDir->usModelDirLen);
		nBodyPos += pMdlDir->usModelDirLen;
		m_CompileParam.vecModelDir.push_back(pMdlDir);
	}
	return TRUE;
}

void CSimuS_GCompileSimuModelCmd::Print(char* szBuffer)
{
	char szTmp[1024];
	CommuModelTargetDir_t *pMdlDir = NULL;

	szBuffer[0] = '\0';
	CSimuS_GCmd::Print(szBuffer);

	sprintf(szTmp, "usModelCfgPathLen:%d, ModelCfg:%s, nModelCount:%d \n", m_CompileParam.usModelCfgPathLen, m_CompileParam.szModelCfgPath, m_CompileParam.vecModelDir.size());
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
	for(unsigned i = 0; i < m_CompileParam.vecModelDir.size(); i++){
		pMdlDir = m_CompileParam.vecModelDir.at(i);
		sprintf(szTmp,"ModelId:%d, ModelDirLen:%d, ModelDir:%s \n",pMdlDir->nModelId,
			pMdlDir->usModelDirLen, pMdlDir->szModelDir);
		strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
	}
}

unsigned long CSimuS_GCompileSimuModelCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;
	int nModelCount = 0;
	CommuModelTargetDir_t *pMdlDir = NULL;

	nLen += 2 + m_CompileParam.usModelCfgPathLen + 4;
	for(unsigned i = 0; i < m_CompileParam.vecModelDir.size(); i++){
		pMdlDir = m_CompileParam.vecModelDir.at(i);
		nLen += 4 + 2 + pMdlDir->usModelDirLen;
	}
	return nLen;
}

int CSimuS_GCompileSimuModelCmd::SetCompileParam(const CommuModelCompileParam_t *pParam)
{
	int nRet = 0;
	CommuModelTargetDir_t *pMdlDir = NULL;
	CommuModelTargetDir_t *pNewMdlDir = NULL;

	strcpy(m_CompileParam.szModelCfgPath, pParam->szModelCfgPath);
	m_CompileParam.usModelCfgPathLen = strlen(m_CompileParam.szModelCfgPath);
	for( unsigned i = 0; i < pParam->vecModelDir.size(); i++){
		pMdlDir = pParam->vecModelDir.at(i);
		pNewMdlDir = new CommuModelTargetDir_t;
		memset(pNewMdlDir->szModelDir, 0x0, sizeof(pNewMdlDir->szModelDir));
		pNewMdlDir->nModelId = pMdlDir->nModelId;
		strcpy(pNewMdlDir->szModelDir, pMdlDir->szModelDir);
		pNewMdlDir->usModelDirLen = strlen(pNewMdlDir->szModelDir);
		m_CompileParam.vecModelDir.push_back(pNewMdlDir);
	}
	return nRet;
}
int CSimuS_GCompileSimuModelCmd::GetCompileParam(CommuModelCompileParam_t *pParam)
{
	int nRet = 0;
	CommuModelTargetDir_t *pMdlDir = NULL;
	CommuModelTargetDir_t *pNewMdlDir = NULL;

	strcpy(pParam->szModelCfgPath,m_CompileParam.szModelCfgPath);
	pParam->usModelCfgPathLen = strlen(m_CompileParam.szModelCfgPath);
	for( unsigned i = 0; i < m_CompileParam.vecModelDir.size(); i++){
		pMdlDir = m_CompileParam.vecModelDir.at(i);
		pNewMdlDir = new CommuModelTargetDir_t;
		memset(pNewMdlDir->szModelDir, 0x0, sizeof(pNewMdlDir->szModelDir));
		pNewMdlDir->nModelId = pMdlDir->nModelId;
		strcpy(pNewMdlDir->szModelDir, pMdlDir->szModelDir);
		pNewMdlDir->usModelDirLen = strlen(pNewMdlDir->szModelDir);
		pParam->vecModelDir.push_back(pNewMdlDir);
	}
	return nRet;
}

const CommuModelCompileParam_t *CSimuS_GCompileSimuModelCmd::GetCompileParam()
{
	return &m_CompileParam;
}

/****************************************************************
 * ���� : CSimuS_GCompileSimuModelRespCmd
 * ���� : CSimuS_GCmd
 * ˵�� : ����ģ��������Ӧ
 ****************************************************************/
CSimuS_GCompileSimuModelRespCmd::CSimuS_GCompileSimuModelRespCmd()
	: CSimuS_GCmd(S_G_CMD_COMPILESIMUMODEL_RESP)
{
	m_ulErrInfoLen = 0;
	m_ulResult = 0;
	memset(m_szErrInfo, 0x0, sizeof(m_szErrInfo));
}

CSimuS_GCompileSimuModelRespCmd::~CSimuS_GCompileSimuModelRespCmd()
{

}

unsigned long CSimuS_GCompileSimuModelRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuS_GCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_ulResult, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_ulErrInfoLen, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, m_szErrInfo, m_ulErrInfoLen);
	nLen += m_ulErrInfoLen;

	return nLen;
}

BOOL CSimuS_GCompileSimuModelRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuS_GCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = S_G_CMD_HEADER_SIZE;
	memset(m_szErrInfo, 0x0, sizeof(m_szErrInfo));

	memcpy(&m_ulResult, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_ulErrInfoLen, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(m_szErrInfo, szBuffer + nBodyPos, m_ulErrInfoLen);
	nBodyPos += m_ulErrInfoLen;

	return TRUE;
}

void CSimuS_GCompileSimuModelRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuS_GCmd::Print(szBuffer);

	sprintf(szTmp, "result:%d, errInfo:%s, errLen:%d \n", m_ulResult, m_szErrInfo, m_ulErrInfoLen);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuS_GCompileSimuModelRespCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen += 4 + 4 + m_ulErrInfoLen;

	return nLen;
}

void CSimuS_GCompileSimuModelRespCmd::SetResult(unsigned long nResult)
{
	m_ulResult = nResult;
}

unsigned long CSimuS_GCompileSimuModelRespCmd::GetResult()
{
	return m_ulResult;
}

void CSimuS_GCompileSimuModelRespCmd::SetErrInfo(const char *szErrInfo, unsigned long ulErrLen)
{
	memset(m_szErrInfo, 0x0, sizeof(m_szErrInfo));
	memcpy(m_szErrInfo, szErrInfo, ulErrLen);
	m_ulErrInfoLen = ulErrLen;
}

unsigned long CSimuS_GCompileSimuModelRespCmd::GetErrInfo(char *pszErrInfo, unsigned long nBuffLen)
{
	if( nBuffLen < m_ulErrInfoLen){
		return 0;
	}
	memset(pszErrInfo, 0x0, nBuffLen);
	memcpy(pszErrInfo, m_szErrInfo, m_ulErrInfoLen);
	return m_ulErrInfoLen;
}

/****************************************************************
 * ���� : CSimuS_GCompileInfoRepCmd
 * ���� : CSimuS_GCmd
 * ˵�� : ������Ϣ�ϱ�����
 ****************************************************************/
CSimuS_GCompileInfoRepCmd::CSimuS_GCompileInfoRepCmd()
	: CSimuS_GCmd(S_G_CMD_COMPILEINFO_REPORT)
{
	m_ulCompileRepType = 0;
	m_ulRepInfoLen = 0;
	memset(m_szCompileInfo, 0x0, sizeof(m_szCompileInfo));
}

CSimuS_GCompileInfoRepCmd::~CSimuS_GCompileInfoRepCmd()
{

}

unsigned long CSimuS_GCompileInfoRepCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuS_GCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_ulCompileRepType, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_ulRepInfoLen, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, m_szCompileInfo, m_ulRepInfoLen);
	nLen += m_ulRepInfoLen;

	return nLen;
}

BOOL CSimuS_GCompileInfoRepCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuS_GCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = S_G_CMD_HEADER_SIZE;
	memset(m_szCompileInfo, 0x0, sizeof(m_szCompileInfo));

	memcpy(&m_ulCompileRepType, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_ulRepInfoLen, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(m_szCompileInfo, szBuffer + nBodyPos, m_ulRepInfoLen);
	nBodyPos += m_ulRepInfoLen;

	return TRUE;
}

void CSimuS_GCompileInfoRepCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuS_GCmd::Print(szBuffer);

	sprintf(szTmp, "RepType:%d, InfoLen:%d, Info:%s \n",m_ulCompileRepType, m_ulRepInfoLen, m_szCompileInfo);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuS_GCompileInfoRepCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen += 4 + 4 + m_ulRepInfoLen;

	return nLen;
}

void CSimuS_GCompileInfoRepCmd::SetCompileInfo(unsigned long ulCompileRepType, const char *szCompileInfo)
{
	m_ulCompileRepType = ulCompileRepType;
	m_ulRepInfoLen = strlen(szCompileInfo);
	if( m_ulRepInfoLen < 8192){
		strcpy(m_szCompileInfo, szCompileInfo);
	}else{
		memcpy(m_szCompileInfo, szCompileInfo, 8191);
		m_szCompileInfo[8191] = 0;
	}
}

unsigned long CSimuS_GCompileInfoRepCmd::GetCompileInfo(unsigned long *pCompileRepType, char *pszCompileInfo, unsigned long nBuffLen)
{
	strcpy(pszCompileInfo, m_szCompileInfo);
	*pCompileRepType = m_ulCompileRepType;
	return m_ulRepInfoLen;
}

