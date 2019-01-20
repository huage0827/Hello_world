#ifndef SIMULINKM_CCMD_H_
#define SIMULINKM_CCMD_H_

typedef enum euM_C_CMD
{
	M_C_CMD_Unknown				= -1 ,		  /* 未知命令*/
	M_C_CMD_ExecSimuModel		= 0x30000001, /* C->M 执行仿真模型请求 */
	M_C_CMD_PauseSimuModel		= 0x30000002, /* C->M 暂停仿真模型请求 */
	M_C_CMD_ResetSimuModel		= 0x30000003, /* C->M 重置仿真模型请求 */
	M_C_CMD_SetModelParam		= 0x30000004, /* C->M 设置模型参数请求 */

	M_C_CMD_RegisterModel		= 0xB0000001, /* M->C 注册仿真模型请求 */
	M_C_CMD_TestEventReport		= 0xB0000002, /* M->C 测试事件上报 */
	M_C_CMD_ExceptEventReport	= 0xB0000003, /* M->C 异常上报 */
	M_C_CMD_DispModelData		= 0xB0000004, /* M->C 显示模型数据*/
//	M_C_CMD_DispStatusData		= 0xB0000005, /* M->C 显示状态数据*/
	M_C_CMD_DispUserData		= 0xB0000006, /* M->C 显示用户数据*/
	M_C_CMD_StorModelData		= 0xB0000007, /* M->C 存储模型数据*/
	M_C_CMD_StorUserData		= 0xB0000008, /* M->C 存储用户数据*/
	M_C_CMD_DispAndStorData		= 0xB0000009,
	M_C_CMD_NonRealTimeRunAdust = 0xB000000A, /* M->C 非实时运行微调*/
	M_C_CMD_BUFFERFULL			= 0xB000000B,
	M_C_CMD_UserPauseModel      = 0xB000000C, /* M->C 用户暂停模型*/
	M_C_CMD_UserStopModel       = 0xB000000D, /* M->C 用户停止模型*/
} M_C_CMD;

#endif /* SIMULINKM_CCMD_H_ */
