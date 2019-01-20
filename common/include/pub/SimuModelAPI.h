	#pragma once

#include "SimuModelDefine.h"

#ifdef __CPLUSPLUS
extern "C"
{
#endif

/* 注册模型 */
bool SLRegisterModel(ModelInputSignal_t *pInput, ModelOutputSignal_t *pOutput, ModelInternalParam_t *pParam,
		ModelInitCallbackFun init, ModelInitCallbackFun exec, ModelInitCallbackFun uninit);

/* 初始化仿真模型 */
bool SLInitialize(int argc , char** argv);

/* 运行仿真模型 */
bool SLRun();

/* 卸载仿真模型，释放相关资源 */
bool SLUninitialize();

/* 打印输出用户数据*/
bool SLPrint(const char *szStr);

/* 记录用户数据*/
bool SLRecord(const char *szStr, const char *szRecordFile);

/* 用户测试结束*/
//bool SLTestEnd(const char *szStr);

/* 返回模型调用时，已运行步数*/
unsigned long SLGetRunStepCount();

/* 用户暂停模型运行*/
bool SLPauseRun(const char *szStr);

/* 用户停止模型运行*/
bool SLStopRun(const char *szStr);

#ifdef __CPLUSPLUS
}
#endif
