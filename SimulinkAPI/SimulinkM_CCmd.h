#ifndef SIMULINKM_CCMD_H_
#define SIMULINKM_CCMD_H_

typedef enum euM_C_CMD
{
	M_C_CMD_Unknown				= -1 ,		  /* δ֪����*/
	M_C_CMD_ExecSimuModel		= 0x30000001, /* C->M ִ�з���ģ������ */
	M_C_CMD_PauseSimuModel		= 0x30000002, /* C->M ��ͣ����ģ������ */
	M_C_CMD_ResetSimuModel		= 0x30000003, /* C->M ���÷���ģ������ */
	M_C_CMD_SetModelParam		= 0x30000004, /* C->M ����ģ�Ͳ������� */

	M_C_CMD_RegisterModel		= 0xB0000001, /* M->C ע�����ģ������ */
	M_C_CMD_TestEventReport		= 0xB0000002, /* M->C �����¼��ϱ� */
	M_C_CMD_ExceptEventReport	= 0xB0000003, /* M->C �쳣�ϱ� */
	M_C_CMD_DispModelData		= 0xB0000004, /* M->C ��ʾģ������*/
//	M_C_CMD_DispStatusData		= 0xB0000005, /* M->C ��ʾ״̬����*/
	M_C_CMD_DispUserData		= 0xB0000006, /* M->C ��ʾ�û�����*/
	M_C_CMD_StorModelData		= 0xB0000007, /* M->C �洢ģ������*/
	M_C_CMD_StorUserData		= 0xB0000008, /* M->C �洢�û�����*/
	M_C_CMD_DispAndStorData		= 0xB0000009,
	M_C_CMD_NonRealTimeRunAdust = 0xB000000A, /* M->C ��ʵʱ����΢��*/
	M_C_CMD_BUFFERFULL			= 0xB000000B,
	M_C_CMD_UserPauseModel      = 0xB000000C, /* M->C �û���ͣģ��*/
	M_C_CMD_UserStopModel       = 0xB000000D, /* M->C �û�ֹͣģ��*/
} M_C_CMD;

#endif /* SIMULINKM_CCMD_H_ */
