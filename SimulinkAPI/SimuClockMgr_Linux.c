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
SimuSem_t 	gSimuSemSig[SLCL_MAX_MODEL_NUM];/*���֧��SLCL_MAX_MODEL_NUM���ź���*/
SimuSem_t 	gSimuCurSem;
int gStepIdx = 0;

///////////////////////////////////////////////
double gPerStepExecTime = 0.0;

struct timeval gStepStartTime,gStepEndTime;
unsigned long gulStepSize = 0;
unsigned long gulExecTime = 0; 


////////////////////////////////////////////////


/* ��̬����*/
static int clock_InitSem();
static int clock_InitTimer();

////////////////////////////////////////////////
int clock_InitSem()
{
	int iIdx = 0;
	int i = 0;
	char szSemName[128];

	if( !gSimuClock.bMaster)
	{/*������ģ�ͣ��������ͬ��ʱ���ź���*/
		sprintf(szSemName, KL_COMMU_ClockM_M_sem_name, gSimuClock.nId);
		gSimuCurSem = sem_open(szSemName, O_CREAT| O_EXCL , S_IRWXU|S_IRWXG, 0);
		if(gSimuCurSem == SEM_FAILED){
			if( errno == EEXIST){/*������ڴ�*/
				gSimuCurSem = sem_open(szSemName, 0);
				if( gSimuCurSem == SEM_FAILED){/*����*/
					return -1;
				}
			}else{/*����*/
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

	/*����ģ��*/
	iIdx = 0;
	for( i = 0; i < gSimuClock.nModelCount; i++){
		sprintf(szSemName, KL_COMMU_ClockM_M_sem_name, gSimuClock.modelList[i].nId);
		gSimuSemSig[iIdx] = sem_open(szSemName, O_CREAT| O_EXCL , S_IRWXU|S_IRWXG, 0);
		if( gSimuSemSig[iIdx] == SEM_FAILED){
			if( errno == EEXIST){/*������ڴ�*/
				gSimuSemSig[iIdx] = sem_open(szSemName, 0);
				if( gSimuSemSig[iIdx] == SEM_FAILED){/*����*/
					return -1;
				}
			}else{/*����*/
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


	// ���Ʋ���
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

	// ��ʼ���ź���
	if( clock_InitSem() != 0){
		return -1;
	}

	// ����ģ�ͳ�ʼ��ʱ��
	if( gSimuClock.bMaster ){
		if( gSimuClock.bUseCallback){
			// �ⲿʱ��
			return gSimuClock.pClockInitFun();
		}else{
			// ���ʱ���ź�
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
	
	/*����ִ��ʱ��*/
	gettimeofday(&gStepEndTime, NULL);
	gulExecTime = (gStepEndTime.tv_sec - gStepStartTime.tv_sec)*1000000 - 	
		(gStepEndTime.tv_usec - gStepStartTime.tv_usec)/1000;
	gPerStepExecTime = gulExecTime/1000000.0;

	/*������ģ�ͣ��ȴ��ź���*/
	if(!gSimuClock.bMaster){
		sem_getvalue(gSimuCurSem, &nValue);
		if( nValue > 0){/*ģ�����㳬ʱ*/
			nClockTick = nValue;
			for( i = 0; i < nValue; i++)
				sem_wait(gSimuCurSem);
		}
		if( ulMaxWaitMs == 0){/*����ʱ�ȴ�*/
			if( sem_wait(gSimuCurSem) == 0)
				nClockTick += 1;
		}else{/*��ʱ�ȴ�*/
			gettimeofday(&tv, NULL);
			ts.tv_sec = tv.tv_sec + ulMaxWaitMs/1000;
			ts.tv_nsec = (tv.tv_usec + ulMaxWaitMs%1000)*1000;

			ts.tv_sec += ts.tv_nsec/(1000*1000*1000);
			ts.tv_nsec = ts.tv_nsec%(1000*1000*1000);
			if( sem_timedwait(gSimuCurSem, &ts) == 0)
				nClockTick += 1;
		}
	}else{/* ����ģ��*/
	
		if(gSimuClock.bUseCallback){
			// ���ⲿ��ȡʱ��
			nClockTick = gSimuClock.pClockWaitFun();
		}else{
			nRemainTime = gulStepSize - gulExecTime;
			if( nRemainTime > 0){
				tv.tv_sec = nRemainTime/1000000;
				tv.tv_usec = nRemainTime%1000000;
				select(0,NULL,NULL,NULL,&tv);
			}else{//��ʱ����
				nClockTick += 1;
			}
			nClockTick += 1;
		}

		// ����ʱ���źŸ�����ģ��
		for(i = 0; i < gSimuClock.nModelCount; i++){
			gSimuClock.modelList[i].nStepIdx++;
			if( gSimuClock.modelList[i].nStepIdx >= gSimuClock.modelList[i].nStepRate){
				sem_post(gSimuSemSig[i]);
				gSimuClock.modelList[i].nStepIdx = 0;
			}
		}
	}
	// ִ�п�ʼ��ʱ
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
		// ��ձ���ģ��ʱ�����
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

	// ������ģ��
	if(!gSimuClock.bMaster){
		return sem_close(gSimuCurSem);
	}

	// ����ģ��
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
