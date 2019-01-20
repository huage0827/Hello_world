/*****************************************************************
 * 文件名: KLBaseComm.h
 * 功能: 模块间通信功能接口定义
 *
 *****************************************************************/
#pragma once

#include "KLCommDef.h"

#define MIN_CLIENT_ID				1				/* 客户端最小ID号 */
#define MAX_CLIENT_ID				65535			/* 客户端最大ID号 */
#define MAX_RCV_DATA_SIZE			10240*3			/* 每次接收数据最大字节数 */
#define MAX_SEND_DATA_SIZE			10240*3			/* 每次发送数据最大字节数 */

#define SLEEP_MAX_TICK	50
#define DATA_DISPLAYSLEEP_TICK      50
#define SHM_MAX_SYNC_SEM 1024

/****************************************************************
 * 结构体名 : KLCommDataBuffer
 * 说明 : 数据通信缓冲区
 ****************************************************************/
typedef struct tagKLCommDataBuffer
{
	unsigned char *szWaitSendDataBuffer;			/* 待发送数据缓冲区 */
	unsigned long nWaitSendDataBufferLen;		/* 待发送数据缓冲区大小 */
	unsigned char *szRcvedDataBuffer;				/* 未处理的接收到的数据缓冲区 */
	unsigned long nRcvedDataBufferLen;				/* 未处理的接收到的数据缓冲区大小 */
} KLCommDataBuffer , *PKLCommDataBuffer;

/****************************************************************
 * 类名 : CKLBaseCommClient
 * 父类 : 无
 * 说明 : 客户端功能接口
 ****************************************************************/
class CKLBaseCommClient
{
public :
	virtual BOOL Initialize(void* pCltParam) = 0;													/* 用指定的参数初始化客户端 */
	virtual void   Uninitialize() = 0;																				/* 结束客户端并释放相关资源 */
	virtual BOOL IsConnected() = 0;																			/* 客户端是否连接到了服务端 */
	virtual BOOL SendData(unsigned char* szData , unsigned long nDataLen , BOOL bDirectSend = FALSE) = 0;			/* 发送指定的数据到服务端 */
	virtual UINT  RecvData(unsigned char* szData , unsigned long nMaxDataLen) = 0;	/* 从服务端接收最多不超过指定长度的数据 */
};

/****************************************************************
 * 类名 : CKLBaseCommServer
 * 父类 : 无
 * 说明 : 服务端功能接口
 ****************************************************************/
class CKLBaseCommServer
{
public:
	virtual BOOL Initialize(void* pSvrParam) = 0;																									/* 用指定的参数初始化服务端 */
	virtual void   Uninitialize() = 0;																																/* 结束服务端并释放相关资源 */
	virtual BOOL SendData(KL_CLIENT_ID nClientID , unsigned char* szData , unsigned long nDataLen) = 0;				/* 发送指定的数据到指定的客户端 */
	virtual UINT  RecvData(KL_CLIENT_ID nClientID , unsigned char* szData , unsigned long nMaxDataLen) = 0;		/* 从指定的客户端接收最多不超过指定长度的数据*/
	virtual BOOL GetClientInfo(KL_CLIENT_ID nClientID , void*& pClientInfo) = 0;											/* 获取指定客户端的信息 */
};
