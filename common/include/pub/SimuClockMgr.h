/*****************************************************************
 * 文件名: SimuClock.h
 * 功能: 仿真时钟管理
 *****************************************************************/
#ifndef SIMUCLOCK_H_
#define SIMUCLOCK_H_

#define SLCL_MAX_MODEL_NUM 64

extern double gPerStepExecTime;//外部可访问该变量，单位微秒

typedef int(*SL_ClockInitCallback)();
typedef int(*SL_ClockWaitCallback)();
typedef int(*SL_ClockCleanCallback)();
typedef int(*SL_ClockUninitCallback)();

typedef struct TagSimuModelStep{
	int nId; /*模型ID*/
	int nStepSize;/*步长,纳秒单位*/
	int nStepRate;
	int nStepIdx;
}SimuModelStep_t;

/******************************************************************
 * 结构体名 : SimuClockParam_t
 * 说明 : 仿真时钟参数
 ******************************************************************/
typedef struct TagSimuClockParam{
	int nId;  				/*模型ID*/
	char szName[128];		/*模型名*/
	char bMaster;  			/*是否是主控模型*/
	double dStepSize;		/*步长,秒为单位*/
	int	nStepSize;			/*步长,纳秒单位*/
	int nModelCount;
	SimuModelStep_t modelList[SLCL_MAX_MODEL_NUM];/*被控模型列表，仅bMaster=true时有效*/
	char bUseCallback;		/*使用外部时钟*/
	SL_ClockInitCallback pClockInitFun;
	SL_ClockWaitCallback pClockWaitFun;
	SL_ClockCleanCallback pClockCleanFun;
	SL_ClockUninitCallback pClockUninitFun;
}SimuClockParam_t;

/******************************************************************
 * 函数名 : SL_ClockInit
 * 说明 : 初始化时钟
 * 参数： SimuClockParam_t *pParam => 仿真时钟参数
 * 返回: 0 => 成功；非0 => 不成功
 ******************************************************************/
int SL_ClockInit(SimuClockParam_t *pParam);

/******************************************************************
 * 函数名 : SL_ClockWait
 * 说明 : 等待时钟信号
 * 参数： unsigned long  ulMaxWaitMs => 最大等待时间毫秒；如果为NULL，无限制等待
 * 返回: 小于0=>失败；大于0 => 时钟到达的次数，正常为1，大于1则失不；
 ******************************************************************/
int SL_ClockWait(unsigned long  ulMaxWaitMs);

/******************************************************************
 * 函数名 : SL_ClockClean
 * 说明 : 当模型由暂停恢复到执行状态时调用
 * 参数：无
 * 返回: 0 => 成功；非0 => 不成功
 ******************************************************************/
int SL_ClockClean();

/******************************************************************
 * 函数名 : SL_ClockUninit
 * 说明 : 释放资源
 * 参数：无
 * 返回: 0 => 成功；非0 => 不成功
 ******************************************************************/
int SL_ClockUninit();

/******************************************************************
 * 函数名 : SL_SetPriority
 * 说明 : 设置线程优先级
 * 参数：无
 * 返回: 0 => 成功；非0 => 不成功
 ******************************************************************/
int SL_SetPriority(int nPri);

/******************************************************************
 * 函数名 : SL_SetThreadAffinity
 * 说明 : 设置线程亲和度
 * 参数：无
 * 返回: 0 => 成功；非0 => 不成功
 ******************************************************************/
int SL_SetThreadAffinity(unsigned char *szCpuCore,unsigned int nCoreNum);

#endif
