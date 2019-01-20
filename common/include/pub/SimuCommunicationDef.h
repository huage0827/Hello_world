/*****************************************************************
 * 文件名: SimuCommunicationDef.h
 * 功能: 模块间通信公共定义
 *****************************************************************/
#pragma  once

#include <vector>
#include "SimuCommuAddrDef.h"
#include "SimuErrCodeDef.h"
#include "SimuModelCommunicationDef.h"

using namespace std;

/******************************************************************
 * 枚举名 : CompileInfoReportType_e
 * 说明 : 连接下位机类型
 ******************************************************************/
typedef enum EnumCommConnectTargetType{
	Connect_Target_Type_CompileModel = 1,
	Connect_Target_Type_ExecModel = 2,
}CommConnectTargetType_e;

/******************************************************************
 * 结构体名 : CommuModelTargetDir_t
 * 说明 : 模型目标机路径
 ******************************************************************/
typedef struct TagCommuModelTargetDir{
	unsigned long nModelId;
	unsigned short usModelDirLen;
	char szModelDir[256];
}CommuModelTargetDir_t;

/******************************************************************
 * 结构体名 : CommuModelCompileParam_t
 * 说明 : 模型目标机路径
 ******************************************************************/
typedef struct TagCommuModelCompileParam{
	unsigned short usModelCfgPathLen;
	char szModelCfgPath[256];
	vector<CommuModelTargetDir_t *> vecModelDir;
}CommuModelCompileParam_t;

/******************************************************************
 * 枚举名 : CompileInfoReportType_e
 * 说明 : 编译信息上报类型
 ******************************************************************/
typedef enum EnumCompileInfoReportType{
	CompileInfoRep_Type_Begin = 1,
	CompileInfoRep_Type_Normal = 2,
	CompileInfoRep_Type_Error = 3,
	CompileInfoRep_Type_Warn = 4,
	CompileInfoRep_Type_End = 5,
}CompileInfoReportType_e;

/******************************************************************
 * 结构体名 : CommuModelProgFilePath_t
 * 说明 : 加载仿真模型命令使用
 ******************************************************************/
typedef struct TagCommuModelProgFilePath{
	unsigned long  nModelId;
	unsigned short nFilePathLen;
	unsigned char  szFilePath[256];
}CommuModelProgFilePath_t;

/******************************************************************
 * 结构体名 : CommuLoadSimuModelParam_t
 * 说明 : 加载仿真模型命令使用
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
 * 结构体名 : CommuSetModelParam_t
 * 说明 : 设置模型参数命令使用
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
 * 结构体名 : CommuTestEventReport_t
 * 说明 : 测试事件上报命令使用
 ******************************************************************/
typedef struct TagCommuTestEventReport{
	unsigned long nReportType;
	unsigned long nTestTaskId;
	unsigned long nTestCaseId;
	unsigned long nEventValueLen;
	unsigned char szEventValue[1024];
}CommuTestEventReport_t;

/******************************************************************
 * 枚举名 : ExceptReportType_e
 * 说明 : 异常事件上报类型
 ******************************************************************/
typedef enum EnumExceptReportType{
	CEXCEPT_RepType_Notice = 1,
	CEXCEPT_RepType_Warn = 2,
	CEXCEPT_RepType_Error = 3,
	CEXCEPT_RepType_UserPauseRun = 4,
	CEXCEPT_RepType_UserStopRun = 5,
}ExceptReportType_e;

/******************************************************************
 * 结构体名 : CommuTestEventReport_t
 * 说明 : 测试事件上报命令使用
 ******************************************************************/
typedef struct TagCommuExceptReport{
	unsigned long nReportType;
	unsigned long nReportLen;
	unsigned char szReportValue[1024];
}CommuExceptReport_t;

/******************************************************************
 * 结构体名 : CommuStorModelDataUnit_t
 * 说明 : 存储模型数据单元定义
 ******************************************************************/
typedef struct TagCommuStorModelDataUnit{
	unsigned long  nStep;			/* 步数*/
	unsigned char  bDataType;		/* 数据类型*/
	union{
		unsigned long  nGroupIdx;	
		char szRecordFile[256];
	}u;
	unsigned long  nTimeStampSec;	/* unix时间 秒 */
	unsigned long  nTimeStampMsec;	/* unix时间 微秒*/
	unsigned long  nDataLen;		/* 数据长度　*/
	unsigned char  *pData;			/* 数据存放地址 */
}CommuStorModelDataUnit_t;

/******************************************************************
 * 结构体名 : CommuTargetInfo_t
 * 说明 : 下位机板卡信息定义
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
 * 结构体名 : CommuTargetInfo_t
 * 说明 : 下位机信息定义
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
 * 结构体名 : CommuStorDataC_DShm_t
 * 说明 : StorDataC_D通信接口
 ******************************************************************/
#define MAX_SDCD_SHM_QUEUE_LEN	1024
#define MAX_SDCD_SHM_BUFF_LEN	(KL_COMMU_STOR_DATA_SEND_MAX_DATA_UNIT)
typedef struct TagCoummSimuStorDataC_DShm{
	int	ReadDataPtr; /*读下标*/
	int	WriteDataPtr; /*写下标*/
	unsigned long	nCoverCount; /*写覆盖次数*/
	unsigned long	nDataLen[MAX_SDCD_SHM_QUEUE_LEN];/* 数据长度 */
	char	szData[MAX_SDCD_SHM_QUEUE_LEN][MAX_SDCD_SHM_BUFF_LEN]; /* 数据区 */
}CommuSimuStorDataC_DShm_t;



