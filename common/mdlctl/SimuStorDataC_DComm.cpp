#include "stdafx.h"
#include "SimuStorDataC_DComm.h"
#include "SimuCommAPI.h"

static void StorSvrConnCallback(BOOL bConnected , void* pConnectInfo , void* pParam);
static void StorSvrRcvedDataCallback(unsigned char* szData , unsigned long nDataLen , void* pParam);

CSimuStorDataC_DComm::CSimuStorDataC_DComm()
{
	m_bInit = false;
	m_nClientID = 0;
	memset(m_szStorIp, 0x0, sizeof(m_szStorIp));
	m_usPort = 0;
}

CSimuStorDataC_DComm::~CSimuStorDataC_DComm()
{
	Uninitialize();
}

bool CSimuStorDataC_DComm::Initialize(const char *szIp, unsigned short usPort, bool bLocalUseShareMem)
{
	if( m_bInit){
		return false;
	}

	if( bLocalUseShareMem)
	{/*共享内存*/
		m_SDataC_DCommType = SDC_DCommType_Shm;

#ifdef WIN_PLATFORM
		m_hWriteShmfd = OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,(LPCTSTR)KL_COMMU_M_C_STOR_DATA_SHM_NAME); 
		if( m_hWriteShmfd == INVALID_HANDLE_VALUE || m_hWriteShmfd == NULL){
			return false;
		}
		m_pWriteShm = (CommuSimuStorDataC_DShm_t *)MapViewOfFile(m_hWriteShmfd, FILE_MAP_ALL_ACCESS,0,0, sizeof(CommuSimuStorDataC_DShm_t));
		if( m_pWriteShm == NULL){
			return false;
		}
		m_hWriteShmLock = OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,(LPCTSTR)KL_COMMU_M_C_STOR_DATA_LOCK_NAME);
		if( m_hWriteShmLock == INVALID_HANDLE_VALUE || m_hWriteShmLock == NULL){
			return false;
		}
		m_hWriteSemSignal = OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,(LPCTSTR)KL_COMMU_M_C_STOR_DATA_SEM_SIG);
		if( m_hWriteSemSignal == INVALID_HANDLE_VALUE || m_hWriteSemSignal == NULL){
			return false;
		}
		m_pWriteShm->ReadDataPtr = 0;
		m_pWriteShm->WriteDataPtr = 0;
		m_pWriteShm->nCoverCount = 0;
#endif
	}else{/*TCP/IP*/
		KLCltParam param;

		m_SDataC_DCommType = SDC_DCommType_TCP;
		memcpy(m_szStorIp, szIp, strlen(szIp));
		m_usPort = usPort;

		param.ct = CT_TCP;
		param.bAutoReconnect = TRUE;
		param.bUseRcvedDataCallback = TRUE;
		param.connect_callback = StorSvrConnCallback;
		param.rcveddata_callback = StorSvrRcvedDataCallback;
		param.readyreconnect_callback = NULL;
		param.nAutoReconnectWaitTime = 20;
		param.nConnTimeout = 20;
		param.pParam = this;
		sprintf(param.ExtraParam.TcpExtraParam.szSvrIp, "%s" , szIp);
		param.ExtraParam.TcpExtraParam.nPort = usPort;

		m_nClientID = KLSC_CreateClient(&param);
		if( m_nClientID <= 0 )
			return false;
	}

	m_bInit = true;
	return true;
}

void CSimuStorDataC_DComm::Uninitialize()
{
	if( !m_bInit){
		return;
	}

	if( m_SDataC_DCommType == SDC_DCommType_TCP)
	{
		KLSC_DeleteClient(m_nClientID);
	}
	else if( m_SDataC_DCommType == SDC_DCommType_Shm)
	{
#ifdef WIN_PLATFORM
		UnmapViewOfFile(m_pWriteShm);
		CloseHandle(m_hWriteSemSignal);
		CloseHandle(m_hWriteShmfd);
		CloseHandle(m_hWriteShmLock);
#endif
	}

	m_bInit = false;
}

int  CSimuStorDataC_DComm::SendData(unsigned char *szData, unsigned long nDataLen)
{
	if( m_SDataC_DCommType == SDC_DCommType_TCP)
	{
		if( !m_bConnect){
			return -1;
		}
		return KLSC_ClientSend(m_nClientID , nDataLen, szData);
	}
	else if( m_SDataC_DCommType == SDC_DCommType_Shm)
	{
		int nWriteIdx = 0;
#ifdef WIN_PLATFORM
		WaitForSingleObject(m_hWriteShmLock, INFINITE);
		if(m_pWriteShm->ReadDataPtr == (m_pWriteShm->WriteDataPtr+1)%MAX_SDCD_SHM_QUEUE_LEN){
			//m_pWriteShm->nCoverCount++;
			ReleaseSemaphore(m_hWriteShmLock, 1, 0);
			return 0;
		}
		nWriteIdx = m_pWriteShm->WriteDataPtr%MAX_SDCD_SHM_QUEUE_LEN;

		memcpy(m_pWriteShm->szData[nWriteIdx], szData, nDataLen);
		m_pWriteShm->nDataLen[nWriteIdx] = nDataLen;

		m_pWriteShm->WriteDataPtr++;
		m_pWriteShm->WriteDataPtr %= MAX_SDCD_SHM_QUEUE_LEN;
		ReleaseSemaphore(m_hWriteShmLock, 1, 0);

		ReleaseSemaphore(m_hWriteSemSignal, 1, 0);
		return nDataLen;
#endif
	}
	
	return -1;
}

void CSimuStorDataC_DComm::ClearShm()
{
#ifdef WIN_PLATFORM
	if( m_SDataC_DCommType == SDC_DCommType_Shm)
	{
		WaitForSingleObject(m_hWriteShmLock, INFINITE);
		m_pWriteShm->WriteDataPtr = 0;
		m_pWriteShm->ReadDataPtr = 0;
		ReleaseSemaphore(m_hWriteShmLock, 1, 0);
	}
#endif
}

void CSimuStorDataC_DComm::OnRcvedStorSvrData(unsigned char* szData , unsigned long nDataLen)
{

}

void CSimuStorDataC_DComm::OnStorSvrConnected(KLConnectInfo* pConnectInfo)
{
	m_bConnect = true;
}

void CSimuStorDataC_DComm::OnStorSvrDisconnected(KLConnectInfo* pConnectInfo)
{
	m_bConnect = false;
}

//////////////////////////////////////////////////////////////////
///   					静态函数实现
//////////////////////////////////////////////////////////////////

void StorSvrConnCallback(BOOL bConnected , void* pConnectInfo , void* pParam)
{
	CSimuStorDataC_DComm *pComm = (CSimuStorDataC_DComm *)pParam;

	if( pComm == NULL )
		return ;

	if( bConnected ){
		pComm->OnStorSvrConnected((KLConnectInfo*)pConnectInfo);
	}else{
		pComm->OnStorSvrDisconnected((KLConnectInfo*)pConnectInfo);
	}
}
void StorSvrRcvedDataCallback(unsigned char* szData , unsigned long nDataLen , void* pParam)
{
	CSimuStorDataC_DComm *pComm = (CSimuStorDataC_DComm *)pParam;

	if( pComm == NULL )
		return ;

	pComm->OnRcvedStorSvrData(szData , nDataLen);
}

