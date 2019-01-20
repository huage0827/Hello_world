#include "QNXPlatform.h"

#define HANDLE_MASK_THREAD 		0x40000000
#define HANDLE_MASK_EVENT		0x20000000
#define HANDLE_MASK_SEMAPHORE	0x02000000
#define HANDLE_MASK_SHAREMEMORY 0x04000000

#define HANDLE_MAX_COUNT		0x00FFFFFF

typedef struct tagThreadNode
{
	HANDLE				handle;
	pthread_t			*pThread;
	pthread_attr_t		*pAttrThread;
} ThreadNode , *PThreadNode;

typedef struct tagEventNode
{
	HANDLE				handle;
	pthread_cond_t		*pCond;
	pthread_condattr_t 	*pAttrCond;
	pthread_mutex_t 	*pMutex;
	BOOL 				bManualReset;
	BOOL				bSignal;
} EventNode , *PEventNode;

typedef struct tagCriticalSectionNode
{
	LPCRITICAL_SECTION	section;
	pthread_mutex_t 	*pMutex;
	pthread_mutexattr_t	*pMutexAttr;
} CriticalSectionNode , *PCriticalSectionNode;

typedef struct tagSemaphoreNode
{
	HANDLE			handle;
	int				nfd;
	sem_t*			pSem;
	char* 			pName;
	BOOL			bCreate;
}SemaphoreNode, *PSemaphoreNode;

typedef struct tagShareMemoryNode
{
	HANDLE			handle;
	HANDLE 			nFd;
	void*			pShm;
	char*			pName;
	DWORD			nLen;
	DWORD 			nAccess;
	BOOL			bCreate;
}ShareMemoryNode, *PShareMemoryNode;

static DWORD s_dwErrCode;

static map<HANDLE , EventNode*> s_Evts;
static vector<CriticalSectionNode*> s_CriSecs;
static map<HANDLE , ThreadNode*> s_Threads;
//static map<HANDLE , ShmEventNode* > s_ShmEvents;
static map<HANDLE , SemaphoreNode*> s_Semaphores;
static map<HANDLE , ShareMemoryNode *> s_ShareMemorys;


static void GetAbsTime(DWORD dwMilliseconds , struct timespec* abstime);
static BOOL IsThreadObject(HANDLE h);
static ThreadNode* GetThreadNode(HANDLE h);
static void DestroyThreadNode(HANDLE h);
static BOOL IsEventObject(HANDLE h);
static EventNode* GetEventNode(HANDLE h);
static void DestroyEventNode(HANDLE h);
static CriticalSectionNode* GetCriticalSectionNode(LPCRITICAL_SECTION cri);
static void DestroyCriticalSectionNode(LPCRITICAL_SECTION cri);

static BOOL IsSemaphoreObject(HANDLE h);
static SemaphoreNode* GetSemaphoreNode(HANDLE h);
static void DestroySemaphoreNode(HANDLE h);

static BOOL IsShareMemoryObject(HANDLE h);
//static ShareMemoryNode* GetShareMemoryNode(HANDLE h);
static void DestroyShareMemoryNode(HANDLE h);

int WSAStartup(WORD wVersionRequested,LPWSADATA lpWSAData)
{
	return 0;
}

int WSACleanup()
{
	return 0;
}

int WSAGetLastError(void)
{
	return 0;
}

HANDLE CreateThread(
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	SIZE_T dwStackSize,
	LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID lpParameter,
	DWORD dwCreationFlags,
	LPDWORD lpThreadId
	)
{
	ThreadNode* pThreadNode = new ThreadNode;
	pthread_t *threadObj = new pthread_t;
	pthread_attr_t *threadAttr = new pthread_attr_t;
	int i = 0;

	pthread_attr_init(threadAttr);
	pthread_attr_setstacksize(threadAttr , dwStackSize);
	pthread_create(threadObj , threadAttr , lpStartAddress , lpParameter);

	if( lpThreadId != NULL )
		*lpThreadId = (DWORD)*threadObj;

	pThreadNode->pAttrThread = threadAttr;
	pThreadNode->pThread = threadObj;

	for( i = 1 ; i < HANDLE_MAX_COUNT ; i++ )
	{
		if( s_Threads.find((HANDLE)(i | HANDLE_MASK_THREAD)) == s_Threads.end() )
		{
			pThreadNode->handle = (HANDLE)(i | HANDLE_MASK_THREAD);
			s_Threads[pThreadNode->handle] = pThreadNode;
			return pThreadNode->handle;
		}
	}

	return INVALID_HANDLE_VALUE;
}

