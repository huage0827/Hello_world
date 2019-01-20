#pragma  once
#include "SimulinkCfgDefine.h"

/****************************************************************
 * �ṹ���� : SimulinkModel_t
 * ˵�� : ����ģ�͵Ĺ��ò�������
 ****************************************************************/
typedef struct tagSimulinkModel
{
	int  nID;				       /* ģ�ͱ��:��ģ��ID*/
	unsigned long nExecMode;	   /* ִ��ģʽ*/
	char bPause;				   /* �Ƿ���ͣ���� */
	char bQuit;					   /* �Ƿ�ֹͣ���� */
	unsigned long nStepTime;	   /* �������ʱ��, ��λ���� */
	unsigned long nDispRate;	   /* ��ز�����*/
	unsigned long nRecordRate;	   /* ��ʾ������*/
	unsigned long nCurStepCount;   /* ��ǰ�ڼ��� */
	unsigned long nOverRunCount;   /* �ܳ�ʱ�� */
}SimulinkModel_t , *PSimulinkModel_t;


extern SimulinkConfig_t g_SimuCfg;	 /*��������*/
extern SimulinkModel_t g_SimuModel;  /*����ģ��*/
