#include "stdafx.h"
#include "SimuM_CComm.h"
#include "SimuM_CCmd.h"
#include "Util.h"
#include <time.h>
#include <sys/time.h>

#ifdef QNX_PLATFORM
#include "SimuProjectMgr.h"
#else
#include "SimuProjectMgr.h"
#endif
//////////////////////////////////////////////////////////////////////////////////////////
//				静态函数接口
//////////////////////////////////////////////////////////////////////////////////////////
static int GetDataTypeByteLen(int type);
static int ParseMdlSetParamValData(int type, char *pData);

int GetDataTypeByteLen(int type)
{
	int nLen = 0;
	switch(type){
	case CONF_DATA_Type_unknown:
		nLen = 0;break;
	case CONF_DATA_Type_int8:
		nLen = 1;break;
	case CONF_DATA_Type_uInt8:
		nLen = 1;break;
	case CONF_DATA_Type_int16:
		nLen = 2;break;
	case CONF_DATA_Type_uInt16:
		nLen = 2;break;
	case CONF_DATA_Type_int32:
		nLen = 4;break;
	case CONF_DATA_Type_uInt32:
		nLen = 4;break;
	case CONF_DATA_Type_int64:
		nLen = 8;break;
	case CONF_DATA_Type_uInt64:
		nLen = 8;break;
	case CONF_DATA_Type_float:
		nLen = 4;break;
	case CONF_DATA_Type_double:
		nLen = 8;break;
	default:
		nLen = 0;
	}
	return nLen;
}

int ParseMdlSetParamValData(int type, char *pData, vector<string> &vecVal)
{
	int nLen = 0;
	switch(type){
	case CONF_DATA_Type_unknown:
		nLen = 0;break;
	case CONF_DATA_Type_int8:
	{
		for(unsigned int i = 0; i < vecVal.size(); i++){
			if( !CUtil::IsInteger(vecVal.at(i).c_str()))
				return 0;
			char cTmp = atoi(vecVal.at(i).c_str());
			memcpy(pData + nLen, &cTmp, 1);
			nLen += 1;
		}
	}
		break;
	case CONF_DATA_Type_uInt8:
	{
		for(unsigned int i = 0; i < vecVal.size(); i++){
			if( !CUtil::IsInteger(vecVal.at(i).c_str()))
				return 0;
			unsigned char cTmp = atoi(vecVal.at(i).c_str());
			memcpy(pData + nLen, &cTmp, 1);
			nLen += 1;
		}
	}
		break;
	case CONF_DATA_Type_int16:
	{
		for(unsigned int i = 0; i < vecVal.size(); i++){
			if( !CUtil::IsInteger(vecVal.at(i).c_str()))
				return 0;
			short nTmp = atoi(vecVal.at(i).c_str());
			memcpy(pData + nLen, &nTmp, 2);
			nLen += 2;
		}
	}
		break;
	case CONF_DATA_Type_uInt16:
	{
		for(unsigned int i = 0; i < vecVal.size(); i++){
			if( !CUtil::IsInteger(vecVal.at(i).c_str()))
				return 0;
			unsigned short nTmp = atoi(vecVal.at(i).c_str());
			memcpy(pData + nLen, &nTmp, 2);
			nLen += 2;
		}
	}
		break;
	case CONF_DATA_Type_int32:
	{
		for(unsigned int i = 0; i < vecVal.size(); i++){
			if( !CUtil::IsInteger(vecVal.at(i).c_str()))
				return 0;
			int nTmp = atol(vecVal.at(i).c_str());
			memcpy(pData + nLen, &nTmp, 4);
			nLen += 4;
		}
	}
		break;
	case CONF_DATA_Type_uInt32:
	{
		for(unsigned int i = 0; i < vecVal.size(); i++){
			if( !CUtil::IsInteger(vecVal.at(i).c_str()))
				return 0;
			unsigned int nTmp = atol(vecVal.at(i).c_str());
			memcpy(pData + nLen, &nTmp, 4);
			nLen += 4;
		}
	}
		break;
	case CONF_DATA_Type_int64:
	{
		for(unsigned int i = 0; i < vecVal.size(); i++){
			if( !CUtil::IsInteger(vecVal.at(i).c_str()))
				return 0;
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
			long long nTmp = atoll(vecVal.at(i).c_str());
#else
			long long nTmp = atol(vecVal.at(i).c_str());
#endif
			memcpy(pData + nLen, &nTmp, 8);
			nLen += 8;
		}
	}
		break;
	case CONF_DATA_Type_uInt64:
	{
		for(unsigned int i = 0; i < vecVal.size(); i++){
			if( !CUtil::IsInteger(vecVal.at(i).c_str()))
				return 0;
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
			long long nTmp = atoll(vecVal.at(i).c_str());
#else
			long long nTmp = atol(vecVal.at(i).c_str());
#endif
			memcpy(pData + nLen, &nTmp, 8);
			nLen += 8;
		}
	}
		break;
	case CONF_DATA_Type_float:
	{
		for(unsigned int i = 0; i < vecVal.size(); i++){
			if( !CUtil::IsFloat(vecVal.at(i).c_str()))
				return 0;
			float nTmp = atof(vecVal.at(i).c_str());
			memcpy(pData + nLen, &nTmp, 4);
			nLen += 4;
		}
	}
		break;
	case CONF_DATA_Type_double:
	{
		for(unsigned int i = 0; i < vecVal.size(); i++){
			if( !CUtil::IsDouble(vecVal.at(i).c_str()))
				return 0;
			double nTmp = atof(vecVal.at(i).c_str());
			memcpy(pData + nLen, &nTmp, 8);
			nLen += 8;
		}
	}
		break;
	default:
		nLen = 0;
	}
	return nLen;
}

//////////////////////////////////////////////////////////////////////////////////////////
//				处理线程
//////////////////////////////////////////////////////////////////////////////////////////
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
void* SimuModelDataRecvThread(LPVOID lpParam)
#else
DWORD WINAPI SimuModelDataRecvThread(LPVOID lpParam)
#endif
{
	CSimuM_CComm *pComm = (CSimuM_CComm*)lpParam;
	pComm->SimuModelDataRecvService();
	return 0;
}


#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
void* SimuModelDataParseThread(LPVOID lpParam)
#else
DWORD WINAPI SimuModelDataParseThread(LPVOID lpParam)
#endif
{
	CSimuM_CComm *pComm = (CSimuM_CComm*)lpParam;
	pComm->SimuModelDataParseService();
	return 0;
}