HANDLE CreateEvent(
    LPSECURITY_ATTRIBUTES lpEventAttributes,
    BOOL bManualReset,
    BOOL bInitialState,
    LPCSTR lpName
    )
{
	EventNode* pEventNode = NULL;
	int i = 0;
	pthread_cond_t *cond = new pthread_cond_t;
	pthread_condattr_t *attr = new pthread_condattr_t;
	pthread_mutex_t *mutex = new pthread_mutex_t;
	int nRet = 0;

	nRet = pthread_condattr_init(attr);
	if( nRet != 0 )
	{
		delete cond;
		delete attr;
		delete mutex;
		return INVALID_HANDLE_VALUE;
	}
	nRet = pthread_cond_init(cond , attr);
	if( nRet != 0 )
	{
		pthread_condattr_destroy(attr);
		delete cond;
		delete attr;
		delete mutex;
		return INVALID_HANDLE_VALUE;
	}
	nRet = pthread_mutex_init(mutex , NULL);
	if( nRet != 0 )
	{
		pthread_condattr_destroy(attr);
		pthread_cond_destroy(cond);
		delete cond;
		delete attr;
		delete mutex;
		return INVALID_HANDLE_VALUE;
	}

	pEventNode = new EventNode;
	pEventNode->pCond = cond;
	pEventNode->pAttrCond = attr;
	pEventNode->pMutex = mutex;
	pEventNode->bSignal = FALSE;

	if( bInitialState )
	{
		pthread_mutex_lock(mutex);
		pEventNode->bSignal = TRUE;
		pthread_cond_signal(cond);
		pthread_mutex_unlock(mutex);
	}

	for( i = 1 ; i < HANDLE_MAX_COUNT ; i++ )
	{
		if( s_Evts.find((HANDLE)(i | HANDLE_MASK_EVENT)) == s_Evts.end() )
		{
			pEventNode->handle = (HANDLE)(i | HANDLE_MASK_EVENT);
			s_Evts[pEventNode->handle] = pEventNode;
			return pEventNode->handle;
		}
	}

	pthread_cond_destroy(cond);
	pthread_condattr_destroy(attr);
	pthread_mutex_destroy(mutex);
	delete mutex;
	delete attr;
	delete cond;
	delete pEventNode;

	return INVALID_HANDLE_VALUE;
}

BOOL SetEvent(HANDLE hEvent)
{
	EventNode* pEventNode = GetEventNode(hEvent);
	if( pEventNode == NULL )
		return FALSE;

	pthread_mutex_lock(pEventNode->pMutex);
	pEventNode->bSignal = TRUE;
	pthread_cond_signal(pEventNode->pCond);
	pthread_mutex_unlock(pEventNode->pMutex);

	return TRUE;
}

BOOL ResetEvent(HANDLE hEvent)
{
	EventNode* pEventNode = GetEventNode(hEvent);
	if( pEventNode == NULL )
		return FALSE;

	pthread_mutex_lock(pEventNode->pMutex);
	pEventNode->bSignal = FALSE;
	pthread_mutex_unlock(pEventNode->pMutex);

	return TRUE;
}

BOOL CloseHandle(HANDLE hObject)
{
	if( IsEventObject(hObject) )
	{
		DestroyEventNode(hObject);
	}
	else if( IsThreadObject(hObject) )
	{
		DestroyThreadNode(hObject);
	}
	else if( IsSemaphoreObject(hObject))
	{
		DestroySemaphoreNode(hObject);
	}
	else if( IsShareMemoryObject(hObject))
	{
		DestroyShareMemoryNode(hObject);
	}
	return TRUE;
}

