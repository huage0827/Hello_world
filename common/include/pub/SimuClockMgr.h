/*****************************************************************
 * �ļ���: SimuClock.h
 * ����: ����ʱ�ӹ���
 *****************************************************************/
#ifndef SIMUCLOCK_H_
#define SIMUCLOCK_H_

#define SLCL_MAX_MODEL_NUM 64

extern double gPerStepExecTime;//�ⲿ�ɷ��ʸñ�������λ΢��

typedef int(*SL_ClockInitCallback)();
typedef int(*SL_ClockWaitCallback)();
typedef int(*SL_ClockCleanCallback)();
typedef int(*SL_ClockUninitCallback)();

typedef struct TagSimuModelStep{
	int nId; /*ģ��ID*/
	int nStepSize;/*����,���뵥λ*/
	int nStepRate;
	int nStepIdx;
}SimuModelStep_t;

/******************************************************************
 * �ṹ���� : SimuClockParam_t
 * ˵�� : ����ʱ�Ӳ���
 ******************************************************************/
typedef struct TagSimuClockParam{
	int nId;  				/*ģ��ID*/
	char szName[128];		/*ģ����*/
	char bMaster;  			/*�Ƿ�������ģ��*/
	double dStepSize;		/*����,��Ϊ��λ*/
	int	nStepSize;			/*����,���뵥λ*/
	int nModelCount;
	SimuModelStep_t modelList[SLCL_MAX_MODEL_NUM];/*����ģ���б���bMaster=trueʱ��Ч*/
	char bUseCallback;		/*ʹ���ⲿʱ��*/
	SL_ClockInitCallback pClockInitFun;
	SL_ClockWaitCallback pClockWaitFun;
	SL_ClockCleanCallback pClockCleanFun;
	SL_ClockUninitCallback pClockUninitFun;
}SimuClockParam_t;

/******************************************************************
 * ������ : SL_ClockInit
 * ˵�� : ��ʼ��ʱ��
 * ������ SimuClockParam_t *pParam => ����ʱ�Ӳ���
 * ����: 0 => �ɹ�����0 => ���ɹ�
 ******************************************************************/
int SL_ClockInit(SimuClockParam_t *pParam);

/******************************************************************
 * ������ : SL_ClockWait
 * ˵�� : �ȴ�ʱ���ź�
 * ������ unsigned long  ulMaxWaitMs => ���ȴ�ʱ����룻���ΪNULL�������Ƶȴ�
 * ����: С��0=>ʧ�ܣ�����0 => ʱ�ӵ���Ĵ���������Ϊ1������1��ʧ����
 ******************************************************************/
int SL_ClockWait(unsigned long  ulMaxWaitMs);

/******************************************************************
 * ������ : SL_ClockClean
 * ˵�� : ��ģ������ͣ�ָ���ִ��״̬ʱ����
 * ��������
 * ����: 0 => �ɹ�����0 => ���ɹ�
 ******************************************************************/
int SL_ClockClean();

/******************************************************************
 * ������ : SL_ClockUninit
 * ˵�� : �ͷ���Դ
 * ��������
 * ����: 0 => �ɹ�����0 => ���ɹ�
 ******************************************************************/
int SL_ClockUninit();

/******************************************************************
 * ������ : SL_SetPriority
 * ˵�� : �����߳����ȼ�
 * ��������
 * ����: 0 => �ɹ�����0 => ���ɹ�
 ******************************************************************/
int SL_SetPriority(int nPri);

/******************************************************************
 * ������ : SL_SetThreadAffinity
 * ˵�� : �����߳��׺Ͷ�
 * ��������
 * ����: 0 => �ɹ�����0 => ���ɹ�
 ******************************************************************/
int SL_SetThreadAffinity(unsigned char *szCpuCore,unsigned int nCoreNum);

#endif
