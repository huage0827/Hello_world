/*****************************************************************
 * 文件名: SimuConfigDef.h
 * 功能: 模型配置定义
 *****************************************************************/

#pragma  once

#include <iostream>
#include <vector>	
#include <string>
#include <map>

using namespace std;

/******************************************************************
 * 枚举类型 : ConfigSimuModeTypeE
 * 说明 : 仿真模型类型
 ******************************************************************/
typedef enum enumConfigSimuModelType{
	CONF_MODEL_Type_Cplusplus_Ipc = 1, /* C/C++模型-工控机*/
	CONF_MODEL_Type_Simulink_Ipc = 2,  /* Simulink模型-工控机*/
	CONF_MODEL_Type_Simulink_Fpga = 3, /* Simulink模型-FPGA*/
}ConfigSimuModelTypeE;

/******************************************************************
 * 枚举类型 : ConfigSyncModeTypeE
 * 说明 : 同步模式
 ******************************************************************/
typedef enum enumConfigSyncModeType{
	CONF_SYNC_MODE_Type_software = 1, /* 软件*/
	CONF_SYNC_MODE_Type_hardware = 2, /* 硬件*/
	CONF_SYNC_MODE_Type_nonreal = 3   /* 非实时*/
}ConfigSyncModeTypeE;

/******************************************************************
 * 枚举类型 : ConfigDataTypeE
 * 说明 : 数据类型定义
 ******************************************************************/
typedef enum enumConfigDataType{
	CONF_DATA_Type_unknown = 0,
	CONF_DATA_Type_int8 = 1, /*1 Byte char*/
	CONF_DATA_Type_uInt8 = 2,/*1 Byte unsigned char*/
	CONF_DATA_Type_int16 = 3,/*2 Byte short*/
	CONF_DATA_Type_uInt16 = 4,/*2 Byte unsigned short*/
	CONF_DATA_Type_int32 = 5,/*4 Byte int、long*/
	CONF_DATA_Type_uInt32 = 6,/*4 Byte unsigned、unsigned int、unsigned long*/
	CONF_DATA_Type_int64 = 7,/*8 Byte long long*/
	CONF_DATA_Type_uInt64 = 8,/*8 Byte unsigned long long*/
	CONF_DATA_Type_float = 9,/*4 Byte float*/
	CONF_DATA_Type_double = 10,/*8 Byte double*/
}ConfigDataTypeE;

typedef char kl_int8;
typedef unsigned char kl_uInt8;
typedef short	kl_int16;
typedef unsigned short	kl_uInt16;
typedef long kl_int32;
typedef unsigned long   kl_uInt32;
typedef long long kl_int64;
typedef unsigned long long kl_uInt64;
typedef float 	kl_float;
typedef double 	kl_double;

/******************************************************************
 * 枚举类型 : ConfigEventTypeE
 * 说明 : 测试事件类型
 ******************************************************************/
typedef enum enumConfigEventType{
	CONF_EVENT_Type_unknown = 0,
	CONF_EVENT_Type_step_limit = 1,
	CONF_EVENT_Type_data_limit = 2,
	CONF_EVENT_Type_time_limit = 3,
}ConfigEventTypeE;

/******************************************************************
 * 枚举类型 : ConfigValueSignTypeE
 * 说明 : 值比较符号
 ******************************************************************/
typedef enum enumConfigValueSignType{
	CONF_VAL_SIGN_Type_unknown = 0,
	CONF_VAL_SIGN_Type_GT = 1,  /*大于*/
	CONF_VAL_SIGN_Type_GTorEQ = 2, /*大于等于*/
	CONF_VAL_SIGN_Type_LT = 3,/*小于*/
	CONF_VAL_SIGN_Type_LTorEQ = 4,/*小于等于*/
	CONF_VAL_SIGN_Type_EQ = 5,/*等于*/
	CONF_VAL_SIGN_Type_notEQ = 6,/*不等于*/
}ConfigValueSignTypeE;

/******************************************************************
 * 结构体名 : ConfigTargetModelDir_t
 * 说明 : 下位机模型路径
 ******************************************************************/
typedef struct TagConfigTargetModelDir{
	int  nModelId;
	string szModelName;
	string szModelDir;
}ConfigTargetModelDir_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/******************************************************************
 * 结构体名 : ConfigProjectInfo_t
 * 说明 : 仿真工程信息
 ******************************************************************/
typedef struct TagConfigProjectInfo{
	string szProjId;
	string szProjName;
	unsigned long ulCreateTime;
}ConfigProjectInfo_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/******************************************************************
 * 结构体名 : ConfigSimuMode_t
 * 说明 : 仿真工程模型仿真模式
 ******************************************************************/
typedef struct TagConfigSimuMode{
	ConfigSyncModeTypeE eSimuMode; /*仿真同步模式*/
	bool bHwClockFromEx; /*硬件时钟信号来自外部*/
	int  nMasterModelId; /*主模型ID*/
	bool bAutoRun; /*自动运行*/
	int  nTaskId; /*测试任务ID*/
	int  nCaseId; /*测试用例ID*/
}ConfigSimuMode_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************
 * 结构体名 : ConfigExSrcIncFile_t
 * 说明 : 额外数据文件
 ******************************************************************/
typedef struct TagConfigExSrcIncFile{
	string szName; /*文件名*/
	string szFullPath; /*文件全路径*/
}ConfigExSrcIncFile_t;

/******************************************************************
 * 结构体名 : ConfigExUserFile_t
 * 说明 : 额外数据文件
 ******************************************************************/
typedef struct TagConfigExUserFile{
	string szName; /*文件名*/
	string szFullPath; /*文件全路径*/
}ConfigExUserFile_t;

