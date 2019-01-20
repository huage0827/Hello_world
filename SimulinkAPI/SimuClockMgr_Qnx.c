#include <stdio.h>
#include <stdlib.h>
#include <sys/siginfo.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <sys/syspage.h>
#include <hw/inout.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

#ifndef QNX_PLATFORM
#define QNX_PLATFORM
#endif

#include "SimuClockMgr.h"
#include "SimuCommuAddrDef.h"
#include "SimuModelCommunicationDef.h"

////////////////////////////////////////////////
SimuClockParam_t gSimuClock;

typedef sem_t* SimuSem_t;
unsigned long gSimuSemNum = 0;
SimuSem_t 	gSimuSemSig[SLCL_MAX_MODEL_NUM];/*最大支持SLCL_MAX_MODEL_NUM个信号量*/
SimuSem_t 	gSimuCurSem;
int gStepIdx = 0;

///////////////////////////////////////////////
double gPerStepExecTime = 0.0;
uint64_t nCyclesPerMircoSec;
uint64_t allStartCycle, allEndCycle;

////////////////////////////////////////////////
volatile char g_bIreqModelExecing = 0;	  /*模型是否在执行*/
volatile unsigned long g_nIreqTotalCount = 0; /*时钟中断产生总次数*/
volatile unsigned long g_nIreqOverCount = 0;  /*一次中断的超时次数*/

int nIterruptId = 0; /*An interrupt function ID*/
struct sigevent SigEvent; /*信号事件*/
#define IRQ_0 0 /*中断向量号->时钟中断*/

/* 中断处理函数 */
const struct sigevent *isr_handler(void *arg, int id)
{
	g_nIreqTotalCount++;
	if(g_bIreqModelExecing){
		g_nIreqOverCount++;
		return NULL;
	}
	return (&SigEvent); /*发送中断信号给注册线程*/
}

/* 静态函数*/
static int clock_InitSem();
static int clock_InitIRQ();

////////////////////////////////////////////////
int clock_InitSem()
{
	int iIdx = 0;
	int i = 0;
	char szSemName[128];

	if( !gSimuClock.bMaster)
	{/*非主控模型，创建或打开同步时钟信号量*/
		sprintf(szSemName, KL_COMMU_ClockM_M_sem_name, gSimuClock.nId);
		////////
		printf("simulink=>cl.SemName:%s \n", szSemName);
		////////
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
		////////
		printf("simulink=>cl.SemName:%s \n", szSemName);
		////////
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

int clock_InitIRQ()
{
	unsigned long long nCallCycle;
	unsigned long long nCyclesPerMircoSec = 0;
	struct _clockperiod clockSet;

	/*the thread must request I/O privileges by calling*/
	ThreadCtl(_NTO_TCTL_IO, NULL);

	 /* find out how many cycles per second */
	nCyclesPerMircoSec = SYSPAGE_ENTRY(qtime)->cycles_per_sec / (1000 * 1000);
	nCallCycle = 10000 * nCyclesPerMircoSec;

	clockSet.nsec = gSimuClock.nStepSize;
	clockSet.fract = 0;

	/* Get a clock period*/
	ClockPeriod(CLOCK_REALTIME , &clockSet , NULL , 0);

	/* Send an interrupt notification to a specific thread. No other fields in the structure are used.
	 * The initialization macro is
	 */
	SIGEV_INTR_INIT(&SigEvent);

	/*
	 * Attach an interrupt handler to an interrupt source
	 */
	nIterruptId = InterruptAttach(IRQ_0, isr_handler , NULL , 0 , 0);
	if(nIterruptId == -1 ){
		printf("InterruptAttach failed, nIterruptId = %d, errno:%d \n",nIterruptId, errno);
		return -1;
	}
	return 0;
}
/////////////////////////////////////////////////
int SL_ClockInit(SimuClockParam_t *pParam)
{
	int i = 0;
	int nRet = 0;

	nCyclesPerMircoSec = SYSPAGE_ENTRY(qtime)->cycles_per_sec / (1000 * 1000);

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
			return clock_InitIRQ();
		}
	}
	return 0;
}

