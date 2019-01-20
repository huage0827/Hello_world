/*****************************************************************
 * �ļ���: LinuxPlatform.h
 * ����: Linuxƽ̨ʵ��WINƽ̨��API����ض���
 *****************************************************************/
#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <dirent.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <map>
#include <vector>
#include <ctype.h>
#include <errno.h>
#include <ifaddrs.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/time.h>

using namespace std;

#define WSADESCRIPTION_LEN	256
#define WSASYS_STATUS_LEN	128
#define	MAX_PATH			256

#define BOOL		bool
#define UINT		unsigned int
#define BYTE		unsigned char
#define	LONG		long
#define LPLONG		long*
#define ULOGN		unsigned long
#define ULONG_PTR	unsigned long*
#define WORD		unsigned short
#define DWORD		unsigned long
#define LPVOID		void*
#define LPCVOID		const void*
#define SOCKET		int
#define HANDLE		int
#define SIZE_T		unsigned long
#define LPDWORD		unsigned long*
#define LPCSTR		const char*
#define LPCTSTR		const char*
#define LPTSTR		char*
#define TCHAR		char
#define LPSTR		char*

#define CA2CT(str)	str
#define CT2CA(str)	str
#define _T(str)		str
#define TEXT(str)	str

#define	WINAPI
#define TRUE			true
#define FALSE			false
#define STATUS_WAIT_0  	((DWORD)0x00000000L)
#define INFINITE		0xEFFFFFFF
#define WAIT_OBJECT_0	((STATUS_WAIT_0 ) + 0)
#define WAIT_TIMEOUT    258L
#define WSAECONNABORTED	10053
#define WSAECONNRESET	10054
#define WSAENOTCONN     10057
#define WSAESHUTDOWN    10058
#define INVALID_HANDLE_VALUE -1
#define INVALID_SOCKET (SOCKET)(~0)

#define IOCPARM_MASK    0x7f            /* parameters must be < 128 bytes */
//#define IOC_VOID        0x20000000      /* no parameters */
//#define IOC_OUT         0x40000000      /* copy out parameters */
//#define IOC_IN          0x80000000      /* copy in parameters */
//#define IOC_INOUT       (IOC_IN|IOC_OUT)
                                        /* 0x20000000 distinguishes new &
                                           old ioctl's */
//#define _IO(x,y)        (IOC_VOID|((x)<<8)|(y))

//#define _IOR(x,y,t)     (IOC_OUT|(((long)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))

//#define _IOW(x,y,t)     (IOC_IN|(((long)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))

//#define FIONREAD    _IOR('f', 127, u_long) /* get # bytes to read */
//#define FIONBIO     _IOW('f', 126, u_long) /* set/clear non-blocking i/o */
//#define FIOASYNC    _IOW('f', 125, u_long) /* set/clear async i/o */

//#define IPPROTO_IP              0               /* dummy for IP */
//#define IPPROTO_ICMP            1               /* control message protocol */
//#define IPPROTO_IGMP            2               /* group management protocol */
//#define IPPROTO_GGP             3               /* gateway^2 (deprecated) */
//#define IPPROTO_TCP             6               /* tcp */
//#define IPPROTO_PUP             12              /* pup */
//#define IPPROTO_UDP             17              /* user datagram protocol */
//#define IPPROTO_IDP             22              /* xns idp */
//#define IPPROTO_ND              77              /* UNOFFICIAL net disk proto */
//#define IPPROTO_RAW             255             /* raw IP packet */
//#define IPPROTO_MAX             256

#define CREATE_SUSPENDED        0x00000004

#define MAKEWORD(low,high) \
        ((WORD)(((BYTE)(low)) | ((WORD)((BYTE)(high))) << 8))

#define ERROR_INVALID_HANDLE	6L
#define ERROR_ALREADY_EXISTS 	183L
#define FILE_MAP_ALL_ACCESS 	PROT_READ|PROT_WRITE
#define PAGE_READWRITE			O_RDWR
#define SEC_COMMIT				O_EXCL|O_CREAT
#define SEMAPHORE_ALL_ACCESS	O_EXCL|O_CREAT


typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY *Flink;
   struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY, *PRLIST_ENTRY;