#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
void* SimuModelRecordDataParseThread(LPVOID lpParam)
#else
DWORD WINAPI SimuModelRecordDataParseThread(LPVOID lpParam)
#endif
{
	CSimuM_CComm *pComm = (CSimuM_CComm*)lpParam;
	pComm->SimuModelRecordDataParseService();
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//				外部接口
//////////////////////////////////////////////////////////////////////////////////////////
CSimuM_CComm::CSimuM_CComm(const unsigned char *szProjId, unsigned long nModelId)
{
	m_bInit = false;
	memset(m_szProjId, 0x0, sizeof(m_szProjId));
	memcpy(m_szProjId, szProjId, strlen((const char *)szProjId));
	m_nModelId = nModelId;
	m_RunState = MODEL_RUN_State_unload;
	m_fpOutPrint = NULL;
}

CSimuM_CComm::~CSimuM_CComm()
{
	Uninitialize();
}

bool CSimuM_CComm::Initialize(CSimuConfigParser *pSimuCfg, CSimuDispDataC_DComm *pDisp, CSimuStorDataC_DComm *pStor)
{
	if( m_bInit ){
		m_szErrMsg = "CSimuM_CComm already init";
		return false;
	}

	m_pSimuCfg = pSimuCfg;
	m_pDispDataComm = pDisp;
	m_pStorDataComm = pStor;

	/*模型通信初始化*/
	if( !ModelCommuInit()){
		return false;
	}

	/*模型线程初始化*/
	if( !ModelThreadInit()){
		return false;
	}

	/*模型配置初始化*/
	if( !ModelCfgInit()){
		return false;
	}

	/*模型时钟初始化*/
	if( !ModelSyncInit()){
		return false;
	}
	
	m_bInit = true;

	return true;
}

void CSimuM_CComm::Uninitialize()
{
	if( !m_bInit){
		return;
	}

	ModelThreadUninit();
	ModelCommuUninit();
	ModelCfgUninit();
	ModelSyncUninit();

	m_bInit = false;
}

bool CSimuM_CComm::Reset()
{
	/*向模型发送停止执行命令*/
	SendResetModelCmd();

	/*模型通信重置*/
	ModelCommuReset();

	/*模型线程重置*/
	ModelThreadReset();

	/*模型同步重置*/
	ModelSyncReset();

	/*修改模型状态*/
	m_RunState = MODEL_RUN_State_unload;

	return true;
}

/*仿真模型数据接收服务*/
void CSimuM_CComm::SimuModelDataRecvService()
{
	int nCount = 0;
	char szData[8192];
	unsigned long nDataLen = 0;
	int nReadIdx = 0;
	CommuSimuM_CShm_t *pReadShm = m_CommuEntity.pReadShm;
	struct timeval nowTimeVal;
	nowTimeVal.tv_sec = 0;
	nowTimeVal.tv_usec = 0;
	unsigned long ulCmdType;
	bool bBufferFull = false;

	while(1)
	{
		/*等待读信号*/
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
		sem_post(m_CommuEntity.pReadSem);
#else 
		WaitForSingleObject(m_CommuEntity.hReadSem, 10);
#endif

		/*检测退出标识*/
		if( m_bRecvQuitFlag){
			break;
		}

		/*检测是否有命令 */
		do{
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
			pthread_mutex_lock(&pReadShm->mutex);
#else
			WaitForSingleObject(m_CommuEntity.hReadMutex, INFINITE);
#endif
			if(pReadShm->ReadDataPtr%MAX_MC_SHM_QUEUE_LEN == pReadShm->WriteDataPtr%MAX_MC_SHM_QUEUE_LEN)
			{
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
				pthread_mutex_unlock(&pReadShm->mutex);
#else
				ReleaseSemaphore(m_CommuEntity.hReadMutex, 1, 0);
#endif
				break;
			}

			/*读取命令*/
			nReadIdx = pReadShm->ReadDataPtr%MAX_MC_SHM_QUEUE_LEN;
			memcpy(szData, pReadShm->szData[nReadIdx], pReadShm->nDataLen[nReadIdx]);
			nDataLen = pReadShm->nDataLen[nReadIdx];

			/*移动读指针*/
			pReadShm->ReadDataPtr += 1;
			pReadShm->ReadDataPtr %= MAX_MC_SHM_QUEUE_LEN;

			/*缓冲区已满*/
			bBufferFull = false;
			if( pReadShm->nCoverCount >= 1){
				pReadShm->nCoverCount = 0;
				bBufferFull = true;
			}

#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
			pthread_mutex_unlock(&pReadShm->mutex);
#else
			ReleaseSemaphore(m_CommuEntity.hReadMutex, 1, 0);
#endif
			/*新建一个数据接口并插入数据缓冲队列中*/
			SimuModelData_t *pMdlData = new SimuModelData_t;
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
			gettimeofday(&nowTimeVal, NULL);
#else
			nowTimeVal.tv_sec = time(NULL);
#endif
			pMdlData->ulTs1 = nowTimeVal.tv_sec;
			pMdlData->ulTs2 = nowTimeVal.tv_usec;
			pMdlData->ulDataLen = nDataLen;
			pMdlData->pData = new char[nDataLen];
			memcpy(pMdlData->pData, szData, nDataLen);

			// 获取消息类型
			memcpy(&ulCmdType, szData, 4);

			if( ulCmdType == M_C_CMD_StorModelData || ulCmdType == M_C_CMD_StorUserData 
				|| ulCmdType == M_C_CMD_DispAndStorData)
			{
				EnterCriticalSection(&m_criRecordDataList);
				m_modelRecordDataList.push_front(pMdlData);
				LeaveCriticalSection(&m_criRecordDataList);
			}else{ 
				EnterCriticalSection(&m_criDataList);
				m_modelDataList.push_front(pMdlData);
				LeaveCriticalSection(&m_criDataList);

				if(bBufferFull){
					pMdlData = new SimuModelData_t;
					ulCmdType = M_C_CMD_BUFFERFULL;
					pMdlData->pData = new char[4];
					memcpy(pMdlData->pData, &ulCmdType, 4);
					pMdlData->ulDataLen = 4;
					EnterCriticalSection(&m_criRecordDataList);
					m_modelDataList.push_front(pMdlData);
					LeaveCriticalSection(&m_criRecordDataList);
				}
			}
		}while(1);
	}
}

/*仿真模型数据解析服务*/
void CSimuM_CComm::SimuModelDataParseService()
{
	int nListSize = 0;
	SimuModelData_t *pMdlData = NULL;

	while(1)
	{
		/*检测退出信号*/
		if( WaitForSingleObject(m_hParseEvent,0) == WAIT_OBJECT_0 ){
			break;
		}

		EnterCriticalSection(&m_criDataList);
		nListSize = m_modelDataList.size();
		LeaveCriticalSection(&m_criDataList);

		if( nListSize <= 0){
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
			usleep(1);
#else
			Sleep(1);
#endif
			continue;
		}

		/*解析模型数据*/
		do
		{
			EnterCriticalSection(&m_criDataList);
			nListSize = m_modelDataList.size();
			if( nListSize >= 1){
				pMdlData = m_modelDataList.back();
				m_modelDataList.pop_back();
			}
			LeaveCriticalSection(&m_criDataList);

			if( nListSize <= 0 || pMdlData == NULL){
				break;
			}
			ParseRecvData(pMdlData);

			/*释放模型数据资源*/
			delete []pMdlData->pData;
			delete pMdlData;
		}while(1);
	}

}

/*仿真模型存储数据解析服务*/
void CSimuM_CComm::SimuModelRecordDataParseService()
{
	int nListSize = 0;
	int nTotalSendNum = 0;
	int nIdx = 0;
	SimuModelData_t *pMdlData = NULL;

	unsigned char *pSendBuff = NULL;
	unsigned long ulSendDataLen = 0;
	int  nRet = 0;

	pSendBuff = new unsigned char[KL_COMMU_STOR_DATA_SEND_MAX_DATA_UNIT];
	ulSendDataLen = 0;

	while(1)
	{
		/*检测退出信号*/
		if( WaitForSingleObject(m_hRecordParseEvent, 0) == WAIT_OBJECT_0 ){
			break;
		}

		EnterCriticalSection(&m_criRecordDataList);
		nListSize = m_modelRecordDataList.size();
		LeaveCriticalSection(&m_criRecordDataList);

		if( nListSize <= 0){
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
			usleep(1);
#else
			Sleep(1);
#endif
			continue;
		}

#ifdef WIN_PLATFORM
		/*非实时反馈回调*/
		EnterCriticalSection(&m_criRecordDataList);
		nListSize = m_modelRecordDataList.size();
		LeaveCriticalSection(&m_criRecordDataList);
		if( nListSize > 10000){
			SendNonRTAdjustCmd(nListSize/10000);
		}
#endif

		//nTotalSendNum = 0;
		/*解析存储模型数据*/
		do
		{
			if( pMdlData == NULL){
				EnterCriticalSection(&m_criRecordDataList);
				nListSize = m_modelRecordDataList.size();
				if( nListSize >= 1){
					pMdlData = m_modelRecordDataList.back();
					m_modelRecordDataList.pop_back();
				}
				LeaveCriticalSection(&m_criRecordDataList);
			}
			
			if( pMdlData == NULL){
				break;
			}

			/*封包发送数据*/
			nTotalSendNum++;
			nRet = ExecRecordData(pMdlData, pSendBuff, ulSendDataLen, false);

			/*释放模型数据资源*/
			if( nRet >= 0){
				delete []pMdlData->pData;
				delete pMdlData;
				pMdlData = NULL;
			}

			/*回调*/
			if( nRet <= 0){
				SendNonRTAdjustCmd(100);
				break;
			}
		}while(1);

		/*将数据未发送数据发出*/
		if( ulSendDataLen > 0){
			m_pStorDataComm->SendData(pSendBuff, ulSendDataLen);
			ulSendDataLen = 0;
		}
	}

	delete []pSendBuff;
}

/*设置模型运行状态*/
void CSimuM_CComm::SetModelState(EModelRunState state)
{
	m_RunState = state;
}

/*获取模型运行状态*/
EModelRunState CSimuM_CComm::GetModelState()
{
	return m_RunState;
}

/*向模型发送命令接口*/
bool CSimuM_CComm::SendExecModelCmd(unsigned long nExecMode)
{
	int nSendLen = 0;
	char szSend[8192];
	unsigned long nCmd = M_C_CMD_ExecSimuModel;

	/*命令组包*/
	memcpy(szSend + nSendLen, &nCmd, 4);
	nSendLen += 4;
	memcpy(szSend + nSendLen, &nExecMode, 4);
	nSendLen += 4;

	return SendModelData(szSend, nSendLen);
}

bool CSimuM_CComm::SendPauseModelCmd()
{
	int nSendLen = 0;
	char szSend[8192];
	unsigned long nCmd = M_C_CMD_PauseSimuModel;

	/*命令组包*/
	memcpy(szSend + nSendLen, &nCmd, 4);
	nSendLen += 4;

	return SendModelData(szSend, nSendLen);
}

bool CSimuM_CComm::SendResetModelCmd()
{
	int nSendLen = 0;
	char szSend[8192];
	unsigned long nCmd = M_C_CMD_ResetSimuModel;

	/*命令组包*/
	memcpy(szSend + nSendLen, &nCmd, 4);
	nSendLen += 4;

	return SendModelData(szSend, nSendLen);
}

int CSimuM_CComm::SendSetModelParamCmd(const vector<CommuSetModelParam_t *> *pVecParam)
{
	CommuSetModelParam_t *pSetParam = NULL;
	unsigned int nCurAdjParamNum = 0;
	int nSendLen = 0;
	char szSend[8192];
	unsigned long nCmd = M_C_CMD_SetModelParam;
	map<string,ModelAdjustParam_t *>::iterator iterFind;
	ModelAdjustParam_t *pAdjParam = NULL;
	char szData[1024*2];
	unsigned long nDataLen;
	vector<string> vecVal;

	memcpy(szSend, &nCmd, 4);
	nSendLen += 4;
	nSendLen += 4;
	for( unsigned int i = 0; i < pVecParam->size();i++){
		pSetParam = pVecParam->at(i);
		if( pSetParam->nModelID != m_nModelId)
			continue;

		nCurAdjParamNum++;
		iterFind = m_mapMdlParam.find((const char *)pSetParam->szParamName);
		if( iterFind == m_mapMdlParam.end())
			return -1;

		pAdjParam = iterFind->second;
		CUtil::StrToVector((const char *)pSetParam->szParamValue, ", ", vecVal);
		if( vecVal.size() != pAdjParam->nSize)
			return -1;

		nDataLen = ParseMdlSetParamValData(pAdjParam->nDataType, szData, vecVal);

		if( nDataLen != pAdjParam->nByteLen){
			return -1;
		}
		memcpy(szSend + nSendLen, &pAdjParam->nId, 4);
		nSendLen += 4;
		memcpy(szSend + nSendLen, &nDataLen, 4);
		nSendLen += 4;
		memcpy(szSend + nSendLen, szData, nDataLen);
		nSendLen += nDataLen;
	}
	if( nCurAdjParamNum <= 0) return 0;

	memcpy(szSend + 4, &nCurAdjParamNum, 4);
	SendModelData(szSend, nSendLen);

	return nCurAdjParamNum;
}

bool CSimuM_CComm::SendNonRTAdjustCmd(unsigned long ulAdjust)
{
	int nSendLen = 0;
	char szSend[8192];
	unsigned long nCmd = M_C_CMD_NonRealTimeRunAdust;

	/*命令组包*/
	memcpy(szSend + nSendLen, &nCmd, 4);
	nSendLen += 4;
	memcpy(szSend + nSendLen, &ulAdjust, 4);
	nSendLen += 4;

	return SendModelData(szSend, nSendLen);
}

bool CSimuM_CComm::SendModelData(char *pData, unsigned nDataLen)
{
	CommuSimuM_CShm_t *pShm = m_CommuEntity.pWriteShm;
	unsigned long nWriteIdx = 0;

	/*写数据到共享内存*/
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	pthread_mutex_lock(&pShm->mutex);
#else
	WaitForSingleObject(m_CommuEntity.hWriteMutex, INFINITE);
#endif
	if(pShm->ReadDataPtr == (pShm->WriteDataPtr+1)%MAX_MC_SHM_QUEUE_LEN){
		pShm->nCoverCount++;
	}
	nWriteIdx = pShm->WriteDataPtr%MAX_MC_SHM_QUEUE_LEN;

	memcpy(pShm->szData[nWriteIdx], pData, nDataLen);
	pShm->nDataLen[nWriteIdx] = nDataLen;

	pShm->WriteDataPtr++;

#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	pthread_mutex_unlock(&pShm->mutex);
#else
	ReleaseSemaphore(m_CommuEntity.hWriteMutex, 1, 0);
#endif
	
	return true;
}

/*未解析的数据*/
unsigned long CSimuM_CComm::ModelDataUnParseCount()
{
	unsigned long nCount = 0;

	EnterCriticalSection(&m_criRecordDataList);
	nCount += m_modelRecordDataList.size();
	LeaveCriticalSection(&m_criRecordDataList);

	EnterCriticalSection(&m_criDataList);
	nCount += m_modelDataList.size();
	LeaveCriticalSection(&m_criDataList);


//#ifdef QNX_PLATFORM
//	pthread_mutex_lock(&pReadShm->mutex);
//#else
//	WaitForSingleObject(m_CommuEntity.hReadMutex, INFINITE);
//#endif
//	if(m_CommuEntity.pReadShm->ReadDataPtr%MAX_MC_SHM_QUEUE_LEN != m_CommuEntity.pReadShm->WriteDataPtr%MAX_MC_SHM_QUEUE_LEN)
//	{
//#ifdef QNX_PLATFORM
//		pthread_mutex_unlock(&pReadShm->mutex);
//#else
//		ReleaseSemaphore(m_CommuEntity.hReadMutex, 1, 0);
//#endif
//		nCount += 1;
//	}

	return nCount;
}

//////////////////////////////////////////////////////////////////////////////////////////
//				内部接口
//////////////////////////////////////////////////////////////////////////////////////////

/*模型通信初始化*/
bool CSimuM_CComm::ModelCommuInit()
{
	HANDLE fd = INVALID_HANDLE_VALUE;
	CommuSimuM_CShm_t *pShm = NULL;
	char szMsg[256] = {0};

	/*通信路径*/
	sprintf(m_CommuEntity.szWriteShmPath, KL_COMMU_M_C_MREAD_SHM_NAME, m_nModelId);
	sprintf(m_CommuEntity.szReadShmPath, KL_COMMU_M_C_MWRITE_SHM_NAME, m_nModelId);
	sprintf(m_CommuEntity.szReadSemPath, KL_COMMU_M_C_MWRITE_SEM_NAME, m_nModelId);

#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	sem_t *pSem = SEM_FAILED;
	
	shm_unlink(m_CommuEntity.szWriteShmPath);
	shm_unlink(m_CommuEntity.szReadShmPath);
	sem_unlink(m_CommuEntity.szReadSemPath);

	/*写共享内存*/
	fd = shm_open(m_CommuEntity.szWriteShmPath, O_CREAT|O_EXCL|O_RDWR,S_IRWXU|S_IRWXG);
	if( fd == -1){
		sprintf(szMsg, "shm_open failed, errno:%d, path:%s \n", errno, m_CommuEntity.szWriteShmPath);
		m_szErrMsg = szMsg;
		return false;
	}
	if (ftruncate(fd, sizeof(CommuSimuM_CShm_t)) == -1){
		sprintf(szMsg, "ftruncate failed, errno:%d, path:%s \n", errno, m_CommuEntity.szWriteShmPath);
		m_szErrMsg = szMsg;
		return false;
	}

	pShm =(CommuSimuM_CShm_t *)mmap(0, sizeof(CommuSimuM_CShm_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if( pShm == NULL){
		sprintf(szMsg, "mmap failed, errno:%d, path:%s \n", errno, m_CommuEntity.szWriteShmPath);
		m_szErrMsg = szMsg;
		return false;
	}
	m_CommuEntity.nWriteShmfd = fd;
	m_CommuEntity.pWriteShm = pShm;

	/*读共享内存*/
	fd = shm_open(m_CommuEntity.szReadShmPath, O_CREAT|O_EXCL|O_RDWR,S_IRWXU|S_IRWXG);
	if( fd == -1){
		sprintf(szMsg, "shm_open failed, errno:%d, path:%s \n", errno, m_CommuEntity.szReadShmPath);
		m_szErrMsg = szMsg;
		return false;
	}
	if(ftruncate(fd, sizeof(CommuSimuM_CShm_t)) == -1){
		sprintf(szMsg, "ftruncate failed, errno:%d, path:%s \n", errno, m_CommuEntity.szWriteShmPath);
		m_szErrMsg = szMsg;
		return false;
	}

	pShm =(CommuSimuM_CShm_t *)mmap(0, sizeof(CommuSimuM_CShm_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if( pShm == NULL){
		sprintf(szMsg, "mmap failed, errno:%d, path:%s \n", errno, m_CommuEntity.szReadShmPath);
		m_szErrMsg = szMsg;
		return false;
	}
	m_CommuEntity.nReadShmfd = fd;
	m_CommuEntity.pReadShm = pShm;

	/*读内存信号量*/
	pSem = sem_open(m_CommuEntity.szReadSemPath, O_CREAT| O_EXCL , S_IRWXU|S_IRWXG, 0);
	if( pSem == SEM_FAILED){
		sprintf(szMsg, "sem_open failed, errno:%d, path:%s \n", errno, m_CommuEntity.szReadSemPath);
		m_szErrMsg = szMsg;
		return false;
	}
	m_CommuEntity.pReadSem = pSem;

#else
	char szReadLock[256];
	char szWriteLock[256];

	sprintf(szWriteLock, KL_COMMU_M_C_MREAD_SHM_LOCK_NAME, m_nModelId);
	sprintf(szReadLock, KL_COMMU_M_C_MWRITE_SHM_LOCK_NAME, m_nModelId);

	/*写共享内存*/
	fd = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0 ,sizeof(CommuSimuM_CShm_t), (LPCTSTR)m_CommuEntity.szWriteShmPath);
	if( GetLastError() == ERROR_ALREADY_EXISTS && fd != NULL){
		CloseHandle(fd);
		fd = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0 ,sizeof(CommuSimuM_CShm_t),(LPCTSTR)m_CommuEntity.szWriteShmPath);
		if( fd == INVALID_HANDLE_VALUE || fd == NULL){
			printf("CreateFileMapping file mapping failed, errno:%d, name:%s \n", GetLastError(), m_CommuEntity.szWriteShmPath);
			return false;
		}
	}else if( fd == NULL){
		printf("CreateFileMapping file mapping failed, errno:%d, name:%s \n", GetLastError(), m_CommuEntity.szWriteShmPath);
		return false;
	}

	pShm = (CommuSimuM_CShm_t *)MapViewOfFile(fd, FILE_MAP_ALL_ACCESS,0,0, sizeof(CommuSimuM_CShm_t));
	if(pShm == NULL){
		printf("MapViewOfFile failed, errno:%d, name:%s \n", GetLastError(), m_CommuEntity.szWriteShmPath);
		return false;
	}
	m_CommuEntity.nWriteShmfd = fd;
	m_CommuEntity.pWriteShm = pShm;

	fd = CreateSemaphore(NULL,1, 1,(LPCTSTR)szWriteLock);
	if( fd == INVALID_HANDLE_VALUE  || fd == NULL){
		printf("OpenSemaphore failed, errno:%d, name:%s \n", GetLastError(), szWriteLock);
		return false;
	}
	m_CommuEntity.hWriteMutex = fd;

	/*读共享内存*/
	fd = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0 ,sizeof(CommuSimuM_CShm_t), (LPCTSTR)m_CommuEntity.szReadShmPath);
	if( fd == INVALID_HANDLE_VALUE || fd == NULL){
		printf("open file mapping failed, errno:%d, name:%s \n", GetLastError(), m_CommuEntity.szReadShmPath);
		return false;
	}
	pShm = (CommuSimuM_CShm_t *)MapViewOfFile(fd, FILE_MAP_ALL_ACCESS,0,0, sizeof(CommuSimuM_CShm_t));
	if(pShm == NULL){
		printf("MapViewOfFile failed, errno:%d, name:%s \n", GetLastError(), m_CommuEntity.szReadShmPath);
		return false;
	}
	m_CommuEntity.nReadShmfd = fd;
	m_CommuEntity.pReadShm  = pShm;

	fd = CreateSemaphore(NULL,1, 1,(LPCTSTR)szReadLock);
	if( fd == INVALID_HANDLE_VALUE  || fd == NULL){
		printf("OpenSemaphore failed, errno:%d, name:%s \n", GetLastError(), szReadLock);
		return false;
	}
	m_CommuEntity.hReadMutex = fd;

	/*读内存信号量*/
	fd = CreateSemaphore(NULL,1, 1,(LPCTSTR)m_CommuEntity.szReadSemPath);
	if( fd == INVALID_HANDLE_VALUE || fd == NULL){
		printf("OpenSemaphore failed, errno:%d, name:%s \n", GetLastError(), m_CommuEntity.szReadSemPath);
		return false;
	}
	m_CommuEntity.hReadSem = fd;
#endif

	m_CommuEntity.pReadShm->ReadDataPtr = 0;
	m_CommuEntity.pReadShm->WriteDataPtr = 0;
	m_CommuEntity.pReadShm->nCoverCount = 0;

	m_CommuEntity.pWriteShm->ReadDataPtr = 0;
	m_CommuEntity.pWriteShm->WriteDataPtr = 0;
	m_CommuEntity.pWriteShm->nCoverCount = 0;

	return true;
}

/*模型通信反初始化*/
bool CSimuM_CComm::ModelCommuUninit()
{
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	munmap(m_CommuEntity.pReadShm, sizeof(CommuSimuM_CShm_t));
	munmap(m_CommuEntity.pWriteShm, sizeof(CommuSimuM_CShm_t));

	close(m_CommuEntity.nReadShmfd);
	close(m_CommuEntity.nWriteShmfd);

	shm_unlink(m_CommuEntity.szReadShmPath);
	shm_unlink(m_CommuEntity.szWriteShmPath);


	sem_close(m_CommuEntity.pReadSem);
	sem_unlink(m_CommuEntity.szReadSemPath);

#else
	UnmapViewOfFile(m_CommuEntity.pReadShm);
	UnmapViewOfFile(m_CommuEntity.pWriteShm);

	CloseHandle(m_CommuEntity.nReadShmfd);
	CloseHandle(m_CommuEntity.hReadMutex);
	CloseHandle(m_CommuEntity.nWriteShmfd);
	CloseHandle(m_CommuEntity.hWriteMutex);

	CloseHandle(m_CommuEntity.hReadSem);
#endif
	return true;
}

/*模型通信重置*/
bool CSimuM_CComm::ModelCommuReset()
{
	CommuSimuM_CShm_t *pShm = NULL;

	pShm = m_CommuEntity.pWriteShm;
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	pthread_mutex_lock(&pShm->mutex);
#else
	WaitForSingleObject(m_CommuEntity.hWriteMutex, INFINITE);
#endif
	pShm->ReadDataPtr = 0;
	pShm->WriteDataPtr = 0;
	pShm->nCoverCount = 0;

#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	pthread_mutex_unlock(&pShm->mutex);
#else
	ReleaseSemaphore(m_CommuEntity.hWriteMutex, 1, 0);
#endif

	pShm = m_CommuEntity.pReadShm;
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	pthread_mutex_lock(&pShm->mutex);
#else
	WaitForSingleObject(m_CommuEntity.hReadMutex, INFINITE);
#endif
	pShm->ReadDataPtr = 0;
	pShm->WriteDataPtr = 0;
	pShm->nCoverCount = 0;
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	pthread_mutex_unlock(&pShm->mutex);
#else
	ReleaseSemaphore(m_CommuEntity.hReadMutex, 1, 0);
#endif

	return true;
}

/*模型线程初始化*/
bool CSimuM_CComm::ModelThreadInit()
{
	DWORD tid1, tid2, tid3;

	/*线程数据*/
	InitializeCriticalSection(&m_criDataList);
	InitializeCriticalSection(&m_criRecordDataList);
	m_modelDataList.clear();
	m_modelRecordDataList.clear();

	/*接收数据线程*/
	m_bRecvQuitFlag = false;
	m_hRecvThread = ::CreateThread(NULL, 0, SimuModelDataRecvThread, this, 0, &tid1);
	if( m_hRecvThread == INVALID_HANDLE_VALUE )
	{
		m_szErrMsg = "create recv thread failed";
		return false;
	}

	/*解析数据线程*/
	m_hParseEvent = CreateEvent(NULL , TRUE , FALSE , NULL);
	m_hParseThread = ::CreateThread(NULL, 0, SimuModelDataParseThread, this, 0, &tid2);
	if( m_hParseThread == INVALID_HANDLE_VALUE )
	{
		CloseHandle(m_hRecvThread);
		CloseHandle(m_hParseEvent);
		m_szErrMsg = "create parse thread failed";
		return false;
	}

	/*解析存储数据线程*/
	m_hRecordParseEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hRecordParseThread = ::CreateThread(NULL, 0 , SimuModelRecordDataParseThread, this, 0 , &tid3);
	if( m_hRecordParseThread == INVALID_HANDLE_VALUE){
		CloseHandle(m_hRecordParseEvent);
		m_szErrMsg = "create record parse thread failed";
		return false;
	}

	return true;
}

/*模型线程反初始化*/
bool CSimuM_CComm::ModelThreadUninit()
{
	/*接收线程退出*/
	m_bRecvQuitFlag = true;

#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	sem_post(m_CommuEntity.pReadSem);
	usleep(10);
#else
	ReleaseSemaphore(m_CommuEntity.hReadSem, 1, 0);	
#endif

	CloseHandle(m_hRecvThread);


	/*解析线程退出*/
	SetEvent(m_hParseEvent);
	WaitForSingleObject(m_hParseThread , INFINITE);
	CloseHandle(m_hParseThread);

	/*数据清空*/
	SimuModelData_t *pMdlData = NULL;
	EnterCriticalSection(&m_criDataList);
	while(m_modelDataList.size() > 0)
	{
		pMdlData = m_modelDataList.back();
		m_modelDataList.pop_back();
		delete []pMdlData->pData;
		delete pMdlData;
	}
	m_modelDataList.clear();
	LeaveCriticalSection(&m_criDataList);

	DeleteCriticalSection(&m_criDataList);

	/*存储数据线程退出*/
	SetEvent(m_hRecordParseEvent);
	WaitForSingleObject(m_hRecordParseThread, INFINITE);
	CloseHandle(m_hRecordParseThread);

	/*存储数据清空*/
	SimuModelData_t *pMdlRecordData = NULL;
	EnterCriticalSection(&m_criRecordDataList);
	while( m_modelRecordDataList.size() > 0){
		pMdlRecordData = m_modelRecordDataList.back();
		m_modelRecordDataList.pop_back();
		delete []pMdlRecordData->pData;
		delete pMdlRecordData;
	}
	m_modelRecordDataList.clear();
	LeaveCriticalSection(&m_criRecordDataList);
	
	DeleteCriticalSection(&m_criRecordDataList);

	return true;
}

/*模型线程重置*/
bool CSimuM_CComm::ModelThreadReset()
{
	/*数据清空*/
	SimuModelData_t *pMdlData = NULL;
	EnterCriticalSection(&m_criDataList);
	while(m_modelDataList.size() > 0)
	{
		pMdlData = m_modelDataList.back();
		m_modelDataList.pop_back();
		delete []pMdlData->pData;
		delete pMdlData;
	}
	m_modelDataList.clear();
	LeaveCriticalSection(&m_criDataList);

	/*存储数据清空*/
	SimuModelData_t *pMdlRecordData = NULL;
	EnterCriticalSection(&m_criRecordDataList);
	while( m_modelRecordDataList.size() > 0){
		pMdlRecordData = m_modelRecordDataList.back();
		m_modelRecordDataList.pop_back();
		delete []pMdlRecordData->pData;
		delete pMdlRecordData;
	}
	m_modelRecordDataList.clear();
	LeaveCriticalSection(&m_criRecordDataList);

	return true;
}

/*模型配置初始化*/
bool CSimuM_CComm::ModelCfgInit()
{
	ConfigOutputSig_t *pCfgOut = NULL;
	ModelOutputData_t *pMdlOut = NULL;
	ConfigInterParam_t *pCfgParam = NULL;
	ModelAdjustParam_t *pMdlParam = NULL;
	const vector<ConfigOutputSig_t *> *pVecCfgOutput = NULL;
	const vector<ConfigInterParam_t *> *pVecCfgParam = NULL;

	pVecCfgOutput = m_pSimuCfg->GetOutputSig();
	pVecCfgParam = m_pSimuCfg->GetParams();

	m_vecOutputData.clear();
	m_mapMdlParam.clear();

	/*模型监视或录制的输出信号*/
	for( unsigned int i = 0; i < pVecCfgOutput->size(); i++){
		pCfgOut = pVecCfgOutput->at(i);
		if( (unsigned long)pCfgOut->nModelId != m_nModelId){
			continue;
		}
		if( !pCfgOut->bDisplay && !pCfgOut->bRecord){
			continue;
		}

		pMdlOut = new ModelOutputData_t;
		pMdlOut->nId = pCfgOut->nId;
		pMdlOut->nDataType = pCfgOut->eDataType;
		pMdlOut->nSize = pCfgOut->nVecSize;
		pMdlOut->bDisplay = pCfgOut->bDisplay;
		pMdlOut->bRecord = pCfgOut->bRecord;
		pMdlOut->nByteLen = GetDataTypeByteLen(pCfgOut->eDataType)*pCfgOut->nVecSize;

		m_vecOutputData.push_back(pMdlOut);
	}

	/*模型参数*/
	for( unsigned int i = 0; i < pVecCfgParam->size(); i++){
		pCfgParam = pVecCfgParam->at(i);
		if( (unsigned long)pCfgParam->nModelId != m_nModelId){
			continue;
		}
		pMdlParam = new ModelAdjustParam_t;
		pMdlParam->szName = pCfgParam->szName;
		pMdlParam->nId = pCfgParam->nId;
		pMdlParam->nDataType = pCfgParam->eDataType;
		pMdlParam->nSize = pCfgParam->nVecSize;
		pMdlParam->nByteLen = GetDataTypeByteLen(pCfgParam->eDataType)*pCfgParam->nVecSize;

		m_mapMdlParam[pMdlParam->szName] = pMdlParam;
	}
	return true;
}

/*模型配置反初始化*/
bool CSimuM_CComm::ModelCfgUninit()
{
	map<string,ModelAdjustParam_t *>::iterator iter;

	for(unsigned int i = 0; i < m_vecOutputData.size(); i++){
		delete m_vecOutputData.at(i);
	}
	m_vecOutputData.clear();

	for(iter = m_mapMdlParam.begin(); iter != m_mapMdlParam.end(); iter++){
		delete iter->second;
	}
	m_mapMdlParam.clear();

	return true;
}

/*模型时钟同步及模型数据同步初始化*/
bool CSimuM_CComm::ModelSyncInit()
{
	const vector<ConfigSimuModelInfo_t *> *pVecModel = NULL;
	ConfigSimuModelInfo_t *pModel = NULL;
	ConfigSimuModelInfo_t *pModel2 = NULL;
	char szShmName[256];
	char szSemName[256];
	char szLockName[128] = {0};

	pVecModel = m_pSimuCfg->GetModelInfo();

	/*时钟同步*/
	for(unsigned int i = 0; i < pVecModel->size(); i++){
		pModel = pVecModel->at(i);
		sprintf(szSemName, KL_COMMU_ClockM_M_sem_name, pModel->nId);

#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
		sem_unlink(szSemName);
#endif
	}

	/*数据同步*/
	ModelDataSyncEntity_t *pMdlSync = NULL;
	for( unsigned int i = 0; i < pVecModel->size(); i++){
		pModel = pVecModel->at(i);
		for( unsigned int j = 0; j < pVecModel->size(); j++){
			pModel2 = pVecModel->at(j);
			if( pModel->nId == pModel2->nId){
				continue;
			}
			sprintf(szShmName, KL_COMMU_DataM_M_shm_name, pModel->nId, pModel2->nId);

#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
			shm_unlink(szShmName);
#else
			sprintf(szLockName, KL_COMMU_DataM_M_shm_lock, pModel->nId, pModel2->nId);
#endif

			pMdlSync = new ModelDataSyncEntity_t;
			pMdlSync->szShmPath = szShmName;
			pMdlSync->pShm = NULL;
			pMdlSync->nShmfd = INVALID_HANDLE_VALUE;

#ifdef WIN_PLATFORM
			pMdlSync->szShmMutex = szLockName;
			pMdlSync->hShmMutex = INVALID_HANDLE_VALUE;
#endif
			m_vecDataSyncEntity.push_back(pMdlSync);
		}
	}
	for( unsigned int i = 0; i < m_vecDataSyncEntity.size(); i++)
	{
		pMdlSync = m_vecDataSyncEntity.at(i);

#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
		pMdlSync->nShmfd = shm_open(pMdlSync->szShmPath.c_str(), O_CREAT|O_EXCL|O_RDWR,S_IRWXU|S_IRWXG);
		if( pMdlSync->nShmfd == -1){
			return false;
		}
		if( ftruncate(pMdlSync->nShmfd, sizeof(CommuSimuM_MShm_t)) == -1){
			return false;
		}
		pMdlSync->pShm =(CommuSimuM_MShm_t *)mmap(0, sizeof(CommuSimuM_MShm_t), PROT_READ|PROT_WRITE, MAP_SHARED, pMdlSync->nShmfd, 0);
		if( pMdlSync->pShm == NULL){
			return false;
		}
#else
		pMdlSync->nShmfd = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,0,sizeof(CommuSimuM_MShm_t),(LPCTSTR)pMdlSync->szShmPath.c_str());
		if( pMdlSync->nShmfd == INVALID_HANDLE_VALUE || pMdlSync->nShmfd == NULL){
			return false;
		}
		pMdlSync->pShm = (CommuSimuM_MShm_t *)MapViewOfFile(pMdlSync->nShmfd, FILE_MAP_ALL_ACCESS,0,0, sizeof(CommuSimuM_MShm_t));
		if( pMdlSync->pShm == NULL){
			return false;
		}

		pMdlSync->hShmMutex = CreateSemaphore(NULL,1, 1,(LPCTSTR)pMdlSync->szShmMutex.c_str());;
		if( pMdlSync->hShmMutex == INVALID_HANDLE_VALUE || pMdlSync->hShmMutex == NULL){
			return false;
		}
#endif
		pMdlSync->pShm->ReadDataPtr = 0;
		pMdlSync->pShm->WriteDataPtr = 0;
		pMdlSync->pShm->nCoverCount = 0;
	}

	return true;
}

/*模型时钟同步及模型数据同步反初始化*/
bool CSimuM_CComm::ModelSyncUninit()
{
	const vector<ConfigSimuModelInfo_t *> *pVecModel = NULL;
	ConfigSimuModelInfo_t *pModel = NULL;

	pVecModel = m_pSimuCfg->GetModelInfo();

	/*时钟同步*/
	for(unsigned int i = 0; i < pVecModel->size(); i++)
	{
		pModel = pVecModel->at(i);
		char szSemName[128] = {0};
		sprintf(szSemName, KL_COMMU_ClockM_M_sem_name, pModel->nId);
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
		sem_unlink(szSemName);
#endif
	}

	/*数据同步*/
	ModelDataSyncEntity_t *pMdlSync = NULL;
	for( unsigned int i = 0; i < m_vecDataSyncEntity.size(); i++)
	{
		pMdlSync = m_vecDataSyncEntity.at(i);
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
		munmap(pMdlSync->pShm, sizeof(CommuSimuM_MShm_t));
		close(pMdlSync->nShmfd);
		shm_unlink(pMdlSync->szShmPath.c_str());
#else
		UnmapViewOfFile(pMdlSync->pShm);
		CloseHandle(pMdlSync->nShmfd);
		CloseHandle(pMdlSync->hShmMutex);
#endif
	}
	return true;
}

/*模型同步重置*/
bool CSimuM_CComm::ModelSyncReset()
{
	const vector<ConfigSimuModelInfo_t *> *pVecModel = NULL;
	ConfigSimuModelInfo_t *pModel = NULL;

	pVecModel = m_pSimuCfg->GetModelInfo();

	/*时钟同步*/
	for(unsigned int i = 0; i < pVecModel->size(); i++){
		pModel = pVecModel->at(i);
		char szSemName[128] = {0};
		sprintf(szSemName, KL_COMMU_ClockM_M_sem_name, pModel->nId);
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
		sem_unlink(szSemName);
#endif
	}

	/*数据同步*/
	ModelDataSyncEntity_t *pMdlSync = NULL;
	for( unsigned int i = 0; i < m_vecDataSyncEntity.size(); i++)
	{
		pMdlSync = m_vecDataSyncEntity.at(i);

#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
		pthread_mutex_lock(&pMdlSync->pShm->mutex);
#else
		WaitForSingleObject(pMdlSync->hShmMutex, INFINITE);
#endif
		pMdlSync->pShm->ReadDataPtr = 0;
		pMdlSync->pShm->WriteDataPtr = 0;

#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
		pthread_mutex_unlock(&pMdlSync->pShm->mutex);
#else
		ReleaseSemaphore(pMdlSync->hShmMutex, 1, 0);
#endif
	}

	return true;
}

/*解析接收数据*/
bool CSimuM_CComm::ParseRecvData(const SimuModelData_t *pMdlData)
{
	unsigned long ulCmdType;
	unsigned long nOffset = 0;
	char *pData = pMdlData->pData;

	/* 解析包头 */
	memcpy(&ulCmdType, pData + nOffset, 4);
	nOffset += 4;

	switch(ulCmdType){
	case M_C_CMD_RegisterModel:
	{/*注册模型*/
		ExecRegisterCmd(pMdlData);
	}
		break;
	case M_C_CMD_TestEventReport:
	{/*测试事件上报*/
		ExecTestEventReportCmd(pMdlData);
	}
		break;
	case M_C_CMD_ExceptEventReport:
	{/*异常事件上报*/
		ExecExceptEventReportCmd(pMdlData);
	}
		break;
	case M_C_CMD_DispUserData:
	{/*显示的用户数据*/
		ExecDispUserData(pMdlData);
	}
		break;
	case M_C_CMD_StorUserData:
	{/*存储的用户数据*/
		ExecStorUserData(pMdlData);
	}
		break;
	case M_C_CMD_DispAndStorData:
	{/*显示和存储的模型数据*/
		ExecDispAndStorData(pMdlData);
	}
		break;
	case M_C_CMD_StorModelData:
	{/*存储的模型数据*/
		ExecStorModelData(pMdlData);
	}
		break;
	case M_C_CMD_DispModelData:
	{/*显示的模型数据*/
		ExecDispModelData(pMdlData);
	}
		break;
	case M_C_CMD_BUFFERFULL:
	{/*缓冲区已满*/
		ExecExceptEventReportCmd(CEXCEPT_RepType_Error, "SimuModel Data Buffer is Full");
	}
	default:
		return false;
	}

	return true;
}

/*处理接收到的模型命令*/
bool CSimuM_CComm::ExecRegisterCmd(const SimuModelData_t *pMdlData)
{
	SetModelState(MODEL_RUN_State_loaded);
	return true;
}

bool CSimuM_CComm::ExecTestEventReportCmd(const SimuModelData_t *pMdlData)
{
	unsigned long ulCmdType;
	unsigned long nOffset = 0;
	char *pData = pMdlData->pData;

	unsigned long nStep = 0;
	unsigned long nTaskId = 0;
	unsigned long nCaseId = 0;

	// 解析
	nOffset += 4;
	memcpy(&nStep, pData + nOffset, 4);
	nOffset += 4;
	memcpy(&nTaskId, pData + nOffset, 4);
	nOffset += 4;
	memcpy(&nCaseId, pData + nOffset, 4);
	nOffset += 4;

	// 处理
	CommuTestEventReport_t report;
	report.nReportType = 0;
	report.nTestTaskId = nTaskId;
	report.nTestCaseId = nCaseId;
	report.nEventValueLen = 0;

	// 上报
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	CSimuProjectMgr::GetInstance()->TestEventReport(m_szProjId, &report);
#else
	CSimuProjectMgr::GetInstance()->TestEventReport(m_szProjId, &report);
#endif
	return true;
}

bool CSimuM_CComm::ExecExceptEventReportCmd(const SimuModelData_t *pMdlData)
{
	unsigned long ulCmdType;
	unsigned long nOffset = 0;
	char *pData = pMdlData->pData;

	unsigned long nStep = 0;
	unsigned long nLevel = 0;
	unsigned long nReportLen = 0;
	char szReport[1024];
	CommuExceptReport_t report;

	// 解析
	nOffset += 4;
	memcpy(&nStep, pData + nOffset, 4);
	nOffset += 4;
	memcpy(&nLevel, pData + nOffset, 4);
	nOffset += 4;
	memcpy(&nReportLen, pData + nOffset, 4);
	nOffset += 4;
	memcpy(szReport, pData + nOffset, nReportLen);
	nOffset += nReportLen;

	// 处理
	report.nReportType = nLevel;
	report.nReportLen = nReportLen;
	memset(report.szReportValue, 0x0, sizeof(report.szReportValue));
	memcpy(report.szReportValue, szReport, nReportLen);

	// 上报
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	CSimuProjectMgr::GetInstance()->ExceptReport(m_szProjId, &report);
#else
	CSimuProjectMgr::GetInstance()->ExceptReport(m_szProjId, &report);
#endif

	return true;
}

bool CSimuM_CComm::ExecExceptEventReportCmd(int nType, const char *szOut)
{
	unsigned long nOffset = 0;

	unsigned long nReportLen = 0;
	CommuExceptReport_t report;

	// 处理
	report.nReportType = nType;
	report.nReportLen = strlen(szOut);
	memset(report.szReportValue, 0x0, sizeof(report.szReportValue));
	memcpy(report.szReportValue, szOut, report.nReportLen);

	// 上报
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	CSimuProjectMgr::GetInstance()->ExceptReport(m_szProjId, &report);
#else
	CSimuProjectMgr::GetInstance()->ExceptReport(m_szProjId, &report);
#endif

	return true;
}

bool CSimuM_CComm::ExecDispModelData(const SimuModelData_t *pMdlData)
{
	unsigned long nOffset = 0;
	char *pData = pMdlData->pData;

	unsigned long nStep = 0;
	unsigned long nOverRun = 0;
	double userTime = 0;
	double totalTime = 0;
	unsigned long nModelDataLen = 0;
	char szModelData[8192];

	/*解析*/
	nOffset += 4;
	memcpy(&nStep, pData + nOffset, 4);
	nOffset += 4;
	memcpy(&nOverRun, pData + nOffset , 4);
	nOffset += 4;
	memcpy(&userTime, pData + nOffset, 8);
	nOffset += 8;
	memcpy(&totalTime, pData + nOffset, 8);
	nOffset += 8;
	memcpy(&nModelDataLen, pData + nOffset, 4);
	nOffset += 4;
	if( nModelDataLen > 0 ){
		memcpy(szModelData, pData + nOffset, nModelDataLen);
		nOffset += nModelDataLen;
	}

	//
	unsigned char szSend[8192];
	unsigned long nSendLen = 0;

	/*状态数据发送*/
	static int nStatusSendTick = 0;
	nStatusSendTick++;
	if( nStatusSendTick >= 2){
		CSimuDispDataC_DSimuStatusCmd *pCmd2 = new CSimuDispDataC_DSimuStatusCmd;
		pCmd2->SetModelID(m_nModelId);
		pCmd2->SetTimeStamp(pMdlData->ulTs1);
		pCmd2->SetData(nStep, nOverRun, userTime);

		nSendLen = pCmd2->Serialize(szSend);
		m_pDispDataComm->SendData(szSend, nSendLen);
		delete pCmd2;
		nStatusSendTick = 0;
	}

	/*监视数据发送*/
	if( nModelDataLen > 0 ){
		CSimuDispDataC_DSimuDataCmd *pCmd = new CSimuDispDataC_DSimuDataCmd;
		pCmd->SetModelID(m_nModelId);
		pCmd->SetTimeStamp(pMdlData->ulTs1);
		pCmd->SetStep(nStep);
		pCmd->SetData((unsigned char *)szModelData, nModelDataLen);

		nSendLen = pCmd->Serialize(szSend);
		m_pDispDataComm->SendData(szSend, nSendLen);
		delete pCmd;
	}

#if 0
	char szPrint[1024];
	static int sCount = 0;
	sCount++;
	if( sCount == 1 || sCount % 1000 == 0)
	{
		sprintf(szPrint, "Step:%d, OverRun:%d, userTime:%lf, totalTime:%lf Value=", nStep , nOverRun, userTime, totalTime);

		char szTmp[128];
		double dValue[32];
		memcpy(dValue, szModelData, nModelDataLen);
		for(unsigned i = 0; i< nModelDataLen/8; i++){
			sprintf(szTmp, "%.6lf-", dValue[i]);
			strcat(szPrint, szTmp);
		}
		strcat(szPrint, "\n");
		OutPrint(szPrint);
	}
#endif

	return true;
}

bool CSimuM_CComm::ExecDispUserData(const SimuModelData_t *pMdlData)
{
	unsigned long nOffset = 0;
	char *pData = pMdlData->pData;

	unsigned long nStep = 0;
	unsigned long nUserDataLen = 0;
	char szUserData[1024];

	/*解析*/
	nOffset += 4;
	memcpy(&nStep, pData + nOffset, 4);
	nOffset += 4;
	memcpy(&nUserDataLen, pData + nOffset, 4);
	nOffset += 4;
	memcpy(szUserData, pData + nOffset, nUserDataLen);
	nOffset += nUserDataLen;

	if( nUserDataLen > 1024)
		return false;

	/*发送*/
	CSimuDispDataC_DSLPrintCmd *pCmd = new CSimuDispDataC_DSLPrintCmd;
	pCmd->SetModelID(m_nModelId);
	pCmd->SetTimeStamp(pMdlData->ulTs1);
	pCmd->SetStep(nStep);
	pCmd->SetData((unsigned char *)szUserData, nUserDataLen);

	unsigned char szSend[8192];
	unsigned long nSendLen = 0;
	nSendLen = pCmd->Serialize(szSend);
	m_pDispDataComm->SendData(szSend, nSendLen);

#if 0
	/*打印输出*/
	char szPrint[1024];
	pCmd->Print(szPrint);
	OutPrint(szPrint);
#endif
	delete pCmd;
	return true;
}

bool CSimuM_CComm::ExecStorModelData(const SimuModelData_t *pMdlData)
{
	unsigned long nOffset = 0;
	char *pData = pMdlData->pData;

	unsigned long nStep = 0;
	unsigned long nGroupIdx = 0;
	unsigned long nModelDataLen = 0;
	char szModelData[8192];

	/*解析*/
	nOffset += 4;
	memcpy(&nStep, pData + nOffset, 4);
	nOffset += 4;
	memcpy(&nGroupIdx, pData + nOffset, 4);
	nOffset += 4;
	memcpy(&nModelDataLen, pData + nOffset, 4);
	nOffset += 4;

	/*存储数据发送*/
	CSimuStorDataC_DCmd *pCmd = new CSimuStorDataC_DCmd;
	pCmd->SetModelID(m_nModelId);
	pCmd->AddModelData(nStep, nGroupIdx, pMdlData->ulTs1,
			pMdlData->ulTs2, (unsigned char *)(pData + nOffset), nModelDataLen);

	unsigned char szSend[8192];
	unsigned long nSendLen = 0;
	nSendLen = pCmd->Serialize(szSend);
	m_pStorDataComm->SendData(szSend, nSendLen);
	delete pCmd;

	return true;
}

bool CSimuM_CComm::ExecStorUserData(const SimuModelData_t *pMdlData)
{
	unsigned long nOffset = 0;
	char *pData = pMdlData->pData;

	unsigned long nStep = 0;
	unsigned long nSaveFileLen = 0;
	char szSaveFile[256] = {0};
	unsigned long nUserDataLen = 0;
	char szUserData[1024] = {0};

	/*解析*/
	nOffset += 4;
	memcpy(&nStep, pData + nOffset, 4);
	nOffset += 4;
	memcpy(&nSaveFileLen, pData + nOffset, 4);
	nOffset += 4;
	memcpy(szSaveFile, pData + nOffset, nSaveFileLen);
	nOffset += 4;
	memcpy(&nUserDataLen, pData + nOffset, 4);
	nOffset += 4;
	memcpy(szUserData, pData + nOffset, nUserDataLen);
	nOffset += nUserDataLen;
	if( nUserDataLen > 1024){
		return false;
	}

	/*发送*/
	CSimuStorDataC_DCmd *pCmd = new CSimuStorDataC_DCmd;
	pCmd->SetModelID(m_nModelId);
	pCmd->AddUserData(nStep, szSaveFile,pMdlData->ulTs1,
			pMdlData->ulTs2, (unsigned char *)szUserData, nUserDataLen);

	unsigned char szSend[8192];
	unsigned long nSendLen = 0;
	nSendLen = pCmd->Serialize(szSend);
	m_pStorDataComm->SendData(szSend, nSendLen);

#if 0
	/*打印输出*/
	char szPrint[1024];
	pCmd->Print(szPrint);
	OutPrint(szPrint);
#endif

	delete pCmd;
	return true;
}

bool CSimuM_CComm::ExecDispAndStorData(const SimuModelData_t *pMdlData)
{
#if 0
	unsigned long nOffset = 0;
	char *pData = pMdlData->pData;

	unsigned long nStep = 0;
	unsigned long nOverRun = 0;
	double userTime = 0;
	double totalTime = 0;
	unsigned long nModelDataLen = 0;

	/*解析*/
	nOffset += 4;
	memcpy(&nStep, pData + nOffset, 4);
	nOffset += 4;
	memcpy(&nOverRun, pData + nOffset , 4);
	nOffset += 4;
	memcpy(&userTime, pData + nOffset, 8);
	nOffset += 8;
	memcpy(&totalTime, pData + nOffset, 8);
	nOffset += 8;
	memcpy(&nModelDataLen, pData + nOffset, 4);
	nOffset += 4;

	/*长度检查*/
	if( nModelDataLen != pMdlData->ulDataLen - nOffset){
		return false;
	}

	/*数据分离*/
	unsigned char szDispData[8192];
	unsigned long nDispDataLen = 0;
	unsigned char szStorData[8192];
	unsigned long nStorDataLen = 0;
	ModelOutputData_t *pMdlOutput = NULL;

	for( unsigned int i = 0; i < m_vecOutputData.size(); i++){
		pMdlOutput = m_vecOutputData.at(i);
		if( pMdlOutput->bDisplay && pMdlOutput->bRecord){/*监视和存储*/
			if( pMdlOutput->nByteLen > pMdlData->ulDataLen - nOffset){
				return false;
			}
			memcpy(szDispData + nDispDataLen, pData + nOffset, pMdlOutput->nByteLen);
			nDispDataLen += pMdlOutput->nByteLen;
			memcpy(szStorData + nStorDataLen, pData + nOffset, pMdlOutput->nByteLen);
			nStorDataLen += pMdlOutput->nByteLen;
			nOffset += pMdlOutput->nByteLen;
		}else if( pMdlOutput->bDisplay){/*仅监视*/
			if( pMdlOutput->nByteLen > pMdlData->ulDataLen - nOffset){
				return false;
			}
			memcpy(szDispData + nDispDataLen, pData + nOffset, pMdlOutput->nByteLen);
			nDispDataLen += pMdlOutput->nByteLen;
			nOffset += pMdlOutput->nByteLen;
		}else if( pMdlOutput->bRecord){/*仅存储*/
			if( pMdlOutput->nByteLen > pMdlData->ulDataLen - nOffset){
				return false;
			}
			memcpy(szStorData + nStorDataLen, pData + nOffset, pMdlOutput->nByteLen);
			nStorDataLen += pMdlOutput->nByteLen;
			nOffset += pMdlOutput->nByteLen;
		}
	}

	/*监视数据发送*/
	CSimuDispDataC_DSimuDataCmd *pCmd = new CSimuDispDataC_DSimuDataCmd;
	pCmd->SetModelID(m_nModelId);
	pCmd->SetTimeStamp(pMdlData->ulTs1);
	pCmd->SetStep(nStep);
	pCmd->SetData((unsigned char *)szDispData, nDispDataLen);

	unsigned char szSend[8192];
	unsigned long nSendLen = 0;
	nSendLen = pCmd->Serialize(szSend);
	m_pDispDataComm->SendData(szSend, nSendLen);
	delete pCmd;

	/*状态数据发送*/
	CSimuDispDataC_DSimuStatusCmd *pCmd2 = new CSimuDispDataC_DSimuStatusCmd;
	pCmd2->SetModelID(m_nModelId);
	pCmd2->SetTimeStamp(pMdlData->ulTs1);
	pCmd2->SetData(nStep, nOverRun, userTime);

	nSendLen = pCmd2->Serialize(szSend);
	m_pDispDataComm->SendData(szSend, nSendLen);
	delete pCmd2;


	char szPrint[1024];
	sprintf(szPrint, "Step:%d, OverRun:%d, userTime:%lf, totalTime:%lf\n", nStep , nOverRun, userTime, totalTime);
	OutPrint(szPrint);


	/*存储数据发送*/
	CSimuStorDataC_DCmd *pCmd3 = new CSimuStorDataC_DCmd;
	pCmd3->SetModelID(m_nModelId);
	pCmd3->AddData(nStep, SDATA_C_D_Type_ModelData,pMdlData->ulTs1,
			pMdlData->ulTs2, szStorData, nStorDataLen);

	nSendLen = pCmd3->Serialize(szSend);
	m_pStorDataComm->SendData(szSend, nSendLen);
	delete pCmd3;
#endif
	return true;
}

int CSimuM_CComm::ExecRecordData(const SimuModelData_t *pMdlData, unsigned char *pszBuffer, unsigned long &ulBuffDataOff, bool bForceSendFlag)
{
	unsigned long ulCmdType;
	unsigned long nOffset = 0;
	char *pUnitData = NULL;
	unsigned char szUnitData[8192];
	unsigned long ulUnitLen = 0;
	CSimuStorDataC_DCmd *pCmd = NULL;

	// 获取命令类型
	pUnitData = pMdlData->pData;;
	nOffset = 0;
	memcpy(&ulCmdType, pUnitData + nOffset, 4);
	nOffset += 4;

	// 解析命令封包
	if( ulCmdType == M_C_CMD_StorModelData){
		unsigned long nStep = 0;
		unsigned long nGroupIdx = 0;
		unsigned long nModelDataLen = 0;

		memcpy(&nStep, pUnitData + nOffset, 4);
		nOffset += 4;
		memcpy(&nGroupIdx, pUnitData + nOffset, 4);
		nOffset += 4;
		memcpy(&nModelDataLen, pUnitData + nOffset, 4);
		nOffset += 4;

		pCmd = new CSimuStorDataC_DCmd;
		pCmd->SetModelID(m_nModelId);
		pCmd->AddModelData(nStep, nGroupIdx, pMdlData->ulTs1,pMdlData->ulTs2,
			(unsigned char *)(pUnitData + nOffset), nModelDataLen);
	}else if( ulCmdType == M_C_CMD_StorUserData){
		unsigned long nStep = 0;
		unsigned long nUserDataLen = 0;
		unsigned long nFileLen = 0;
		char szFileName[256] = {0};
		char szUserData[1024] = {0};

		/*解析*/
		memcpy(&nStep, pUnitData + nOffset, 4);
		nOffset += 4;
		memcpy(&nFileLen, pUnitData + nOffset, 4);
		nOffset += 4;
		memcpy(szFileName, pUnitData + nOffset, nFileLen);
		nOffset += nFileLen;
		memcpy(&nUserDataLen, pUnitData + nOffset, 4);
		nOffset += 4;
		memcpy(szUserData, pUnitData + nOffset, nUserDataLen);
		nOffset += nUserDataLen;

		pCmd = new CSimuStorDataC_DCmd;
		pCmd->SetModelID(m_nModelId);
		pCmd->AddUserData(nStep, szFileName, pMdlData->ulTs1, pMdlData->ulTs2,
			(unsigned char *)szUserData, nUserDataLen);
	}

	// 单元数据
	ulUnitLen = pCmd->Serialize(szUnitData);
	delete pCmd;

	// 发送
	if( ulUnitLen + ulBuffDataOff > KL_COMMU_STOR_DATA_SEND_MAX_DATA_UNIT){
		/*临时缓存区已满，发送失败，故解析失败*/
		if( m_pStorDataComm->SendData(pszBuffer, ulBuffDataOff) <= 0){
			return -1;
		}
		ulBuffDataOff = 0;
	}

	memcpy(pszBuffer+ulBuffDataOff, szUnitData, ulUnitLen);
	ulBuffDataOff += ulUnitLen;

	if(bForceSendFlag){
		/*解析成功，但发送失败*/
		if( m_pStorDataComm->SendData(pszBuffer, ulBuffDataOff) <= 0){
			return 0;
		}
		ulBuffDataOff = 0;
	} 
	return ulUnitLen;
}

/*打印输出*/
void CSimuM_CComm::OutPrint(char *szMsg)
{
#if 0
	if( m_fpOutPrint == NULL){
		char szFile[128];
		time_t totalSec;

		totalSec = time(NULL);
#ifdef QNX_PLATFORM
		sprintf(szFile,"/home/ntuser/GuardPrint_%ld_%ld.txt", m_nModelId, totalSec);
#else
		sprintf(szFile,"SimuControlPrint_%ld_%ld.txt", m_nModelId, totalSec);
#endif
		m_fpOutPrint = fopen(szFile, "w");
	}

	if( m_fpOutPrint != NULL){
		fprintf(m_fpOutPrint, "%s\n",szMsg);
		fflush(m_fpOutPrint);
	}
#endif
}