DWORD WaitForSingleObject(HANDLE hHandle,DWORD dwMilliseconds)
{
	if( IsEventObject(hHandle) )
	{
		int nRet = 0;
		DWORD dwRet = -1;
		EventNode* pEventNode = GetEventNode(hHandle);
		if( pEventNode == NULL )
			return -1;
		struct timespec abstime;
		GetAbsTime(dwMilliseconds , &abstime);
		pthread_mutex_lock(pEventNode->pMutex);
		nRet = pthread_cond_timedwait(pEventNode->pCond ,
									  pEventNode->pMutex ,
									  &abstime);
		if( nRet == ETIMEDOUT )
			dwRet = -1;
		else if( nRet != 0 )
			dwRet = -1;
		if( pEventNode->bSignal )
			dwRet = WAIT_OBJECT_0;
		if( !pEventNode->bManualReset )
			pEventNode->bSignal = FALSE;
		pthread_mutex_unlock(pEventNode->pMutex);
		return dwRet;

	}
	else if( IsThreadObject(hHandle) )
	{
		ThreadNode* pThreadNode = GetThreadNode(hHandle);
		if( pThreadNode == NULL )
			return -1;
		int *value = NULL;
		struct timespec abstime;
		GetAbsTime(dwMilliseconds , &abstime);

		if( pthread_timedjoin(*pThreadNode->pThread , (void**)&value , &abstime) == 0 )
		{
			return WAIT_OBJECT_0;
		}
		else
		{
			return -1;
		}
	}
	else if( IsSemaphoreObject(hHandle))
	{
		//printf("sem_timedwait: %0x \n ",hHandle);
		SemaphoreNode *pSemNode = GetSemaphoreNode(hHandle);
		if(pSemNode == NULL )
			return -1;
		struct timespec abstime;
		GetAbsTime(dwMilliseconds , &abstime);

		if( sem_timedwait(pSemNode->pSem, &abstime) == 0)
		{
			return WAIT_OBJECT_0;
		}
		else
		{
			return -1;
		}
	}
	else if( IsShareMemoryObject(hHandle))
	{
		return -1;
	}

	return -1;
}

void InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	pthread_mutex_t *mutex = new pthread_mutex_t;
	pthread_mutexattr_t* attr = new pthread_mutexattr_t;
	CriticalSectionNode* pCriticalSectionNode = new CriticalSectionNode;
	vector<CriticalSectionNode*>::iterator iter;

	pthread_mutexattr_init(attr);
	pthread_mutexattr_settype(attr,PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(mutex , attr);

	pCriticalSectionNode->pMutex = mutex;
	pCriticalSectionNode->pMutexAttr = attr;
	pCriticalSectionNode->section = lpCriticalSection;

	for( iter = s_CriSecs.begin() ; iter != s_CriSecs.end() ; iter++ )
	{
		if( (*iter)->section == lpCriticalSection )
		{
			DeleteCriticalSection(lpCriticalSection);
			break;
		}
	}
	s_CriSecs.push_back(pCriticalSectionNode);
}

void DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	DestroyCriticalSectionNode(lpCriticalSection);
}

void EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	CriticalSectionNode* pCriticalSectionNode = GetCriticalSectionNode(lpCriticalSection);
	if( pCriticalSectionNode == NULL )
	{
		return ;
	}
	pthread_mutex_lock(pCriticalSectionNode->pMutex);
}

void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	CriticalSectionNode* pCriticalSectionNode = GetCriticalSectionNode(lpCriticalSection);
	if( pCriticalSectionNode == NULL )
	{
		return ;
	}
	pthread_mutex_unlock(pCriticalSectionNode->pMutex);
}

void Sleep(DWORD dwMilliseconds)
{
	while( dwMilliseconds > 0 )
	{
		if( dwMilliseconds % 1000 == 0 )
		{
			usleep(1000 * 1000);
			dwMilliseconds = dwMilliseconds - 1000;
		}
		else
		{
			usleep((dwMilliseconds % 1000)*1000);
			dwMilliseconds = dwMilliseconds / 1000 * 1000;
		}
	}
}

