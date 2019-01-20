/*****************************************************************
 * �ļ���: SimuConfigDef.h
 * ����: ģ�����ö���
 *****************************************************************/

#pragma  once

#include <iostream>
#include <vector>	
#include <string>
#include <map>

using namespace std;

/******************************************************************
 * ö������ : ConfigSimuModeTypeE
 * ˵�� : ����ģ������
 ******************************************************************/
typedef enum enumConfigSimuModelType{
	CONF_MODEL_Type_Cplusplus_Ipc = 1, /* C/C++ģ��-���ػ�*/
	CONF_MODEL_Type_Simulink_Ipc = 2,  /* Simulinkģ��-���ػ�*/
	CONF_MODEL_Type_Simulink_Fpga = 3, /* Simulinkģ��-FPGA*/
}ConfigSimuModelTypeE;

/******************************************************************
 * ö������ : ConfigSyncModeTypeE
 * ˵�� : ͬ��ģʽ
 ******************************************************************/
typedef enum enumConfigSyncModeType{
	CONF_SYNC_MODE_Type_software = 1, /* ���*/
	CONF_SYNC_MODE_Type_hardware = 2, /* Ӳ��*/
	CONF_SYNC_MODE_Type_nonreal = 3   /* ��ʵʱ*/
}ConfigSyncModeTypeE;

/******************************************************************
 * ö������ : ConfigDataTypeE
 * ˵�� : �������Ͷ���
 ******************************************************************/
