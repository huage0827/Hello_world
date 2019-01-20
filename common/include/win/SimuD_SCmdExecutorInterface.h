/*******************************************************************
 * �ļ���: SimuD_SCmdExecutorInterface.h
 * ����: SimuMonitor��SimuService֮���ͨ������ִ�����ӿڶ���
 ******************************************************************/

#pragma once

#include "SimuD_SCmd.h"

/****************************************************************
 * ���� : CSimuD_SCmdExecutorInferface
 * ���� : ��
 * ˵�� : SimuMonitor��SimuService֮��ͨ������ִ�����ӿ�
 ****************************************************************/
class CSimuD_SCmdExecutorInferface
{
public:
	virtual BOOL ExecInitMonitorCmd(CSimuD_SInitMonitorCmd* pCmd);/*�����ʼ���������*/
	virtual BOOL ExecInitMonitorRespCmd(CSimuD_SInitMonitorRespCmd *pCmd);/*�����ʼ�������Ӧ����*/
	virtual BOOL ExecLoadSimuModelReportCmd(CSimuD_SLoadSimuModelReportCmd *pCmd);/*���ط���ģ���ϱ�����*/
	virtual BOOL ExecExecSimuModelCmd(CSimuD_SExecSimuModelCmd *pCmd);/*����ִ��ģ������*/
	virtual BOOL ExecExecSimuModelRespCmd(CSimuD_SExecSimuModelRespCmd *pCmd);/*����ִ��ģ����Ӧ����*/
	virtual BOOL ExecPauseSimuModelCmd(CSimuD_SPauseSimuModelCmd *pCmd);/*������ͣģ������*/
	virtual BOOL ExecPauseSimuModelRespCmd(CSimuD_SPauseSimuModelRespCmd *pCmd);/*������ͣģ����Ӧ����*/
	virtual BOOL ExecResetSimuModelCmd(CSimuD_SResetSimuModelCmd *pCmd);/*��������ģ������*/
	virtual BOOL ExecResetSimuModelRespCmd(CSimuD_SResetSimuModelRespCmd *pCmd);/*��������ģ����Ӧ����*/
	virtual BOOL ExecSetSimuModelParamCmd(CSimuD_SSetSimuModelParamCmd *pCmd);/*��������ģ�Ͳ�������*/
	virtual BOOL ExecSetSimuModelParamRespCmd(CSimuD_SSetSimuModelParamRespCmd *pCmd);/*��������ģ�Ͳ�����Ӧ����*/
	virtual BOOL ExecTestEventReportCmd(CSimuD_STestEventReportCmd *pCmd);/*��������¼��ϱ�����*/
	virtual BOOL ExecExceptEventReportCmd(CSimuD_SExceptEventReportCmd *pCmd);/*�����쳣�¼��ϱ�����*/
	virtual BOOL ExecEnableSimuMonitorCmd(CSimuD_SEnableSimuMonitorCmd *pCmd);/*����ʹ�ܼ�������*/
	virtual BOOL ExecEnableSimuMonitorRespCmd(CSimuD_SEnableSimuMonitorRespCmd *pCmd);/*����ʹ�ܼ�����Ӧ����*/
};
