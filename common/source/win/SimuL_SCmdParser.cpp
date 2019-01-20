/*****************************************************************
 * �ļ���: SimuL_SCmdParser.cpp
 * ����: SimuLab��SimuService֮��ͨ������ִ�кͽ�����ʵ��
 *****************************************************************/

#include "stdafx.h"
#include "SimuL_SCmdParser.h"

/*****************************************************************
 *  ��������	SimuL_SCmdParseThread
 *  ���ܣ�		SimuLab��SimuService֮�������ִ���߳�
 *  ���:		LPVOID lpParam -- �����߳�ʱ���ݽ������̶߳�����Ϣ
 *  ����:		��
 *  ����ֵ:		DWORD(QNX: void*) -- �߳��˳�ʱ���ݸ�����ϵͳ��ֵ
 *****************************************************************/
DWORD WINAPI SimuL_SCmdParseThread(LPVOID lpParam)
{
	CSimuL_SCmdParser* pParser = (CSimuL_SCmdParser*)lpParam;
	unsigned long nTick = 1;
	
	while( 1 )
	{
		if( WaitForSingleObject(pParser->m_hParseEvent , 0) == WAIT_OBJECT_0 )
		{
			break;
		}
		pParser->ExecAllCmd();
		/*if( nTick++ == 1000 )
		{
			nTick = 1;
			Sleep(1);
		}*/
		Sleep(2);
	}
	return 0;
}

/*****************************************************************
 *  ��������	CSimuL_SCmdParser::CSimuL_SCmdParser
 *  ���ܣ�		SimuLab��SimuService֮�������������๹�캯��
 *  ���:		��
 *  ����:		��
 *  ����ֵ:		��
 *****************************************************************/
CSimuL_SCmdParser::CSimuL_SCmdParser()
{
	m_bInitialize = FALSE;
	m_szBuffer = NULL;
}

/*****************************************************************
 *  ��������	CSimuL_SCmdParser::~CSimuL_SCmdParser
 *  ���ܣ�		SimuLab��SimuService֮����������������������
 *  ���:		��
 *  ����:		��
 *  ����ֵ:		��
 *****************************************************************/
CSimuL_SCmdParser::~CSimuL_SCmdParser()
{
	Uninitialize();
}

/*****************************************************************
 *  ��������	CSimuD_SCmdParser::Initialize
 *  ���ܣ�		��ʼ�����������
 *  ���:		CSimuL_SCmdExecutorInterface* pCmdExecutorInterface -- ����ִ����
 *  ����:		��
 *  ����ֵ:		TRUE -- ��ʼ���ɹ� , FALSE -- ��ʼ��ʧ��
 *****************************************************************/
BOOL CSimuL_SCmdParser::Initialize(CSimuL_SCmdExecutorInterface* pCmdExecutorInterface)
{
	if( m_bInitialize )
	{
		delete pCmdExecutorInterface;
		return FALSE;
	}

	if( pCmdExecutorInterface == NULL )
		return FALSE;

	m_pCmdExecutorInterface = pCmdExecutorInterface;

	m_szBuffer = NULL;
	m_nBufferLen = 0;
	InitializeCriticalSection(&m_criCmd);

	DWORD threadID;

	m_hParseEvent = CreateEvent(NULL , TRUE , FALSE , NULL);

	m_hParseThread = ::CreateThread(NULL, 0, SimuL_SCmdParseThread, this, 0, &threadID);
	if( m_hParseThread == INVALID_HANDLE_VALUE )
	{
		CloseHandle(m_hParseEvent);
		DeleteCriticalSection(&m_criCmd);
		delete m_pCmdExecutorInterface;
		return FALSE;
	}

	m_bInitialize = TRUE;
	return TRUE;
}

/*****************************************************************
 *  ��������	CSimuL_SCmdParser::Uninitialize
 *  ���ܣ�		ж��������������ͷ���Դ
 *  ���:		��
 *  ����:		��
 *  ����ֵ:		��
 *****************************************************************/
