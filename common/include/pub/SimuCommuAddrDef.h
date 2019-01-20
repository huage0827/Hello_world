/*****************************************************************
 * �ļ���: SimuCommuAddrDef.h
 * ����: ģ���ͨ�ŵ�ַ����
 *****************************************************************/
#pragma  once

//#include "Platform.h"

/******************************************************************
 * ͨ�Žӿ�: 1-> SimuL_SComm�ӿ�
 * ��ƣ�L_S
 * ��ʽ��TCPͨ��
 * ˵����������ΪSimuService,�ͻ���ΪSimuLab,���Ϳ�������
 ******************************************************************/
#define KL_COMMU_L_S_TCP_PORT  2001

/******************************************************************
 * ͨ�Žӿ�: 2-> SimuD_SComm�ӿ�
 * ��ƣ�D_S
 * ��ʽ��TCPͨ��
 * ˵����������ΪSimuService,�ͻ���ΪSimuMonitor,���Ϳ�������
 ******************************************************************/
#define KL_COMMU_D_S_TCP_PORT  2002

/******************************************************************
 * ͨ�Žӿ�: 3-> SimuS_GComm�ӿ�
 * ��ƣ�S_G
 * ��ʽ��TCPͨ��
 * ˵����������ΪSimuGuard,�ͻ���ΪSimuService,���Ϳ�������
 ******************************************************************/
#define KL_COMMU_S_G_TCP_PORT  2003

/******************************************************************
 * ͨ�Žӿ�: 4-> SimuDispDataC_DComm�ӿ�
 * ��ƣ�DispDataC_D
 * ��ʽ��UDPͨ��
 * ˵����������ΪSimuMonitor,�ͻ���ΪSimuService��SimuGuard
 ******************************************************************/
#define KL_COMMU_DISP_DATA_C_D_UDP_PORT  2004

/******************************************************************
 * ͨ�Žӿ�: 5-> SimuStorDataC_DComm�ӿ�
 * ��ƣ�StorDataC_D
 * ��ʽ��TCPͨ��
 * ˵����������ΪSimuMonitor,�ͻ���ΪSimuService��SimuGuard
   ���ݴ��䣺
   ��·�� => ����䵥Ԫ 1500
   ����� => MTU 1480
   ����� => UPD MTU = 1472
			IP���ݰ���󳤶�Ϊ64K �� 65535
 ******************************************************************/
#ifdef WIN_PLATFORM 
#define KL_COMMU_M_C_STOR_DATA_SHM_NAME "tmp_klcommu_StorDataC_D_shm_name"
#define KL_COMMU_M_C_STOR_DATA_LOCK_NAME "tmp_klcommu_StorDataC_D_sem_lock"
#define KL_COMMU_M_C_STOR_DATA_SEM_SIG	"tmp_klcommu_StorDataC_D_sem_signal"
#endif

#define KL_COMMU_STOR_DATA_C_D_TCP_PORT  2005
#define KL_COMMU_STOR_DATA_SEND_MAX_DATA_UNIT 1024*40 // 40K

/******************************************************************
 * ͨ�Žӿ�: 6-> SimuM_CComm�ӿ�
 * ��ƣ�M_C
 * ��ʽ�������ڴ�+�ź���
 * ˵����������ΪSimuGuard��SimuService,�ͻ���ΪSimuModel,
 * 		���Ϳ�������ɼ�ģ�����ݡ�
 ******************************************************************/
#if defined(QNX_PLATFORM)
#define KL_COMMU_M_C_MREAD_SHM_NAME "/tmp/klcommu_MC_Mread_shm_path_%ld"
#define KL_COMMU_M_C_MWRITE_SHM_NAME "/tmp/klcommu_MC_Mwrite_shm_path_%ld"
#define KL_COMMU_M_C_MWRITE_SEM_NAME "/tmp/klcommu_MC_Mwrite_sem_path_%ld"

#elif defined(LINUX_PLATFORM)
#define KL_COMMU_M_C_MREAD_SHM_NAME "tmp_klcommu_MC_Mread_shm_path_%ld"
#define KL_COMMU_M_C_MWRITE_SHM_NAME "tmp_klcommu_MC_Mwrite_shm_path_%ld"
#define KL_COMMU_M_C_MWRITE_SEM_NAME "tmp_klcommu_MC_Mwrite_sem_path_%ld"