int closesocket(SOCKET s)
{
	return close(s);
}

int ioctlsocket(SOCKET s, long cmd, u_long *argp)
{
	return ioctl_socket(s , cmd , argp);
}
/*
int sprintf_s (
        char *string,
        size_t sizeInBytes,
        const char *format,
        ...
        )
{
	va_list arglist;
	va_start(arglist, format);
	vsprintf(string, format, arglist);
	va_end(arglist);
	return strlen(string);
}*/

void GetAbsTime(DWORD dwMilliseconds , struct timespec* abstime)
{
	struct timeval now;

	gettimeofday(&now, NULL);

	abstime->tv_nsec = now.tv_usec * 1000 + (dwMilliseconds % 1000) * 1000000;
	abstime->tv_sec = now.tv_sec + dwMilliseconds / 1000;
}

BOOL IsThreadObject(HANDLE h)
{
	if( h == INVALID_HANDLE_VALUE )
		return FALSE;
	return (h & HANDLE_MASK_THREAD) == HANDLE_MASK_THREAD;
}

ThreadNode* GetThreadNode(HANDLE h)
{
	map<HANDLE , ThreadNode*>::iterator iter;

	iter = s_Threads.find(h);

	if( iter == s_Threads.end() )
		return NULL;

	return (*iter).second;
}

void DestroyThreadNode(HANDLE h)
{
	ThreadNode* pThreadNode = NULL;
	map<HANDLE , ThreadNode*>::iterator iter;

	iter = s_Threads.find(h);
	if( iter == s_Threads.end() )
		return ;

	pThreadNode = (*iter).second;

	pthread_cancel(*pThreadNode->pThread);
	pthread_attr_destroy(pThreadNode->pAttrThread);
	delete pThreadNode->pAttrThread;
	delete pThreadNode->pThread;
	s_Threads.erase(iter);
}

BOOL IsEventObject(HANDLE h)
{
	if( h == INVALID_HANDLE_VALUE )
		return FALSE;
	return (h & HANDLE_MASK_EVENT) == HANDLE_MASK_EVENT;
}

EventNode* GetEventNode(HANDLE h)
{
	map<HANDLE , EventNode*>::iterator iter;

	iter = s_Evts.find(h);

	if( iter == s_Evts.end() )
		return NULL;

	return (*iter).second;
}

void DestroyEventNode(HANDLE h)
{
	EventNode* pEventNode = NULL;
	map<HANDLE , EventNode*>::iterator iter;

	iter = s_Evts.find(h);
	if( iter == s_Evts.end() )
		return ;

	pEventNode = (*iter).second;

	pthread_cond_destroy(pEventNode->pCond);
	pthread_condattr_destroy(pEventNode->pAttrCond);
	pthread_mutex_destroy(pEventNode->pMutex);
	delete pEventNode->pMutex;
	delete pEventNode->pAttrCond;
	delete pEventNode->pCond;
	delete pEventNode;
	s_Evts.erase(iter);
}

CriticalSectionNode* GetCriticalSectionNode(LPCRITICAL_SECTION cri)
{
	vector<CriticalSectionNode*>::iterator iter;

	for( iter = s_CriSecs.begin() ; iter != s_CriSecs.end() ; iter++ )
	{
		if( (*iter)->section == cri )
		{
			return *iter;
		}
	}

	return NULL;
}

void DestroyCriticalSectionNode(LPCRITICAL_SECTION cri)
{
	CriticalSectionNode* pCriticalSectionNode = NULL;
	vector<CriticalSectionNode*>::iterator iter;

	for( iter = s_CriSecs.begin() ; iter != s_CriSecs.end() ; iter++ )
	{
		if( (*iter)->section == cri )
		{
			pCriticalSectionNode = *iter;

			pthread_mutex_destroy(pCriticalSectionNode->pMutex);
			pthread_mutexattr_destroy(pCriticalSectionNode->pMutexAttr);

			delete pCriticalSectionNode->pMutex;
			delete pCriticalSectionNode->pMutexAttr;
			delete pCriticalSectionNode;

			s_CriSecs.erase(iter);
			break;
		}
	}
}


