#include "stdafx.h"
#include "SimuDispDataC_DComm.h"

//////////
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
void* SimuModelDispDataSendThread(LPVOID lpParam)
#else
DWORD WINAPI SimuModelDispDataSendThread(LPVOID lpParam)
#endif
{
	CSimuDispDataC_DComm *pComm = (CSimuDispDataC_DComm*)lpParam;
	pComm->SimuModelDispDataSendService();
	return 0;
}

//////////
CSimuDispDataC_DComm::CSimuDispDataC_DComm()
{
	m_bInit = false;
	memset(m_szDispIp, 0x0, sizeof(m_szDispIp));
	m_usPort = 0;
}

CSimuDispDataC_DComm::~CSimuDispDataC_DComm()
{
	Uninitialize();
}

bool CSimuDispDataC_DComm::Initialize(const char *szIp, unsigned short usPort)
{
	if( m_bInit){
		return false;
	}

	strcpy(m_szDispIp, szIp);
	m_usPort = usPort;

	if( !InitSocket()){
		return false;
	}

	InitializeCriticalSection(&m_criSocket);
	InitializeCriticalSection(&m_criWaitData);

	DWORD tid;
	m_hSendEvent = CreateEvent(NULL , TRUE , FALSE , NULL);
	m_hSendThread = ::CreateThread(NULL, 0, SimuModelDispDataSendThread, this, 0, &tid);
	if( m_hSendThread == INVALID_HANDLE_VALUE )
	{
		CloseHandle(m_hSendEvent);
		return false;
	}

	m_bInit = true;
	return true;
}

void CSimuDispDataC_DComm::Uninitialize()
{
	if( !m_bInit){
		return;
	}
	closesocket(m_cltSocket);
	DeleteCriticalSection(&m_criSocket);

	SetEvent(m_hSendEvent);
	WaitForSingleObject(m_hSendThread, INFINITE);
	CloseHandle(m_hSendThread);
	CloseHandle(m_hSendEvent);

	list<DispDataWaitSend_t *>::iterator iter;
	DispDataWaitSend_t *pDispData = NULL;
	for( iter = m_lstWaitData.begin(); iter != m_lstWaitData.end(); iter++){
		pDispData = *iter;
		delete[] pDispData->pData;
		delete pDispData;
	}
	m_lstWaitData.clear();

	m_bInit = false;
}

/* 发送数据 */
int  CSimuDispDataC_DComm::SendData(unsigned char *szData, unsigned long nDataLen, bool bDirectSend)
{
	if( bDirectSend){
		int nSendLen = 0;
		struct sockaddr_in remoteAddr;

		memset(&remoteAddr, 0x0, sizeof(sockaddr_in));
		remoteAddr.sin_family = AF_INET;
		remoteAddr.sin_addr.s_addr = inet_addr(m_szDispIp);
		remoteAddr.sin_port = htons(m_usPort);

		EnterCriticalSection(&m_criSocket);
		nSendLen = sendto(m_cltSocket, (char *)szData, nDataLen, 0,(const struct sockaddr*)&remoteAddr, sizeof(sockaddr_in));
		LeaveCriticalSection(&m_criSocket);
		return nSendLen;
	}
	else
	{
		DispDataWaitSend_t *pDispData = new DispDataWaitSend_t;
		pDispData->pData = new char[nDataLen];
		memcpy(pDispData->pData, szData, nDataLen);
		pDispData->nDataLen = nDataLen;

		EnterCriticalSection(&m_criWaitData);
		m_lstWaitData.push_front(pDispData);
		LeaveCriticalSection(&m_criWaitData);
		return nDataLen;
	}
}

