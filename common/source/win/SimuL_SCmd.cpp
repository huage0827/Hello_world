/*******************************************************************
 * �ļ���: SimuL_SCmd.cpp
 * ����: SimuLab��SimuService֮��ͨ������ʵ��
 ******************************************************************/
#include "stdafx.h"
#include "SimuL_SCmd.h"
#include "Util.h"

#define MAX_PRINT_BUFFER_LEN		8192


/****************************************************************
 * ���� : CSimuL_SCmd
 * ���� : ��
 * ˵�� : SimuLab��SimuService֮��ͨ�ŵ���������ĸ���
 ****************************************************************/
CSimuL_SCmd::CSimuL_SCmd(L_S_CMD slc)
{
	m_slc = slc;
	memset(m_szVersion , 0 , sizeof(m_szVersion));
	m_szVersion[0] = 0;
	m_szVersion[2] = 1;
	m_szVersion[3] = 0;
	m_szVersion[4] = 0;
	m_nFrameLen = 0;
	m_nCmd = slc;
	m_nFrameBodyLen = 0;
	memset(m_szSimuProjectID , 0 , sizeof(m_szSimuProjectID));
	memset(m_szSimuModelID , 0 , sizeof(m_szSimuModelID));
	memset(m_szReserved2 , 0 , sizeof(m_szReserved2));
	memset(m_szReserved3 , 0 , sizeof(m_szReserved3));
	memset(m_szReserved4 , 0 , sizeof(m_szReserved4));
}

CSimuL_SCmd::~CSimuL_SCmd(){

}

L_S_CMD CSimuL_SCmd::PreParseCmd(unsigned char* szBuffer , unsigned long nBufferLen , unsigned long& nCmdDataLen)
{
	L_S_CMD slc = L_S_CMD_UNKNOWN;

	if( nBufferLen < L_S_CMD_HEADER_SIZE )
		return L_S_CMD_UNKNOWN;

	memcpy((void*)&nCmdDataLen , szBuffer + L_S_CMD_POS_FRAMELEN , L_S_CMD_SIZE_FRAMELEN);

	if( nBufferLen < nCmdDataLen )
		return L_S_CMD_UNKNOWN;

	memcpy((void*)&slc , szBuffer + L_S_CMD_POS_COMMAND , L_S_CMD_SIZE_COMMAND);

	return slc;
}

L_S_CMD CSimuL_SCmd::GetCmdType() const
{
	return m_slc;
}

void CSimuL_SCmd::SetSimuProjectID(const unsigned char* szId)
{
	memset(m_szSimuProjectID , 0 , sizeof(m_szSimuProjectID));
	memcpy(m_szSimuProjectID , szId , sizeof(m_szSimuProjectID));
}

unsigned long CSimuL_SCmd::GetFrameLen()
{
	return m_nFrameLen;
}

unsigned long CSimuL_SCmd::GetFrameBodyLen()
{
	return m_nFrameBodyLen;
}

void CSimuL_SCmd::GetSimuProjectID(unsigned char* szId)
{
	if( szId == NULL )
		return ;
	memcpy(szId , m_szSimuProjectID , sizeof(m_szSimuProjectID));
}

void CSimuL_SCmd::SetSimuModelID(const unsigned char* szModelId)
{
	memset(m_szSimuModelID, 0x0, sizeof(m_szSimuModelID));
	memcpy(m_szSimuModelID, szModelId, sizeof(m_szSimuModelID));
}
void CSimuL_SCmd::GetSimuModelID(unsigned char *szModelId)
{
	if( szModelId == NULL)
		return;
	memcpy(szModelId, m_szSimuModelID, sizeof(m_szSimuModelID));
}

