/*****************************************************************
 * �ļ���: SimuD_SCmdParser.cpp
 * ����: SimuMonitor �� SimuService֮��ͨ������ִ�кͽ�����ʵ��
 *****************************************************************/

#include "stdafx.h" 
#include "SimuD_SCmdParser.h"

/*****************************************************************
 *  ��������	    SimuD_SCmdParseThread
 *  ���ܣ�		SimuDisplay��SimuService֮�������ִ���߳�
 *  ���:		LPVOID lpParam -- �����߳�ʱ���ݽ������̶߳�����Ϣ
 *  ����:		��
 *  ����ֵ:		DWORD(QNX: void*) -- �߳��˳�ʱ���ݸ�����ϵͳ��ֵ
 *****************************************************************/
DWORD WINAPI SimuD_SCmdParseThread(LPVOID lpParam)
{
	CSimuD_SCmdParser* pParser = (CSimuD_SCmdParser*)lpParam;
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
 *  ��������	CSimuD_SCmdParser::CSimuD_SCmdParser
 *  ���ܣ�		SimuDisplay��SimuService֮�������������๹�캯��
 *  ���:		CSimuD_SCmdExecutorInferface* pExecutorInterface -- ����ִ����
 *  ����:		��
 *  ����ֵ:		��
 *****************************************************************/
CSimuD_SCmdParser::CSimuD_SCmdParser(CSimuD_SCmdExecutorInferface* pExecutorInterface)
{
	m_bInitialize = FALSE;
	m_pCmdExecutorInterface = pExecutorInterface;
	m_szBuffer = NULL;
}

/*****************************************************************
 *  ��������	CSimuD_SCmdParser::~CSimuD_SCmdParser
 *  ���ܣ�		SimuDisplay��SimuService֮����������������������
 *  ���:		��
 *  ����:		��
 *  ����ֵ:		��
 *****************************************************************/
CSimuD_SCmdParser::~CSimuD_SCmdParser()
{
	Uninitialize();
}

/*****************************************************************
 *  ��������	CSimuD_SCmdParser::Initialize
 *  ���ܣ�		��ʼ�����������
 *  ���:		��
 *  ����:		��
 *  ����ֵ:		TRUE -- ��ʼ���ɹ� , FALSE -- ��ʼ��ʧ��
 *****************************************************************/
BOOL CSimuD_SCmdParser::Initialize()
{
	if( m_bInitialize )
		return FALSE;
	m_szBuffer = NULL;
	m_nBufferLen = 0;
	InitializeCriticalSection(&m_criCmd);

	DWORD threadID;

	m_hParseEvent = CreateEvent(NULL , TRUE , FALSE , NULL);

	m_hParseThread = ::CreateThread(NULL, 0, SimuD_SCmdParseThread, this, 0, &threadID);
	if( m_hParseThread == INVALID_HANDLE_VALUE )
	{
		CloseHandle(m_hParseEvent);
		DeleteCriticalSection(&m_criCmd);
		return FALSE;
	}

	m_bInitialize = TRUE;
	return TRUE;
}

/*****************************************************************
 *  ��������	CSimuD_SCmdParser::Uninitialize
 *  ���ܣ�		ж��������������ͷ���Դ
 *  ���:		��
 *  ����:		��
 *  ����ֵ:		��
 *****************************************************************/
void CSimuD_SCmdParser::Uninitialize()
{
	if( !m_bInitialize )
		return ;

	SetEvent(m_hParseEvent);
	WaitForSingleObject(m_hParseThread , INFINITE);
	CloseHandle(m_hParseThread);
	CloseHandle(m_hParseEvent);

	vector<CSimuD_SCmd*>::iterator iter;
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
	m_bInitialize = FALSE;
}

/*****************************************************************
 *  ��������	CSimuD_SCmdParser::Parse
 *  ���ܣ�		��ָ�������ݽ���������ִ������ִ�е�����
 *  ���:		unsigned char* szData -- �����������ݻ�����
 *				unsigned long nDataLen -- �����������ݻ���������
 *  ����:		��
 *  ����ֵ:		TRUE -- �����ɹ� , FALSE -- ����ʧ��
 *****************************************************************/
BOOL CSimuD_SCmdParser::Parse(unsigned char* szData , unsigned long nDataLen)
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

	while( 1 )
	{
		EnterCriticalSection(&m_criCmd);

		D_S_CMD cmd = CSimuD_SCmd::PreParseCmd(m_szBuffer , m_nBufferLen , nCmdDataLen);
		if( cmd != D_S_CMD_UNKNOWN )
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

			CSimuD_SCmd* pCmd = NULL;

			switch( cmd )
			{
			case D_S_CMD_INITMONITOR:
				{
					pCmd = new CSimuD_SInitMonitorCmd;
					break;
				}

			case D_S_CMD_INITMONITOR_RESP:
				{
					pCmd = new CSimuD_SInitMonitorRespCmd;
					break;
				}
			case D_S_CMD_LOADSIMUMODEL_REPORT:
				{
					pCmd = new CSimuD_SLoadSimuModelReportCmd;
					break;
				}
			case D_S_CMD_EXECSIMUMODEL:
				{
					pCmd = new CSimuD_SExecSimuModelCmd;
					break;
				}
			case D_S_CMD_EXECSIMUMODEL_RESP:
				{
					pCmd = new CSimuD_SExecSimuModelRespCmd;
					break;
				}
			case D_S_CMD_PAUSESIMUMODEL:
				{
					pCmd = new CSimuD_SPauseSimuModelCmd;
					break;
				}
			case D_S_CMD_PAUSESIMUMODEL_RESP:
				{
					pCmd = new CSimuD_SPauseSimuModelRespCmd;
					break;
				}
			case D_S_CMD_RESETSIMUMODEL:
				{
					pCmd = new CSimuD_SResetSimuModelCmd;
					break;
				}
			case D_S_CMD_RESETSIMUMODEL_RESP:
				{
					pCmd = new CSimuD_SResetSimuModelRespCmd;
					break;
				}
			case D_S_CMD_SETSIMUMODELPARAM:
				{
					pCmd = new CSimuD_SSetSimuModelParamCmd;
					break;
				}
			case D_S_CMD_SETSIMUMODELPARAM_RESP:
				{
					pCmd = new CSimuD_SSetSimuModelParamRespCmd;
					break;
				}
			case D_S_CMD_TESTEVENT_REPORT:
				{
					pCmd = new CSimuD_STestEventReportCmd;
					break;
				}
			case D_S_CMD_EXCEPTEVENT_REPORT:
				{
					pCmd = new CSimuD_SExceptEventReportCmd;
					break;
				}
			case D_S_CMD_ENABLESIMUMONITOR:
				{
					pCmd = new CSimuD_SEnableSimuMonitorCmd;
					break;
				}
			case D_S_CMD_ENABLESIMUMONITOR_RESP:
				{
					pCmd = new CSimuD_SEnableSimuMonitorRespCmd;
					break;
				}

			case D_S_CMD_UNKNOWN:
				break;

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
 *  ��������	CSimuD_SCmdParser::ExecAllCmd
 *  ���ܣ�		ִ�������ѽ���������
 *  ���:		��
 *  ����:		��
 *  ����ֵ:		TRUE -- ִ�гɹ� , FALSE -- ִ��ʧ��
 *****************************************************************/
BOOL CSimuD_SCmdParser::ExecAllCmd()
{
	vector<CSimuD_SCmd*>::iterator iter;

	EnterCriticalSection(&m_criCmd);
	for( iter = m_vecCmd.begin() ; iter != m_vecCmd.end() ; iter++ )
	{
		switch( (*iter)->GetCmdType() )
		{
		case D_S_CMD_INITMONITOR:
			{
				m_pCmdExecutorInterface->ExecInitMonitorCmd((CSimuD_SInitMonitorCmd*)(*iter));
				break;
			}

		case D_S_CMD_INITMONITOR_RESP:
			{
				m_pCmdExecutorInterface->ExecInitMonitorRespCmd((CSimuD_SInitMonitorRespCmd*)(*iter));
				break;
			}

		case D_S_CMD_LOADSIMUMODEL_REPORT:
			{
				m_pCmdExecutorInterface->ExecLoadSimuModelReportCmd((CSimuD_SLoadSimuModelReportCmd*)(*iter));
				break;
			}

		case D_S_CMD_EXECSIMUMODEL:
			{
				m_pCmdExecutorInterface->ExecExecSimuModelCmd((CSimuD_SExecSimuModelCmd*)(*iter));
				break;
			}

		case D_S_CMD_EXECSIMUMODEL_RESP:
			{
				m_pCmdExecutorInterface->ExecExecSimuModelRespCmd((CSimuD_SExecSimuModelRespCmd*)(*iter));
				break;
			}

		case D_S_CMD_PAUSESIMUMODEL:
			{
				m_pCmdExecutorInterface->ExecPauseSimuModelCmd((CSimuD_SPauseSimuModelCmd*)(*iter));
				break;
			}

		case D_S_CMD_PAUSESIMUMODEL_RESP:
			{
				m_pCmdExecutorInterface->ExecPauseSimuModelRespCmd((CSimuD_SPauseSimuModelRespCmd*)(*iter));
				break;
			}

		case D_S_CMD_RESETSIMUMODEL:
			{
				m_pCmdExecutorInterface->ExecResetSimuModelCmd((CSimuD_SResetSimuModelCmd*)(*iter));
				break;
			}

		case D_S_CMD_RESETSIMUMODEL_RESP:
			{
				m_pCmdExecutorInterface->ExecResetSimuModelRespCmd((CSimuD_SResetSimuModelRespCmd*)(*iter));
				break;
			}
		case D_S_CMD_SETSIMUMODELPARAM:
			{
				m_pCmdExecutorInterface->ExecSetSimuModelParamCmd((CSimuD_SSetSimuModelParamCmd*)(*iter));
				break;
			}
		case D_S_CMD_SETSIMUMODELPARAM_RESP:
			{
				m_pCmdExecutorInterface->ExecSetSimuModelParamRespCmd((CSimuD_SSetSimuModelParamRespCmd*)(*iter));
				break;
			}
		case D_S_CMD_TESTEVENT_REPORT:
			{
				m_pCmdExecutorInterface->ExecTestEventReportCmd((CSimuD_STestEventReportCmd*)(*iter));
				break;
			}
		case D_S_CMD_EXCEPTEVENT_REPORT:
			{
				m_pCmdExecutorInterface->ExecExceptEventReportCmd((CSimuD_SExceptEventReportCmd*)(*iter));
				break;
			}
		case D_S_CMD_ENABLESIMUMONITOR:
			{
				m_pCmdExecutorInterface->ExecEnableSimuMonitorCmd((CSimuD_SEnableSimuMonitorCmd*)(*iter));
				break;
			}
		case D_S_CMD_ENABLESIMUMONITOR_RESP:
			{
				m_pCmdExecutorInterface->ExecEnableSimuMonitorRespCmd((CSimuD_SEnableSimuMonitorRespCmd*)(*iter));
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