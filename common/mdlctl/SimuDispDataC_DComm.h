#pragma  once
#include "SimuCommAPI.h"
#include "SimuDispDataC_DCmd.h"
#include <list>

typedef struct{
	unsigned long nDataLen;
	char *pData;
}DispDataWaitSend_t;

/*������ʾͨ�Žӿ�*/
class CSimuDispDataC_DComm
{
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	friend void* SimuModelDispDataSendThread(LPVOID lpParam);
#else
	friend DWORD WINAPI SimuModelDispDataSendThread(LPVOID lpParam);
#endif

public:
	/*���캯��*/
	CSimuDispDataC_DComm();
	/*��������*/
	virtual ~CSimuDispDataC_DComm();

	/*��ʼ��*/
	bool Initialize(const char *szIp, unsigned short usPort);
	/*����ʼ��*/
	void Uninitialize();

	/*��������*/
	int  SendData(unsigned char *szData, unsigned long nDataLen, bool bDirectSend = false);

	void SimuModelDispDataSendService();

private:
	bool InitSocket(); /*��ʼ���ͻ���socket��Ϣ */

private:
	bool m_bInit; /*��ʼ����־*/

	char m_szDispIp[128];/*��ʾ������IP��ַ*/
	unsigned short m_usPort;/*��ʾ������IP�˿�*/

	SOCKET m_cltSocket;/*��������Socket*/
	CRITICAL_SECTION m_criSocket;/*Socket������*/

	list<DispDataWaitSend_t *> m_lstWaitData;
	CRITICAL_SECTION m_criWaitData;

	HANDLE m_hSendThread;
	HANDLE m_hSendEvent;
};