int SL_ClockWait(unsigned long  ulMaxWaitMs)
{
	int nClockTick = 0;
	int i = 0;
	int nValue = -1;
	struct timespec tm;

	/*计算执行时间*/
	allEndCycle = ClockCycles();
	gPerStepExecTime = ((double)(allEndCycle - allStartCycle)/nCyclesPerMircoSec);

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
			clock_gettime(CLOCK_REALTIME, &tm);
			tm.tv_sec += ulMaxWaitMs/1000;
			tm.tv_nsec += ulMaxWaitMs%1000;
			
			if( sem_timedwait(gSimuCurSem, &tm) == 0)
				nClockTick += 1;
		}
		// 执行开始计时
		allStartCycle = ClockCycles();
		return nClockTick;
	}

	if(gSimuClock.bUseCallback){
		// 从外部获取时钟
		nClockTick = gSimuClock.pClockWaitFun();
	}else{
		// 等待时钟中断
		if(g_nIreqOverCount > 0){/*主控模型运算超时*/
			nClockTick += g_nIreqOverCount;
		}
		g_bIreqModelExecing = 0;
		if( InterruptWait(NULL, NULL) == -1 ){/*等待时钟信号*/
			return 0;
		}
		g_bIreqModelExecing = 1;
		g_nIreqOverCount = 0;
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

	// 执行开始计时
	allStartCycle = ClockCycles();
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
			g_nIreqOverCount = 0;
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
		InterruptDetach(nIterruptId);
	}
	return 0;
}

int SL_SetPriority(int nPri)
{
	/*提高模型运行优先级*/
	struct sched_param param;
	param.sched_priority = nPri;

	if(sched_setscheduler( 0, SCHED_FIFO, &param ) == -1){
		fprintf(stderr, "Unable to change priority: %s\n",
		strerror(errno));
		return -1;
	}
	return 0;
}

int SL_SetThreadAffinity(unsigned char *szCpuCore,unsigned int nCoreNum)
{
	unsigned    num_elements = 0;
	int         *rsizep, masksize_bytes, size;
	unsigned    *rmaskp, *imaskp;
	void        *my_data;
	unsigned 	i = 0;
	
	if(nCoreNum <= 0){
		return 0;
	}

/*	printf(" SetThreadAffinity total num_cpu:%d, sysconf:%ld  \n",
				_syspage_ptr->num_cpu, sysconf(_SC_NPROCESSORS_CONF));*/

	num_elements = RMSK_SIZE(_syspage_ptr->num_cpu);

	/* Determine the size of the runmask, in bytes. */
	masksize_bytes = num_elements * sizeof(unsigned);

	size = sizeof(int) + 2 * masksize_bytes;
	if ((my_data = malloc(size)) == NULL){
		return -1;
	}

	memset(my_data, 0x0, size);

	/* Set up pointers to the "members" of the structure. */
	rsizep = (int *)my_data;
	rmaskp =(unsigned *) rsizep + 1;
	imaskp = rmaskp + num_elements;

	/* Set the size. */
	*rsizep = num_elements;


	for( i = 0; i < nCoreNum; i++){
		if( szCpuCore[i] >= _syspage_ptr->num_cpu){
			printf("CpuCoreIdx[%d] > CpuCoreNum[%d] \n", szCpuCore[i], _syspage_ptr->num_cpu);
			continue;
		}
		RMSK_SET(szCpuCore[i], rmaskp);
		RMSK_SET(szCpuCore[i], imaskp);
	}

	if( ThreadCtl( _NTO_TCTL_RUNMASK_GET_AND_SET_INHERIT,my_data) == -1 )
	{
		printf("SetThreadAffinity failed \n");
		free(my_data);
		return -1;
	}

	free(my_data);
	printf("SetThreadAffinity ok \n");

	return 0;
}