#elif defined(WIN_PLATFORM)
#define KL_COMMU_M_C_MREAD_SHM_NAME "tmp_klcommu_MC_Mread_shm_path_%ld"
#define KL_COMMU_M_C_MWRITE_SHM_NAME "tmp_klcommu_MC_Mwrite_shm_path_%ld"
#define KL_COMMU_M_C_MWRITE_SEM_NAME "tmp_klcommu_MC_Mwrite_sem_path_%ld"
#define KL_COMMU_M_C_MREAD_SHM_LOCK_NAME "tmp_klcommu_MC_Mread_shm_lock_%ld"
#define KL_COMMU_M_C_MWRITE_SHM_LOCK_NAME "tmp_klcommu_MC_Mwrite_shm_lock_%ld"
#else

#endif

/******************************************************************
 * ͨ�Žӿ�: 7.SimuDataM_MComm�ӿ�
 * ��ƣ�DataM_M
 * ��ʽ�������ڴ�+�ź���
 * ˵��������ͬ������һ��%ldΪ���ݷ���ģ��id���ڶ���%ldΪ���ݽ���ģ��id
 ******************************************************************/
#if defined(QNX_PLATFORM)
#define KL_COMMU_DataM_M_shm_name "/tmp/klcommu_DataM_M_shm_M%ld_to_M%ld"

#elif defined(LINUX_PLATFORM)
#define KL_COMMU_DataM_M_shm_name "tmp_klcommu_DataM_M_shm_M%ld_to_M%ld"

#elif defined(WIN_PLATFORM)
#define KL_COMMU_DataM_M_shm_name "tmp_klcommu_DataM_M_shm_M%ld_to_M%ld"
#define KL_COMMU_DataM_M_shm_lock "tmp_klcommu_DataM_M_shm_lock_M%ld_to_M%ld"

#endif

/******************************************************************
 * ͨ�Žӿ�: 8.SimuClockM_MComm�ӿ�
 * ��ƣ�ClockM_M
 * ��ʽ���ź���
 * ˵����ʱ��ͬ��������ģ�ͷ����źţ�����ģ�͵ȴ��źţ�%ld->����ģ��ģ��id
 ******************************************************************/
#if defined(QNX_PLATFORM)
#define KL_COMMU_ClockM_M_sem_name "/tmp/klcommu_ClockM_M_sem_name_%d"

#elif defined(LINUX_PLATFORM)
#define KL_COMMU_ClockM_M_sem_name "tmp_klcommu_ClockM_M_sem_name_%d"

#elif defined(WIN_PLATFORM)
#define KL_COMMU_ClockM_M_sem_name "tmp_klcommu_ClockM_M_sem_name_%d"
#define KL_COMMU_ClockM_M_stepExec_shmName "tmp_klcommu_ClockM_M_stepExec_shmName"
#define KL_COMMU_ClockM_M_stepExec_semName "tmp_klcommu_ClockM_M_stepExec_semName"

#else

#endif

/******************************************************************
 * ͨ�Žӿ�: 9-> SimuL_TComm�ӿ�
 * ��ƣ�L_T
 * ��ʽ��UDPͨ��
 * ˵����������ΪSimuGuard,�ͻ���ΪSimuLab
 ******************************************************************/
#define KL_COMMU_L_T_UDP_PORT  2009


/******************************************************************
 * ͨ�Žӿ�: 10-> SimuDebugL_GComm�ӿ�
 * ��ƣ�DebugL_G
 * ��ʽ��TCPͨ��
 * ˵����������Ϊgdbserver,�ͻ���ΪSimuLab(gdb)
 ******************************************************************/
#define KL_COMMU_DEBUG_L_G_TCP_BASE_PORT  2010
#define KL_COMMU_DEBUG_L_G_TCP_PORT(nModelId) (nModelId + KL_COMMU_DEBUG_L_G_TCP_BASE_PORT)


/******************************************************************
 * ͨ�Žӿ�: 
 * ��ƣ�
 * ��ʽ��
 * ˵����
 ******************************************************************/
#define KL_COMMU_SINGLE_INST_NAME_SimuService "kl_commu_single_inst_name_simuservice"
#define KL_COMMU_SINGLE_INST_NAME_SimuMonitor "kl_commu_single_inst_name_simumonitor"

#define KL_APP_NAME_SimuService "SimuService.exe"
#define KL_APP_NAME_SimuMonitor "SimuMonitor.exe"
