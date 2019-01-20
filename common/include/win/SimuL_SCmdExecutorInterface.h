/*******************************************************************
 * 文件名: SimuL_SCmdExecutorInterface.h
 * 功能: SimuLab与SimuService之间的通信命令执行器接口定义
 ******************************************************************/

#pragma once

#include "SimuL_SCmd.h"

/****************************************************************
 * 类名 : CSimuL_SCmdExecutorInterface
 * 父类 : 无
 * 说明 : SimuLab和SimuService之间通信命令执行器接口
 ****************************************************************/
class CSimuL_SCmdExecutorInterface
{
public:
	virtual BOOL ExecConnectTargetCmd(CSimuL_SConnectTargetCmd* pCmd);		/* 执行连接下位机请求命令 */
	virtual BOOL ExecConnectTargetRespCmd(CSimuL_SConnectTargetRespCmd* pCmd);/* 执行连接下位机响应命令 */
	virtual BOOL ExecLoadSimuModelCmd(CSimuL_SLoadSimuModelCmd* pCmd);			/* 执行加载仿真模型请求命令 */
	virtual BOOL ExecLoadSimuModelRespCmd(CSimuL_SLoadSimuModelRespCmd* pCmd);	/* 执行加载仿真模型响应命令 */
	virtual BOOL ExecEnableSimuMonitorCmd(CSimuL_SEnableSimuMonitorCmd *pCmd); /*执行使能显示监控界面命令*/
	virtual BOOL ExecEnableSimuMonitorRespCmd(CSimuL_SEnableSimuMonitorRespCmd *pCmd);/*执行使能显示监控界面响应命令*/
	virtual BOOL ExecCompileSimuModelCmd(CSimuL_SCompileSimuModelCmd *pCmd); /*执行编译模型命令*/
	virtual BOOL ExecCompileSimuModelRespCmd(CSimuL_SCompileSimuModelRespCmd *pCmd); /*执行编译模型响应命令*/
	virtual BOOL ExecCompileInfoReportCmd(CSimuL_SCompileInfoRepCmd *pCmd); /*执行编译信息上报命令*/
};