unsigned long CSimuL_SCmd::SerializeHeader(unsigned char* szBuffer)
{
	m_nFrameBodyLen = CalcFrameBodyLen();
	m_nFrameLen = CalcFrameLen();
	memcpy(szBuffer , m_szVersion , L_S_CMD_SIZE_VERSION);
	memcpy(szBuffer + L_S_CMD_POS_FRAMELEN , (void*)&m_nFrameLen , L_S_CMD_SIZE_FRAMELEN);
	memcpy(szBuffer + L_S_CMD_POS_COMMAND , (void*)&m_nCmd , L_S_CMD_SIZE_COMMAND);
	memcpy(szBuffer + L_S_CMD_POS_FRAMEBODYLEN , (void*)&m_nFrameBodyLen , L_S_CMD_SIZE_FRAMEBODYLEN);
	memcpy(szBuffer + L_S_CMD_POS_SIMUPROJECTID , m_szSimuProjectID , L_S_CMD_SIZE_SIMUPROJECTID);
	memcpy(szBuffer + L_S_CMD_POS_SIMUMODEL , m_szSimuModelID , L_S_CMD_SIZE_SIMUMODEL);
	memcpy(szBuffer + L_S_CMD_POS_RESERVED2 , m_szReserved2 , L_S_CMD_SIZE_RESERVED2);
	memcpy(szBuffer + L_S_CMD_POS_RESERVED3 , m_szReserved3 , L_S_CMD_SIZE_RESERVED3);
	memcpy(szBuffer + L_S_CMD_POS_RESERVED4 , m_szReserved4 , L_S_CMD_SIZE_RESERVED4);
	return L_S_CMD_HEADER_SIZE;
}

unsigned long CSimuL_SCmd::Serialize(unsigned char* szBuffer)
{
	return SerializeHeader(szBuffer);
}

BOOL CSimuL_SCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( nBufferLen < L_S_CMD_HEADER_SIZE ){
		return FALSE;
	}

	memcpy(m_szVersion , szBuffer + L_S_CMD_POS_VERSION , L_S_CMD_SIZE_VERSION);
	memcpy((void*)&m_nFrameLen , szBuffer + L_S_CMD_POS_FRAMELEN , L_S_CMD_SIZE_FRAMELEN);
	memcpy((void*)&m_nCmd , szBuffer + L_S_CMD_POS_COMMAND , L_S_CMD_SIZE_COMMAND);
	memcpy((void*)&m_nFrameBodyLen , szBuffer + L_S_CMD_POS_FRAMEBODYLEN , L_S_CMD_SIZE_FRAMEBODYLEN);
	memcpy(m_szSimuProjectID , szBuffer + L_S_CMD_POS_SIMUPROJECTID , L_S_CMD_SIZE_SIMUPROJECTID);
	memcpy(m_szSimuModelID , szBuffer + L_S_CMD_POS_SIMUMODEL , L_S_CMD_SIZE_SIMUMODEL);
	memcpy(m_szReserved2 , szBuffer + L_S_CMD_POS_RESERVED2 , L_S_CMD_SIZE_RESERVED2);
	memcpy(m_szReserved3 , szBuffer + L_S_CMD_POS_RESERVED3 , L_S_CMD_SIZE_RESERVED3);
	memcpy(m_szReserved4 , szBuffer + L_S_CMD_POS_RESERVED4 , L_S_CMD_SIZE_RESERVED4);

	return TRUE;
}

void CSimuL_SCmd::Print(char* szBuffer)
{
	char szTmp[256];
	char szStr[128];
	int i = 0;

	switch( m_nCmd )
	{
		case L_S_CMD_UNKNOWN	 :
			sprintf_s(szTmp , 64 , "Cmd: L_S_CMD_UNKNOWN");break;
		case L_S_CMD_CONNECTTARGET	 :
			sprintf_s(szTmp , 64 , "Cmd: L_S_CMD_CONNECTTARGET");break;
		case L_S_CMD_CONNECTTARGET_RESP	 :
			sprintf_s(szTmp , 64 , "Cmd: L_S_CMD_CONNECTTARGET_RESP");break;
		case L_S_CMD_LOADSIMUMODEL	 :
			sprintf_s(szTmp , 64 , "Cmd: L_S_CMD_LOADSIMUMODEL");break;
		case L_S_CMD_LOADSIMUMODEL_RESP	 :
			sprintf_s(szTmp , 64 , "Cmd: L_S_CMD_LOADSIMUMODEL_RESP");break;
		case L_S_CMD_ENABLESIMUMONITOR	:
			sprintf_s(szTmp , 64 , "Cmd: L_S_CMD_ENABLESIMUMONITOR");break;
		case L_S_CMD_ENABLESIMUMONITOR_RESP	:
			sprintf_s(szTmp , 64 , "Cmd: L_S_CMD_ENABLESIMUMONITOR_RESP");break;
		case L_S_CMD_COMPILESIMUMODEL :
			sprintf_s(szTmp , 64 , "Cmd: L_S_CMD_COMPILESIMUMODEL");break;
		case L_S_CMD_COMPILESIMUMODEL_RESP :
			sprintf_s(szTmp , 64 , "Cmd: L_S_CMD_COMPILESIMUMODEL_RESP");break;
		case L_S_CMD_COMPILEINFO_REP :
			sprintf_s(szTmp , 64 , "Cmd: L_S_CMD_COMPILEINFO_REP");break;
		default :
			break;
	}
	strcat_s(szBuffer, MAX_PRINT_BUFFER_LEN, szTmp);

	sprintf_s(szTmp , sizeof(szTmp) , ", FrameLen: %ld" , m_nFrameLen);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);

	sprintf_s(szTmp , sizeof(szTmp) , ", FrameBodyLen:%ld \n" , m_nFrameBodyLen);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);

	memset(szStr, 0x0, sizeof(szStr));
	memcpy(szStr, m_szSimuProjectID, L_S_CMD_SIZE_SIMUPROJECTID);
	sprintf_s(szTmp , sizeof(szTmp) , "ProjID=%s," , szStr);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);

	memset(szStr, 0x0, sizeof(szStr));
	memcpy(szStr, m_szSimuModelID, L_S_CMD_SIZE_SIMUMODEL);
	sprintf_s(szTmp , sizeof(szTmp) , "ModelID=%s \n" , szStr);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);

	return ;
}

