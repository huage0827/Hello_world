#pragma  once
#include "SimuStorDataC_DCmd.h"
#include "SimuCommAPI.h"

/*ͨ������*/
typedef enum{
	SDC_DCommType_TCP = 1, /*TCP_IP*/
	SDC_DCommType_Shm = 2, /*ShareMemory*/
}StorDataC_DComm_Type;

/*���ݴ洢ͨ�Žӿ�*/
class CSimuStorDataC_DComm
{
public:
	CSimuStorDataC_DComm();/*���캯��*/
	virtual ~CSimuStorDataC_DComm();/*��������*/

	/*��ʼ��*/
	bool Initialize(const char *szIp, unsigned short usPort, bool bLocalUseShareMem = false);

	/*����ʼ��*/
	void Uninitialize();

	/*���ݷ��ͽӿ�*/
	int  SendData(unsigned char *szData, unsigned long nDataLen);/*��������*/
	void ClearShm();/*�����ڴ�������*/

	void OnRcvedStorSvrData(unsigned char* szData , unsigned long nDataLen);/*���ݽ��մ���*/
	void OnStorSvrConnected(KLConnectInfo* pConnectInfo);/*���ӵ�����������*/
	void OnStorSvrDisconnected(KLConnectInfo* pConnectInfo);/*�ӷ������Ͽ�����*/

private:
	bool m_bInit; /*��ʼ����־*/
	StorDataC_DComm_Type m_SDataC_DCommType;/*ͨ������*/

	/*TCP_IP*/
	bool m_bConnect;/*���ӷ�������־*/
	char m_szStorIp[128];/*�洢������IP��ַ*/
	unsigned short m_usPort;/*�洢������IP�˿�*/
	unsigned long  m_nClientID;/*ͨ�ſͻ���*/
	
	/*ShareMemory*/
	HANDLE m_hWriteShmfd; /*�����ڴ��ļ�������*/
	CommuSimuStorDataC_DShm_t *m_pWriteShm;/*�����ڴ�ָ��*/
	HANDLE m_hWriteShmLock;/*�����ڴ汣����*/
	HANDLE m_hWriteSemSignal;/*�����ڴ�д����֪ͨ�ź���*/
};
