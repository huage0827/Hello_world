#pragma  once
#include "SimuStorDataC_DCmd.h"
#include "SimuCommAPI.h"

/*通信类型*/
typedef enum{
	SDC_DCommType_TCP = 1, /*TCP_IP*/
	SDC_DCommType_Shm = 2, /*ShareMemory*/
}StorDataC_DComm_Type;

/*数据存储通信接口*/
class CSimuStorDataC_DComm
{
public:
	CSimuStorDataC_DComm();/*构造函数*/
	virtual ~CSimuStorDataC_DComm();/*析构函数*/

	/*初始化*/
	bool Initialize(const char *szIp, unsigned short usPort, bool bLocalUseShareMem = false);

	/*反初始化*/
	void Uninitialize();

	/*数据发送接口*/
	int  SendData(unsigned char *szData, unsigned long nDataLen);/*发送数据*/
	void ClearShm();/*共享内存区清理*/

	void OnRcvedStorSvrData(unsigned char* szData , unsigned long nDataLen);/*数据接收处理*/
	void OnStorSvrConnected(KLConnectInfo* pConnectInfo);/*连接到服务器处理*/
	void OnStorSvrDisconnected(KLConnectInfo* pConnectInfo);/*从服务器断开处理*/

private:
	bool m_bInit; /*初始化标志*/
	StorDataC_DComm_Type m_SDataC_DCommType;/*通信类型*/

	/*TCP_IP*/
	bool m_bConnect;/*连接服务器标志*/
	char m_szStorIp[128];/*存储服务器IP地址*/
	unsigned short m_usPort;/*存储服务器IP端口*/
	unsigned long  m_nClientID;/*通信客户端*/
	
	/*ShareMemory*/
	HANDLE m_hWriteShmfd; /*共享内存文件描述符*/
	CommuSimuStorDataC_DShm_t *m_pWriteShm;/*共享内存指针*/
	HANDLE m_hWriteShmLock;/*共享内存保护锁*/
	HANDLE m_hWriteSemSignal;/*共享内存写数据通知信号量*/
};