unsigned long CSimuL_SCmd::CalcFrameLen()
{
	return L_S_CMD_HEADER_SIZE + CalcFrameBodyLen();
}

/****************************************************************
 * ���� : CSimuL_SConnectTargetCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ���ӵ�SimuService��������
 ****************************************************************/
CSimuL_SConnectTargetCmd::CSimuL_SConnectTargetCmd()
	: CSimuL_SCmd(L_S_CMD_CONNECTTARGET)
{
	memset(m_szTargetIp, 0, sizeof(m_szTargetIp));
	m_usPort = 0;
}

CSimuL_SConnectTargetCmd::~CSimuL_SConnectTargetCmd()
{

}

unsigned long CSimuL_SConnectTargetCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuL_SCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen , m_szTargetIp , sizeof(m_szTargetIp));
	nLen += sizeof(m_szTargetIp);
	memcpy(szBuffer + nLen , &m_usPort , sizeof(m_usPort));
	nLen += sizeof(m_usPort);
	memcpy(szBuffer + nLen, &m_ulConnectType, 4);
	nLen += 4;

	return nLen;
}

BOOL CSimuL_SConnectTargetCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuL_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = L_S_CMD_HEADER_SIZE;
	memcpy(m_szTargetIp , szBuffer + nBodyPos , sizeof(m_szTargetIp));
	nBodyPos += sizeof(m_szTargetIp);
	memcpy(&m_usPort , szBuffer + nBodyPos , sizeof(unsigned short));
	nBodyPos += 2;
	memcpy(&m_ulConnectType, szBuffer + nBodyPos, 4);
	nBodyPos += 4;

	return TRUE;
}

void CSimuL_SConnectTargetCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuL_SCmd::Print(szBuffer);

	sprintf_s(szTmp, sizeof(szTmp),"Target:%d.%d.%d.%d, port:%d ,connectType:%d\n", (unsigned char )m_szTargetIp[0],
		(unsigned char )m_szTargetIp[1],(unsigned char )m_szTargetIp[2],
		(unsigned char )m_szTargetIp[3],m_usPort, m_ulConnectType);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
	return;
}

unsigned long CSimuL_SConnectTargetCmd::CalcFrameBodyLen()
{
	return 4 + 2 + 4;
}

void CSimuL_SConnectTargetCmd::GetTargetInfo(unsigned char *szTargetIP, unsigned short &usPort)
{
	if (szTargetIP != NULL){
		memcpy(szTargetIP, m_szTargetIp, sizeof(m_szTargetIp));
	}

	usPort = m_usPort;
}

void CSimuL_SConnectTargetCmd::SetTargetInfo(const char* szTargetIP , unsigned short usPort)
{
	memcpy(m_szTargetIp , szTargetIP , sizeof(m_szTargetIp));
	m_usPort = usPort;
}

void CSimuL_SConnectTargetCmd::SetConnectType(unsigned int ConnectType)
{
	m_ulConnectType = ConnectType;
}

unsigned int CSimuL_SConnectTargetCmd::GetConnectType()
{
	return m_ulConnectType;
}
/****************************************************************
 * ���� : CSimuL_SConnectTargetRespCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ���ӵ�SimuService��Ӧ����
 ****************************************************************/
