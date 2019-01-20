/*******************************************************************
 * �ļ���: SimuL_SCmdExecutorInterface.h
 * ����: SimuLab��SimuService֮���ͨ������ִ�����ӿڶ���
 ******************************************************************/

#pragma once

#include "SimuL_SCmd.h"

/****************************************************************
 * ���� : CSimuL_SCmdExecutorInterface
 * ���� : ��
 * ˵�� : SimuLab��SimuService֮��ͨ������ִ�����ӿ�
 ****************************************************************/
class CSimuL_SCmdExecutorInterface
{
public:
	virtual BOOL ExecConnectTargetCmd(CSimuL_SConnectTargetCmd* pCmd);		/* ִ��������λ���������� */
	virtual BOOL ExecConnectTargetRespCmd(CSimuL_SConnectTargetRespCmd* pCmd);/* ִ��������λ����Ӧ���� */
	virtual BOOL ExecLoadSimuModelCmd(CSimuL_SLoadSimuModelCmd* pCmd);			/* ִ�м��ط���ģ���������� */
	virtual BOOL ExecLoadSimuModelRespCmd(CSimuL_SLoadSimuModelRespCmd* pCmd);	/* ִ�м��ط���ģ����Ӧ���� */
	virtual BOOL ExecEnableSimuMonitorCmd(CSimuL_SEnableSimuMonitorCmd *pCmd); /*ִ��ʹ����ʾ��ؽ�������*/
	virtual BOOL ExecEnableSimuMonitorRespCmd(CSimuL_SEnableSimuMonitorRespCmd *pCmd);/*ִ��ʹ����ʾ��ؽ�����Ӧ����*/
	virtual BOOL ExecCompileSimuModelCmd(CSimuL_SCompileSimuModelCmd *pCmd); /*ִ�б���ģ������*/
	virtual BOOL ExecCompileSimuModelRespCmd(CSimuL_SCompileSimuModelRespCmd *pCmd); /*ִ�б���ģ����Ӧ����*/
	virtual BOOL ExecCompileInfoReportCmd(CSimuL_SCompileInfoRepCmd *pCmd); /*ִ�б�����Ϣ�ϱ�����*/
};
