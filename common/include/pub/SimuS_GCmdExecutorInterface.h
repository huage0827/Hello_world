/*******************************************************************
 * 文件名: SimuS_GCmdExecutorInterface.h
 * 功能: SimuService与SimuGuard之间的通信命令执行器接口定义
 ******************************************************************/
#pragma once

#ifndef WIN_PLATFORM
#include "stdafx.h"
#endif

#include "SimuS_GCmd.h"

/****************************************************************
 * 类名 : CSimuS_GCmdExecutorInterface
 * 父类 : 无
 * 说明 : SimuService和SimuGuard之间通信命令执行器父类
 ****************************************************************/
class CSimuS_GCmdExecutorInterface
{
public:
	/*处理加载模型命令*/
	virtual BOOL ExecLoadSimuModelCmd(CSimuS_GLoadSimuModelCmd* pCmd);	
	/*处理加载模型响应命令*/
	virtual BOOL ExecLoadSimuModelRespCmd(CSimuS_GLoadSimuModelRespCmd* pCmd);	
	/*处理执行模型命令*/
	virtual BOOL ExecExecSimuModelCmd(CSimuS_GExecSimuModelCmd* pCmd);	
	/*处理执行模型响应命令*/
	virtual BOOL ExecExecSimuModelRespCmd(CSimuS_GExecSimuModelRespCmd* pCmd);	
	/*处理暂停模型命令*/
	virtual BOOL ExecPauseSimuModelCmd(CSimuS_GPauseSimuModelCmd* pCmd);	
	/*处理暂停模型响应命令*/
	virtual BOOL ExecPauseSimuModelRespCmd(CSimuS_GPauseSimuModelRespCmd* pCmd);	
	/*处理重置模型命令*/
	virtual BOOL ExecResetSimuModelCmd(CSimuS_GResetSimuModelCmd* pCmd);
	/*处理重置模型响应命令*/
	virtual BOOL ExecResetSimuModelRespCmd(CSimuS_GResetSimuModelRespCmd* pCmd);	
	/*处理设置模型参数命令*/
	virtual BOOL ExecSetModelParamCmd(CSimuS_GSetSimuModelParamCmd* pCmd);	
	/*处理设置模型参数响应命令*/
	virtual BOOL ExecSetModelParamRespCmd(CSimuS_GSetSimuModelParamRespCmd* pCmd);
	/*处理测试事件上报命令*/
	virtual BOOL ExecTestEventReportCmd(CSimuS_GTestEventReportCmd* pCmd);	
	/*处理异常事件上报命令*/
	virtual BOOL ExecExceptEventReportCmd(CSimuS_GExceptEventReportCmd* pCmd);
	/*执行编译模型命令*/
	virtual BOOL ExecCompileSimuModelCmd(CSimuS_GCompileSimuModelCmd *pCmd);
	/*执行编译模型响应命令*/
	virtual BOOL ExecCompileSimuModelRespCmd(CSimuS_GCompileSimuModelRespCmd *pCmd);
	/*执行编译信息上报命令*/
	virtual BOOL ExecCompileInfoReportCmd(CSimuS_GCompileInfoRepCmd *pCmd); 
};
