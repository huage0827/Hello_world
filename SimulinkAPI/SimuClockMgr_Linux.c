#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "SimuCommuAddrDef.h"
#include "SimuModelCommunicationDef.h"

#include "SimuClockMgr.h"

////////////////////////////////////////////////
SimuClockParam_t gSimuClock;

typedef sem_t* SimuSem_t;
unsigned long gSimuSemNum = 0;
SimuSem_t 	gSimuSemSig[SLCL_MAX_MODEL_NUM];/*最大支持SLCL_MAX_MODEL_NUM个信号量*/
SimuSem_t 	gSimuCurSem;
int gStepIdx = 0;

///////////////////////////////////////////////
double gPerStepExecTime = 0.0;

struct timeval gStepStartTime,gStepEndTime;
unsigned long gulStepSize = 0;
unsigned long gulExecTime = 0; 


////////////////////////////////////////////////


/* 静态函数*/
static int clock_InitSem();
static int clock_InitTimer();

////////////////////////////////////////////////
int clock_InitSem()
{
	int iIdx = 0;
	int i = 0;
	char szSemName[128];

	if( !gSimuClock.bMaster)
	{/*非主控模型，创建或打开同步时钟信号量*/
		sprintf(szSemName, KL_COMMU_ClockM_M_sem_name, gSimuClock.nId);
		gSimuCurSem = sem_open(szSemName, O_CREAT| O_EXCL , S_IRWXU|S_IRWXG, 0);
		if(gSimuCurSem == SEM_FAILED){
			if( errno == EEXIST){/*如果存在打开*/
				gSimuCurSem = sem_open(szSemName, 0);
				if( gSimuCurSem == SEM_FAILED){/*出错*/
					return -1;
				}
			}else{/*出错*/
				return -1;
			}
		}
		int nValue = -1;
		sem_getvalue(gSimuCurSem, &nValue);
		if( nValue > 0){
			for(i = 0; i < nValue; i++)
				sem_wait(gSimuCurSem);
		}
		return 0;
	}

	/*主控模型*/
	iIdx = 0;
	for( i = 0; i < gSimuClock.nModelCount; i++){
		sprintf(szSemName, KL_COMMU_ClockM_M_sem_name, gSimuClock.modelList[i].nId);
		gSimuSemSig[iIdx] = sem_open(szSemName, O_CREAT| O_EXCL , S_IRWXU|S_IRWXG, 0);
		if( gSimuSemSig[iIdx] == SEM_FAILED){
			if( errno == EEXIST){/*如果存在打开*/
				gSimuSemSig[iIdx] = sem_open(szSemName, 0);
				if( gSimuSemSig[iIdx] == SEM_FAILED){/*出错*/
					return -1;
				}
			}else{/*出错*/
				return -1;
			}
		}
		iIdx++;
	}
	gSimuSemNum = iIdx;
	return 0;
}

int clock_InitTimer()
{
	gulStepSize = gSimuClock.nStepSize/1000;
	return 0;
}

/////////////////////////////////////////////////
int SL_ClockInit(SimuClockParam_t *pParam)
{
	int i = 0;
	int nRet = 0;


	// 复制参数
	gSimuClock.nId = pParam->nId;
	strcpy(gSimuClock.szName, pParam->szName);
	gSimuClock.bMaster = pParam->bMaster;
	gSimuClock.dStepSize = pParam->dStepSize;
	gSimuClock.nStepSize = pParam->nStepSize;
	gSimuClock.nModelCount = pParam->nModelCount;
	if( gSimuClock.bMaster){
		for(i = 0; i < pParam->nModelCount; i++){
			gSimuClock.modelList[i].nId = pParam->modelList[i].nId;
			gSimuClock.modelList[i].nStepSize = pParam->modelList[i].nStepSize;
			gSimuClock.modelList[i].nStepIdx = 0;
			gSimuClock.modelList[i].nStepRate = pParam->modelList[i].nStepSize/gSimuClock.nStepSize;
		}
	}

	gSimuClock.bUseCallback = pParam->bUseCallback;
	if( gSimuClock.bUseCallback){
		gSimuClock.pClockInitFun = pParam->pClockInitFun;
		gSimuClock.pClockWaitFun = pParam->pClockWaitFun;
		gSimuClock.pClockCleanFun = pParam->pClockCleanFun;
		gSimuClock.pClockUninitFun = pParam->pClockUninitFun;
	}

	// 初始化信号量
	if( clock_InitSem() != 0){
		return -1;
	}

	// 主控模型初始化时钟
	if( gSimuClock.bMaster ){
		if( gSimuClock.bUseCallback){
			// 外部时钟
			return gSimuClock.pClockInitFun();
		}else{
			// 软件时钟信号
			return clock_InitTimer();
		}
	}
	return 0;
}

