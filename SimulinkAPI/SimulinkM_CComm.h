#pragma once

/******************************************************************
 * �ṹ���� : ModelOutputData_t
 * ˵�� : ģ������ź�
 ******************************************************************/
typedef struct tagModelOutputData{
	int nID;
	int nNO;
	int nLen;
	int nStartIdx;
}ModelOutputData_t;

/******************************************************************
 * �ṹ���� : ModelAdjustParam_t
 * ˵�� : ģ�Ϳɵ�����
 ******************************************************************/
typedef struct tagModelAdjustParam{
	int nID;
	int nNO;
	int nLen;
	int nStartIdx;
}ModelAdjustParam_t;

/******************************************************************
 * ˵�� : M_C�ӿں�������
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
