#pragma  once
#include "SimuCommAPI.h"
#include "SimuDispDataC_DCmd.h"
#include <list>

typedef struct{
	unsigned long nDataLen;
	char *pData;
}DispDataWaitSend_t;

/*数据显示通信接口*/
class CSimuDispDataC_DComm
{
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	friend void* SimuModelDispDataSendThread(LPVOID lpParam);
#else
	friend DWORD WINAPI SimuModelDispDataSendThread(LPVOID lpParam);
#endif

public:
	/*构造函数*/
	CSimuDispDataC_DComm();
	/*析构函数*/
	virtual ~CSimuDispDataC_DComm();

	/*初始化*/
	bool Initialize(const char *szIp, unsigned short usPort);
	/*反初始化*/
	void Uninitialize();

	/*发送数据*/
	int  SendData(unsigned char *szData, unsigned long nDataLen, bool bDirectSend = false);

	void SimuModelDispDataSendService();

private:
	bool InitSocket(); /*初始化客户端socket信息 */

private:
	bool m_bInit; /*初始化标志*/

	char m_szDispIp[128];/*显示服务器IP地址*/
	unsigned short m_usPort;/*显示服务器IP端口*/

	SOCKET m_cltSocket;/*网络连接Socket*/
	CRITICAL_SECTION m_criSocket;/*Socket保护锁*/

	list<DispDataWaitSend_t *> m_lstWaitData;
	CRITICAL_SECTION m_criWaitData;

	HANDLE m_hSendThread;
	HANDLE m_hSendEvent;
};
