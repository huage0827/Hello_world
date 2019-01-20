/*****************************************************************
 * �ļ���: SimuCommunicationDef.h
 * ����: ģ���ͨ�Ź�������
 *****************************************************************/
#pragma  once

#include <vector>
#include "SimuCommuAddrDef.h"
#include "SimuErrCodeDef.h"
#include "SimuModelCommunicationDef.h"

using namespace std;

/******************************************************************
 * ö���� : CompileInfoReportType_e
 * ˵�� : ������λ������
 ******************************************************************/
typedef enum EnumCommConnectTargetType{
	Connect_Target_Type_CompileModel = 1,
	Connect_Target_Type_ExecModel = 2,
}CommConnectTargetType_e;

/******************************************************************
 * �ṹ���� : CommuModelTargetDir_t
 * ˵�� : ģ��Ŀ���·��
 ******************************************************************/
typedef struct TagCommuModelTargetDir{
	unsigned long nModelId;
	unsigned short usModelDirLen;
	char szModelDir[256];
}CommuModelTargetDir_t;

/******************************************************************
 * �ṹ���� : CommuModelCompileParam_t
 * ˵�� : ģ��Ŀ���·��
 ******************************************************************/
typedef struct TagCommuModelCompileParam{
	unsigned short usModelCfgPathLen;
	char szModelCfgPath[256];
	vector<CommuModelTargetDir_t *> vecModelDir;
}CommuModelCompileParam_t;

/******************************************************************
 * ö���� : CompileInfoReportType_e
 * ˵�� : ������Ϣ�ϱ�����
 ******************************************************************/
typedef enum EnumCompileInfoReportType{
	CompileInfoRep_Type_Begin = 1,
	CompileInfoRep_Type_Normal = 2,
	CompileInfoRep_Type_Error = 3,
	CompileInfoRep_Type_Warn = 4,
	CompileInfoRep_Type_End = 5,
}CompileInfoReportType_e;

/******************************************************************
 * �ṹ���� : CommuModelProgFilePath_t
 * ˵�� : ���ط���ģ������ʹ��
 ******************************************************************/
typedef struct TagCommuModelProgFilePath{
	unsigned long  nModelId;
	unsigned short nFilePathLen;
	unsigned char  szFilePath[256];
}CommuModelProgFilePath_t;

/******************************************************************
 * �ṹ���� : CommuLoadSimuModelParam_t
 * ˵�� : ���ط���ģ������ʹ��
 ******************************************************************/
typedef struct TagCommuLoadSimuModelParam{
	unsigned char  szMonitorIp[4];
	unsigned short usPort;
	unsigned char  bDebugMode;
	unsigned short usDebugParamLen;
	unsigned char  szDebugParam[256];
	unsigned short usModelConfPathLen;
	unsigned char  szModelConfPath[256];
	unsigned short usDriverLibPathLen;
	unsigned char  szDriverLibPath[256];
	unsigned long  nModelCount;
	std::vector<CommuModelProgFilePath_t *> vecModelFilePath;
}CommuLoadSimuModelParam_t;

/******************************************************************
 * �ṹ���� : CommuSetModelParam_t
 * ˵�� : ����ģ�Ͳ�������ʹ��
 ******************************************************************/
#define COMMU_MODEL_PARAM_NAME_LEN  64
typedef struct TagCommuSetModelParam{
	unsigned long nModelID;
	unsigned char szParamName[COMMU_MODEL_PARAM_NAME_LEN + 1];
	unsigned long nParamId;
	unsigned long nParamValueLen;
	unsigned char szParamValue[1024];
}CommuSetModelParam_t;

/******************************************************************
 * �ṹ���� : CommuTestEventReport_t
 * ˵�� : �����¼��ϱ�����ʹ��
 ******************************************************************/
typedef struct TagCommuTestEventReport{
	unsigned long nReportType;
	unsigned long nTestTaskId;
	unsigned long nTestCaseId;
	unsigned long nEventValueLen;
	unsigned char szEventValue[1024];
}CommuTestEventReport_t;

