#pragma  once
#include <list>
#include <iostream>
#include "SimuCommAPI.h"
#include "SimuDispDataC_DComm.h"
#include "SimuStorDataC_DComm.h"
#include "SimuConfigParser.h"

using namespace std;

class CSimuDispDataC_DComm;
class CSimuStorDataC_DComm;

/*仿真模型数据定义*/
typedef struct TagSimuModelData{
	unsigned long ulTs1;
	unsigned long ulTs2;
	unsigned long ulDataLen;
	char *pData;
}SimuModelData_t;

/*模型通信管理实体*/
typedef struct TagModelCommuEntity{
	HANDLE 	nReadShmfd;
	HANDLE 	nWriteShmfd;
	CommuSimuM_CShm_t *pReadShm;
	CommuSimuM_CShm_t *pWriteShm;
#ifdef WIN_PLATFORM
	HANDLE hReadMutex;
	HANDLE hWriteMutex;
	HANDLE hReadSem;
#else
	sem_t	*pReadSem;
#endif
	char szReadShmPath[256];
	char szWriteShmPath[256];
	char szReadSemPath[256];
}ModelCommuEntity_t;

/*模型同步设施*/
typedef struct TagModelDataSyncEntity{
	CommuSimuM_MShm_t *pShm;
	HANDLE	nShmfd;
#ifdef WIN_PLATFORM
	HANDLE hShmMutex;
	string szShmMutex;
#endif
	string szShmPath;
}ModelDataSyncEntity_t;

/*模型输出数据*/
typedef struct tagModelOutputData{
	unsigned long nId;
	unsigned long nDataType;
	unsigned long nSize;
	unsigned long nByteLen;
	bool bRecord;
	bool bDisplay;
}ModelOutputData_t;

/*模型可调参数*/
typedef struct tagModelAdjustParam{
	unsigned long nId;
	string szName;
	unsigned long nDataType;
	unsigned long nSize;
	unsigned long nByteLen;
}ModelAdjustParam_t;


/*模型运行状态枚举定义*/
typedef enum EnumModelRunState{
	MODEL_RUN_State_unload = 0,
	MODEL_RUN_State_loading = 1,
	MODEL_RUN_State_loaded = 2,
	MODEL_RUN_State_pause = 3,
	MODEL_RUN_State_runing = 4,
	MODEL_RUN_State_reset = 5,
}EModelRunState;

/*M_C通信接口*/
class CSimuM_CComm
{
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	friend void* SimuModelDataRecvThread(LPVOID lpParam);/*模型数据接收处理线程*/
	friend void* SimuModelDataParseThread(LPVOID lpParam);/*模型数据解析处理线程*/
	friend void* SimuModelRecordParseThread(LPVOID lpParam);/*模型数据录制数据处理线程*/
#else
	friend DWORD WINAPI SimuModelDataRecvThread(LPVOID lpParam);/*模型数据接收处理线程*/
	friend DWORD WINAPI SimuModelDataParseThread(LPVOID lpParam);/*模型数据解析处理线程*/
	friend DWORD WINAPI SimuModelRecordDataParseThread(LPVOID lpParam);/*模型数据录制数据处理线程*/
#endif

public:
	CSimuM_CComm(const unsigned char *szProjId, unsigned long nModelId);/*构造函数*/
	~CSimuM_CComm();/*析构函数*/

	bool Initialize(CSimuConfigParser *pSimuCfg, CSimuDispDataC_DComm *pDisp, CSimuStorDataC_DComm *pStor);/*初始化*/
	void Uninitialize();/*反初始化*/

	bool Reset();/*重置*/

	void SimuModelDataRecvService(); /*仿真模型数据接收服务*/
	void SimuModelDataParseService(); /*仿真模型数据解析服务*/
	void SimuModelRecordDataParseService();/*仿真模型录制数据解析服务*/

	void SetModelState(EModelRunState state);/*设置模型运行状态*/
	EModelRunState GetModelState();/*获取模型运行状态*/

	const char *GetLastErrMsg();/*获取最后一次执行错误信息*/

	/*向模型发送命令接口*/
	bool SendExecModelCmd(unsigned long nExecMode);/*发送执行命令给模型*/
	bool SendPauseModelCmd();/*发送暂停模型给模型*/
	bool SendResetModelCmd();/*发送重置命令给模型*/
	int  SendSetModelParamCmd(const vector<CommuSetModelParam_t *> *pVecParam);/*发送动态调参命令给模型*/
	bool SendNonRTAdjustCmd(unsigned long ulAdjust);/*发送非实时运行回调命令给模型*/
	bool SendModelData(char *pData, unsigned nDataLen);/*发送数据给模型*/

