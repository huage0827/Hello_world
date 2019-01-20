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

/*����ģ�����ݶ���*/
typedef struct TagSimuModelData{
	unsigned long ulTs1;
	unsigned long ulTs2;
	unsigned long ulDataLen;
	char *pData;
}SimuModelData_t;

/*ģ��ͨ�Ź���ʵ��*/
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

/*ģ��ͬ����ʩ*/
typedef struct TagModelDataSyncEntity{
	CommuSimuM_MShm_t *pShm;
	HANDLE	nShmfd;
#ifdef WIN_PLATFORM
	HANDLE hShmMutex;
	string szShmMutex;
#endif
	string szShmPath;
}ModelDataSyncEntity_t;

/*ģ���������*/
typedef struct tagModelOutputData{
	unsigned long nId;
	unsigned long nDataType;
	unsigned long nSize;
	unsigned long nByteLen;
	bool bRecord;
	bool bDisplay;
}ModelOutputData_t;

/*ģ�Ϳɵ�����*/
typedef struct tagModelAdjustParam{
	unsigned long nId;
	string szName;
	unsigned long nDataType;
	unsigned long nSize;
	unsigned long nByteLen;
}ModelAdjustParam_t;


/*ģ������״̬ö�ٶ���*/
typedef enum EnumModelRunState{
	MODEL_RUN_State_unload = 0,
	MODEL_RUN_State_loading = 1,
	MODEL_RUN_State_loaded = 2,
	MODEL_RUN_State_pause = 3,
	MODEL_RUN_State_runing = 4,
	MODEL_RUN_State_reset = 5,
}EModelRunState;

/*M_Cͨ�Žӿ�*/
class CSimuM_CComm
{
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	friend void* SimuModelDataRecvThread(LPVOID lpParam);/*ģ�����ݽ��մ����߳�*/
	friend void* SimuModelDataParseThread(LPVOID lpParam);/*ģ�����ݽ��������߳�*/
	friend void* SimuModelRecordParseThread(LPVOID lpParam);/*ģ������¼�����ݴ����߳�*/
#else
	friend DWORD WINAPI SimuModelDataRecvThread(LPVOID lpParam);/*ģ�����ݽ��մ����߳�*/
	friend DWORD WINAPI SimuModelDataParseThread(LPVOID lpParam);/*ģ�����ݽ��������߳�*/
	friend DWORD WINAPI SimuModelRecordDataParseThread(LPVOID lpParam);/*ģ������¼�����ݴ����߳�*/
#endif

public:
	CSimuM_CComm(const unsigned char *szProjId, unsigned long nModelId);/*���캯��*/
	~CSimuM_CComm();/*��������*/

	bool Initialize(CSimuConfigParser *pSimuCfg, CSimuDispDataC_DComm *pDisp, CSimuStorDataC_DComm *pStor);/*��ʼ��*/
	void Uninitialize();/*����ʼ��*/

	bool Reset();/*����*/

	void SimuModelDataRecvService(); /*����ģ�����ݽ��շ���*/
	void SimuModelDataParseService(); /*����ģ�����ݽ�������*/
	void SimuModelRecordDataParseService();/*����ģ��¼�����ݽ�������*/

	void SetModelState(EModelRunState state);/*����ģ������״̬*/
	EModelRunState GetModelState();/*��ȡģ������״̬*/

	const char *GetLastErrMsg();/*��ȡ���һ��ִ�д�����Ϣ*/

	/*��ģ�ͷ�������ӿ�*/
	bool SendExecModelCmd(unsigned long nExecMode);/*����ִ�������ģ��*/
	bool SendPauseModelCmd();/*������ͣģ�͸�ģ��*/
	bool SendResetModelCmd();/*�������������ģ��*/
	int  SendSetModelParamCmd(const vector<CommuSetModelParam_t *> *pVecParam);/*���Ͷ�̬���������ģ��*/
	bool SendNonRTAdjustCmd(unsigned long ulAdjust);/*���ͷ�ʵʱ���лص������ģ��*/
	bool SendModelData(char *pData, unsigned nDataLen);/*�������ݸ�ģ��*/

	/*�����Ƿ���*/
	unsigned long ModelDataUnParseCount();

private:
	bool ModelCommuInit();/*ģ��ͨ�ų�ʼ��*/
	bool ModelCommuUninit();/*ģ��ͨ�ŷ���ʼ��*/
	bool ModelCommuReset();/*ģ��ͨ������*/

