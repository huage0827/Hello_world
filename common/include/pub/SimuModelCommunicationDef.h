/*****************************************************************
 * �ļ���: SimuCommunicationDef.h
 * ����: ģ���ͨ�Ź�������(��ģ�����)
 *****************************************************************/
#pragma  once

#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
#include <pthread.h>
#endif

/******************************************************************
 * �ṹ���� : CommuSimuM_CShm_t
 * ˵�� : M_Cͨ���ڴ�ӿ�
 ******************************************************************/
#define MAX_MC_SHM_QUEUE_LEN 1024
#define MAX_MC_SHM_BUFF_LEN	 1024*2
typedef struct TagCommuSimuM_CShm
{
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	pthread_mutexattr_t attr;	/* ���������ԣ����̹��� */
	pthread_mutex_t	mutex;	/* �����������������ڴ�֮ǰ����� */
#endif
	int	ReadDataPtr;/* ���±�*/
	int	WriteDataPtr;/* д�±�*/
	unsigned long	nCoverCount; /*д���Ǵ���*/
	unsigned long	nDataLen[MAX_MC_SHM_QUEUE_LEN];/* ���ݳ��� */
	char	szData[MAX_MC_SHM_QUEUE_LEN][MAX_MC_SHM_BUFF_LEN]; /* ������ */
}CommuSimuM_CShm_t;

/******************************************************************
 * �ṹ���� : CommuSimuM_MShm_t
 * ˵�� : M_Mͨ�Žӿ�
 ******************************************************************/
//#define MAX_MM_SHM_QUEUE_LEN 4
#define MAX_MM_SHM_QUEUE_LEN 1024
#define MAX_MM_SHM_BUFF_LEN	 1024*2
typedef struct TagCoummSimuM_MShm{
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	pthread_mutexattr_t attr;	/* ���������ԣ����̹��� */
	pthread_mutex_t	mutex;	/* �����������������ڴ�֮ǰ����� */
#endif
	int	ReadDataPtr; /*���±�*/
	int	WriteDataPtr; /*д�±�*/
	unsigned long	nCoverCount; /*д���Ǵ���*/
	unsigned long	nDataLen[MAX_MM_SHM_QUEUE_LEN];/* ���ݳ��� */
	char	szData[MAX_MM_SHM_QUEUE_LEN][MAX_MM_SHM_BUFF_LEN]; /* ������ */
}CommuSimuM_MShm_t;


