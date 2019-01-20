/*****************************************************************
 * 文件名: KLCommDef.h
 * 功能: 模块间通信通用信息定义
 *****************************************************************/
#pragma once

#include "Platform.h"

#define KL_CLIENT_ID UINT	

/*BEGIN 通信接口回调函数定义 */
typedef void (*SvrConnectCallback)(BOOL bConnected , KL_CLIENT_ID nClientId , void* pClientInfo , void* pParam);
typedef void (*SvrRcvedDataCallback)(KL_CLIENT_ID nClientId , unsigned char* szData , unsigned long nDataLen , void* pParam);
typedef void (*CltConnectCallback)(BOOL bConnected , void* pConnectInfo , void* pParam);
typedef void (*CltReadyReconnectCallback)(void* pReconnectInfo , void* pParam);
typedef void (*CltRcvedDataCallback)(unsigned char* szData , unsigned long nDataLen , void* pParam);
/*END 通信接口回调函数定义*/

/****************************************************************
 * 枚举名 : CommType
 * 说明 : 通信方式
 ****************************************************************/
typedef enum euCommType
{
	CT_TCP = 0 ,			/* TCP方式 */
	CT_SHAREMEMORY = 2  	/* 共享内存方式 */
} CommType;

/****************************************************************
 * 枚举名: ShmControlMsgType
 * 说明 : 共享内存控制消息类型
 ****************************************************************/
typedef enum{
	SHM_MSG_TYPE_INVAILD = 0x0,				/* 无效消息 */

	/* 服务器端消息 */
	SHM_MSG_TYPE_SVR_REBUILD = 0x10,		/* 内存区将被重建,客户端重新打开 */
	SHM_MSG_TYPE_SVR_CLOSE = 0x11,			/* 内存区将关闭 ,客户端断开连接 */
	SHM_MSG_TYPE_SVR_BUILD_ACC = 0x12,  	/* 建立请求接受*/
	SHM_MSG_TYPE_SVR_BUILD_REJ = 0x13,  	/* 建立请求拒绝*/

	/* 客户端消息 */
	SHM_MSG_TYPE_CLT_BEGIN	= 0X20,
	SHM_MSG_TYPE_CLT_BUILD_REQ = 0x21,		/* 客户端建立请求消息 */
	SHM_MSG_TYPE_CLT_DISCONN = 0x22,		/* 客户端断开连接消息 */
	SHM_MSG_TYPE_CLT_ERROR_REP = 0x23,		/* 客户端错误报告 */
}ShmCtlMsgType;

typedef struct{
	ShmCtlMsgType	Type;					/* 控制消息类型 */
	DWORD			MsgDest;				/* 消息目的地：以进程号填充 ，为0时为广播消息 */
	DWORD   		MsgSrc;					/* 消息发送者 */
//	unsigned char 	MsgBuf[8];				/* 消息描述 */
}ShmControlMsg_t;
/****************************************************************
 * 结构体名 : KLShareMem_t
 * 说明 : 共享内存映射结构体
 ****************************************************************/
#define KL_SHARE_MEM_MAX_DATABUF_LEN	(1024)
#define KL_SHARE_MEM_MMAP_MARK_CHECK	(0xfeca)
typedef struct tagKLShareMem{
	unsigned int		ShareMemMark;		/* 标识共享内存*/

	unsigned int		DataBufLen;			/* 数据区（DataBuf）总长度 */
	ShmControlMsg_t 	ControlMsg;			/* 控制消息区 */

	unsigned int		nSvrDataLen;		/* 服务器数据区可读长度 */
	unsigned int 		nSvrReadIdx;		/* 服务器数据区读标*/
	unsigned int		nCltDataLen;		/* 客户端数据区可读长度 */
	unsigned int 		nCltReadIdx;		/* 客户端数据区读标*/

#ifdef QNX_PLATFORM
	unsigned char 		DataBuf[0];			/* 共享内存数据区*/
#endif
#ifdef LINUX_PLATFORM
	unsigned char 		DataBuf[0];			/* 共享内存数据区*/
#endif
#ifdef WIN_PLATFORM
	unsigned char		DataBuf[1];
#endif
/*
 * 共享内存数据区  【0,DataBufLen/2-1】为 服务器数据区，服务器写，客户端读；
 * 共享内存数据区  【DataBufLen/2,DataBufLen-1】为 客户端数据区，客户端写，服务器读；
 */
}KLShareMem_t, *PKLShareMem_t;

/****************************************************************
 * 结构体名 : KLSvrParam
 * 说明 : 服务端参数
 ****************************************************************/