	bool ModelThreadInit();/*ģ���̳߳�ʼ��*/
	bool ModelThreadUninit();/*ģ���̷߳���ʼ��*/
	bool ModelThreadReset();/*ģ���߳�����*/

	bool ModelCfgInit();/*ģ�����ó�ʼ��*/
	bool ModelCfgUninit();/*ģ�����÷���ʼ��*/

	bool ModelSyncInit();/*ģ��ͬ����ʼ��*/
	bool ModelSyncUninit();/*ģ��ͬ������ʼ��*/
	bool ModelSyncReset();/*ģ��ͬ������*/

	bool ParseRecvData(const SimuModelData_t *pMdlData);/*������������*/

	/*������յ���ģ������*/
	bool ExecRegisterCmd(const SimuModelData_t *pMdlData);/*��������ע������*/
	bool ExecTestEventReportCmd(const SimuModelData_t *pMdlData);/*������������¼��ϱ�����*/
	bool ExecExceptEventReportCmd(const SimuModelData_t *pMdlData);/*���������쳣�¼��ϱ�����*/
	bool ExecExceptEventReportCmd(int nType, const char *szOut);/*ִ���쳣�¼��ϱ�*/
	bool ExecDispModelData(const SimuModelData_t *pMdlData);/*����������ʾģ����������*/
	bool ExecDispUserData(const SimuModelData_t *pMdlData);/*����������ʾ�û���������*/
	bool ExecStorModelData(const SimuModelData_t *pMdlData);/*��������洢ģ����������*/
	bool ExecStorUserData(const SimuModelData_t *pMdlData);/*��������洢�û���������*/
	bool ExecDispAndStorData(const SimuModelData_t *pMdlData);/*����������ʾ�ʹ洢��������*/
	int  ExecRecordData(const SimuModelData_t *pMdlData, unsigned char *pszBuffer, unsigned long &nDataOff, bool bForceSendFlag);/*��������洢ģ����������*/

	/*��ӡ���*/
	void OutPrint(char *szMsg);

private:
	bool m_bInit;/*��ʼ��*/
	EModelRunState m_RunState;/*ģ������״̬*/
	string m_szErrMsg;/*������Ϣ*/
	unsigned char m_szProjId[128];
	unsigned long m_nModelId;/*����ģ��id*/

	FILE* m_fpOutPrint;/*�ļ����������*/

	/*ģ��ͨ����ʩ*/
	ModelCommuEntity_t m_CommuEntity;/*ģ��ͨ�Ź���ʵ��*/

	/*ģ���߳���ʩ*/
	HANDLE m_hRecvThread;/*�����߳̾��*/
	HANDLE m_hParseThread;/*�����߳̾��*/
	HANDLE m_hRecordParseThread;/*¼�ƽ����߳̾��*/
	bool m_bRecvQuitFlag;/*�����߳��Ƴ���־*/
	HANDLE m_hParseEvent;/*�����̹߳����¼�*/
	HANDLE m_hRecordParseEvent;/*����¼�������̹߳����¼�*/
	list<SimuModelData_t *>	m_modelDataList;/*ģ�������б�*/
	CRITICAL_SECTION m_criDataList;/*�����б�����*/
	list<SimuModelData_t *> m_modelRecordDataList;/*ģ�ʹ洢�����б�*/
	CRITICAL_SECTION m_criRecordDataList;/*�洢�����б�����*/

	/*ģ������*/
	vector<ModelOutputData_t *>	m_vecOutputData;/*ģ����������б�*/
	map<string,ModelAdjustParam_t *> m_mapMdlParam;/*ģ�Ϳɵ������б�*/

	/*ģ��ͬ����ʩ*/
	vector<ModelDataSyncEntity_t *> m_vecDataSyncEntity;

	/*�ⲿָ��*/
	CSimuConfigParser *m_pSimuCfg;/*ģ�����ý���ָ��*/
	CSimuDispDataC_DComm *m_pDispDataComm;/*��ʾ����ͨ�Žӿ�*/
	CSimuStorDataC_DComm *m_pStorDataComm;/*�洢����ͨ�Žӿ�*/
};
