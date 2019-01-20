#pragma once
#include "SimuConfigDef.h"
#include <map>
#include <string>

/*
 *  ģ�������źŶ���
 */
typedef struct{
	char 			szName[256]; /*���� */
	unsigned char 	ucValType;	 /*�������� */
	unsigned long   ulValLen;	 /*���ݳ��� */
	void 			*pValAddr;	 /*��ַ */
}ModelInputSignal_t;

/*
 *  ģ������źŶ���
 */
typedef struct{
	char 			szName[256]; /*���� */
	unsigned char 	ucValType;	 /*�������� */
	unsigned long   ulValLen;	 /*���ݳ��� */
	void 			*pValAddr;	 /*��ַ */
}ModelOutputSignal_t;

/*
 *  ģ���ڲ���������
 */
typedef struct{
	char 			szName[256]; /*���� */
	unsigned char 	ucValType;	 /*�������� */
	unsigned long   ulValLen;	 /*���ݳ��� */
	void 			*pValAddr;	 /*��ַ */
}ModelInternalParam_t;


/* ģ�ͳ�ʼ���ص���������  */
typedef bool (*ModelInitCallbackFun)();

/* ģ��ִ�лص���������  */
typedef bool (*ModelExecCallbackFun)();

/* ģ�ͷ���ʼ���ص���������  */
typedef bool (*ModelUninitCallbackFun)();

/****************************************************************
 * �ṹ���� : Model
 * ˵�� : ����ģ�͵Ĺ��ò�������
 ****************************************************************/
typedef struct tagSimuModel
{
	char szName[256];			   /* ģ������ */
	char szProjID[37];			   /* ģ�͹���ID*/
	int  nID;				       /* ģ�ͱ��:��ģ��ID*/
	bool bMaster;				   /* �Ƿ�������ģ��*/

	unsigned long nExecMode;	   /* ִ��ģʽ*/
	bool bHClockEx;				   /* 2110�忨�Ƿ���ⲿ��ȡʱ���ź�*/
	bool bPause;				   /* �Ƿ���ͣ���� */
	bool bQuit;					   /* �Ƿ�ֹͣ���� */

	unsigned long nStepTime;	   /* �������ʱ��, ��λ���� */
	unsigned long nStepRate;	   /* �������ģ�ͱ���*/
	unsigned long nStepIdx;
	unsigned long nDisplayTime;	   /* ��ʾ�����ʣ���λ����*/
	unsigned long nRecordTime;	   /* ��ز����ʣ���λ����*/

	bool bAutoRun;				   /* �Ƿ��Զ�����*/
	unsigned long nTestTaskId;	   /* ִ�еĲ�������*/
	unsigned long nTestCaseId;	   /* ִ�еĲ�������*/

	unsigned long nCurStepCount;   /* ��ǰ�ڼ��� */
	unsigned long nOverRunCount;   /* �ܳ�ʱ�� */
	time_t  nBeginRunTime;   	   /* ģ�Ϳ�ʼ����ʱ��*/
	bool	bNeedInitWait;		   /* �Ƿ���Ҫ�ȴ�ģ�ͳ�ʼ��*/
	unsigned long nNeedInitWaitTime;   /* ģ�ͳ�ʼ���ȴ�ʱ��*/

	bool  bNonRealTimeRunAdjust;   /* ��ʵʱ����΢��*/
	unsigned long nNonRTAdjustLen; /* ��ʵʱ΢������*/

	vector<ModelInputSignal_t *> vecInputSigs; /*ģ�������ź��б�*/
	vector<ModelOutputSignal_t *> vecOutputSigs;/*ģ������ź��б�*/
	vector<ModelInternalParam_t *> vecParams; /*ģ�Ͳ����б�*/

	ModelInitCallbackFun pInitCallbackFun; /*�û�ģ�ͳ�ʼ���ص�����*/
	ModelExecCallbackFun pExecCallbackFun; /*�û�ģ��ִ�лص�����*/
	ModelUninitCallbackFun	pUninitCallbackFun;/*�û�ģ�ͷ���ʼ���ص�����*/

	char szMonitorIp[32];
}SimuModel_t , *PSimuModel_t;

/*
 * ����ģ��ȫ�ֱ���
 */
extern SimuModel_t	g_SimuModel;