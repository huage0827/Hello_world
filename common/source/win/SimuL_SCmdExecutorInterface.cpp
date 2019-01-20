/*******************************************************************
 * 文件名: SimuL_SCmdExecutorInterface.cpp
 * 功能: SimuLab与SimuService之间的通信命令执行器接口实现
 ******************************************************************/
#include "stdafx.h"
#include "SimuL_SCmdExecutorInterface.h"

BOOL CSimuL_SCmdExecutorInterface::ExecConnectTargetCmd(CSimuL_SConnectTargetCmd* pCmd)
{
	printf("ExecConnectTargetCmd\n");
	return TRUE;
}

BOOL CSimuL_SCmdExecutorInterface::ExecConnectTargetRespCmd(CSimuL_SConnectTargetRespCmd* pCmd)
{
	printf("ExecConnectTargetRespCmd\n");
	return TRUE;
}

BOOL CSimuL_SCmdExecutorInterface::ExecLoadSimuModelCmd(CSimuL_SLoadSimuModelCmd* pCmd)
{
	printf("ExecLoadSimuModelCmd\n");
	return TRUE;
}

BOOL CSimuL_SCmdExecutorInterface::ExecLoadSimuModelRespCmd(CSimuL_SLoadSimuModelRespCmd* pCmd)
{
	printf("ExecLoadSimuModelRespCmd\n");
	return TRUE;
}

BOOL CSimuL_SCmdExecutorInterface::ExecEnableSimuMonitorCmd(CSimuL_SEnableSimuMonitorCmd *pCmd)
{
	printf("ExecEnableSimuMonitorCmd\n");
	return TRUE;
}

BOOL CSimuL_SCmdExecutorInterface::ExecEnableSimuMonitorRespCmd(CSimuL_SEnableSimuMonitorRespCmd *pCmd)
{
	printf("ExecEnableSimuMonitorRespCmd\n");
	return TRUE;
}

BOOL CSimuL_SCmdExecutorInterface::ExecCompileSimuModelCmd(CSimuL_SCompileSimuModelCmd *pCmd)
{
	printf("ExecCompileSimuModelCmd\n");
	return TRUE;	
}

BOOL CSimuL_SCmdExecutorInterface::ExecCompileSimuModelRespCmd(CSimuL_SCompileSimuModelRespCmd *pCmd)
{
	printf("ExecCompileSimuModelRespCmd\n");
	return TRUE;
}

BOOL CSimuL_SCmdExecutorInterface::ExecCompileInfoReportCmd(CSimuL_SCompileInfoRepCmd *pCmd)
{
	printf("ExecCompileInfoReportCmd\n");
	return TRUE;
}