BOOL IsSemaphoreObject(HANDLE h)
{
	if( h == INVALID_HANDLE_VALUE )
			return FALSE;
	return (h & HANDLE_MASK_SEMAPHORE) == HANDLE_MASK_SEMAPHORE;
}
SemaphoreNode* GetSemaphoreNode(HANDLE h)
{
	map<HANDLE , SemaphoreNode*>::iterator iter;

	iter = s_Semaphores.find(h);

	if( iter == s_Semaphores.end() )
		return NULL;

	return (*iter).second;
}
void DestroySemaphoreNode(HANDLE h)
{
	SemaphoreNode* pNode = NULL;
	map<HANDLE , SemaphoreNode*>::iterator iter;

	iter = s_Semaphores.find(h);
	if( iter == s_Semaphores.end() )
		return ;

	pNode = (*iter).second;

//	printf("DestroySemaphoreNode size:%d, h=%0x create:%d \n", s_Semaphores.size(), h, pNode->bCreate);

	if( pNode->bCreate)
		sem_unlink(pNode->pName);

	sem_close(pNode->pSem);
	delete[] pNode->pName;
	delete pNode;
	s_Semaphores.erase(iter);

}

BOOL IsShareMemoryObject(HANDLE h)
{
	if( h == INVALID_HANDLE_VALUE )
				return FALSE;
	return (h & HANDLE_MASK_SHAREMEMORY) == HANDLE_MASK_SHAREMEMORY;
}

/*
ShareMemoryNode* GetShareMemoryNode(HANDLE h)
{
	map<HANDLE , ShareMemoryNode*>::iterator iter;
	iter = s_ShareMemorys.find(h);
	if( iter == s_ShareMemorys.end() )
		return NULL;
	return (*iter).second;
}
*/

void DestroyShareMemoryNode(HANDLE h)
{
	ShareMemoryNode* pNode = NULL;
	map<HANDLE , ShareMemoryNode*>::iterator iter;

	iter = s_ShareMemorys.find(h);
	if( iter == s_ShareMemorys.end() )
		return ;

	pNode = (*iter).second;

//	printf(" DestroyShareMemoryNode:%0x, %s \n", pNode->handle, pNode->pName);

	if( pNode->bCreate)
		shm_unlink(pNode->pName);

	close(pNode->nFd);

	delete[] pNode->pName;
	delete pNode;
	s_ShareMemorys.erase(iter);
}


BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	return TRUE;
}

BOOL RemoveDirectory(LPCTSTR lpPathName)
{
	return TRUE;
}

BOOL DeleteFile(LPCTSTR lpFileName)
{
	return TRUE;
}

DWORD GetCurrentDirectory(DWORD nBufferLength, LPTSTR lpBuffer)
{
	return 0;
}

BOOL PathIsRelative(LPCSTR pszPath)
{
	return TRUE;
}

BOOL PathCanonicalize(LPSTR pszBuf, LPCSTR pszPath)
{
	return TRUE;
}

