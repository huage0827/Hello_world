/*****************************************************************
 * �ļ���: KLBaseComm.h
 * ����: ģ���ͨ�Ź��ܽӿڶ���
 *
 *****************************************************************/
#pragma once

#include "KLCommDef.h"

#define MIN_CLIENT_ID				1				/* �ͻ�����СID�� */
#define MAX_CLIENT_ID				65535			/* �ͻ������ID�� */
#define MAX_RCV_DATA_SIZE			10240*3			/* ÿ�ν�����������ֽ��� */
#define MAX_SEND_DATA_SIZE			10240*3			/* ÿ�η�����������ֽ��� */

#define SLEEP_MAX_TICK	50
#define DATA_DISPLAYSLEEP_TICK      50
#define SHM_MAX_SYNC_SEM 1024

/****************************************************************
 * �ṹ���� : KLCommDataBuffer
 * ˵�� : ����ͨ�Ż�����
 ****************************************************************/
typedef struct tagKLCommDataBuffer
{
	unsigned char *szWaitSendDataBuffer;			/* ���������ݻ����� */
	unsigned long nWaitSendDataBufferLen;		/* ���������ݻ�������С */
	unsigned char *szRcvedDataBuffer;				/* δ����Ľ��յ������ݻ����� */
	unsigned long nRcvedDataBufferLen;				/* δ����Ľ��յ������ݻ�������С */
} KLCommDataBuffer , *PKLCommDataBuffer;

/****************************************************************
 * ���� : CKLBaseCommClient
 * ���� : ��
 * ˵�� : �ͻ��˹��ܽӿ�
 ****************************************************************/
class CKLBaseCommClient
{
public :
	virtual BOOL Initialize(void* pCltParam) = 0;													/* ��ָ���Ĳ�����ʼ���ͻ��� */
	virtual void   Uninitialize() = 0;																				/* �����ͻ��˲��ͷ������Դ */
	virtual BOOL IsConnected() = 0;																			/* �ͻ����Ƿ����ӵ��˷���� */
	virtual BOOL SendData(unsigned char* szData , unsigned long nDataLen , BOOL bDirectSend = FALSE) = 0;			/* ����ָ�������ݵ������ */
	virtual UINT  RecvData(unsigned char* szData , unsigned long nMaxDataLen) = 0;	/* �ӷ���˽�����಻����ָ�����ȵ����� */
};

/****************************************************************
 * ���� : CKLBaseCommServer
 * ���� : ��
 * ˵�� : ����˹��ܽӿ�
 ****************************************************************/
class CKLBaseCommServer
{
public:
	virtual BOOL Initialize(void* pSvrParam) = 0;																									/* ��ָ���Ĳ�����ʼ������� */
	virtual void   Uninitialize() = 0;																																/* ��������˲��ͷ������Դ */
	virtual BOOL SendData(KL_CLIENT_ID nClientID , unsigned char* szData , unsigned long nDataLen) = 0;				/* ����ָ�������ݵ�ָ���Ŀͻ��� */
	virtual UINT  RecvData(KL_CLIENT_ID nClientID , unsigned char* szData , unsigned long nMaxDataLen) = 0;		/* ��ָ���Ŀͻ��˽�����಻����ָ�����ȵ�����*/
	virtual BOOL GetClientInfo(KL_CLIENT_ID nClientID , void*& pClientInfo) = 0;											/* ��ȡָ���ͻ��˵���Ϣ */
};
