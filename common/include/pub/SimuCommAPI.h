/*****************************************************************
 * 文件名: SimuCommAPI.h
 * 功能: 模块间通信API定义

 *****************************************************************/
#pragma once

#include "SimuCommunicationDef.h"
#include "KLCommDef.h"
#include "KLBaseComm.h"

/****************************************************************
 * 结构体名 : SimuCommParams
 * 说明 : 通信API的参数定义
 ****************************************************************/
typedef struct tagSimuCommParams
{
	char szAppName[256];									/* 调用此API的上层应用程序名 */
} SimuCommParams , *PSimuCommParams;

extern SimuCommParams g_SimuCommParams;

#ifdef __CPLUSPLUS
extern "C"
{
#endif
	int EXPORT_DLL KLSC_Initialize(SimuCommParams* pSimuCommParams);																									/* 初始化库 */
	int EXPORT_DLL KLSC_Uninitialize();																																										/* 卸载库并释放相关资源 */
	
	int EXPORT_DLL KLSC_CreateClient(KLCltParam* pParam , CKLBaseCommClient*& pClient);																																	/* 用指定参数创建一个客户端 */
	int EXPORT_DLL KLSC_CreateServer(KLSvrParam* pParam , CKLBaseCommServer*& pServer);		/* 用指定参数创建一个服务端 */
	
	int EXPORT_DLL KLSC_CreateClient(KLCltParam* pParam);																																	/* 用指定参数创建一个客户端 */
	int EXPORT_DLL KLSC_CreateServer(KLSvrParam* pParam);

	int EXPORT_DLL KLSC_DeleteClient(unsigned long nClientHandle);																																		/* 删除指定的客户端 */
	int EXPORT_DLL KLSC_DeleteServer(unsigned long nServerHandle);				 /* 删除指定的服务端 */

	BOOL EXPORT_DLL KLSC_IsConnected(unsigned long nClientHandle);	            /* 判断指定的客户端是否已连接到服务端 */
	
	int EXPORT_DLL KLSC_ServerSend(unsigned long nServerHandle , unsigned long IN nDataLen , unsigned char* IN pData , KL_CLIENT_ID nClientID);				/* 通过指定的服务端向指定的客户端发送数据 */
	int EXPORT_DLL KLSC_ServerRecv(unsigned long nServerHandle , unsigned long* OUT pDataLen , unsigned char* OUT pData , KL_CLIENT_ID nClientID);	/* 通过指定的服务端从指定的客户端接收数据 */

	int EXPORT_DLL KLSC_ClientSend(unsigned long nClientHandle , unsigned long IN nDataLen , unsigned char* IN pData);															/* 通过指定的客户端向其连接到的服务端发送数据 */
	int EXPORT_DLL KLSC_ClientRecv(unsigned long nClientHandle , unsigned long* OUT pDataLen , unsigned char* OUT pData);													/* 通过指定的客户端从其连接到的服务端接收数据 */


	/* UDP 接口 */
	/* 接收数据端为服务器，服务器接收、客户端只能发送 */
	int EXPORT_DLL KLSC_CreateUdpServer(KLUdpSvrParam* pParam);
	int EXPORT_DLL KLSC_CreateUdpClient(KLUdpCltParam* pParam);
	int EXPORT_DLL KLSC_DeleteUdpServer(unsigned long nServerHandle);
	int EXPORT_DLL KLSC_DeleteUdpClient(unsigned long nClientHandle);

	int EXPORT_DLL KLSC_UdpCltSend(unsigned long nCltHandle,const unsigned char *pSendBuf, unsigned long nSendLen,const char *ip,unsigned short nPort, bool bDirectSend = TRUE);
	int EXPORT_DLL KLSC_UdpCltSend(unsigned long nCltHandle,const unsigned char *pSendBuf, unsigned long nSendLen,bool bDirectSend);

#ifdef __CPLUSPLUS
}
#endif
