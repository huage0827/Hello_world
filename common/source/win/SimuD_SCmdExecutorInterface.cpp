/*******************************************************************
 * 文件名: SimuD_SCmdExecutorInterface.cpp
 * 功能: SimuDisplay与SimuService之间的通信命令执行器接口实现
 ******************************************************************/
#include "stdafx.h" 
#include "SimuD_SCmdExecutorInterface.h"

BOOL CSimuD_SCmdExecutorInferface::ExecInitMonitorCmd(CSimuD_SInitMonitorCmd* pCmd)
{
	printf("ExecInitMonitorCmd\n");
	return TRUE;
}

BOOL CSimuD_SCmdExecutorInferface::ExecInitMonitorRespCmd(CSimuD_SInitMonitorRespCmd *pCmd)
{
	printf("ExecInitMonitorRespCmd\n");
	return TRUE;
}

BOOL CSimuD_SCmdExecutorInferface::ExecLoadSimuModelReportCmd(CSimuD_SLoadSimuModelReportCmd *pCmd)
{
	printf("ExecLoadSimuModelReportCmd\n");
	return TRUE;
}

BOOL CSimuD_SCmdExecutorInferface::ExecExecSimuModelCmd(CSimuD_SExecSimuModelCmd *pCmd)
{
	printf("ExecExecSimuModelCmd\n");
	return TRUE;
}

BOOL CSimuD_SCmdExecutorInferface::ExecExecSimuModelRespCmd(CSimuD_SExecSimuModelRespCmd *pCmd)
{
	printf("ExecExecSimuModelRespCmd\n");
	return TRUE;
}

BOOL CSimuD_SCmdExecutorInferface::ExecPauseSimuModelCmd(CSimuD_SPauseSimuModelCmd *pCmd)
{
	printf("ExecPauseSimuModelCmd\n");
	return TRUE;
}

BOOL CSimuD_SCmdExecutorInferface::ExecPauseSimuModelRespCmd(CSimuD_SPauseSimuModelRespCmd *pCmd)
{
	printf("ExecPauseSimuModelRespCmd\n");
	return TRUE;
}

BOOL CSimuD_SCmdExecutorInferface::ExecResetSimuModelCmd(CSimuD_SResetSimuModelCmd *pCmd)
{
	printf("ExecResetSimuModelCmd\n");
	return TRUE;
}

BOOL CSimuD_SCmdExecutorInferface::ExecResetSimuModelRespCmd(CSimuD_SResetSimuModelRespCmd *pCmd)
{
	printf("ExecResetSimuModelRespCmd\n");
	return TRUE;
}

BOOL CSimuD_SCmdExecutorInferface::ExecSetSimuModelParamCmd(CSimuD_SSetSimuModelParamCmd *pCmd)
{
	printf("ExecSetSimuModelParamCmd\n");
	return TRUE;
}

BOOL CSimuD_SCmdExecutorInferface::ExecSetSimuModelParamRespCmd(CSimuD_SSetSimuModelParamRespCmd *pCmd)
{
	printf("ExecSetSimuModelParamRespCmd\n");
	return TRUE;
}

BOOL CSimuD_SCmdExecutorInferface::ExecTestEventReportCmd(CSimuD_STestEventReportCmd *pCmd)
{
	printf("ExecTestEventReportCmd\n");
	return TRUE;
}

BOOL CSimuD_SCmdExecutorInferface::ExecExceptEventReportCmd(CSimuD_SExceptEventReportCmd *pCmd)
{
	printf("ExecExceptEventReportCmd\n");
	return TRUE;
}

BOOL CSimuD_SCmdExecutorInferface::ExecEnableSimuMonitorCmd(CSimuD_SEnableSimuMonitorCmd *pCmd)
{
	printf("ExecEnableSimuMonitorCmd\n");
	return TRUE;
}

BOOL CSimuD_SCmdExecutorInferface::ExecEnableSimuMonitorRespCmd(CSimuD_SEnableSimuMonitorRespCmd *pCmd)
{
	printf("ExecEnableSimuMonitorRespCmd\n");
	return TRUE;
}