CSimuL_SConnectTargetRespCmd::CSimuL_SConnectTargetRespCmd()
	: CSimuL_SCmd(L_S_CMD_CONNECTTARGET_RESP)
{
	m_ulResult = 0;
	m_ulErrInfoLen = 0;
	memset(m_szErrInfo, 0x0, sizeof(m_szErrInfo));
}

CSimuL_SConnectTargetRespCmd::~CSimuL_SConnectTargetRespCmd()
{

}

unsigned long CSimuL_SConnectTargetRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuL_SCmd::Serialize(szBuffer);
	
	memcpy(szBuffer +  nLen, &m_ulResult, sizeof(unsigned long));
	nLen += sizeof(unsigned long);
	memcpy(szBuffer + nLen, &m_ulErrInfoLen, sizeof(unsigned long));
	nLen += 4;
	memcpy(szBuffer + nLen, m_szErrInfo, m_ulErrInfoLen);
	nLen += m_ulErrInfoLen;

	return nLen;
}

BOOL CSimuL_SConnectTargetRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuL_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	unsigned nPos = L_S_CMD_HEADER_SIZE;
	memcpy(&m_ulResult , szBuffer + nPos , sizeof(unsigned long));
	nPos += 4;
	memcpy(&m_ulErrInfoLen, szBuffer + nPos, 4);
	nPos += 4;
	memcpy(m_szErrInfo, szBuffer + nPos, m_ulErrInfoLen);
	nPos += m_ulErrInfoLen;

	return TRUE;
}

void CSimuL_SConnectTargetRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuL_SCmd::Print(szBuffer);

	sprintf_s(szTmp , sizeof(szTmp) , "%d,%d,info:%s\n" , m_ulResult, m_ulErrInfoLen, m_szErrInfo);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuL_SConnectTargetRespCmd::CalcFrameBodyLen()
{
	return 4 + 4 + m_ulErrInfoLen;
}

void CSimuL_SConnectTargetRespCmd::SetResult(unsigned long ret)
{
	m_ulResult = ret;
}

unsigned long CSimuL_SConnectTargetRespCmd::GetResult()
{
	return m_ulResult;
}

void CSimuL_SConnectTargetRespCmd::SetErrInfo(unsigned char *szErrInfo, unsigned long nErrLen)
{
	memcpy(m_szErrInfo, szErrInfo, nErrLen);
	m_ulErrInfoLen = nErrLen;
}

int  CSimuL_SConnectTargetRespCmd::GetErrInfo(unsigned char *szBuffer)
{
	strcpy((char *)szBuffer, (char *)m_szErrInfo);
	return strlen((char *)m_szErrInfo);
}

/****************************************************************
 * ���� : CSimuL_SLoadSimuModelCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ���ط���ģ��
 ****************************************************************/
CSimuL_SLoadSimuModelCmd::CSimuL_SLoadSimuModelCmd()
	: CSimuL_SCmd(L_S_CMD_LOADSIMUMODEL)
{
	memset(m_CommuParam.szDebugParam, 0x0, sizeof(m_CommuParam.szDebugParam));
	memset(m_CommuParam.szDriverLibPath, 0x0, sizeof(m_CommuParam.szDriverLibPath));
	memset(m_CommuParam.szModelConfPath, 0x0, sizeof(m_CommuParam.szModelConfPath));
	memset(m_CommuParam.szMonitorIp, 0x0, sizeof(m_CommuParam.szMonitorIp));
}

CSimuL_SLoadSimuModelCmd::~CSimuL_SLoadSimuModelCmd()
{
	for(unsigned int i = 0; i < m_CommuParam.vecModelFilePath.size(); i++)
		delete m_CommuParam.vecModelFilePath.at(i);
	m_CommuParam.vecModelFilePath.clear();
}

unsigned long CSimuL_SLoadSimuModelCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuL_SCmd::Serialize(szBuffer);
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

BOOL CSimuL_SLoadSimuModelCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuL_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	CommuModelProgFilePath_t *pModelPath = NULL;

	int nBodyPos = L_S_CMD_HEADER_SIZE;
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

