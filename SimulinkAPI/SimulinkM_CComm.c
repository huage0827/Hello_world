#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#if defined(QNX_PLATFORM)
#include <sys/neutrino.h>

#elif defined(LINUX_PLATFORM)
#include <sys/types.h>
#include <sys/stat.h>

#else

#endif

#include "SimulinkM_CCmd.h"
#include "SimulinkM_CComm.h"
#include "SimulinkModelDef.h"
#include "SimulinkCommuDef.h"


/******************************************************************
 *   本地全局变量定义
 ******************************************************************/
typedef struct{
	char bInit;
	int nDispSigNum;
	int nRecordSigNum;
	int nParamNum;
	ModelOutputData_t *pDispSigList;
	ModelOutputData_t *pRecordSigList;
	ModelAdjustParam_t *pParamList;
	void *pBlockSignalAddr;
	void *pBlockParamAddr;
	int nReadFd;
	int nWriteFd;
	CommuSimuM_CShm_t *pReadShm;
	CommuSimuM_CShm_t *pWriteShm;
	sem_t *pWriteSem;
}SimulinkM_CComm;

SimulinkM_CComm g_SimuM_CComm;

/******************************************************************
 * 说明 : M_C内部静态函数
 ******************************************************************/
static int SendCmdToSimuGuard(const char *szSend, unsigned long nSendLen);
static int ParseRecvSimuGuardCmd(const char *pData, unsigned long nDataLen);

int SendCmdToSimuGuard(const char *szSend, unsigned long nSendLen)
{
	int nWriteIdx = 0;

	/*写数据到共享内存*/
	pthread_mutex_lock(&g_SimuM_CComm.pWriteShm->mutex);
	if(g_SimuM_CComm.pWriteShm->ReadDataPtr == (g_SimuM_CComm.pWriteShm->WriteDataPtr+1)%MAX_MC_SHM_QUEUE_LEN){
		g_SimuM_CComm.pWriteShm->nCoverCount = 1;
		pthread_mutex_unlock(&g_SimuM_CComm.pWriteShm->mutex);
		return 0;
	}
	nWriteIdx = g_SimuM_CComm.pWriteShm->WriteDataPtr%MAX_MC_SHM_QUEUE_LEN;

	memcpy(g_SimuM_CComm.pWriteShm->szData[nWriteIdx], szSend, nSendLen);
	g_SimuM_CComm.pWriteShm->nDataLen[nWriteIdx] = nSendLen;

	g_SimuM_CComm.pWriteShm->WriteDataPtr++;
	g_SimuM_CComm.pWriteShm->WriteDataPtr %= MAX_MC_SHM_QUEUE_LEN;

	pthread_mutex_unlock(&g_SimuM_CComm.pWriteShm->mutex);

	/*发送写数据信号*/
	sem_post(g_SimuM_CComm.pWriteSem);
	return nSendLen;
}

int ParseRecvSimuGuardCmd(const char *pData, unsigned long nDataLen)
{
	unsigned long ulCmdType;
	unsigned long nRemainLen;
	unsigned long nOffset = 0;

	/* 解析包头 */
	memcpy(&ulCmdType, pData + nOffset, 4);
	nOffset += 4;

	switch(ulCmdType){
	case M_C_CMD_ExecSimuModel:
	{/*处理执行仿真模型命令*/
		unsigned long ulExecMode = 0;
		memcpy(&ulExecMode, pData + nOffset, 4);
		g_SimuModel.bPause = 0;
		printf("Recv Exec Cmd \n");
	}
		break;
	case M_C_CMD_PauseSimuModel:
	{/*处理暂停仿真模型命令*/
		g_SimuModel.bPause = 1;
		printf("Recv Pause Cmd \n");
	}
		break;
	case M_C_CMD_ResetSimuModel:
	{/*处理重置仿真模型命令*/
		g_SimuModel.bQuit = 1;
		printf("Recv Quit Cmd \n");
	}
		break;
	case M_C_CMD_SetModelParam:
	{/*处理动态调参命令*/
		ModelAdjustParam_t *pInteParam = NULL;
		unsigned long nParamCount;
		unsigned long nParamId;
		unsigned long nParamValLen;
		unsigned char szParamVal[1024];
		unsigned i,idx = 0;
		char bFind = 0;
		double *pVarAddr = (double *)g_SimuM_CComm.pBlockParamAddr;

		memcpy(&nParamCount, pData + nOffset, 4);
		nOffset += 4;

		if( nParamCount < 0 || nParamCount > 100 )
			return -1;

		for( i = 0; i < nParamCount; i++)
		{
			nRemainLen = nDataLen - nOffset;
			if( nRemainLen <= 9)
				return -1;

			/*读取调参信息*/
			memcpy(&nParamId, pData + nOffset, 4);
			nOffset += 4;
			memcpy(&nParamValLen, pData + nOffset, 4);
			nOffset += 4;
			memcpy(szParamVal, pData + nOffset, nParamValLen);
			nOffset += nParamValLen;

			/*查找对应参数地址*/
			bFind = 0;
			for(idx = 0; idx < g_SimuM_CComm.nParamNum; idx++){
				if( g_SimuM_CComm.pParamList[idx].nID == nParamId){
					bFind = 1;
					break;
				}
			}
			if( nParamValLen != g_SimuM_CComm.pParamList[idx].nLen * 8)
				return -1;

			/*动态修改变量参数值 */
			if( bFind && idx < g_SimuM_CComm.nParamNum){
				memcpy(pVarAddr + g_SimuM_CComm.pParamList[idx].nStartIdx,
						szParamVal, g_SimuM_CComm.pParamList[idx].nLen * 8);
			}
		}
	}
		break;
	default:
		return -1;
	}
	return 0;
}