typedef enum enumConfigDataType{
	CONF_DATA_Type_unknown = 0,
	CONF_DATA_Type_int8 = 1, /*1 Byte char*/
	CONF_DATA_Type_uInt8 = 2,/*1 Byte unsigned char*/
	CONF_DATA_Type_int16 = 3,/*2 Byte short*/
	CONF_DATA_Type_uInt16 = 4,/*2 Byte unsigned short*/
	CONF_DATA_Type_int32 = 5,/*4 Byte int��long*/
	CONF_DATA_Type_uInt32 = 6,/*4 Byte unsigned��unsigned int��unsigned long*/
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
 * ö������ : ConfigEventTypeE
 * ˵�� : �����¼�����
 ******************************************************************/
typedef enum enumConfigEventType{
	CONF_EVENT_Type_unknown = 0,
	CONF_EVENT_Type_step_limit = 1,
	CONF_EVENT_Type_data_limit = 2,
	CONF_EVENT_Type_time_limit = 3,
}ConfigEventTypeE;

/******************************************************************
 * ö������ : ConfigValueSignTypeE
 * ˵�� : ֵ�ȽϷ���
 ******************************************************************/
typedef enum enumConfigValueSignType{
	CONF_VAL_SIGN_Type_unknown = 0,
	CONF_VAL_SIGN_Type_GT = 1,  /*����*/
	CONF_VAL_SIGN_Type_GTorEQ = 2, /*���ڵ���*/
	CONF_VAL_SIGN_Type_LT = 3,/*С��*/
	CONF_VAL_SIGN_Type_LTorEQ = 4,/*С�ڵ���*/
	CONF_VAL_SIGN_Type_EQ = 5,/*����*/
	CONF_VAL_SIGN_Type_notEQ = 6,/*������*/
}ConfigValueSignTypeE;

/******************************************************************
 * �ṹ���� : ConfigTargetModelDir_t
 * ˵�� : ��λ��ģ��·��
 ******************************************************************/
typedef struct TagConfigTargetModelDir{
	int  nModelId;
	string szModelName;
	string szModelDir;
}ConfigTargetModelDir_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/******************************************************************
 * �ṹ���� : ConfigProjectInfo_t
 * ˵�� : ���湤����Ϣ
 ******************************************************************/
typedef struct TagConfigProjectInfo{
	string szProjId;
	string szProjName;
	unsigned long ulCreateTime;
}ConfigProjectInfo_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/******************************************************************
 * �ṹ���� : ConfigSimuMode_t
 * ˵�� : ���湤��ģ�ͷ���ģʽ
 ******************************************************************/
typedef struct TagConfigSimuMode{
	ConfigSyncModeTypeE eSimuMode; /*����ͬ��ģʽ*/
	bool bHwClockFromEx; /*Ӳ��ʱ���ź������ⲿ*/
	int  nMasterModelId; /*��ģ��ID*/
	bool bAutoRun; /*�Զ�����*/
	int  nTaskId; /*��������ID*/
	int  nCaseId; /*��������ID*/
}ConfigSimuMode_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************
 * �ṹ���� : ConfigExSrcIncFile_t
 * ˵�� : ���������ļ�
 ******************************************************************/
typedef struct TagConfigExSrcIncFile{
	string szName; /*�ļ���*/
	string szFullPath; /*�ļ�ȫ·��*/
}ConfigExSrcIncFile_t;

/******************************************************************
 * �ṹ���� : ConfigExUserFile_t
 * ˵�� : ���������ļ�
 ******************************************************************/
typedef struct TagConfigExUserFile{
	string szName; /*�ļ���*/
	string szFullPath; /*�ļ�ȫ·��*/
}ConfigExUserFile_t;

/******************************************************************
 * �ṹ���� : ConfigInputSig_t
 * ˵�� : �����ź�
 ******************************************************************/
typedef struct TagConfigInputSig{
	int  nModelId;
	int  nId; /* �ź�ID*/
	string szName;/*�ź����� */
	int  nNO;
	string szPath;
	string szBlockType;
	ConfigDataTypeE  eDataType;/* ��������*/
	string szTypeName;/* ������������*/
	int  nVecSize;/* �����С*/
	int  nRow;
	int  nCol;
	int  nAssOutSigId;/* ����������ź�ID*/
}ConfigInputSig_t;

/******************************************************************
 * �ṹ���� : ConfigRecordGroup_t
 * ˵�� : ���ݴ洢��
 ******************************************************************/
typedef struct TagConfigRecordGroup{
	int nModelId;
	int nId;/*��ID*/
	string szName;/*����*/
	int nRecordTimes;/*���ݴ洢����*/
	string szRecordFile;/*�洢�ļ���*/
	string szRecordPath;/*�洢·��*/
	bool bSaveIntegerHex;/*�洢Ϊ16����*/
	bool bSaveScieNotation;/*��ѧ�������洢*/
	int nSaveEffectBits;/*�洢����Чλ*/
}ConfigRecordGroup_t;

/******************************************************************
 * �ṹ���� : ConfigOutputSig_t
 * ˵�� : ����ź�
 ******************************************************************/
typedef struct TagConfigOutputSig{
	int  nModelId;
	int  nId; /* �ź�ID*/
	string szName;/*�ź����� */
	int  nNO;
	string szPath;
	string szBlockType;
	ConfigDataTypeE  eDataType;/* ��������*/
	string szTypeName;/* ������������*/
	int  nVecSize;/* �����С*/
	int  nRow;
	int  nCol;
	bool bRecord;/* �Ƿ�¼��*/
	int  nRecordGroup;/*������¼��*/
	bool bDisplay;/* �Ƿ���*/
	bool bSyncOutput;/* �Ƿ�ͬ�����*/
}ConfigOutputSig_t;

/******************************************************************
 * �ṹ���� : ConfigInterParam_t
 * ˵�� : �ڲ�����
 ******************************************************************/
typedef struct TagConfigInterParam{
	int  nModelId;
	int  nId; /* ����ID*/
	string szName;/*�������� */
	int  nNO;
	string szPath;
	string szBlockType;
	ConfigDataTypeE  eDataType;/* ��������*/
	string szTypeName;/* ������������*/
	int  nVecSize;/* �����С*/
	int  nRow;
	int  nCol;
	string szInitValue;
}ConfigInterParam_t;

/******************************************************************
 * �ṹ���� : ConfigSimuModelInfo_t
 * ˵�� : ����ģ����Ϣ
 ******************************************************************/
typedef struct TagConfigSimuModelInfo{
	int nId;/*ģ��ID*/
	ConfigSimuModelTypeE nModelType;/*ģ������*/
	string szName;/*ģ������*/

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
	string szTargetIp;/*Ŀ���IP*/
	int  nCoreNum;/*CPU����Ŀ*/
	unsigned char szCore[32];/*CPU������*/
	int  nRunStep;/*���沽������λ΢��*/
	int  nRecordRate;/*¼�Ʋ����ʣ���λ΢��*/
	int  nDispRate;/*��ز����ʣ���λ΢��*/

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
 * �ṹ���� : ConfigTestAction_t
 * ˵�� : ���Զ���
 ******************************************************************/
typedef struct TagConfigTestAction{
	int  nDDId;	/* ����ID*/
	string szParamName; /* ��������*/
	string szSetValue;/* �趨ֵ*/
}ConfigTestAction_t;

/******************************************************************
 * �ṹ���� : ConfigTestEvent_t
 * ˵�� : �����¼�
 ******************************************************************/
typedef struct TagConfigTestEvent{
	ConfigEventTypeE  eType; 
	int  nDDId;
	ConfigValueSignTypeE  eSign;
	string szReferValue;
}ConfigTestEvent_t;

/******************************************************************
 * �ṹ���� : ConfigTestCase_t
 * ˵�� : ��������
 ******************************************************************/
typedef struct TagConfigTestCase{
	int  nId;
	string szName;
	int  nWaitTime;
	vector<ConfigTestAction_t *> vecStartCond;
	vector<ConfigTestEvent_t *> vecEndCond;
}ConfigTestCase_t;

/******************************************************************
 * �ṹ���� : ConfigTestExecCase_t
 * ˵�� : ����ִ������
 ******************************************************************/
typedef struct TagConfigTestExecCase{
	int nId;
	int nExecTimes;
}ConfigTestExecCase_t;

/******************************************************************
 * �ṹ���� : ConfigTestTask_t
 * ˵�� : ��������
 ******************************************************************/
typedef struct TagConfigTestTask{
	int nId;
	string szName;
	vector<ConfigTestExecCase_t *> vecExecCase; 
}ConfigTestTask_t;