	/*数据是否处理*/
	unsigned long ModelDataUnParseCount();

private:
	bool ModelCommuInit();/*模型通信初始化*/
	bool ModelCommuUninit();/*模型通信反初始化*/
	bool ModelCommuReset();/*模型通信重置*/

	bool ModelThreadInit();/*模型线程初始化*/
	bool ModelThreadUninit();/*模型线程反初始化*/
	bool ModelThreadReset();/*模型线程重置*/

	bool ModelCfgInit();/*模型配置初始化*/
	bool ModelCfgUninit();/*模型配置反初始化*/

	bool ModelSyncInit();/*模型同步初始化*/
	bool ModelSyncUninit();/*模型同步反初始化*/
	bool ModelSyncReset();/*模型同步重置*/

	bool ParseRecvData(const SimuModelData_t *pMdlData);/*解析接收数据*/

	/*处理接收到的模型命令*/
	bool ExecRegisterCmd(const SimuModelData_t *pMdlData);/*解析处理注册命令*/
	bool ExecTestEventReportCmd(const SimuModelData_t *pMdlData);/*解析处理测试事件上报命令*/
	bool ExecExceptEventReportCmd(const SimuModelData_t *pMdlData);/*解析处理异常事件上报命令*/
	bool ExecExceptEventReportCmd(int nType, const char *szOut);/*执行异常事件上报*/
	bool ExecDispModelData(const SimuModelData_t *pMdlData);/*解析处理显示模型数据命令*/
	bool ExecDispUserData(const SimuModelData_t *pMdlData);/*解析处理显示用户数据命令*/
	bool ExecStorModelData(const SimuModelData_t *pMdlData);/*解析处理存储模型数据命令*/
	bool ExecStorUserData(const SimuModelData_t *pMdlData);/*解析处理存储用户数据命令*/
	bool ExecDispAndStorData(const SimuModelData_t *pMdlData);/*解析处理显示和存储数据命令*/
	int  ExecRecordData(const SimuModelData_t *pMdlData, unsigned char *pszBuffer, unsigned long &nDataOff, bool bForceSendFlag);/*解析处理存储模型数据命令*/

	/*打印输出*/
	void OutPrint(char *szMsg);

private:
	bool m_bInit;/*初始化*/
	EModelRunState m_RunState;/*模型运行状态*/
	string m_szErrMsg;/*错误信息*/
	unsigned char m_szProjId[128];
	unsigned long m_nModelId;/*仿真模型id*/

	FILE* m_fpOutPrint;/*文件输出描述符*/

	/*模型通信设施*/
	ModelCommuEntity_t m_CommuEntity;/*模型通信管理实体*/

	/*模型线程设施*/
	HANDLE m_hRecvThread;/*接收线程句柄*/
	HANDLE m_hParseThread;/*解析线程句柄*/
	HANDLE m_hRecordParseThread;/*录制解析线程句柄*/
	bool m_bRecvQuitFlag;/*接收线程推出标志*/
	HANDLE m_hParseEvent;/*解析线程关联事件*/
	HANDLE m_hRecordParseEvent;/*解析录制数据线程关联事件*/
	list<SimuModelData_t *>	m_modelDataList;/*模型数据列表*/
	CRITICAL_SECTION m_criDataList;/*数据列表互斥锁*/
	list<SimuModelData_t *> m_modelRecordDataList;/*模型存储数据列表*/
	CRITICAL_SECTION m_criRecordDataList;/*存储数据列表互斥锁*/

	/*模型配置*/
	vector<ModelOutputData_t *>	m_vecOutputData;/*模型输出数据列表*/
	map<string,ModelAdjustParam_t *> m_mapMdlParam;/*模型可调参数列表*/

	/*模型同步设施*/
	vector<ModelDataSyncEntity_t *> m_vecDataSyncEntity;

	/*外部指针*/
	CSimuConfigParser *m_pSimuCfg;/*模型配置解析指针*/
	CSimuDispDataC_DComm *m_pDispDataComm;/*显示数据通信接口*/
	CSimuStorDataC_DComm *m_pStorDataComm;/*存储数据通信接口*/
};