typedef struct tagKLSvrParam
{
	CommType ct;																/* 通信方式 */
	int nMaxClient;															/* 允许连接的最大客户端数 */
	BOOL bUseRcvedDataCallback;								/* 是否使用回调的方式处理接收到的数据 */
	void* pParam;																/* 回调函数的额外数据信息 */
	SvrConnectCallback			connect_callback;		/*	 有客户端连接或断开的回调函数 */
	SvrRcvedDataCallback		rcveddata_callback;	/* 接收到了客户端的数据时的回调函数 */
	union
	{
		struct
		{
			char szLocalIp[32];				/* 如果不为空，则使用此IP地址，如果为空，则搜寻本地IP地址进行监听 */
			int nPort;								/* 服务端监听端口号 */
		} TcpExtraParam;						/* TCP方式通信时的额外参数 */
		struct
		{
			bool				bUseExist;
			char 				sShmName[128];		/* 共享内存关联的路径名 */
			unsigned int		nDataBufLen;		/* 用户数据区大小 */
		} MemoryExtraParam;												/* 共享内存方式通信时的额外参数 */
	} ExtraParam;
} KLSvrParam , *PKLSvrParam;

#define KL_SHM_FIX_MUTEX_NAME 		"_mutex"
#define KL_SHM_FIX_SVR_READ_NAME	"_svrRead"
#define KL_SHM_FIX_SVR_WRITE_NAME	"_svrWrite"
#define KL_SHM_FIX_CLT_READ_NAME	"_cltRead"
#define KL_SHM_FIX_CLT_WRITE_NAME	"_cltWrite"
/****************************************************************
 * 结构体名 : KLShmConnectInfo
 * 说明 : 共享内存连接信息
 ****************************************************************/
typedef struct tagKLShmConnectInfo
{
	DWORD			nInstID;
	time_t			nConnectStartTime;										/* 连接时的时间 */
} KLShmConnectInfo , *PKLShmConnectInfo;


/****************************************************************
 * 结构体名 : KLClientInfo
 * 说明 : 连接到服务端的客户端信息
 ****************************************************************/
typedef struct tagKLClientInfo
{
	KL_CLIENT_ID nId;														/* 客户端ID号 */
	SOCKET  sock;																/* 客户端的socket号*/
	char			szIp[32];														/* 客户端IP */
	int			nPort;																/* 客户端端口号 */
	time_t		nConnectStartTime;										/* 连接时的时间 */
} KLClientInfo , *PKLClientInfo;


/****************************************************************
 * 结构体名 : KLCltParam
 * 说明 : 客户端参数
 ****************************************************************/
typedef struct tagKLCltParam
{
	CommType ct;																/* 通信方式 */
	BOOL	bAutoReconnect;												/* 是否自动重连 */
	int		nAutoReconnectWaitTime;							/* 重连等待时间 */
	int		nReconnectNum;												/* 重连次数 */
	int		nConnTimeout;													/* 连接超时时间 */
	void*   pParam;															/* 回调函数的额外数据信息 */
	BOOL bUseRcvedDataCallback;								/* 是否使用回调的方式处理接收到的数据 */
	CltConnectCallback connect_callback;					/*	 连接到服务端或从服务端断开时的回调函数 */
	CltRcvedDataCallback rcveddata_callback;			/* 接收到了服务端的数据时的回调函数 */
	CltReadyReconnectCallback readyreconnect_callback;				/*	 准备重连服务器时的回调函数 */

	union
	{
		struct
		{
			char	szSvrIp[32];												/* 服务端IP地址 */
			int		nPort;															/* 服务端端口号 */
		} TcpExtraParam;														/* TCP方式通信时的额外参数 */
		struct 
		{
			char    szSvrIp[32];                                                 /* 服务端IP地址 */
			int     nPort;                                                       /* 服务端端口号 */
		}UdpExtraParam;                                                          /* UDP方式通信时的额外参数 */
		struct
		{
			char 				sShmName[128];							/* 共享内存关联的路径名 */
			unsigned int		nDataBufLen;							/* 用户数据区大小 */
		} MemoryExtraParam;												/* 共享内存方式通信时的额外参数 */
	} ExtraParam;
} KLCltParam , *PKLCltParam;

/****************************************************************
 * 结构体名 : KLConnectInfo
 * 说明 : 客户端连接信息
 ****************************************************************/
typedef struct tagKLConnectInfo
{
	SOCKET  sock;																/* 客户端socket号 */
	char		szSvrIp[32];													/* 服务端IP地址 */
	int			nPort;																/* 服务端端口号 */
	time_t		nConnectStartTime;										/* 连接到服务端时的时间 */
} KLConnectInfo , *PKLConnectInfo;