void CSimuL_SLoadSimuModelCmd::Print(char* szBuffer)
{
	char szTmp[1024];
	CommuModelProgFilePath_t *pModelPath = NULL;

	szBuffer[0] = '\0';
	CSimuL_SCmd::Print(szBuffer);

	sprintf_s(szTmp, sizeof(szTmp), "Monitor:%d.%d.%d.%d, port:%d \n", (unsigned char )m_CommuParam.szMonitorIp[0],
		(unsigned char )m_CommuParam.szMonitorIp[1],(unsigned char )m_CommuParam.szMonitorIp[2],
		(unsigned char )m_CommuParam.szMonitorIp[3],m_CommuParam.usPort);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);

	sprintf_s(szTmp, sizeof(szTmp), "Debug: model->%d, nLen:%d, param:%s \n", m_CommuParam.bDebugMode,
		m_CommuParam.usDebugParamLen, m_CommuParam.szDebugParam);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);

	sprintf_s(szTmp, sizeof(szTmp), "Conf:%s, nLen:%d, Dirver:%s, nLen:%d \n",m_CommuParam.szModelConfPath,
		m_CommuParam.usModelConfPathLen, m_CommuParam.szDriverLibPath, m_CommuParam.usDriverLibPathLen);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);

	sprintf_s(szTmp, sizeof(szTmp), "ModelCount:%d \n", m_CommuParam.nModelCount);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);

	for(unsigned int i = 0; i < m_CommuParam.vecModelFilePath.size(); i++){
		pModelPath = m_CommuParam.vecModelFilePath.at(i);

		sprintf_s(szTmp, sizeof(szTmp), "modelId:%d, nLen:%d, path:%s \n", pModelPath->nModelId,
			pModelPath->nFilePathLen, pModelPath->szFilePath);
		strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
	}
}

unsigned long CSimuL_SLoadSimuModelCmd::CalcFrameBodyLen()
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

void CSimuL_SLoadSimuModelCmd::SetParam(const CommuLoadSimuModelParam_t *pParam)
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

const CommuLoadSimuModelParam_t *CSimuL_SLoadSimuModelCmd::GetParam()
{
	return &m_CommuParam;
}

/****************************************************************
 * ���� : CSimuL_SLoadSimuModelRespCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ���ط���ģ����Ӧ
 ****************************************************************/
CSimuL_SLoadSimuModelRespCmd::CSimuL_SLoadSimuModelRespCmd()
	: CSimuL_SCmd(L_S_CMD_LOADSIMUMODEL_RESP)
{
	m_ulResult = 0;
	m_ulErrInfoLen = 0;
	memset(m_szErrInfo, 0x0, sizeof(m_szErrInfo));
}

CSimuL_SLoadSimuModelRespCmd::~CSimuL_SLoadSimuModelRespCmd()
{

}

unsigned long CSimuL_SLoadSimuModelRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuL_SCmd::Serialize(szBuffer);

	memcpy(szBuffer +  nLen, &m_ulResult, sizeof(unsigned long));
	nLen += sizeof(unsigned long);
	memcpy(szBuffer + nLen, &m_ulErrInfoLen, sizeof(unsigned long));
	nLen += 4;
	memcpy(szBuffer + nLen, m_szErrInfo, m_ulErrInfoLen);
	nLen += m_ulErrInfoLen;

	return nLen;
}

BOOL CSimuL_SLoadSimuModelRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuL_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	unsigned nPos = L_S_CMD_HEADER_SIZE;
	memcpy(&m_ulResult , szBuffer + nPos , sizeof(unsigned long));
	nPos += 4;
	memcpy(&m_ulErrInfoLen, szBuffer + nPos, 4);
	nPos += 4;
	memcpy(m_szErrInfo, szBuffer + nPos, m_ulErrInfoLen);
	nPos += m_ulErrInfoLen;

	return TRUE;
}

