/*****************************************************************
 * 文件名: SimuCommunicationDef.h
 * 功能: 模块间通信公共定义(与模型相关)
 *****************************************************************/
#pragma  once

#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
#include <pthread.h>
#endif

/******************************************************************
 * 结构体名 : CommuSimuM_CShm_t
 * 说明 : M_C通信内存接口
 ******************************************************************/
#define MAX_MC_SHM_QUEUE_LEN 1024
#define MAX_MC_SHM_BUFF_LEN	 1024*2
typedef struct TagCommuSimuM_CShm
{
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	pthread_mutexattr_t attr;	/* 互斥锁属性：进程共享 */
	pthread_mutex_t	mutex;	/* 互斥锁：操作共享内存之前需加锁 */
#endif
	int	ReadDataPtr;/* 读下标*/
	int	WriteDataPtr;/* 写下标*/
	unsigned long	nCoverCount; /*写覆盖次数*/
	unsigned long	nDataLen[MAX_MC_SHM_QUEUE_LEN];/* 数据长度 */
	char	szData[MAX_MC_SHM_QUEUE_LEN][MAX_MC_SHM_BUFF_LEN]; /* 数据区 */
}CommuSimuM_CShm_t;

/******************************************************************
 * 结构体名 : CommuSimuM_MShm_t
 * 说明 : M_M通信接口
 ******************************************************************/
//#define MAX_MM_SHM_QUEUE_LEN 4
#define MAX_MM_SHM_QUEUE_LEN 1024
#define MAX_MM_SHM_BUFF_LEN	 1024*2
typedef struct TagCoummSimuM_MShm{
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	pthread_mutexattr_t attr;	/* 互斥锁属性：进程共享 */
	pthread_mutex_t	mutex;	/* 互斥锁：操作共享内存之前需加锁 */
#endif
	int	ReadDataPtr; /*读下标*/
	int	WriteDataPtr; /*写下标*/
	unsigned long	nCoverCount; /*写覆盖次数*/
	unsigned long	nDataLen[MAX_MM_SHM_QUEUE_LEN];/* 数据长度 */
	char	szData[MAX_MM_SHM_QUEUE_LEN][MAX_MM_SHM_BUFF_LEN]; /* 数据区 */
}CommuSimuM_MShm_t;