/****************************************************************
 * 结构体名 : KLReadyReconnectInfo
 * 说明 : 客户端准备重连信息
 ****************************************************************/
typedef struct tagKLReadyReconnectInfo
{
	char		szSvrIp[32];													/* 服务端IP地址 */
	int			nPort;																/* 服务端端口号 */
	time_t		nReconnectStartTime;									/* 重连服务端时的时间 */
	int			nReconnectNum;											/* 重连次数 */
} KLReadyReconnectInfo , *PKLReadyReconnectInfo;



/****************************************************************************
*****************************************************************************/

/**********************************************
说    明 ： UDP通信的结构定义，创建服务端或者客户端的参数结构，
时    间 ： 2015-3-6  
**********************************************/
#pragma  once

#define MAX_RCV_DATA_SIZE_UDP			10240*3			/* 每次接收数据最大字节数 */
#define MAX_SEND_DATA_SIZE_UDP			10240*3			/* 每次发送数据最大字节数 */

#define SLEEP_MAX_TICK_UDP	50

/************************************
 *   BOOL bCloseRecv : TRUE值 表示 接收通路已经关闭
                       FALSE值 表示 通路没有关闭

***********************************/
typedef void (*UdpRcvedDataCallback)(const unsigned char* szData , unsigned long nDataLen,const char *FromIp,void* pParam, BOOL bCloseRecv);

// 服务端参数结构，
typedef struct tagKLUdpSvrParam
{
	bool bCreateSendThread;
	bool bReSendCount;
	bool bUseRcvedDataCallback;
	void* pParam;
	UdpRcvedDataCallback		rcveddata_callback;
	char szLocalIp[32];
	int nPort;
}KLUdpSvrParam, *PKLUdpSvrParam;

/****************************************************************
 * 结构体名 : KLCommUdpDataBuffer
 * 说明 : UDP数据通信发送数据 结构定义
 ****************************************************************/
typedef struct TagSimuUdpWaitingSendData
{
	char szIpAddr[32];
	int  nPort;
	unsigned char *szWaitSendDataBuffer;		/* 待发送数据缓冲区 */
	unsigned long nWaitSendDataBufferLen;		/* 待发送数据缓冲区大小 */
}TSimuUdpWaittingSendData , *PTSimuUdpWaittingSendData;

/****************************************************************
 * 结构体名 : KLCommUdpDataBuffer
 * 说明 : UDP数据通信 接收数据 结构定义
 ****************************************************************/
typedef struct tagKLCommUdpDataBuffer
{
	unsigned char *szRcvedDataBuffer;				/* 未处理的接收到的数据缓冲区 */
	unsigned long nRcvedDataBufferLen;				/* 未处理的接收到的数据缓冲区大小 */
} KLCommUdpDataBuffer , *PKLCommUdpDataBuffer;


/***************************************************
 * 时  间 : 2015-3-8
 * 说  明 ：下面 是UDP通信客户端结构信息
 * UDP通信的结构定义，创建服务端或者客户端的结构信息
****************************************************/

/************************************
 *   BOOL bCloseRecv : TRUE值 表示 接收通路已经关闭
                       FALSE值 表示 通路没有关闭

***********************************/
typedef void (*ClientUdpRcvedDataCallback)(const unsigned char* szData , unsigned long nDataLen,void* pParam, BOOL bCloseRecv);

// 客户端的结构信息
typedef struct tagKLUdpCltParam
{
	bool bReSendCount;
	bool bUseRcvedDataCallback;
	void* pParam;
	ClientUdpRcvedDataCallback		rcveddata_callback;
	char szLocalIp[32];
	int nPort;
}KLUdpCltParam, *PKLUdpCltParam;


/****************************************************************
 * 结构体名 : TSimuUdpClientWaittingSendData
 * 说明 : UDP数据通信发送数据 结构定义
 ****************************************************************/
typedef struct TagSimuUdpClientWaitingSendData
{
	unsigned char *szWaitSendDataBuffer;		/* 待发送数据缓冲区 */
	unsigned long nWaitSendDataBufferLen;		/* 待发送数据缓冲区大小 */
}TSimuUdpClientWaittingSendData , *PTSimuUdpClientWaittingSendData;

/****************************************************************
 * 结构体名 : KLCommUdpClientDataBuffer
 * 说明 : UDP数据通信 接收数据 结构定义
 ****************************************************************/
typedef struct tagKLCommUdpClientDataBuffer
{
	unsigned char *szRcvedDataBuffer;				/* 未处理的接收到的数据缓冲区 */
	unsigned long nRcvedDataBufferLen;				/* 未处理的接收到的数据缓冲区大小 */
} KLCommUdpClientDataBuffer , *PKLCommUdpClientDataBuffer;