/* 初始化客户端socket信息  */
bool CSimuDispDataC_DComm::InitSocket()
{
	char		localIP[32];
	sockaddr_in svrAddr;

#ifdef WIN_PLATFORM
	hostent*	localHost;
	u_long		ul = 1;
#else
	unsigned long ul = 1;
	memset(localIP , 0 , sizeof(localIP));
#endif

	if( strcmp(m_szDispIp , "") == 0 )
	{
#ifdef WIN_PLATFORM
		localHost = gethostbyname("");
		char *tmpIp = inet_ntoa(*(struct in_addr *)*localHost->h_addr_list);
		if( tmpIp != NULL){
			strcpy(localIP, tmpIp);
		}else{
			return false;
		}
#else
		GetLocalIp(localIP);
#endif
	}
	else
	{
#ifdef WIN_PLATFORM
		sprintf_s(localIP , sizeof(localIP) , "%s" , m_szDispIp);
#else
		sprintf(localIP , "%s" , m_szDispIp);
#endif
	}
	if( localIP != NULL)
		printf("szTargetIP = %s\n", localIP);

	svrAddr.sin_family = AF_INET;
	svrAddr.sin_port = htons(m_usPort);
	svrAddr.sin_addr.s_addr = inet_addr(localIP);
	memset(svrAddr.sin_zero, 0, 8 * sizeof(char));

	m_cltSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_cltSocket == INVALID_SOCKET){
		return false;
	}

	int zero = 0;
	BOOL bOpt = FALSE;

#if defined(QNX_PLATFORM)
	ioctlsocket(m_cltSocket, FIONBIO, &ul);
#elif defined(LINUX_PLATFORM)

#else
	ioctlsocket(m_cltSocket, FIONBIO, &ul);
#endif
	setsockopt(m_cltSocket , SOL_SOCKET , SO_BROADCAST, (const char *)&bOpt, sizeof (BOOL));
	setsockopt(m_cltSocket , SOL_SOCKET , SO_SNDBUF , (const char*)&zero , sizeof(int));
	setsockopt(m_cltSocket , SOL_SOCKET , SO_RCVBUF , (const char*)&zero , sizeof(int));

	return true;
}

#define MAX_SNED_DISP_DATA_BUFF_LEN 10240
void CSimuDispDataC_DComm::SimuModelDispDataSendService()
{
	int nListSize = 0;

	unsigned char *pSendBuff = NULL;
	unsigned long ulSendDataLen = 0;
	int  nRet = 0;
	DispDataWaitSend_t *pDispData = NULL;

	pSendBuff = new unsigned char[MAX_SNED_DISP_DATA_BUFF_LEN];
	ulSendDataLen = 0;

	struct sockaddr_in remoteAddr;
	memset(&remoteAddr, 0x0, sizeof(sockaddr_in));
	remoteAddr.sin_family = AF_INET;
	remoteAddr.sin_addr.s_addr = inet_addr(m_szDispIp);
	remoteAddr.sin_port = htons(m_usPort);

	while(1)
	{
		/*检测退出信号*/
		if( WaitForSingleObject(m_hSendEvent, 0) == WAIT_OBJECT_0 ){
			break;
		}

		EnterCriticalSection(&m_criWaitData);
		nListSize = m_lstWaitData.size();
		LeaveCriticalSection(&m_criWaitData);

		if( nListSize <= 0){
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
			usleep(1);
#else
			Sleep(1);
#endif
			continue;
		}

		do
		{
			if( pDispData == NULL){
				EnterCriticalSection(&m_criWaitData);
				nListSize = m_lstWaitData.size();
				if( nListSize >= 1){
					pDispData = m_lstWaitData.back();
					m_lstWaitData.pop_back();
				}
				LeaveCriticalSection(&m_criWaitData);
			}

			if( pDispData == NULL){
				break;
			}

			if( ulSendDataLen + pDispData->nDataLen > MAX_SNED_DISP_DATA_BUFF_LEN){
				nRet = sendto(m_cltSocket, (char *)pSendBuff, ulSendDataLen, 0,(const struct sockaddr*)&remoteAddr, sizeof(sockaddr_in));
				ulSendDataLen = 0;
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
				usleep(1);
#else
				Sleep(1);
#endif
			}

			memcpy(pSendBuff + ulSendDataLen, pDispData->pData, pDispData->nDataLen);
			ulSendDataLen += pDispData->nDataLen;

			delete []pDispData->pData;
			delete pDispData;
			pDispData = NULL;
		}while(1);

		if( ulSendDataLen > 0){
			nRet = sendto(m_cltSocket, (char *)pSendBuff, ulSendDataLen, 0,(const struct sockaddr*)&remoteAddr, sizeof(sockaddr_in));
			ulSendDataLen = 0;
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
			usleep(1);
#else
			Sleep(1);
#endif
		}
	}
	delete []pSendBuff;
}