int SL_ClockWait(unsigned long  ulMaxWaitMs)
{
	int nClockTick = 0;
	int i = 0;
	int nValue = -1;
	struct timespec ts;
	struct timeval tv;
	static int sFirstFlag = 0;
	int nRemainTime = 0;
	

	if( sFirstFlag == 0){
		gettimeofday(&gStepStartTime, NULL);
		sFirstFlag = 1;
	}
	
	/*计算执行时间*/
	gettimeofday(&gStepEndTime, NULL);
	gulExecTime = (gStepEndTime.tv_sec - gStepStartTime.tv_sec)*1000000 - 	
		(gStepEndTime.tv_usec - gStepStartTime.tv_usec)/1000;
	gPerStepExecTime = gulExecTime/1000000.0;

	/*非主控模型，等待信号量*/
	if(!gSimuClock.bMaster){
		sem_getvalue(gSimuCurSem, &nValue);
		if( nValue > 0){/*模型运算超时*/
			nClockTick = nValue;
			for( i = 0; i < nValue; i++)
				sem_wait(gSimuCurSem);
		}
		if( ulMaxWaitMs == 0){/*无限时等待*/
			if( sem_wait(gSimuCurSem) == 0)
				nClockTick += 1;
		}else{/*定时等待*/
			gettimeofday(&tv, NULL);
			ts.tv_sec = tv.tv_sec + ulMaxWaitMs/1000;
			ts.tv_nsec = (tv.tv_usec + ulMaxWaitMs%1000)*1000;

			ts.tv_sec += ts.tv_nsec/(1000*1000*1000);
			ts.tv_nsec = ts.tv_nsec%(1000*1000*1000);
			if( sem_timedwait(gSimuCurSem, &ts) == 0)
				nClockTick += 1;
		}
	}else{/* 主控模型*/
	
		if(gSimuClock.bUseCallback){
			// 从外部获取时钟
			nClockTick = gSimuClock.pClockWaitFun();
		}else{
			nRemainTime = gulStepSize - gulExecTime;
			if( nRemainTime > 0){
				tv.tv_sec = nRemainTime/1000000;
				tv.tv_usec = nRemainTime%1000000;
				select(0,NULL,NULL,NULL,&tv);
			}else{//超时运行
				nClockTick += 1;
			}
			nClockTick += 1;
		}

		// 发送时钟信号给其他模型
		for(i = 0; i < gSimuClock.nModelCount; i++){
			gSimuClock.modelList[i].nStepIdx++;
			if( gSimuClock.modelList[i].nStepIdx >= gSimuClock.modelList[i].nStepRate){
				sem_post(gSimuSemSig[i]);
				gSimuClock.modelList[i].nStepIdx = 0;
			}
		}
	}
	// 执行开始计时
	gettimeofday(&gStepStartTime, NULL);
	return nClockTick;
}


int SL_ClockClean()
{
	int i = 0;

	if(gSimuClock.bMaster){
		// Clean
		if(gSimuClock.bUseCallback){
			gSimuClock.pClockCleanFun();
		}else{
			gettimeofday(&gStepStartTime, NULL);
		}
		// 清空被控模型时钟序号
		for(i = 0; i < gSimuClock.nModelCount;i++){
			gSimuClock.modelList[i].nStepIdx = 0;
		}
	}
	return 0;
}

int SL_ClockUninit()
{
	int i = 0;
	char szSemName[128];

	// 非主控模型
	if(!gSimuClock.bMaster){
		return sem_close(gSimuCurSem);
	}

	// 主控模型
	if(gSimuClock.bUseCallback){
		gSimuClock.pClockUninitFun();
	}else{
		for(i = 0; i < gSimuSemNum; i++){
			sem_close(gSimuSemSig[i]);
		}
		for(i = 0; i < gSimuClock.nModelCount; i++){
			sprintf(szSemName, KL_COMMU_ClockM_M_sem_name, gSimuClock.modelList[i].nId);
			sem_unlink(szSemName);
		}
	}
	return 0;
}

int SL_SetPriority(int nPri)
{
	return 0;
}

int SL_SetThreadAffinity(unsigned char *szCpuCore,unsigned int nCoreNum)
{
	return 0;
}