void CSimuL_SCmdParser::Uninitialize()
{
	if( !m_bInitialize )
		return ;

	SetEvent(m_hParseEvent);
	WaitForSingleObject(m_hParseThread , INFINITE);
	CloseHandle(m_hParseThread);
	CloseHandle(m_hParseEvent);

	vector<CSimuL_SCmd*>::iterator iter;
	EnterCriticalSection(&m_criCmd);
	for( iter = m_vecCmd.begin() ; iter != m_vecCmd.end() ; iter++ )
	{
		delete (*iter);
	}
	m_vecCmd.clear();
	if( m_szBuffer != NULL )
		delete []m_szBuffer;
	m_szBuffer = NULL;
	m_nBufferLen = 0;
	LeaveCriticalSection(&m_criCmd);
	DeleteCriticalSection(&m_criCmd);
	delete m_pCmdExecutorInterface;
	m_bInitialize = FALSE;
}

/*****************************************************************
 *  ��������	CSimuL_SCmdParser::Parse
 *  ���ܣ�		��ָ�������ݽ���������ִ������ִ�е�����
 *  ���:		unsigned char* szData -- �����������ݻ�����
 *				unsigned long nDataLen -- �����������ݻ���������
 *  ����:		��
 *  ����ֵ:		TRUE -- �����ɹ� , FALSE -- ����ʧ��
 *****************************************************************/
BOOL CSimuL_SCmdParser::Parse(unsigned char* szData , unsigned long nDataLen)
{
	if( !m_bInitialize )
	{
		return FALSE;
	}

	if( nDataLen > 0 )
	{
		unsigned char *szBufferTmp = NULL;
		if( m_nBufferLen > 0 )
		{
			szBufferTmp = new unsigned char[m_nBufferLen];
			memcpy(szBufferTmp , m_szBuffer , m_nBufferLen);
			delete []m_szBuffer;
			m_szBuffer = NULL;
		}
		m_szBuffer = new unsigned char[m_nBufferLen + nDataLen];
		if( m_nBufferLen > 0 )
		{
			memcpy(m_szBuffer , szBufferTmp , m_nBufferLen);
		}
		memcpy(m_szBuffer + m_nBufferLen , szData , nDataLen);
		m_nBufferLen += nDataLen;
	}

	unsigned long nCmdDataLen = 0;

	while(1)
	{
		EnterCriticalSection(&m_criCmd);
		L_S_CMD cmd = CSimuL_SCmd::PreParseCmd(m_szBuffer , m_nBufferLen , nCmdDataLen);
		if( cmd != L_S_CMD_UNKNOWN )
		{
			unsigned char *szCmdData = new unsigned char[nCmdDataLen];
			unsigned char *szNewBuffer = NULL;

			memcpy(szCmdData , m_szBuffer , nCmdDataLen);

			if( m_nBufferLen - nCmdDataLen > 0 )
			{
				szNewBuffer = new unsigned char[m_nBufferLen - nCmdDataLen];
				m_nBufferLen -= nCmdDataLen;
				memcpy(szNewBuffer , m_szBuffer + nCmdDataLen , m_nBufferLen);
				delete []m_szBuffer;
				m_szBuffer = szNewBuffer;
			}
			else
			{
				delete []m_szBuffer;
				m_szBuffer = NULL;
				m_nBufferLen = 0;
			}

			CSimuL_SCmd* pCmd = NULL;

			switch( cmd )
			{
			case L_S_CMD_CONNECTTARGET :
				{
					pCmd = new CSimuL_SConnectTargetCmd;
					break;
				}
			case L_S_CMD_CONNECTTARGET_RESP :
				{
					pCmd = new CSimuL_SConnectTargetRespCmd;
					break;
				}
			case L_S_CMD_LOADSIMUMODEL :
				{
					pCmd = new CSimuL_SLoadSimuModelCmd;
					break;
				}
			case L_S_CMD_LOADSIMUMODEL_RESP :
				{
					pCmd = new CSimuL_SLoadSimuModelRespCmd;
					break;
				}
			case L_S_CMD_ENABLESIMUMONITOR:
				{
					pCmd = new CSimuL_SEnableSimuMonitorCmd;
					break;
				}
			case L_S_CMD_ENABLESIMUMONITOR_RESP:
				{
					pCmd = new CSimuL_SEnableSimuMonitorRespCmd;
					break;
				}
			case L_S_CMD_COMPILESIMUMODEL:
				{
					pCmd = new CSimuL_SCompileSimuModelCmd;
					break;
				}
			case L_S_CMD_COMPILESIMUMODEL_RESP:
				{
					pCmd = new CSimuL_SCompileSimuModelRespCmd;
					break;
				}
			case L_S_CMD_COMPILEINFO_REP:
				{
					pCmd = new CSimuL_SCompileInfoRepCmd;
					break;
				}
			default:
				break;
			}

			if( pCmd != NULL )
			{
				if( pCmd->Unserialize(szCmdData , nCmdDataLen) )
				{
					m_vecCmd.push_back(pCmd);
				}
				else
				{
					delete pCmd;
				}
			}
			delete []szCmdData;
		}
		else
		{
			LeaveCriticalSection(&m_criCmd);
			break;
		}
		LeaveCriticalSection(&m_criCmd);
		//Sleep(30);
	}

	return TRUE;
}