typedef struct _RTL_CRITICAL_SECTION_DEBUG {
    WORD   Type;
    WORD   CreatorBackTraceIndex;
    struct _RTL_CRITICAL_SECTION *CriticalSection;
    LIST_ENTRY ProcessLocksList;
    DWORD EntryCount;
    DWORD ContentionCount;
    DWORD Flags;
    WORD   CreatorBackTraceIndexHigh;
    WORD   SpareWORD;
} RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG, RTL_RESOURCE_DEBUG, *PRTL_RESOURCE_DEBUG;

typedef struct _RTL_CRITICAL_SECTION {
    PRTL_CRITICAL_SECTION_DEBUG DebugInfo;
    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;

typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;
typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;

typedef struct tagWSAData {
	WORD			wVersion;
	WORD			wHighVersion;
	char			szDescription[WSADESCRIPTION_LEN+1];
	char			szSystemStatus[WSASYS_STATUS_LEN+1];
	unsigned short	iMaxSockets;
	unsigned short	iMaxUdpDg;
	char			*lpVendorInfo;
} WSADATA;
typedef WSADATA *LPWSADATA;

typedef struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef void* (*PTHREAD_START_ROUTINE)(LPVOID lpThreadParameter);
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

extern int WSAStartup(WORD wVersionRequested,LPWSADATA lpWSAData);
extern int WSACleanup();
extern int WSAGetLastError(void);
extern HANDLE CreateThread(
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	SIZE_T dwStackSize,
	LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID lpParameter,
	DWORD dwCreationFlags,
	LPDWORD lpThreadId
	);
extern HANDLE CreateEvent(
    LPSECURITY_ATTRIBUTES lpEventAttributes,
    BOOL bManualReset,
    BOOL bInitialState,
    LPCSTR lpName
    );
extern BOOL SetEvent(HANDLE hEvent);
extern BOOL ResetEvent(HANDLE hEvent);
extern BOOL CloseHandle(HANDLE hObject);
extern DWORD WaitForSingleObject(HANDLE hHandle,DWORD dwMilliseconds);
extern void InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
extern void DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
extern void EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
extern void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
extern int closesocket(SOCKET s);
extern int ioctlsocket(SOCKET s, long cmd, u_long argp);
/*extern int sprintf_s (
        char *string,
        unsigned long sizeInBytes,
        const char *format,
        ...
        );*/
extern void Sleep(DWORD dwMilliseconds);

extern BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
extern BOOL RemoveDirectory(LPCTSTR lpPathName);
extern BOOL DeleteFile(LPCTSTR lpFileName);
extern DWORD GetCurrentDirectory(DWORD nBufferLength, LPTSTR lpBuffer);
extern BOOL PathIsRelative(LPCSTR pszPath);
extern BOOL PathCanonicalize(LPSTR pszBuf, LPCSTR pszPath);

extern void GetLocalIp(char* localIP);


extern void SetLastError(DWORD dwErrCode);
extern DWORD GetLastError(void);
extern DWORD GetCurrentProcessId(void);


extern HANDLE CreateSemaphore(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,LONG lInitialCount,LONG lMaximumCount,LPCTSTR lpName);
extern HANDLE OpenSemaphore(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName);
extern BOOL ReleaseSemaphore(HANDLE hSemaphore, LONG lReleaseCount, LPLONG lpPreviousCount);

extern HANDLE CreateFileMapping(HANDLE hFile,LPSECURITY_ATTRIBUTES lpAttributes,
		DWORD flProtect,DWORD dwMaximumSizeHigh,DWORD dwMaximumSizeLow,LPCTSTR lpName);
extern HANDLE OpenFileMapping(DWORD dwDesiredAccess,BOOL bInheritHandle,LPCTSTR lpName);
extern LPVOID MapViewOfFile(HANDLE hFileMappingObject,DWORD dwDesiredAccess,
		DWORD dwFileOffsetHigh,DWORD dwFileOffsetLow,SIZE_T dwNumberOfBytesToMap);
extern BOOL UnmapViewOfFile(LPCVOID lpBaseAddress);




#define strcat_s(src , srcsize , dest)	strcat(src , dest)
#define strcpy_s(src , srcsize , dest)	strcpy(src , dest)
#define strncpy_s(src , srcsize , findpos , findlen)	strncpy(src , findpos , findlen)
#define strtok_s(_Str, _Delim, _Context) strtok(_Str, _Delim)