/******************************************************************
 * 结构体名 : ConfigInputSig_t
 * 说明 : 输入信号
 ******************************************************************/
typedef struct TagConfigInputSig{
	int  nModelId;
	int  nId; /* 信号ID*/
	string szName;/*信号名称 */
	int  nNO;
	string szPath;
	string szBlockType;
	ConfigDataTypeE  eDataType;/* 数据类型*/
	string szTypeName;/* 数据类型名称*/
	int  nVecSize;/* 数组大小*/
	int  nRow;
	int  nCol;
	int  nAssOutSigId;/* 关联的输出信号ID*/
}ConfigInputSig_t;

/******************************************************************
 * 结构体名 : ConfigRecordGroup_t
 * 说明 : 数据存储组
 ******************************************************************/
typedef struct TagConfigRecordGroup{
	int nModelId;
	int nId;/*组ID*/
	string szName;/*组名*/
	int nRecordTimes;/*数据存储倍数*/
	string szRecordFile;/*存储文件名*/
	string szRecordPath;/*存储路径*/
	bool bSaveIntegerHex;/*存储为16进制*/
	bool bSaveScieNotation;/*科学计数法存储*/
	int nSaveEffectBits;/*存储的有效位*/
}ConfigRecordGroup_t;

/******************************************************************
 * 结构体名 : ConfigOutputSig_t
 * 说明 : 输出信号
 ******************************************************************/
typedef struct TagConfigOutputSig{
	int  nModelId;
	int  nId; /* 信号ID*/
	string szName;/*信号名称 */
	int  nNO;
	string szPath;
	string szBlockType;
	ConfigDataTypeE  eDataType;/* 数据类型*/
	string szTypeName;/* 数据类型名称*/
	int  nVecSize;/* 数组大小*/
	int  nRow;
	int  nCol;
	bool bRecord;/* 是否录制*/
	int  nRecordGroup;/*所属记录组*/
	bool bDisplay;/* 是否监控*/
	bool bSyncOutput;/* 是否同步输出*/
}ConfigOutputSig_t;

/******************************************************************
 * 结构体名 : ConfigInterParam_t
 * 说明 : 内部参数
 ******************************************************************/
typedef struct TagConfigInterParam{
	int  nModelId;
	int  nId; /* 参数ID*/
	string szName;/*参数名称 */
	int  nNO;
	string szPath;
	string szBlockType;
	ConfigDataTypeE  eDataType;/* 数据类型*/
	string szTypeName;/* 数据类型名称*/
	int  nVecSize;/* 数组大小*/
	int  nRow;
	int  nCol;
	string szInitValue;
}ConfigInterParam_t;

/******************************************************************
 * 结构体名 : ConfigSimuModelInfo_t
 * 说明 : 仿真模型信息
 ******************************************************************/
typedef struct TagConfigSimuModelInfo{
	int nId;/*模型ID*/
	ConfigSimuModelTypeE nModelType;/*模型类型*/
	string szName;/*模型名称*/

	// ExModelInfo
	union{
		struct{
			char szMatlabVer[32];
			int nLinked;
			char szPath[256];
		}Simulink; 
		struct{
			int  nLinked;
		}Cplusplus;
		struct{
			char szMatlaVer[32];
		}Fpga;
	}ExModelInfo;

	// SimulationCofnig
	string szTargetIp;/*目标机IP*/
	int  nCoreNum;/*CPU核数目*/
	unsigned char szCore[32];/*CPU核数组*/
	int  nRunStep;/*仿真步长，单位微秒*/
	int  nRecordRate;/*录制采样率，单位微秒*/
	int  nDispRate;/*监控采样率，单位微秒*/

	double dRunStep;
	double dRecordRate;
	double dDispRate;

	// DevelopmentConfig
	string szCompilerOpt;
	string szLinkerOpt;
	vector<ConfigExSrcIncFile_t *> vecSrcIncFile;
	vector<ConfigExUserFile_t *> vecUserFile;

	// DataDictionary
	vector<ConfigInputSig_t *> vecInputSig;
	vector<ConfigOutputSig_t *> vecOutputSig;
	vector<ConfigInterParam_t *> vecInteParam;
}ConfigSimuModelInfo_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************
 * 结构体名 : ConfigTestAction_t
 * 说明 : 测试动作
 ******************************************************************/
typedef struct TagConfigTestAction{
	int  nDDId;	/* 参数ID*/
	string szParamName; /* 参数名称*/
	string szSetValue;/* 设定值*/
}ConfigTestAction_t;

/******************************************************************
 * 结构体名 : ConfigTestEvent_t
 * 说明 : 测试事件
 ******************************************************************/
typedef struct TagConfigTestEvent{
	ConfigEventTypeE  eType; 
	int  nDDId;
	ConfigValueSignTypeE  eSign;
	string szReferValue;
}ConfigTestEvent_t;

/******************************************************************
 * 结构体名 : ConfigTestCase_t
 * 说明 : 测试用例
 ******************************************************************/
typedef struct TagConfigTestCase{
	int  nId;
	string szName;
	int  nWaitTime;
	vector<ConfigTestAction_t *> vecStartCond;
	vector<ConfigTestEvent_t *> vecEndCond;
}ConfigTestCase_t;

/******************************************************************
 * 结构体名 : ConfigTestExecCase_t
 * 说明 : 测试执行用例
 ******************************************************************/
typedef struct TagConfigTestExecCase{
	int nId;
	int nExecTimes;
}ConfigTestExecCase_t;

/******************************************************************
 * 结构体名 : ConfigTestTask_t
 * 说明 : 测试任务
 ******************************************************************/
typedef struct TagConfigTestTask{
	int nId;
	string szName;
	vector<ConfigTestExecCase_t *> vecExecCase; 
}ConfigTestTask_t;