/*****************************************************************
 *  ��������	CSimuL_SCmdParser::ExecAllCmd
 *  ���ܣ�		ִ�������ѽ���������
 *  ���:		��
 *  ����:		��
 *  ����ֵ:		TRUE -- ִ�гɹ� , FALSE -- ִ��ʧ��
 *****************************************************************/
BOOL CSimuL_SCmdParser::ExecAllCmd()
{
	vector<CSimuL_SCmd*>::iterator iter;
	EnterCriticalSection(&m_criCmd);
	for( iter = m_vecCmd.begin() ; iter != m_vecCmd.end() ; iter++ )
	{
		switch( (*iter)->GetCmdType() )
		{
		case L_S_CMD_CONNECTTARGET :
			{
				m_pCmdExecutorInterface->ExecConnectTargetCmd((CSimuL_SConnectTargetCmd*)(*iter));
				break;
			}
		case L_S_CMD_CONNECTTARGET_RESP :
			{
				m_pCmdExecutorInterface->ExecConnectTargetRespCmd((CSimuL_SConnectTargetRespCmd*)(*iter));
				break;
			}
		case L_S_CMD_LOADSIMUMODEL :
			{
				m_pCmdExecutorInterface->ExecLoadSimuModelCmd((CSimuL_SLoadSimuModelCmd*)(*iter));
				break;
			}
		case L_S_CMD_LOADSIMUMODEL_RESP :
			{
				m_pCmdExecutorInterface->ExecLoadSimuModelRespCmd((CSimuL_SLoadSimuModelRespCmd*)(*iter));
				break;
			}
		case L_S_CMD_ENABLESIMUMONITOR:
			{
				m_pCmdExecutorInterface->ExecEnableSimuMonitorCmd((CSimuL_SEnableSimuMonitorCmd *)(*iter));
				break;
			}
		case L_S_CMD_ENABLESIMUMONITOR_RESP:
			{
				m_pCmdExecutorInterface->ExecEnableSimuMonitorRespCmd((CSimuL_SEnableSimuMonitorRespCmd*)(*iter));
				break;
			}
		case L_S_CMD_COMPILESIMUMODEL:
			{
				m_pCmdExecutorInterface->ExecCompileSimuModelCmd((CSimuL_SCompileSimuModelCmd*)(*iter));
				break;
			}
		case L_S_CMD_COMPILESIMUMODEL_RESP:
			{
				m_pCmdExecutorInterface->ExecCompileSimuModelRespCmd((CSimuL_SCompileSimuModelRespCmd*)(*iter));
				break;
			}
		case L_S_CMD_COMPILEINFO_REP:
			{
				m_pCmdExecutorInterface->ExecCompileInfoReportCmd((CSimuL_SCompileInfoRepCmd*)(*iter));
				break;
			}
		default:
			break;
		}
		delete *iter;
	}
	m_vecCmd.clear();
	LeaveCriticalSection(&m_criCmd);
	return TRUE;
}