/******************************************************************
 * ö���� : ExceptReportType_e
 * ˵�� : �쳣�¼��ϱ�����
 ******************************************************************/
typedef enum EnumExceptReportType{
	CEXCEPT_RepType_Notice = 1,
	CEXCEPT_RepType_Warn = 2,
	CEXCEPT_RepType_Error = 3,
	CEXCEPT_RepType_UserPauseRun = 4,
	CEXCEPT_RepType_UserStopRun = 5,
}ExceptReportType_e;

/******************************************************************
 * �ṹ���� : CommuTestEventReport_t
 * ˵�� : �����¼��ϱ�����ʹ��
 ******************************************************************/
typedef struct TagCommuExceptReport{
	unsigned long nReportType;
	unsigned long nReportLen;
	unsigned char szReportValue[1024];
}CommuExceptReport_t;

/******************************************************************
 * �ṹ���� : CommuStorModelDataUnit_t
 * ˵�� : �洢ģ�����ݵ�Ԫ����
 ******************************************************************/
typedef struct TagCommuStorModelDataUnit{
	unsigned long  nStep;			/* ����*/
	unsigned char  bDataType;		/* ��������*/
	union{
		unsigned long  nGroupIdx;	
		char szRecordFile[256];
	}u;
	unsigned long  nTimeStampSec;	/* unixʱ�� �� */
	unsigned long  nTimeStampMsec;	/* unixʱ�� ΢��*/
	unsigned long  nDataLen;		/* ���ݳ��ȡ�*/
	unsigned char  *pData;			/* ���ݴ�ŵ�ַ */
}CommuStorModelDataUnit_t;

/******************************************************************
 * �ṹ���� : CommuTargetInfo_t
 * ˵�� : ��λ���忨��Ϣ����
 ******************************************************************/
typedef struct TagCommuTargetIOCardInfo{
	unsigned short  usVendorID;
	unsigned short  usDeviceID;
	unsigned int  ulClassCode;
	char szVendorID[16];
	char szDeviceID[16];
	char szClassCode[16];
	unsigned int ulCardNum;
	char szVendor[64];
	char szDevice[64];
	char szClass[64];
}CommuTargetIOCardInfo_t;

/******************************************************************
 * �ṹ���� : CommuTargetInfo_t
 * ˵�� : ��λ����Ϣ����
 ******************************************************************/
typedef struct TagCommuTargetInfo{
	char szTargetName[128];
	char szMac[128];
	char szPlatform[128];
	char szOsVersion[128];
	char szSimuLabVer[128];
	char szSysArch[128];
	unsigned long nCpuNum;
	unsigned long nCpuSpeed;
	unsigned long nFreeDisk;
	unsigned long nMemSize;
	//
	unsigned long nIOCardCount;
	std::vector<CommuTargetIOCardInfo_t *> vecIOCard;
}CommuTargetInfo_t;

/******************************************************************
 * �ṹ���� : CommuStorDataC_DShm_t
 * ˵�� : StorDataC_Dͨ�Žӿ�
 ******************************************************************/
#define MAX_SDCD_SHM_QUEUE_LEN	1024
#define MAX_SDCD_SHM_BUFF_LEN	(KL_COMMU_STOR_DATA_SEND_MAX_DATA_UNIT)
typedef struct TagCoummSimuStorDataC_DShm{
	int	ReadDataPtr; /*���±�*/
	int	WriteDataPtr; /*д�±�*/
	unsigned long	nCoverCount; /*д���Ǵ���*/
	unsigned long	nDataLen[MAX_SDCD_SHM_QUEUE_LEN];/* ���ݳ��� */
	char	szData[MAX_SDCD_SHM_QUEUE_LEN][MAX_SDCD_SHM_BUFF_LEN]; /* ������ */
}CommuSimuStorDataC_DShm_t;



