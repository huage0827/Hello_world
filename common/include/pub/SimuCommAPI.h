/*****************************************************************
 * �ļ���: SimuCommAPI.h
 * ����: ģ���ͨ��API����

 *****************************************************************/
#pragma once

#include "SimuCommunicationDef.h"
#include "KLCommDef.h"
#include "KLBaseComm.h"

/****************************************************************
 * �ṹ���� : SimuCommParams
 * ˵�� : ͨ��API�Ĳ�������
 ****************************************************************/
typedef struct tagSimuCommParams
{
	char szAppName[256];									/* ���ô�API���ϲ�Ӧ�ó����� */
} SimuCommParams , *PSimuCommParams;

extern SimuCommParams g_SimuCommParams;

#ifdef __CPLUSPLUS
extern "C"
{
#endif
	int EXPORT_DLL KLSC_Initialize(SimuCommParams* pSimuCommParams);																									/* ��ʼ���� */
	int EXPORT_DLL KLSC_Uninitialize();																																										/* ж�ؿⲢ�ͷ������Դ */
	
	int EXPORT_DLL KLSC_CreateClient(KLCltParam* pParam , CKLBaseCommClient*& pClient);																																	/* ��ָ����������һ���ͻ��� */
	int EXPORT_DLL KLSC_CreateServer(KLSvrParam* pParam , CKLBaseCommServer*& pServer);		/* ��ָ����������һ������� */
	
	int EXPORT_DLL KLSC_CreateClient(KLCltParam* pParam);																																	/* ��ָ����������һ���ͻ��� */
	int EXPORT_DLL KLSC_CreateServer(KLSvrParam* pParam);

	int EXPORT_DLL KLSC_DeleteClient(unsigned long nClientHandle);																																		/* ɾ��ָ���Ŀͻ��� */
	int EXPORT_DLL KLSC_DeleteServer(unsigned long nServerHandle);				 /* ɾ��ָ���ķ���� */

	BOOL EXPORT_DLL KLSC_IsConnected(unsigned long nClientHandle);	            /* �ж�ָ���Ŀͻ����Ƿ������ӵ������ */
	
	int EXPORT_DLL KLSC_ServerSend(unsigned long nServerHandle , unsigned long IN nDataLen , unsigned char* IN pData , KL_CLIENT_ID nClientID);				/* ͨ��ָ���ķ������ָ���Ŀͻ��˷������� */
	int EXPORT_DLL KLSC_ServerRecv(unsigned long nServerHandle , unsigned long* OUT pDataLen , unsigned char* OUT pData , KL_CLIENT_ID nClientID);	/* ͨ��ָ���ķ���˴�ָ���Ŀͻ��˽������� */

	int EXPORT_DLL KLSC_ClientSend(unsigned long nClientHandle , unsigned long IN nDataLen , unsigned char* IN pData);															/* ͨ��ָ���Ŀͻ����������ӵ��ķ���˷������� */
	int EXPORT_DLL KLSC_ClientRecv(unsigned long nClientHandle , unsigned long* OUT pDataLen , unsigned char* OUT pData);													/* ͨ��ָ���Ŀͻ��˴������ӵ��ķ���˽������� */


	/* UDP �ӿ� */
	/* �������ݶ�Ϊ�����������������ա��ͻ���ֻ�ܷ��� */
	int EXPORT_DLL KLSC_CreateUdpServer(KLUdpSvrParam* pParam);
	int EXPORT_DLL KLSC_CreateUdpClient(KLUdpCltParam* pParam);
	int EXPORT_DLL KLSC_DeleteUdpServer(unsigned long nServerHandle);
	int EXPORT_DLL KLSC_DeleteUdpClient(unsigned long nClientHandle);

	int EXPORT_DLL KLSC_UdpCltSend(unsigned long nCltHandle,const unsigned char *pSendBuf, unsigned long nSendLen,const char *ip,unsigned short nPort, bool bDirectSend = TRUE);
	int EXPORT_DLL KLSC_UdpCltSend(unsigned long nCltHandle,const unsigned char *pSendBuf, unsigned long nSendLen,bool bDirectSend);

#ifdef __CPLUSPLUS
}
#endif