/******************************************************************
 * 说明 : M_C接口函数实现
 ******************************************************************/
int SL_InitM_CComm()
{
	char szTmpPath[256];
	int fd = -1;
	CommuSimuM_CShm_t *pShm = NULL;
	sem_t *pSem = SEM_FAILED;

	g_SimuM_CComm.bInit = 0;
	// 读内存
	sprintf(szTmpPath, KL_COMMU_M_C_MREAD_SHM_NAME, g_SimuModel.nID);
	fd = shm_open(szTmpPath, O_RDWR, S_IRWXU);
	if( fd < 0){
		return -1;
	}
	pShm = (CommuSimuM_CShm_t *)mmap(0, sizeof(CommuSimuM_CShm_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if( pShm == NULL){
		return -1;
	}
	g_SimuM_CComm.nReadFd = fd;
	g_SimuM_CComm.pReadShm = pShm;

	// 写内存
	sprintf(szTmpPath, KL_COMMU_M_C_MWRITE_SHM_NAME, g_SimuModel.nID);
	fd = shm_open(szTmpPath, O_RDWR, S_IRWXU);
	if( fd < 0){
		return -1;
	}
	pShm = (CommuSimuM_CShm_t *)mmap(0, sizeof(CommuSimuM_CShm_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if( pShm == NULL){
		return -1;
	}
	g_SimuM_CComm.nWriteFd = fd;
	g_SimuM_CComm.pWriteShm = pShm;

	// 写信号
	sprintf(szTmpPath, KL_COMMU_M_C_MWRITE_SEM_NAME, g_SimuModel.nID);
	pSem = sem_open(szTmpPath,0);
	if( pSem == SEM_FAILED){
		return -1;
	}
	g_SimuM_CComm.pWriteSem = pSem;

	// 初始化完成
	g_SimuM_CComm.bInit = 1;

	return 0;
}

void SL_UninitM_CComm()
{
	munmap(g_SimuM_CComm.pReadShm, sizeof(CommuSimuM_CShm_t));
	close(g_SimuM_CComm.nReadFd);

	munmap(g_SimuM_CComm.pWriteShm, sizeof(CommuSimuM_CShm_t));
	close(g_SimuM_CComm.nWriteFd);

	sem_close(g_SimuM_CComm.pWriteSem);
}

int SL_SetModelOutput(int numSig, double *pValAddr)
{
	int idx = 0;
	int nDispSigNum = 0;
	int nRecordSigNum = 0;
	ModelOutputData_t *pOutput = NULL;
	SimulinkModelCfg_t *pSimuCfg = NULL;

	pSimuCfg = &g_SimuCfg.curModel;
	g_SimuM_CComm.pBlockSignalAddr = pValAddr;
	for(idx = 0; idx < pSimuCfg->outputNum; idx++){
		if( pSimuCfg->plstOutput[idx].bDisp)
			nDispSigNum++;
		if( pSimuCfg->plstOutput[idx].bRecord)
			nRecordSigNum++;
	}
	g_SimuM_CComm.nDispSigNum = nDispSigNum;
	g_SimuM_CComm.nRecordSigNum = nRecordSigNum;
	if( nDispSigNum > 0){
		pOutput = (ModelOutputData_t *)calloc(nDispSigNum, sizeof(ModelOutputData_t));
		if( pOutput == NULL ){
			return -1;
		}
		g_SimuM_CComm.pDispSigList = pOutput;
	}
	if( nRecordSigNum > 0){
		pOutput = (ModelOutputData_t *)calloc(nRecordSigNum, sizeof(ModelOutputData_t));
		if( pOutput == NULL ){
			return -1;
		}
		g_SimuM_CComm.pRecordSigList = pOutput;
	}

	nDispSigNum = 0;
	nRecordSigNum = 0;
	for(idx = 0; idx < pSimuCfg->outputNum; idx++){
		if( pSimuCfg->plstOutput[idx].bDisp){
			g_SimuM_CComm.pDispSigList[nDispSigNum].nID = pSimuCfg->plstOutput[idx].id;
			g_SimuM_CComm.pDispSigList[nDispSigNum].nLen = pSimuCfg->plstOutput[idx].width;
			g_SimuM_CComm.pDispSigList[nDispSigNum].nStartIdx = pSimuCfg->plstOutput[idx].startIdx;
			g_SimuM_CComm.pDispSigList[nDispSigNum].nNO = pSimuCfg->plstOutput[idx].no;
			nDispSigNum++;
		}
		if( pSimuCfg->plstOutput[idx].bRecord){
			g_SimuM_CComm.pRecordSigList[nRecordSigNum].nID = pSimuCfg->plstOutput[idx].id;
			g_SimuM_CComm.pRecordSigList[nRecordSigNum].nLen = pSimuCfg->plstOutput[idx].width;
			g_SimuM_CComm.pRecordSigList[nRecordSigNum].nStartIdx = pSimuCfg->plstOutput[idx].startIdx;
			g_SimuM_CComm.pRecordSigList[nRecordSigNum].nNO = pSimuCfg->plstOutput[idx].no;
			nRecordSigNum++;
		}
	}
	return 0;
}

int SL_SetModelParam(int numParam, double *pParamAddr)
{
	int nParam = 0;
	int i = 0;
	ModelAdjustParam_t *pParam = NULL;
	SimulinkModelCfg_t *pSimuCfg = NULL;

	pSimuCfg = &g_SimuCfg.curModel;
	nParam = pSimuCfg->paramNum;
	g_SimuM_CComm.pBlockParamAddr = pParamAddr;

	if( nParam > 0){
		pParam = (ModelAdjustParam_t *)calloc(nParam, sizeof(ModelAdjustParam_t));
		if( pParam == NULL){
			return -1;
		}
		for(i = 0; i < pSimuCfg->paramNum; i++){
			pParam[i].nID = pSimuCfg->plstParam[i].id;
			pParam[i].nLen = pSimuCfg->plstParam[i].width;
			pParam[i].nNO = pSimuCfg->plstParam[i].no;
			pParam[i].nStartIdx = pSimuCfg->plstParam[i].startIdx;
		}
		g_SimuM_CComm.nParamNum = nParam;
		g_SimuM_CComm.pParamList = pParam;
	}
	return 0;
}

int SL_SendRegisterModelCmd()
{
	int nSendLen = 0;
	char szSend[8192];
	unsigned long nCmd = M_C_CMD_RegisterModel;

	/*命令组包*/
	memcpy(szSend, &nCmd, 4);
	nSendLen += 4;

	return SendCmdToSimuGuard(szSend, nSendLen);
}

int SL_SendExceptReportCmd(unsigned long nStep, int level, const char *szData)
{
	int nSendLen = 0;
	char szSend[8192];
	unsigned long nCmd = M_C_CMD_ExceptEventReport;
	int nDataLen = strlen(szData);

	printf("Execept: %s \n",szData);

	/*命令组包*/
	memcpy(szSend + nSendLen, &nCmd, 4);
	nSendLen += 4;

	memcpy(szSend + nSendLen, &nStep, 4);
	nSendLen += 4;
	memcpy(szSend + nSendLen, &level, 4);
	nSendLen += 4;
	memcpy(szSend + nSendLen, &nDataLen, 4);
	nSendLen += 4;
	memcpy(szSend + nSendLen, szData, nDataLen);
	nSendLen += nDataLen;

	return SendCmdToSimuGuard(szSend, nSendLen);
}

int SL_SendRecordDataCmd(unsigned long nStep)
{
	int nSendLen = 0;
	char szSend[8192];
	unsigned long nCmd;
	ModelOutputData_t *pOutputData = NULL;
	int nRecordGroupId = 1;
	unsigned long nModelDataLen = 0;
	char szModelData[2048];
	int idx = 0;
	double *pVarAddr = (double*)g_SimuM_CComm.pBlockSignalAddr;

	if( g_SimuM_CComm.nRecordSigNum <= 0){
		return 0;
	}

	nCmd = M_C_CMD_StorModelData;/*录制数据*/
	for( idx = 0; idx < g_SimuM_CComm.nRecordSigNum; idx++){
		memcpy(szModelData + nModelDataLen, pVarAddr + g_SimuM_CComm.pParamList[idx].nStartIdx,
				g_SimuM_CComm.pParamList[idx].nLen * 8);
		nModelDataLen += g_SimuM_CComm.pParamList[idx].nLen * 8;
	}

	/*命令组包->帧头*/
	memcpy(szSend + nSendLen, &nCmd, 4);
	nSendLen += 4;

	memcpy(szSend + nSendLen, &nStep, 4);
	nSendLen += 4;
	memcpy(szSend + nSendLen, &nRecordGroupId, 4);
	nSendLen += 4;
	memcpy(szSend + nSendLen, &nModelDataLen, 4);
	nSendLen += 4;
	memcpy(szSend + nSendLen, szModelData, nModelDataLen);
	nSendLen += nModelDataLen;

	return SendCmdToSimuGuard(szSend, nSendLen);
}

int SL_SendDispDataCmd(unsigned long nStep, unsigned long nOverRun, double ExecTime, double AllTime)
{
	int nSendLen = 0;
	char szSend[8192];
	unsigned long nModelDataLen = 0;
	char szModelData[8192];
	unsigned long nCmd;
	int i = 0;
	double *pVarAddr = (double*)g_SimuM_CComm.pBlockSignalAddr;

	nCmd = M_C_CMD_DispModelData;/*仅显示数据*/
	for( i = 0; i < g_SimuM_CComm.nDispSigNum; i++){
		memcpy(szModelData + nModelDataLen, pVarAddr + g_SimuM_CComm.pDispSigList[i].nStartIdx,
				g_SimuM_CComm.pDispSigList[i].nLen * 8);
		nModelDataLen += g_SimuM_CComm.pDispSigList[i].nLen * 8;
	}

	/*命令组包->帧头*/
	memcpy(szSend + nSendLen, &nCmd, 4);
	nSendLen += 4;

	memcpy(szSend + nSendLen, &nStep, 4);
	nSendLen += 4;
	memcpy(szSend + nSendLen, &nOverRun, 4);
	nSendLen += 4;
	memcpy(szSend + nSendLen, &ExecTime, 8);
	nSendLen += 8;
	memcpy(szSend + nSendLen, &AllTime, 8);
	nSendLen += 8;
	memcpy(szSend + nSendLen, &nModelDataLen, 4);
	nSendLen += 4;
	if( nModelDataLen > 0){
		memcpy(szSend + nSendLen, szModelData, nModelDataLen);
		nSendLen += nModelDataLen;
	}
	return SendCmdToSimuGuard(szSend, nSendLen);
}

int SL_RecvControlCmd()
{
	int nCount = 0;
	char szData[MAX_MC_SHM_BUFF_LEN];
	unsigned long nDataLen = 0;
	int nReadIdx = 0;

	do{
		/*检测是否有命令 */
		pthread_mutex_lock(&g_SimuM_CComm.pReadShm->mutex);
		if(g_SimuM_CComm.pReadShm->ReadDataPtr%MAX_MC_SHM_QUEUE_LEN == g_SimuM_CComm.pReadShm->WriteDataPtr%MAX_MC_SHM_QUEUE_LEN){
			pthread_mutex_unlock(&g_SimuM_CComm.pReadShm->mutex);
			break;
		}

		/*取命令*/
		nReadIdx = g_SimuM_CComm.pReadShm->ReadDataPtr%MAX_MC_SHM_QUEUE_LEN;
		memcpy(szData, g_SimuM_CComm.pReadShm->szData[nReadIdx], g_SimuM_CComm.pReadShm->nDataLen[nReadIdx]);
		nDataLen = g_SimuM_CComm.pReadShm->nDataLen[nReadIdx];

		/*移动读指针*/
		g_SimuM_CComm.pReadShm->ReadDataPtr += 1;
		g_SimuM_CComm.pReadShm->ReadDataPtr %= MAX_MC_SHM_QUEUE_LEN;

		pthread_mutex_unlock(&g_SimuM_CComm.pReadShm->mutex);

		/*解析处理*/
		ParseRecvSimuGuardCmd(szData, nDataLen);
		nCount++;
	}while(1);
	return nCount;
}
