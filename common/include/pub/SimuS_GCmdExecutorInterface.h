/*******************************************************************
 * �ļ���: SimuS_GCmdExecutorInterface.h
 * ����: SimuService��SimuGuard֮���ͨ������ִ�����ӿڶ���
 ******************************************************************/
#pragma once

#ifndef WIN_PLATFORM
#include "stdafx.h"
#endif

#include "SimuS_GCmd.h"

/****************************************************************
 * ���� : CSimuS_GCmdExecutorInterface
 * ���� : ��
 * ˵�� : SimuService��SimuGuard֮��ͨ������ִ��������
 ****************************************************************/
class CSimuS_GCmdExecutorInterface
{
public:
	/*�������ģ������*/
	virtual BOOL ExecLoadSimuModelCmd(CSimuS_GLoadSimuModelCmd* pCmd);	
	/*�������ģ����Ӧ����*/
	virtual BOOL ExecLoadSimuModelRespCmd(CSimuS_GLoadSimuModelRespCmd* pCmd);	
	/*����ִ��ģ������*/
	virtual BOOL ExecExecSimuModelCmd(CSimuS_GExecSimuModelCmd* pCmd);	
	/*����ִ��ģ����Ӧ����*/
	virtual BOOL ExecExecSimuModelRespCmd(CSimuS_GExecSimuModelRespCmd* pCmd);	
	/*������ͣģ������*/
	virtual BOOL ExecPauseSimuModelCmd(CSimuS_GPauseSimuModelCmd* pCmd);	
	/*������ͣģ����Ӧ����*/
	virtual BOOL ExecPauseSimuModelRespCmd(CSimuS_GPauseSimuModelRespCmd* pCmd);	
	/*��������ģ������*/
	virtual BOOL ExecResetSimuModelCmd(CSimuS_GResetSimuModelCmd* pCmd);
	/*��������ģ����Ӧ����*/
	virtual BOOL ExecResetSimuModelRespCmd(CSimuS_GResetSimuModelRespCmd* pCmd);	
	/*��������ģ�Ͳ�������*/
	virtual BOOL ExecSetModelParamCmd(CSimuS_GSetSimuModelParamCmd* pCmd);	
	/*��������ģ�Ͳ�����Ӧ����*/
	virtual BOOL ExecSetModelParamRespCmd(CSimuS_GSetSimuModelParamRespCmd* pCmd);
	/*��������¼��ϱ�����*/
	virtual BOOL ExecTestEventReportCmd(CSimuS_GTestEventReportCmd* pCmd);	
	/*�����쳣�¼��ϱ�����*/
	virtual BOOL ExecExceptEventReportCmd(CSimuS_GExceptEventReportCmd* pCmd);
	/*ִ�б���ģ������*/
	virtual BOOL ExecCompileSimuModelCmd(CSimuS_GCompileSimuModelCmd *pCmd);
	/*ִ�б���ģ����Ӧ����*/
	virtual BOOL ExecCompileSimuModelRespCmd(CSimuS_GCompileSimuModelRespCmd *pCmd);
	/*ִ�б�����Ϣ�ϱ�����*/
	virtual BOOL ExecCompileInfoReportCmd(CSimuS_GCompileInfoRepCmd *pCmd); 
};
