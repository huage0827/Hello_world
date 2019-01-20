#pragma once
#include "SimuConfigDef.h"
#include <map>
#include <string>

/*
 *  模型输入信号定义
 */
typedef struct{
	char 			szName[256]; /*名称 */
	unsigned char 	ucValType;	 /*数据类型 */
	unsigned long   ulValLen;	 /*数据长度 */
	void 			*pValAddr;	 /*地址 */
}ModelInputSignal_t;

/*
 *  模型输出信号定义
 */
typedef struct{
	char 			szName[256]; /*名称 */
	unsigned char 	ucValType;	 /*数据类型 */
	unsigned long   ulValLen;	 /*数据长度 */
	void 			*pValAddr;	 /*地址 */
}ModelOutputSignal_t;

/*
 *  模型内部参数定义
 */
typedef struct{
	char 			szName[256]; /*名称 */
	unsigned char 	ucValType;	 /*数据类型 */
	unsigned long   ulValLen;	 /*数据长度 */
	void 			*pValAddr;	 /*地址 */
}ModelInternalParam_t;


/* 模型初始化回调函数定义  */
typedef bool (*ModelInitCallbackFun)();

/* 模型执行回调函数定义  */
typedef bool (*ModelExecCallbackFun)();

/* 模型反初始化回调函数定义  */
typedef bool (*ModelUninitCallbackFun)();

/****************************************************************
 * 结构体名 : Model
 * 说明 : 仿真模型的公用参数定义
 ****************************************************************/
typedef struct tagSimuModel
{
	char szName[256];			   /* 模型名称 */
	char szProjID[37];			   /* 模型工程ID*/
	int  nID;				       /* 模型编号:子模型ID*/
	bool bMaster;				   /* 是否是主控模型*/

	unsigned long nExecMode;	   /* 执行模式*/
	bool bHClockEx;				   /* 2110板卡是否从外部获取时钟信号*/
	bool bPause;				   /* 是否暂停运行 */
	bool bQuit;					   /* 是否停止运行 */

	unsigned long nStepTime;	   /* 步长间隔时间, 单位纳秒 */
	unsigned long nStepRate;	   /* 相对于主模型比率*/
	unsigned long nStepIdx;
	unsigned long nDisplayTime;	   /* 显示采样率，单位纳秒*/
	unsigned long nRecordTime;	   /* 监控采样率，单位纳秒*/

	bool bAutoRun;				   /* 是否自动运行*/
	unsigned long nTestTaskId;	   /* 执行的测试任务*/
	unsigned long nTestCaseId;	   /* 执行的测试用例*/

	unsigned long nCurStepCount;   /* 当前第几步 */
	unsigned long nOverRunCount;   /* 总超时数 */
	time_t  nBeginRunTime;   	   /* 模型开始运算时间*/
	bool	bNeedInitWait;		   /* 是否需要等待模型初始化*/
	unsigned long nNeedInitWaitTime;   /* 模型初始化等待时间*/

	bool  bNonRealTimeRunAdjust;   /* 非实时运行微调*/
	unsigned long nNonRTAdjustLen; /* 非实时微调步长*/

	vector<ModelInputSignal_t *> vecInputSigs; /*模型输入信号列表*/
	vector<ModelOutputSignal_t *> vecOutputSigs;/*模型输出信号列表*/
	vector<ModelInternalParam_t *> vecParams; /*模型参数列表*/

	ModelInitCallbackFun pInitCallbackFun; /*用户模型初始化回调函数*/
	ModelExecCallbackFun pExecCallbackFun; /*用户模型执行回调函数*/
	ModelUninitCallbackFun	pUninitCallbackFun;/*用户模型反初始化回调函数*/

	char szMonitorIp[32];
}SimuModel_t , *PSimuModel_t;

/*
 * 仿真模型全局变量
 */
extern SimuModel_t	g_SimuModel;
