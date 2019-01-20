/*******************************************************************
 * 文件名: SimuD_SCmdExecutorInterface.h
 * 功能: SimuMonitor与SimuService之间的通信命令执行器接口定义
 ******************************************************************/

#pragma once

#include "SimuD_SCmd.h"

/****************************************************************
 * 类名 : CSimuD_SCmdExecutorInferface
 * 父类 : 无
 * 说明 : SimuMonitor和SimuService之间通信命令执行器接口
 ****************************************************************/
class CSimuD_SCmdExecutorInferface
{
public:
	virtual BOOL ExecInitMonitorCmd(CSimuD_SInitMonitorCmd* pCmd);/*处理初始化监控命令*/
	virtual BOOL ExecInitMonitorRespCmd(CSimuD_SInitMonitorRespCmd *pCmd);/*处理初始化监控响应命令*/
	virtual BOOL ExecLoadSimuModelReportCmd(CSimuD_SLoadSimuModelReportCmd *pCmd);/*加载仿真模型上报命令*/
	virtual BOOL ExecExecSimuModelCmd(CSimuD_SExecSimuModelCmd *pCmd);/*处理执行模型命令*/
	virtual BOOL ExecExecSimuModelRespCmd(CSimuD_SExecSimuModelRespCmd *pCmd);/*处理执行模型响应命令*/
	virtual BOOL ExecPauseSimuModelCmd(CSimuD_SPauseSimuModelCmd *pCmd);/*处理暂停模型命令*/
	virtual BOOL ExecPauseSimuModelRespCmd(CSimuD_SPauseSimuModelRespCmd *pCmd);/*处理暂停模型响应命令*/
	virtual BOOL ExecResetSimuModelCmd(CSimuD_SResetSimuModelCmd *pCmd);/*处理重置模型命令*/
	virtual BOOL ExecResetSimuModelRespCmd(CSimuD_SResetSimuModelRespCmd *pCmd);/*处理重置模型响应命令*/
	virtual BOOL ExecSetSimuModelParamCmd(CSimuD_SSetSimuModelParamCmd *pCmd);/*处理设置模型参数命令*/
	virtual BOOL ExecSetSimuModelParamRespCmd(CSimuD_SSetSimuModelParamRespCmd *pCmd);/*处理设置模型参数响应命令*/
	virtual BOOL ExecTestEventReportCmd(CSimuD_STestEventReportCmd *pCmd);/*处理测试事件上报命令*/
	virtual BOOL ExecExceptEventReportCmd(CSimuD_SExceptEventReportCmd *pCmd);/*处理异常事件上报命令*/
	virtual BOOL ExecEnableSimuMonitorCmd(CSimuD_SEnableSimuMonitorCmd *pCmd);/*处理使能监视命令*/
	virtual BOOL ExecEnableSimuMonitorRespCmd(CSimuD_SEnableSimuMonitorRespCmd *pCmd);/*处理使能监视响应命令*/
};