void GetLocalIp(char* localIP)
{
	char  			cLocalIpBuf[32] ;
	char 			sInterface[4];
	struct 			ifaddrs *Ifa , *cIfa;
	struct 			sockaddr_in addr;

	memset(cLocalIpBuf , 0 , sizeof(cLocalIpBuf));
	sprintf(sInterface , "wm0");
	getifaddrs( &Ifa ) ;
	for( cIfa = Ifa ; cIfa != NULL ; cIfa = cIfa->ifa_next )
	{
		if( cIfa->ifa_addr->sa_family == AF_INET )
		{
			if( memcmp( cIfa->ifa_name , sInterface , 3 ) != 0 )
			{
				continue ;
			}
			memcpy( &addr , cIfa->ifa_addr , sizeof( struct sockaddr_in) ) ;
			inet_ntop( AF_INET , &addr.sin_addr , cLocalIpBuf , sizeof( cLocalIpBuf ) ) ;
		}
	}
	if( _stricmp(cLocalIpBuf , "") == 0 )
	{
		sprintf(sInterface , "en0");
		getifaddrs( &Ifa ) ;
		for( cIfa = Ifa ; cIfa != NULL ; cIfa = cIfa->ifa_next )
		{
			if( cIfa->ifa_addr->sa_family == AF_INET )
			{
				if( memcmp( cIfa->ifa_name , sInterface , 3 ) != 0 )
				{
					continue ;
				}
				memcpy( &addr , cIfa->ifa_addr , sizeof( struct sockaddr_in) ) ;
				inet_ntop( AF_INET , &addr.sin_addr , cLocalIpBuf , sizeof( cLocalIpBuf ) ) ;
			}
		}
	}

	if( _stricmp(cLocalIpBuf , "") == 0 )
	{
		sprintf(sInterface , "rt0");
		getifaddrs( &Ifa ) ;
		for( cIfa = Ifa ; cIfa != NULL ; cIfa = cIfa->ifa_next )
		{
			if( cIfa->ifa_addr->sa_family == AF_INET )
			{
				if( memcmp( cIfa->ifa_name , sInterface , 3 ) != 0 )
				{
					continue ;
				}
				memcpy( &addr , cIfa->ifa_addr , sizeof( struct sockaddr_in) ) ;
				inet_ntop( AF_INET , &addr.sin_addr , cLocalIpBuf , sizeof( cLocalIpBuf ) ) ;
			}
		}
	}
	strcpy( localIP , cLocalIpBuf );
}

HANDLE CreateSemaphore(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
		LONG lInitialCount,LONG lMaximumCount,LPCTSTR lpName)
{
	if(lpName == NULL )
		return 0;

	SemaphoreNode* pNode = NULL;
	sem_t *sem = SEM_FAILED;
	BOOL bExist = FALSE;

	sem = sem_open(lpName ,O_CREAT| O_EXCL , S_IRWXU|S_IRWXG, lInitialCount);
	if( sem == SEM_FAILED )
	{
		if(  errno != EEXIST )
		{
			//printf(" sem_open failed errno:%d name:%d", errno, lpName);
			SetLastError(ERROR_INVALID_HANDLE);
			return 0;
		}

		//printf(" sem_open already exists:%s \n",  lpName);
		SetLastError(ERROR_ALREADY_EXISTS);
		sem = sem_open(lpName,0);
		bExist = TRUE;
		if( sem == NULL)
		{
			SetLastError(ERROR_INVALID_HANDLE);
			return 0;
		}
	}
	else
	{
		SetLastError(0);
	}

	char *name = new char[strlen(lpName)+1];
	memset(name, 0x0, strlen(lpName)+1);
	strcpy(name, lpName);

	pNode = new SemaphoreNode;
	pNode->bCreate = bExist;
	pNode->pSem = sem;
	pNode->pName = name;

	for(DWORD i = 1 ; i < HANDLE_MAX_COUNT ; i++ )
	{
		if( s_Semaphores.find((HANDLE)(i | HANDLE_MASK_SEMAPHORE)) == s_Semaphores.end() )
		{
			pNode->handle = (HANDLE)(i | HANDLE_MASK_SEMAPHORE);
			s_Semaphores[pNode->handle] = pNode;
			//printf("size:%d \n",s_Semaphores.size());
			return pNode->handle;
		}
	}

	sem_close(pNode->pSem);
	if( pNode->bCreate)
		sem_unlink(pNode->pName);
	delete[] pNode->pName;
	delete pNode;
	SetLastError(ERROR_INVALID_HANDLE);
	return 0;
}

