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



#include "SimulinkDataSync.h"
#include "SimulinkCommuDef.h"
#include "MyList.h"

/******************************************************************
 *   本地全局变量定义
 ******************************************************************/
typedef struct{
	int nKey;
	int nAssModelId;
	int nDoubleNum;
	int nShmfd;
	int bShmInit;
	int nCount;
	char szShmName[128];
	CommuSimuM_MShm_t *pM_MShm;
}SimulinkDataSyncNode_t;

int g_bInitFlag = 0;
MyList_t glstInSync;
MyList_t glstOutSync;

/******************************************************************
 *   静态接口声明实现
 ******************************************************************/
static int SimuDataInputSyncInit(int nModelId, SimulinkInputCfg_t *pInCfg, SimulinkDataSyncNode_t *pSync);
static int SimuDataOutputSyncInit(int nModelId, SimulinkOutputCfg_t *pInCfg, SimulinkDataSyncNode_t *pSync);
static void FreeSyncNode(void *pNodeData);
static void PrintSyncNode(void *pNodeData);

int SimuDataInputSyncInit(int nModelId, SimulinkInputCfg_t *pInCfg, SimulinkDataSyncNode_t *pSync)
{
	pSync->bShmInit = 0;
	pSync->nAssModelId = pInCfg->modelId;
	pSync->nDoubleNum = pInCfg->width;
	pSync->nCount = 0;
	pSync->nKey = pInCfg->key;
	pSync->nShmfd = -1;
	sprintf(pSync->szShmName, KL_COMMU_DataM_M_shm_name, pInCfg->modelId, nModelId);

	pSync->nShmfd = shm_open(pSync->szShmName, O_RDWR, S_IRWXU);
	if( pSync->nShmfd == -1){
		return -1;
	}
	pSync->pM_MShm =(CommuSimuM_MShm_t *)mmap(0, sizeof(CommuSimuM_MShm_t),
			PROT_READ|PROT_WRITE, MAP_SHARED, pSync->nShmfd, 0);
	if( pSync->pM_MShm == NULL){
		return -1;
	}
	pSync->bShmInit = 1;
	return 0;
}

int SimuDataOutputSyncInit(int nModelId, SimulinkOutputCfg_t *pOutCfg, SimulinkDataSyncNode_t *pSync)
{
	pSync->bShmInit = 0;
	pSync->nAssModelId = pOutCfg->modelId;
	pSync->nDoubleNum = pOutCfg->width;
	pSync->nCount = 0;
	pSync->nKey = pOutCfg->key;
	pSync->nShmfd = -1;
	sprintf(pSync->szShmName, KL_COMMU_DataM_M_shm_name, nModelId, pOutCfg->modelId);

	pSync->nShmfd = shm_open(pSync->szShmName, O_RDWR, S_IRWXU);
	if( pSync->nShmfd == -1){
		return -1;
	}
	pSync->pM_MShm =(CommuSimuM_MShm_t *)mmap(0, sizeof(CommuSimuM_MShm_t),
			PROT_READ|PROT_WRITE, MAP_SHARED, pSync->nShmfd, 0);
	if( pSync->pM_MShm == NULL){
		return -1;
	}
	pSync->bShmInit = 1;
	return 0;
}

void FreeSyncNode(void *pNodeData)
{
	SimulinkDataSyncNode_t *pSync = (SimulinkDataSyncNode_t *)pNodeData;
	munmap(pSync->pM_MShm, sizeof(CommuSimuM_MShm_t));
	close(pSync->nShmfd);
	free(pSync);
}

void PrintSyncNode(void *pNodeData)
{
	SimulinkDataSyncNode_t *pSync = (SimulinkDataSyncNode_t *)pNodeData;
	printf("nKey=%d, nAssModeId=%d,nDouNum=%d,nShFd=%d, bInit:%d,nCount:%d, szShmName=%s \n",
			pSync->nKey, pSync->nAssModelId, pSync->nDoubleNum, pSync->nShmfd, pSync->bShmInit,
			pSync->nCount, pSync->szShmName);
}

/******************************************************************
 *   外部接口实现
 ******************************************************************/
