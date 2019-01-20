	#pragma once

#include "SimuModelDefine.h"

#ifdef __CPLUSPLUS
extern "C"
{
#endif

/* ע��ģ�� */
bool SLRegisterModel(ModelInputSignal_t *pInput, ModelOutputSignal_t *pOutput, ModelInternalParam_t *pParam,
		ModelInitCallbackFun init, ModelInitCallbackFun exec, ModelInitCallbackFun uninit);

/* ��ʼ������ģ�� */
bool SLInitialize(int argc , char** argv);

/* ���з���ģ�� */
bool SLRun();

/* ж�ط���ģ�ͣ��ͷ������Դ */
bool SLUninitialize();

/* ��ӡ����û�����*/
bool SLPrint(const char *szStr);

/* ��¼�û�����*/
bool SLRecord(const char *szStr, const char *szRecordFile);

/* �û����Խ���*/
//bool SLTestEnd(const char *szStr);

/* ����ģ�͵���ʱ�������в���*/
unsigned long SLGetRunStepCount();

/* �û���ͣģ������*/
bool SLPauseRun(const char *szStr);

/* �û�ֹͣģ������*/
bool SLStopRun(const char *szStr);

#ifdef __CPLUSPLUS
}
#endif