void CSimuL_SLoadSimuModelRespCmd::Print(char* szBuffer)
{
	char szTmp[64];

	szBuffer[0] = '\0';
	CSimuL_SCmd::Print(szBuffer);

	sprintf_s(szTmp , sizeof(szTmp) , "%d,%d,info:%s\n" , m_ulResult, m_ulErrInfoLen, m_szErrInfo);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuL_SLoadSimuModelRespCmd::CalcFrameBodyLen()
{
	return 4 + 4 + m_ulErrInfoLen;
}

void CSimuL_SLoadSimuModelRespCmd::SetResult(unsigned long ret)
{
	m_ulResult = ret;
}

unsigned long CSimuL_SLoadSimuModelRespCmd::GetResult()
{
	return m_ulResult;
}

void CSimuL_SLoadSimuModelRespCmd::SetErrInfo(unsigned char *szErrInfo, unsigned long nErrLen)
{
	memcpy(m_szErrInfo, szErrInfo, nErrLen);
	m_ulErrInfoLen = nErrLen;
}

int  CSimuL_SLoadSimuModelRespCmd::GetErrInfo(unsigned char *szBuffer)
{
	strcpy((char *)szBuffer, (char *)m_szErrInfo);
	return strlen((char *)m_szErrInfo);
}


/****************************************************************
 * ���� : CSimuL_SEnableSimuMonitorCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ʹ�ܼ������
 ****************************************************************/
CSimuL_SEnableSimuMonitorCmd::CSimuL_SEnableSimuMonitorCmd()
	: CSimuL_SCmd(L_S_CMD_ENABLESIMUMONITOR)
{
	m_ulEnableFlag = 0;
}

CSimuL_SEnableSimuMonitorCmd::~CSimuL_SEnableSimuMonitorCmd()
{

}

unsigned long CSimuL_SEnableSimuMonitorCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuL_SCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_ulEnableFlag, 4);
	nLen += 4;

	return nLen;
}

BOOL CSimuL_SEnableSimuMonitorCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuL_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = L_S_CMD_HEADER_SIZE;

	memcpy(&m_ulEnableFlag, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	
	return TRUE;
}

void CSimuL_SEnableSimuMonitorCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuL_SCmd::Print(szBuffer);

	sprintf_s(szTmp, sizeof(szTmp), "EnableFlag:%d \n", m_ulEnableFlag);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuL_SEnableSimuMonitorCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen += 4;

	return nLen;
}

void CSimuL_SEnableSimuMonitorCmd::SetFlag(unsigned long ulFlag)
{
	m_ulEnableFlag = ulFlag;
}

unsigned long CSimuL_SEnableSimuMonitorCmd::GetFlag()
{
	return m_ulEnableFlag;
}

/****************************************************************
 * ���� : CSimuL_SEnableSimuMonitorRespCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ʹ�ܼ��������Ӧ
 ****************************************************************/
CSimuL_SEnableSimuMonitorRespCmd::CSimuL_SEnableSimuMonitorRespCmd()
	: CSimuL_SCmd(L_S_CMD_ENABLESIMUMONITOR_RESP)
{
	m_ulErrInfoLen = 0;
	m_ulResult = 0;
}

CSimuL_SEnableSimuMonitorRespCmd::~CSimuL_SEnableSimuMonitorRespCmd()
{

}

unsigned long CSimuL_SEnableSimuMonitorRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuL_SCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_ulResult, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_ulErrInfoLen, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, m_szErrInfo, m_ulErrInfoLen);
	nLen += m_ulErrInfoLen;

	return nLen;
}

BOOL CSimuL_SEnableSimuMonitorRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuL_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = L_S_CMD_HEADER_SIZE;
	memset(m_szErrInfo, 0x0, sizeof(m_szErrInfo));

	memcpy(&m_ulResult, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_ulErrInfoLen, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(m_szErrInfo, szBuffer + nBodyPos, m_ulErrInfoLen);
	nBodyPos += m_ulErrInfoLen;

	return TRUE;
}

void CSimuL_SEnableSimuMonitorRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuL_SCmd::Print(szBuffer);

	sprintf_s(szTmp, sizeof(szTmp), "result:%d, errInfo:%s, errLen:%d \n", m_ulResult, m_szErrInfo, m_ulErrInfoLen);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuL_SEnableSimuMonitorRespCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen += 4 + 4 + m_ulErrInfoLen;

	return nLen;
}

void CSimuL_SEnableSimuMonitorRespCmd::SetResult(unsigned long nResult)
{
	m_ulResult = nResult;
}

unsigned long CSimuL_SEnableSimuMonitorRespCmd::GetResult()
{
	return m_ulResult;
}

void CSimuL_SEnableSimuMonitorRespCmd::SetErrInfo(const char *szErrInfo, unsigned long ulErrLen)
{
	memset(m_szErrInfo, 0x0, sizeof(m_szErrInfo));
	memcpy(m_szErrInfo, szErrInfo, ulErrLen);
	m_ulErrInfoLen = ulErrLen;
}

unsigned long CSimuL_SEnableSimuMonitorRespCmd::GetErrInfo(char *pszErrInfo, unsigned long nBuffLen)
{
	if( nBuffLen < m_ulErrInfoLen){
		return 0;
	}
	memset(pszErrInfo, 0x0, nBuffLen);
	memcpy(pszErrInfo, m_szErrInfo, m_ulErrInfoLen);
	return m_ulErrInfoLen;
}

