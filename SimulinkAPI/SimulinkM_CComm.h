#pragma once

/******************************************************************
 * 结构体名 : ModelOutputData_t
 * 说明 : 模型输出信号
 ******************************************************************/
typedef struct tagModelOutputData{
	int nID;
	int nNO;
	int nLen;
	int nStartIdx;
}ModelOutputData_t;

/******************************************************************
 * 结构体名 : ModelAdjustParam_t
 * 说明 : 模型可调参数
 ******************************************************************/
typedef struct tagModelAdjustParam{
	int nID;
	int nNO;
	int nLen;
	int nStartIdx;
}ModelAdjustParam_t;

/******************************************************************
 * 说明 : M_C接口函数声明
 ******************************************************************/
int SL_InitM_CComm();
void SL_UninitM_CComm();
int SL_SetModelOutput(int numSig, double *pValAddr);
int SL_SetModelParam(int numParam, double *pParamAddr);

int SL_SendRegisterModelCmd();
int SL_SendExceptReportCmd(unsigned long nStep, int level, const char *szData);
int SL_SendRecordDataCmd(unsigned long nStep);
int SL_SendDispDataCmd(unsigned long nStep, unsigned long nOverRun, double ExecTime, double AllTime);
int SL_RecvControlCmd();
