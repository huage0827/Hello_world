#pragma  once
#include "SimulinkCfgDefine.h"

/****************************************************************
 * 结构体名 : SimulinkModel_t
 * 说明 : 仿真模型的公用参数定义
 ****************************************************************/
typedef struct tagSimulinkModel
{
	int  nID;				       /* 模型编号:子模型ID*/
	unsigned long nExecMode;	   /* 执行模式*/
	char bPause;				   /* 是否暂停运行 */
	char bQuit;					   /* 是否停止运行 */
	unsigned long nStepTime;	   /* 步长间隔时间, 单位纳秒 */
	unsigned long nDispRate;	   /* 监控采样率*/
	unsigned long nRecordRate;	   /* 显示采样率*/
	unsigned long nCurStepCount;   /* 当前第几步 */
	unsigned long nOverRunCount;   /* 总超时数 */
}SimulinkModel_t , *PSimulinkModel_t;


extern SimulinkConfig_t g_SimuCfg;	 /*仿真配置*/
extern SimulinkModel_t g_SimuModel;  /*仿真模型*/