/****************************************************************
 * ���� : CSimuL_SCompileSimuModelCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ����ģ������
 ****************************************************************/
CSimuL_SCompileSimuModelCmd::CSimuL_SCompileSimuModelCmd()
	: CSimuL_SCmd(L_S_CMD_COMPILESIMUMODEL)
{

}

CSimuL_SCompileSimuModelCmd::~CSimuL_SCompileSimuModelCmd()
{
	for( unsigned i = 0; i < m_CompileParam.vecModelDir.size();i++){
		delete m_CompileParam.vecModelDir.at(i);
	}
	m_CompileParam.vecModelDir.clear();
}

unsigned long CSimuL_SCompileSimuModelCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuL_SCmd::Serialize(szBuffer);

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

BOOL CSimuL_SCompileSimuModelCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuL_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = L_S_CMD_HEADER_SIZE;
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
		memset(pMdlDir->szModelDir, 0x0, sizeof(pMdlDir->szModelDir));
		
		memcpy(&pMdlDir->nModelId, szBuffer + nBodyPos, 4);
		nBodyPos += 4;
		memcpy(&pMdlDir->usModelDirLen, szBuffer + nBodyPos, 2);
		nBodyPos += 2;
		memcpy(pMdlDir->szModelDir, szBuffer + nBodyPos, pMdlDir->usModelDirLen);
		nBodyPos += pMdlDir->usModelDirLen;
		m_CompileParam.vecModelDir.push_back(pMdlDir);
	}
	return TRUE;
}

void CSimuL_SCompileSimuModelCmd::Print(char* szBuffer)
{
	char szTmp[1024];
	CommuModelTargetDir_t *pMdlDir = NULL;

	szBuffer[0] = '\0';
	CSimuL_SCmd::Print(szBuffer);

	sprintf_s(szTmp, sizeof(szTmp), "usModelCfgPathLen:%d, ModelCfg:%s, nModelCount:%d \n", m_CompileParam.usModelCfgPathLen, m_CompileParam.szModelCfgPath, m_CompileParam.vecModelDir.size());
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
	for(unsigned i = 0; i < m_CompileParam.vecModelDir.size(); i++){
		pMdlDir = m_CompileParam.vecModelDir.at(i);
		sprintf_s(szTmp, sizeof(szTmp),"ModelId:%d, ModelDirLen:%d, ModelDir:%s \n",pMdlDir->nModelId,
			pMdlDir->usModelDirLen, pMdlDir->szModelDir);
		strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
	}
}

unsigned long CSimuL_SCompileSimuModelCmd::CalcFrameBodyLen()
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

int CSimuL_SCompileSimuModelCmd::SetCompileParam(const CommuModelCompileParam_t *pParam)
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
int CSimuL_SCompileSimuModelCmd::GetCompileParam(CommuModelCompileParam_t *pParam)
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

/****************************************************************
 * ���� : CSimuL_SCompileSimuModelRespCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ����ģ��������Ӧ
 ****************************************************************/
CSimuL_SCompileSimuModelRespCmd::CSimuL_SCompileSimuModelRespCmd()
	: CSimuL_SCmd(L_S_CMD_COMPILESIMUMODEL_RESP)
{
	m_ulErrInfoLen = 0;
	m_ulResult = 0;
	memset(m_szErrInfo, 0x0, sizeof(m_szErrInfo));
}

CSimuL_SCompileSimuModelRespCmd::~CSimuL_SCompileSimuModelRespCmd()
{

}

unsigned long CSimuL_SCompileSimuModelRespCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuL_SCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_ulResult, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_ulErrInfoLen, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, m_szErrInfo, m_ulErrInfoLen);
	nLen += m_ulErrInfoLen;

	return nLen;
}

BOOL CSimuL_SCompileSimuModelRespCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuL_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = L_S_CMD_HEADER_SIZE;
	memset(m_szErrInfo, 0x0, sizeof(m_szErrInfo));

	memcpy(&m_ulResult, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_ulErrInfoLen, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(m_szErrInfo, szBuffer + nBodyPos, m_ulErrInfoLen);
	nBodyPos += m_ulErrInfoLen;

	return TRUE;
}