HANDLE OpenSemaphore(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName)
{
	if(lpName == NULL )
			return 0;

	SemaphoreNode* pNode = NULL;
	sem_t *sem = SEM_FAILED;

	sem = sem_open(lpName ,0);
	if( sem == SEM_FAILED )
	{
		return 0;
	}

	char *name = new char[strlen(lpName)+1];
	memset(name, 0x0, strlen(lpName)+1);
	strcpy(name, lpName);

	pNode = new SemaphoreNode;
	pNode->bCreate = false;
	pNode->pSem = sem;
	pNode->pName = name;

	for(DWORD i = 1 ; i < HANDLE_MAX_COUNT ; i++ )
	{
		if( s_Semaphores.find((HANDLE)(i | HANDLE_MASK_SEMAPHORE)) == s_Semaphores.end() )
		{
			pNode->handle = (HANDLE)(i | HANDLE_MASK_SEMAPHORE);
			s_Semaphores[pNode->handle] = pNode;
			return pNode->handle;
		}
	}
	sem_close(pNode->pSem);
	delete[] pNode->pName;
	delete pNode;
	return 0;
}
extern BOOL ReleaseSemaphore(HANDLE hSemaphore, LONG lReleaseCount, LPLONG lpPreviousCount)
{
	SemaphoreNode* pNode = GetSemaphoreNode(hSemaphore);
	if( pNode == NULL )
		return FALSE;

//	printf(" ReleaseSemaphore:%0x \n", pNode->handle);
	int value = 0;

	if( lpPreviousCount )
	{
		sem_getvalue(pNode->pSem, &value);
		*lpPreviousCount = (LONG)value;
	}

	LONG cou = lReleaseCount;
	BOOL ret = FALSE;
	while(cou > 0)
	{
		ret = sem_post(pNode->pSem)==0 ? TRUE:FALSE;
		cou--;
	}
	return ret;
}

void SetLastError(DWORD dwErrCode)
{
	s_dwErrCode = dwErrCode;
}
DWORD GetLastError(void)
{
	return s_dwErrCode;
}

extern DWORD GetCurrentProcessId(void)
{
	return getpid();
}

HANDLE CreateFileMapping(HANDLE hFile,LPSECURITY_ATTRIBUTES lpAttributes,
		DWORD flProtect,DWORD dwMaximumSizeHigh,DWORD dwMaximumSizeLow,LPCTSTR lpName)
{
	if(lpName == NULL )
		return 0;

	ShareMemoryNode* pNode = NULL;
	int fd = -1;

	fd = shm_open(lpName, O_CREAT|O_EXCL|O_RDWR,S_IRWXU|S_IRWXG);
	if( fd == -1 )
	{
		if( errno != EEXIST)
		{
			SetLastError(ERROR_INVALID_HANDLE);
			return 0;
		}
	//	printf(" shm_open: already exist->%s \n", lpName);
		SetLastError(ERROR_ALREADY_EXISTS);
		fd = shm_open(lpName, O_RDWR,S_IRWXU|S_IRWXG);
		if( fd == -1)
		{
			SetLastError(ERROR_INVALID_HANDLE);
			return 0;
		}
	}
	else
	{
		//printf(" shm_open ok %s \n", lpName);
		SetLastError(0);
		ftruncate(fd, dwMaximumSizeLow);
	}

	char *name = new char[strlen(lpName)+1];
	memset(name, 0x0, strlen(lpName)+1);
	strcpy(name, lpName);

	pNode = new ShareMemoryNode;
	pNode->bCreate = TRUE;
	pNode->pShm = NULL;
	pNode->pName = name;
	pNode->nLen = dwMaximumSizeLow;
	pNode->nFd = fd;

	for(DWORD i = 1 ; i < HANDLE_MAX_COUNT ; i++ )
	{
		if( s_ShareMemorys.find((HANDLE)(i | HANDLE_MASK_SHAREMEMORY)) == s_ShareMemorys.end() )
		{
			pNode->handle = (HANDLE)(i | HANDLE_MASK_SHAREMEMORY);
			s_ShareMemorys[pNode->handle] = pNode;

			return pNode->handle;
		}
	}

	close(pNode->nFd);
	delete[] pNode->pName;
	delete pNode;
	SetLastError(ERROR_INVALID_HANDLE);
	return 0;
}