int SL_InitDataSync(const SimulinkConfig_t *pSimuCfg)
{
	int idx = 0;
	int bFind = 0;
	SimulinkDataSyncNode_t *pSync= NULL;
	SimulinkInputCfg_t *pInCfg = NULL;
	SimulinkOutputCfg_t *pOutCfg = NULL;
	MyListNode_t *pNode = NULL;

	if( g_bInitFlag != 0){
		return -1;
	}
	if( MyList_Create(&glstInSync) != 0){
		return -1;
	}
	if( MyList_Create(&glstOutSync) != 0){
		return -1;
	}

	// Create Input
	for(idx = 0; idx < pSimuCfg->curModel.inputNum; idx++){
		pInCfg = &pSimuCfg->curModel.plstInput[idx];
		pNode = MyList_Get(&glstInSync, pInCfg->key);
		if( pNode != NULL){
			return -1;
		}
		pSync = (SimulinkDataSyncNode_t *)malloc(sizeof(SimulinkDataSyncNode_t));
		if( pSync == NULL)return -1;
		if( SimuDataInputSyncInit(pSimuCfg->curModel.id, pInCfg, pSync) != 0){
			return -1;
		}
		if( MyList_Add(&glstInSync, pInCfg->key, (void *)pSync) != 0){
			return -1;
		}
	}

	// Create Output
	for(idx = 0; idx < pSimuCfg->curModel.outputNum; idx++){
		pOutCfg = &pSimuCfg->curModel.plstOutput[idx];
		if( pOutCfg->bSync != 1)continue;

		pNode = MyList_Get(&glstOutSync, pOutCfg->key);
		if( pNode != NULL){
			return -1;
		}
		pSync = (SimulinkDataSyncNode_t *)malloc(sizeof(SimulinkDataSyncNode_t));
		if( pSync == NULL)return -1;
		if( SimuDataOutputSyncInit(pSimuCfg->curModel.id, pOutCfg, pSync) != 0){
			return -1;
		}
		if( MyList_Add(&glstOutSync, pOutCfg->key, (void*)pSync) != 0){
			return -1;
		}
	}

	// Print
	printf(">>>> glistInSync <<<<<<<\n");
	MyList_Print(&glstInSync, PrintSyncNode);
	
	printf(">>>> glstOutSync <<<<<<<\n");
	MyList_Print(&glstOutSync, PrintSyncNode);

	g_bInitFlag = 1;
	return 0;
}

int SL_UninitDataSync()
{
	if( g_bInitFlag != 1){
		return -1;
	}
	MyList_Destory(&glstInSync, FreeSyncNode);
	MyList_Destory(&glstOutSync, FreeSyncNode);
	g_bInitFlag = 0;
	return 0;
}

int SL_RecvSyncData(int nKey,const double *pInitValue,double *pDataAddr, int nDouNum)
{
	unsigned  idx = 0;
	int  nReadIdx = 0;
	CommuSimuM_MShm_t *pShm = NULL;
	SimulinkDataSyncNode_t *pSync= NULL;
	MyListNode_t *pNode = NULL;
	int nDataLen = nDouNum * 8;

	/****查询，检查*****/
	pNode = MyList_Get(&glstInSync, nKey);
	if( pNode == NULL){
		return -1;
	}
	pSync = (SimulinkDataSyncNode_t *)pNode->pData;

	if( pSync->bShmInit == 0){
		return -1;
	}
	if( pSync->nDoubleNum != nDouNum){
		return -1;
	}
	// 第一步取初值
	if( pSync->nCount == 0){
		memcpy(pDataAddr, pInitValue, nDataLen);
		pSync->nCount++;
	}

	/*****接收数据 ******/
	pShm = pSync->pM_MShm;

	pthread_mutex_lock(&pShm->mutex);
	if(pShm->ReadDataPtr%MAX_MM_SHM_QUEUE_LEN == pShm->WriteDataPtr%MAX_MM_SHM_QUEUE_LEN){
		pthread_mutex_unlock(&pShm->mutex);
		return -1;
	}

	/*读取数据*/
	nReadIdx = pShm->ReadDataPtr%MAX_MM_SHM_QUEUE_LEN;
	memcpy((void*)pDataAddr, pShm->szData[nReadIdx], nDataLen);

	/*移动读指针*/
	pShm->ReadDataPtr += 1;
	pShm->ReadDataPtr %= MAX_MM_SHM_QUEUE_LEN;
	pthread_mutex_unlock(&pShm->mutex);

	pSync->nCount++;
	return 0;
}

int SL_SendSyncData(int nKey,const double *pDataAddr, int nDouNum)
{
	unsigned  idx = 0;
	int  nWriteIdx = 0;
	CommuSimuM_MShm_t *pShm = NULL;
	SimulinkDataSyncNode_t *pSync= NULL;
	MyListNode_t *pNode = NULL;
	int nDataLen = nDouNum * 8;

	/****查询，检查*****/
	pNode = MyList_Get(&glstOutSync, nKey);
	if( pNode == NULL){
		return -1;
	}
	pSync = (SimulinkDataSyncNode_t *)pNode->pData;

	if( pSync->bShmInit == 0){
		return -1;
	}
	if( pSync->nDoubleNum != nDouNum){
		return -1;
	}

	/*写数据到共享内存*/
	pShm = pSync->pM_MShm;

	pthread_mutex_lock(&pShm->mutex);
	nWriteIdx = pShm->WriteDataPtr%MAX_MM_SHM_QUEUE_LEN;
	//nReadIdx = pShm->ReadDataPtr;

	memcpy(pShm->szData[nWriteIdx], (void*)pDataAddr, nDataLen);
	pShm->nDataLen[nWriteIdx] = nDataLen;

	pShm->WriteDataPtr++;
	pthread_mutex_unlock(&pShm->mutex);

	pSync->nCount++;
	return 0;
}