void CSimuL_SCompileSimuModelRespCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuL_SCmd::Print(szBuffer);

	sprintf_s(szTmp, sizeof(szTmp), "result:%d, errInfo:%s, errLen:%d \n", m_ulResult, m_szErrInfo, m_ulErrInfoLen);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuL_SCompileSimuModelRespCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen += 4 + 4 + m_ulErrInfoLen;

	return nLen;
}

void CSimuL_SCompileSimuModelRespCmd::SetResult(unsigned long nResult)
{
	m_ulResult = nResult;
}

unsigned long CSimuL_SCompileSimuModelRespCmd::GetResult()
{
	return m_ulResult;
}

void CSimuL_SCompileSimuModelRespCmd::SetErrInfo(const char *szErrInfo, unsigned long ulErrLen)
{
	memset(m_szErrInfo, 0x0, sizeof(m_szErrInfo));
	memcpy(m_szErrInfo, szErrInfo, ulErrLen);
	m_ulErrInfoLen = ulErrLen;
}

unsigned long CSimuL_SCompileSimuModelRespCmd::GetErrInfo(char *pszErrInfo, unsigned long nBuffLen)
{
	if( nBuffLen < m_ulErrInfoLen){
		return 0;
	}
	memset(pszErrInfo, 0x0, nBuffLen);
	memcpy(pszErrInfo, m_szErrInfo, m_ulErrInfoLen);
	return m_ulErrInfoLen;
}

/****************************************************************
 * ���� : CSimuL_SCompileInfoRepCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ������Ϣ�ϱ�����
 ****************************************************************/
CSimuL_SCompileInfoRepCmd::CSimuL_SCompileInfoRepCmd()
	: CSimuL_SCmd(L_S_CMD_COMPILEINFO_REP)
{
	m_ulCompileRepType = 0;
	m_ulRepInfoLen = 0;
	memset(m_szCompileInfo, 0x0, sizeof(m_szCompileInfo));
}

CSimuL_SCompileInfoRepCmd::~CSimuL_SCompileInfoRepCmd()
{

}

unsigned long CSimuL_SCompileInfoRepCmd::Serialize(unsigned char* szBuffer)
{
	unsigned long nLen = CSimuL_SCmd::Serialize(szBuffer);

	memcpy(szBuffer + nLen, &m_ulCompileRepType, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_ulRepInfoLen, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, m_szCompileInfo, m_ulRepInfoLen);
	nLen += m_ulRepInfoLen;

	return nLen;
}

BOOL CSimuL_SCompileInfoRepCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen)
{
	if( !CSimuL_SCmd::Unserialize(szBuffer , nBufferLen) )
		return FALSE;
	if( nBufferLen < m_nFrameLen )
		return FALSE;

	int nBodyPos = L_S_CMD_HEADER_SIZE;
	memset(m_szCompileInfo, 0x0, sizeof(m_szCompileInfo));

	memcpy(&m_ulCompileRepType, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(&m_ulRepInfoLen, szBuffer + nBodyPos, 4);
	nBodyPos += 4;
	memcpy(m_szCompileInfo, szBuffer + nBodyPos, m_ulRepInfoLen);
	nBodyPos += m_ulRepInfoLen;

	return TRUE;
}

void CSimuL_SCompileInfoRepCmd::Print(char* szBuffer)
{
	char szTmp[1024];

	szBuffer[0] = '\0';
	CSimuL_SCmd::Print(szBuffer);

	sprintf_s(szTmp, sizeof(szTmp), "RepType:%d, InfoLen:%d, Info:%s \n",m_ulCompileRepType, m_ulRepInfoLen, m_szCompileInfo);
	strcat_s(szBuffer , MAX_PRINT_BUFFER_LEN , szTmp);
}

unsigned long CSimuL_SCompileInfoRepCmd::CalcFrameBodyLen()
{
	unsigned long nLen = 0;

	nLen += 4 + 4 + m_ulRepInfoLen;

	return nLen;
}

void CSimuL_SCompileInfoRepCmd::SetCompileInfo(unsigned long ulCompileRepType, const char *szCompileInfo)
{
	m_ulCompileRepType = ulCompileRepType;
	m_ulRepInfoLen = strlen(szCompileInfo);
	strcpy(m_szCompileInfo, szCompileInfo);
}

unsigned long CSimuL_SCompileInfoRepCmd::GetCompileInfo(unsigned long *pCompileRepType, char *pszCompileInfo, unsigned long nBuffLen)
{
	strcpy(pszCompileInfo, m_szCompileInfo);
	*pCompileRepType = m_ulCompileRepType;
	return m_ulRepInfoLen;
}