HANDLE OpenFileMapping(DWORD dwDesiredAccess,BOOL bInheritHandle,LPCTSTR lpName)
{
	if(lpName == NULL )
		return 0;

//	printf(" openFileMapping:%s \n",lpName);

	ShareMemoryNode* pNode = NULL;
	int fd = -1;
	fd = shm_open(lpName, O_RDWR,S_IRWXU|S_IRWXG);
	if( fd == -1)
		return 0;

	char *name = new char[strlen(lpName)+1];
	memset(name, 0x0, strlen(lpName)+1);
	strcpy(name, lpName);

	pNode = new ShareMemoryNode;
	pNode->bCreate = FALSE;
	pNode->pShm = NULL;
	pNode->pName = name;
	pNode->nFd = fd;
	pNode->nLen = 0;
	pNode->nAccess = dwDesiredAccess;

	for(DWORD i = 1 ; i < HANDLE_MAX_COUNT ; i++ )
	{
		if( s_ShareMemorys.find((HANDLE)(i | HANDLE_MASK_SHAREMEMORY)) == s_ShareMemorys.end() )
		{
			pNode->handle = (HANDLE)(i | HANDLE_MASK_SHAREMEMORY);
			s_ShareMemorys[pNode->handle] = pNode;
			return pNode->handle;
		}
	}

	close(pNode->nFd);
	delete[] pNode->pName;
	delete pNode;
	return 0;
}

LPVOID MapViewOfFile(HANDLE fd,DWORD dwDesiredAccess,
		DWORD dwFileOffsetHigh,DWORD dwFileOffsetLow,SIZE_T dwNumberOfBytesToMap)
{
	ShareMemoryNode* pNode = NULL;
//	printf("MapViewOfFile fd:%0x, size:%d \n",fd, s_ShareMemorys.size());
	for(DWORD i = 1 ; i < HANDLE_MAX_COUNT; i++)
	{
		if( s_ShareMemorys.find((HANDLE)(i|HANDLE_MASK_SHAREMEMORY))!= s_ShareMemorys.end()
			&& (s_ShareMemorys[i|HANDLE_MASK_SHAREMEMORY])->handle == fd )
		{
			pNode = s_ShareMemorys[i|HANDLE_MASK_SHAREMEMORY];
	//		printf(" find out i=%d \n",i);
			break;
		}
	}

	if( pNode == NULL)
		return NULL;

	if( pNode->pShm != NULL )
		return NULL;

//	printf(" MapViewOfFile:%s ,%0x \n", pNode->pName, pNode->handle);

	if( pNode->bCreate)
	{
		if( pNode->nLen != dwNumberOfBytesToMap)
			return NULL;
	}
	void *ptr = mmap(NULL,dwNumberOfBytesToMap,PROT_READ|PROT_WRITE, MAP_SHARED,pNode->nFd,0);
	if( ptr == MAP_FAILED)
	{
//		printf(" map _failed \n");
		return NULL;
	}
	pNode->pShm = ptr;
	return ptr;
}

BOOL UnmapViewOfFile( LPCVOID lpBaseAddress)
{
	if( lpBaseAddress != NULL)
		return FALSE;

	ShareMemoryNode* pNode = NULL;
	for(DWORD i = 0 ; i < HANDLE_MAX_COUNT; i++)
	{
		if( s_ShareMemorys.find((HANDLE)(i|HANDLE_MASK_SHAREMEMORY))!= s_ShareMemorys.end()
			&& (s_ShareMemorys[i|HANDLE_MASK_SHAREMEMORY])->pShm == lpBaseAddress )
		{
			pNode = s_ShareMemorys[i|HANDLE_MASK_SHAREMEMORY];
			break;
		}
	}
	if( pNode == NULL)
		return NULL;
	//printf(" UnmapViewOfFile:%s ,%0x \n", pNode->pName, pNode->handle);
	return (munmap(pNode->pShm, pNode->nLen)==0 ? TRUE:FALSE);
}
