/*****************************************************************
 * 文件名: SimuConfigParser.cpp
 * 功能: 解析模型配置文件源文件
 *****************************************************************/
#include <stdio.h>
#include <string.h>
#include "stdafx.h"
#include "SimuConfigParser.h"
#include <time.h>

using namespace std;

static void StrToVector(const char *src, const char* separateChar,std::vector<std::string>& vecDest);

/* string to vector */
static void StrToVector(const char *src, const char* separateChar,std::vector<std::string>& vecDest)
{
	vecDest.clear();
	char* str = new char[strlen(src)+1];
	memset(str , 0 , strlen(src)+1);
	memcpy(str , src , strlen(src));
	char* token = strtok(str, separateChar);
	while( token != NULL )
	{
		vecDest.push_back(token);
		token = strtok( NULL , separateChar);
	}
	delete []str;
}

CSimuConfigParser::CSimuConfigParser()
{
	m_bLoadCfg = false;
	m_szErrorMsg = "";
}

CSimuConfigParser::~CSimuConfigParser()
{
	UnLoad();
}

/* 加载配置文件 */
bool  CSimuConfigParser::LoadCfg(const char *szCfgFile)
{
	/* 释放资源 */
	UnLoad();

	TiXmlDocument   XmlDoc;
	TiXmlElement	*pProjXmlEle = NULL;
	TiXmlElement	*pSimuModeEle = NULL;
	TiXmlElement	*pSimuModelEle = NULL;
	TiXmlElement	*pTestCaseEle = NULL;
	TiXmlElement	*pTestTaskEle = NULL;
	const char	*pTmpStr = NULL;

	m_bLoadCfg = false;

	/* 加载配置文件 */
	if( szCfgFile == NULL){
		m_szErrorMsg = "config file is null";
		return false;
	}

	if( !XmlDoc.LoadFile(szCfgFile)){
		m_szErrorMsg = "xmlDoc.loadFile failed";
		printf("ErrorMsg:%s \n", XmlDoc.ErrorDesc());
		return false;
	}

	/* 解析工程信息节点 */
	pProjXmlEle = XmlDoc.RootElement();
	if( pProjXmlEle == NULL){
		m_szErrorMsg = "RootElement is null";
		return false;
	}

	pTmpStr = pProjXmlEle->Value();
	if( pTmpStr == NULL || strcmp(pTmpStr, "SimuModelProject") != 0){
		m_szErrorMsg = "SimuModelProject ele is null";
		return false;
	}

	pTmpStr = pProjXmlEle->Attribute("ID");
	if( pTmpStr == NULL){
		m_szErrorMsg = "Project id is null";
		return false;
	}
	m_ProjInfo.szProjId = pTmpStr;

	pTmpStr = pProjXmlEle->Attribute("Name");
	if( pTmpStr == NULL){
		m_szErrorMsg = "project name is null";
		return false;
	}
	m_ProjInfo.szProjName = pTmpStr;

	pTmpStr = pProjXmlEle->Attribute("CreateTimes");
	if( pTmpStr == NULL){
		m_szErrorMsg = "project createTime is null";
		return false;
	}
	m_ProjInfo.ulCreateTime = atol(pTmpStr);

	/* 解析仿真模式 */
	pSimuModeEle = pProjXmlEle->FirstChildElement("SimulationMode");
	if( pSimuModeEle == NULL){
		m_szErrorMsg = "SimulationMode node is null";
		return false;
	}
	if( !ParseSimuModeNode(pSimuModeEle)){
		return false;
	}

	/* 解析仿真模型信息节点 */
	pSimuModelEle = pProjXmlEle->FirstChildElement("SimuModels");
	if( pSimuModelEle == NULL){
		m_szErrorMsg = "SimuModels node is null";
		return false;
	}
	if( !ParseSimuModelsNode(pSimuModelEle)){
		return false;
	}

#if 0
	/* 解析测试用例信息节点 */
	pTestCaseEle = pProjXmlEle->FirstChildElement("TestCases");
	if( pTestCaseEle == NULL){
		m_szErrorMsg = "TestCases node is null";
		return false;
	}
	if( !ParseTestCaseNode(pTestCaseEle)){
		return false;
	}

	/* 解析测试任务信息节点 */
	pTestTaskEle = pProjXmlEle->FirstChildElement("TestTasks");
	if( pTestTaskEle == NULL){
		m_szErrorMsg = "TestTasks node is null";
		return false;
	}
	if( !ParseTestTaskNode(pTestTaskEle)){
		return false;
	}
#endif

	/* 解析成功 */
	m_szErrorMsg = "";
	m_bLoadCfg = true;
	m_szCfgFile = szCfgFile;

	return true;
}

/* 释放资源 */
void  CSimuConfigParser::UnLoad()
{
	ConfigSimuModelInfo_t *pModelInfo = NULL;
	ConfigExUserFile_t *pExFile = NULL;
	ConfigInputSig_t *pInputSig = NULL;
	ConfigOutputSig_t *pOutputSig = NULL;
	ConfigInterParam_t *pParam = NULL;
	ConfigTestCase_t *pTestCase = NULL;
	ConfigTestTask_t *pTestTask = NULL;

	m_bLoadCfg = false;
	m_szCfgFile = "";
	
	/*模型信息*/
	for( unsigned int i = 0; i < m_vecModelInfo.size(); i++){
		pModelInfo = m_vecModelInfo.at(i);
		for( unsigned int j = 0; j < pModelInfo->vecInputSig.size();j++)
			delete pModelInfo->vecInputSig.at(j);
		pModelInfo->vecInputSig.clear();
		for( unsigned int j = 0; j < pModelInfo->vecOutputSig.size();j++)
			delete pModelInfo->vecOutputSig.at(j);
		pModelInfo->vecOutputSig.clear();
		for( unsigned int j = 0; j < pModelInfo->vecInteParam.size();j++)
			delete pModelInfo->vecInteParam.at(j);
		pModelInfo->vecInteParam.clear();
		for( unsigned int j = 0; j < pModelInfo->vecSrcIncFile.size();j++)
			delete pModelInfo->vecSrcIncFile.at(j);
		pModelInfo->vecSrcIncFile.clear();
		for( unsigned int j = 0; j < pModelInfo->vecUserFile.size();j++)
			delete pModelInfo->vecUserFile.at(j);
		pModelInfo->vecUserFile.clear();
		delete pModelInfo;
	}
	m_vecModelInfo.clear();

	/*模型输入信号*/
	for( unsigned int i = 0; i < m_vecInputSig.size(); i++){
		pInputSig = m_vecInputSig.at(i);
		delete pInputSig;
	}
	m_vecInputSig.clear();

	/*模型输出信号*/
	for( unsigned int i = 0; i < m_vecOutputSig.size(); i++){
		pOutputSig = m_vecOutputSig.at(i);
		delete pOutputSig;
	}
	m_vecOutputSig.clear();

	/*模型内部参数*/
	for( unsigned int i = 0; i < m_vecParams.size(); i++){
		pParam = m_vecParams.at(i);
		delete pParam;
	}
	m_vecParams.clear();

	/*测试用例*/
	ConfigTestAction_t *pAction = NULL;
	ConfigTestEvent_t *pEvent = NULL;

	for( unsigned int i = 0; i < m_vecTestCase.size(); i++){
		pTestCase = m_vecTestCase.at(i);

		for( unsigned int j = 0; j < pTestCase->vecStartCond.size(); j++){
			pAction = pTestCase->vecStartCond.at(j);
			delete pAction;
		}
		pTestCase->vecStartCond.clear();

		for( unsigned int j = 0; j < pTestCase->vecEndCond.size(); j++){
			pEvent = pTestCase->vecEndCond.at(j);
			delete pEvent;
		}
		pTestCase->vecEndCond.clear();
		delete pTestCase;
	}
	m_vecTestCase.clear();

	/*测试任务*/
	ConfigTestExecCase_t *pExecCase = NULL;

	for( unsigned int i = 0; i < m_vecTestTask.size(); i++){
		pTestTask = m_vecTestTask.at(i);

		for( unsigned int j = 0; j < pTestTask->vecExecCase.size(); j++){
			pExecCase = pTestTask->vecExecCase.at(j);
			delete pExecCase;
		}
		pTestTask->vecExecCase.clear();
		delete pTestTask;
	}
	m_vecTestTask.clear();
}

/* 配置合法性检查*/
bool  CSimuConfigParser::CheckCfg()
{
	if( !m_bLoadCfg){
		m_szErrorMsg = "chekcCfg: not load config file";
		return false;
	}

	CheckProject();
	CheckSimuModel();
	CheckModelInfo();
//	CheckExUserFile();
	CheckInputSig();
	CheckOutputSig();
	CheckParams();
	CheckTestCase();
	CheckTestTask();

	return true;
}

/* 获取错误信息*/
const char *CSimuConfigParser::GetLastErrMsg()
{
	return m_szErrorMsg.c_str();
}

const char *CSimuConfigParser::GetCfgFile()
{
	if( !m_bLoadCfg)
		return NULL;
	else
		return m_szCfgFile.c_str();
}

/* 数据返回 */
const ConfigProjectInfo_t* CSimuConfigParser::GetProjInfo()
{
	return &m_ProjInfo;
}

const ConfigSimuMode_t* CSimuConfigParser::GetSimuMode()
{
	return &m_SimuMode;
}

const vector<ConfigSimuModelInfo_t *> *CSimuConfigParser::GetModelInfo()
{
	return &m_vecModelInfo;
}
//
//const vector<ConfigExUserFile_t *> *CSimuConfigParser::GetExUserFile()
//{
//	return &m_vecExUserFile;
//}

const vector<ConfigInputSig_t *> *CSimuConfigParser::GetInputSig()
{
	return &m_vecInputSig;
}

const vector<ConfigRecordGroup_t *> *CSimuConfigParser::GetRecordGroup()
{
	return &m_vecRecordGroup;
}

const vector<ConfigOutputSig_t *> *CSimuConfigParser::GetOutputSig()
{
	return &m_vecOutputSig;
}

const vector<ConfigInterParam_t *> *CSimuConfigParser::GetParams()
{
	return &m_vecParams;
}

const vector<ConfigTestCase_t *> *CSimuConfigParser::GetTestCase()
{
	return &m_vecTestCase;
}

const vector<ConfigTestTask_t *> *CSimuConfigParser::GetTestTask()
{
	return &m_vecTestTask;
}


/* 打印输出 */
void CSimuConfigParser::PrintAll()
{
	PrintProject();
	PrintSimuMode();
	PrintModeInfo();
//	PrintExUserFile();
	PrintInputSig();
	PrintRecordGroup();
	PrintOutputSig();
	PrintParams();
	PrintTestCase();
	PrintTestTask();
}

void CSimuConfigParser::PrintProject()
{
	printf("------------------- Project -----------------\n");
	printf("ProjId:%s, ProjName:%s, CreateTime:%d \n", m_ProjInfo.szProjId.c_str(),
		m_ProjInfo.szProjName.c_str(), m_ProjInfo.ulCreateTime);
}

void CSimuConfigParser::PrintSimuMode()
{
	printf("-------------------- SimuMode----------------\n");
	printf("SimuMode:%d, MasterID:%d, bAutoRun:%d, TaskId:%d, CaseId:%d \n",
		m_SimuMode.eSimuMode, m_SimuMode.nMasterModelId, m_SimuMode.bAutoRun,
		m_SimuMode.nTaskId, m_SimuMode.nCaseId);
}

void CSimuConfigParser::PrintModeInfo()
{
	printf("------------------ Model info ----------------\n");
	ConfigSimuModelInfo_t *pModeInfo = NULL;

	for( unsigned int i = 0; i < m_vecModelInfo.size(); i++){
		pModeInfo = m_vecModelInfo.at(i);

		printf("ID:%d, Name:%s, TargetIP:%s, nCoreNum:%d",pModeInfo->nId, 
			pModeInfo->szName.c_str(), pModeInfo->szTargetIp.c_str(), pModeInfo->nCoreNum);
		for( int j = 0; j < pModeInfo->nCoreNum; j++){
			if( j == 0 )printf(",(%d", pModeInfo->szCore[j]);
			else if( j == pModeInfo->nCoreNum -1 ) printf(",%d)",pModeInfo->szCore[j]);
			else printf(",%d", pModeInfo->szCore[j]);
		}
		printf(",Step:%d, RecordRate:%d, DispRate:%d \n", pModeInfo->nRunStep,
			pModeInfo->nRecordRate, pModeInfo->nDispRate);
	}
}

//void CSimuConfigParser::PrintExUserFile()
//{
//	printf("-------------------- ExUserFile ----------------\n");
//	ConfigExUserFile_t *pUserFile = NULL;
//
//	for( unsigned int i = 0; i < m_vecExUserFile.size(); i++){
//		pUserFile = m_vecExUserFile.at(i);
//
//		printf("i = %d, fileName:%s, fullPath:%s \n",i, pUserFile->szName.c_str(), pUserFile->szFullPath.c_str());
//	}
//}

void CSimuConfigParser::PrintInputSig()
{
	printf("------------------ Input Signal ----------------\n");
	ConfigInputSig_t *pInput = NULL;

	for( unsigned int i = 0; i < m_vecInputSig.size(); i++){
		pInput = m_vecInputSig.at(i);

		printf("ModelID:%d => Id:%d, Name:%s, Type:%d, VecSize:%d, AssOutSigID:%d ,path:%s, block:%s\n",
			pInput->nModelId, pInput->nId, pInput->szName.c_str(), pInput->eDataType, 
			pInput->nVecSize, pInput->nAssOutSigId, pInput->szPath.c_str(), pInput->szBlockType.c_str());
	}
}

void CSimuConfigParser::PrintRecordGroup()
{
	printf("------------------ Record Group -----------------\n");
	ConfigRecordGroup_t *pGroup = NULL;

	for( unsigned i = 0; i < m_vecRecordGroup.size(); i++){
		pGroup = m_vecRecordGroup.at(i);

		printf("ModelID:%d => Id:%d, Name:%s, Times:%d, RFile:%s, RPath:%s \n",
			pGroup->nModelId, pGroup->nId, pGroup->szName.c_str(), pGroup->nRecordTimes,
			pGroup->szRecordFile.c_str(), pGroup->szRecordPath.c_str());
	}
}

void CSimuConfigParser::PrintOutputSig()
{
	printf("------------------ Output Signal ----------------\n");
	ConfigOutputSig_t *pOutput = NULL;

	for( unsigned int i = 0; i < m_vecOutputSig.size(); i++){
		pOutput = m_vecOutputSig.at(i);
	
		/*
		printf("ModelID:%d => Id:%d, Name:%s, Type:%d, VecSize:%d, bRecord:%d, recordTime:%d, recordFile:%s, bDisplay:%d, bSyncOut:%d \n",
			pOutput->nModelId, pOutput->nId, pOutput->szName.c_str(), pOutput->eDataType, pOutput->nVecSize, 
			pOutput->bRecord, pOutput->recordTimes, pOutput->szRecordFile.c_str(), pOutput->bDisplay, pOutput->bSyncOutput);*/
		printf("ModelID:%d => Id:%d, Name:%s, Type:%d, VecSize:%d, bRecord:%d, RecordGroup:%d bDisplay:%d, bSyncOut:%d, Path:%s, block:%s \n",
			pOutput->nModelId, pOutput->nId, pOutput->szName.c_str(), pOutput->eDataType, pOutput->nVecSize, 
			pOutput->bRecord, pOutput->nRecordGroup, pOutput->bDisplay, pOutput->bSyncOutput,
			pOutput->szPath.c_str(), pOutput->szBlockType.c_str());
	}
}

void CSimuConfigParser::PrintParams()
{
	printf("---------------------  Param -------------------\n");
	ConfigInterParam_t *pParam = NULL;

	for( unsigned int i = 0; i < m_vecParams.size(); i++){
		pParam = m_vecParams.at(i);

		printf("ModelID:%d => Id:%d, Name:%s, Type:%d, VecSize:%d, path:%s, blockType:%s, value:%s\n",
			pParam->nModelId, pParam->nId, pParam->szName.c_str(),
			pParam->eDataType, pParam->nVecSize, 
			pParam->szPath.c_str(), pParam->szBlockType.c_str(), pParam->szInitValue.c_str());
	}
}

void CSimuConfigParser::PrintTestCase()
{
	printf("---------------------  TestCase -------------------\n");
	ConfigTestCase_t *pTestCase = NULL;
	ConfigTestAction_t *pAction = NULL;
	ConfigTestEvent_t *pEvent = NULL;

	for( unsigned int i = 0; i < m_vecTestCase.size(); i++){
		pTestCase = m_vecTestCase.at(i);

		printf("========= TestCase ID:%d, Name:%s, WaitTime:%d \n", pTestCase->nId, pTestCase->szName.c_str(), pTestCase->nWaitTime);
		for( unsigned int j = 0; j < pTestCase->vecStartCond.size(); j++){
			pAction = pTestCase->vecStartCond.at(j);
			printf("==> DD_ID:%d, SetValue:%s \n", pAction->nDDId, pAction->szSetValue.c_str());
		}
		for( unsigned int j = 0; j < pTestCase->vecEndCond.size(); j++){
			pEvent = pTestCase->vecEndCond.at(j);
			printf("<< Type:%d, DD_ID:%d, Sign:%d, ReferValue:%s \n", pEvent->eType,
				pEvent->nDDId, pEvent->eSign, pEvent->szReferValue.c_str());
		}
	}
}

void CSimuConfigParser::PrintTestTask()
{
	printf("---------------------  TestTask -------------------\n");
	ConfigTestTask_t *pTestTask = NULL;
	ConfigTestExecCase_t *pExec = NULL;

	for( unsigned int i = 0; i < m_vecTestTask.size(); i++){
		pTestTask = m_vecTestTask.at(i);
		printf("======= Task -> ID:%d, Name:%s \n", pTestTask->nId, pTestTask->szName.c_str());
		
		for( unsigned int j = 0; j < pTestTask->vecExecCase.size(); j++){
			pExec = pTestTask->vecExecCase.at(j);

			printf("=> TestCase ID:%d, ExecTime:%d \n", pExec->nId, pExec->nExecTimes);
		}
	}
}

int CSimuConfigParser::GetDataTypeByteLen(int type)
{
	int nLen = 0;
	switch(type){
	case CONF_DATA_Type_unknown:
		nLen = 0;break;
	case CONF_DATA_Type_int8:
		nLen = 1;break;
	case CONF_DATA_Type_uInt8:
		nLen = 1;break;
	case CONF_DATA_Type_int16:
		nLen = 2;break;
	case CONF_DATA_Type_uInt16:
		nLen = 2;break;
	case CONF_DATA_Type_int32:
		nLen = 4;break;
	case CONF_DATA_Type_uInt32:
		nLen = 4;break;
	case CONF_DATA_Type_int64:
		nLen = 8;break;
	case CONF_DATA_Type_uInt64:
		nLen = 8;break;
	case CONF_DATA_Type_float:
		nLen = 4;break;
	case CONF_DATA_Type_double:
		nLen = 8;break;
	default:
		nLen = 0;break;
	}
	return nLen;
}

int CSimuConfigParser::CopyProjectInfo(const ConfigProjectInfo_t *pSrcProj,ConfigProjectInfo_t *pDesProj)
{
	pDesProj->szProjId = pSrcProj->szProjId;
	pDesProj->szProjName = pSrcProj->szProjName;
	pDesProj->ulCreateTime = pSrcProj->ulCreateTime;
	return 0;
}

//int CSimuConfigParser::CopyModelInfo(const ConfigSimuModelInfo_t *pSrcModel, ConfigSimuModelInfo_t *pDesModel)
//{
//	pDesModel->nId = pSrcModel->nId;
//	pDesModel->nCoreNum = pSrcModel->nCoreNum;
//	pDesModel->nDispRate = pSrcModel->nDispRate;
//	pDesModel->nRecordRate = pSrcModel->nRecordRate;
//	pDesModel->nRunStep = pSrcModel->nRunStep;
//	pDesModel->szName = pSrcModel->szName;
//	pDesModel->szTargetIp = pSrcModel->szTargetIp;
//	memcpy(pDesModel->szCore, pSrcModel->szCore, pSrcModel->nCoreNum);
//	return 0;
//}

int CSimuConfigParser::CopyInputSig(const ConfigInputSig_t *pSrcSig, ConfigInputSig_t *pDesSig)
{
	pDesSig->eDataType = pSrcSig->eDataType;
	pDesSig->nAssOutSigId = pSrcSig->nAssOutSigId;
	pDesSig->nId = pSrcSig->nId;
	pDesSig->nModelId = pSrcSig->nModelId;
	pDesSig->nVecSize = pSrcSig->nVecSize;
	pDesSig->szName = pSrcSig->szName;
	pDesSig->szTypeName = pSrcSig->szTypeName;
	
	pDesSig->nNO = pSrcSig->nNO;
	pDesSig->nCol = pSrcSig->nCol;
	pDesSig->nRow = pSrcSig->nRow;
	pDesSig->szPath = pSrcSig->szPath;
	pDesSig->szBlockType = pSrcSig->szBlockType;

	return 0;
}

int CSimuConfigParser::CopyOutputSig(const ConfigOutputSig_t *pSrcSig, ConfigOutputSig_t* pDesSig)
{
	pDesSig->bDisplay = pSrcSig->bDisplay;
	pDesSig->bRecord = pSrcSig->bRecord;
	pDesSig->bSyncOutput = pSrcSig->bSyncOutput;
	pDesSig->eDataType = pSrcSig->eDataType;
	pDesSig->nId = pSrcSig->nId;
	pDesSig->nModelId = pSrcSig->nModelId;
	pDesSig->nVecSize = pSrcSig->nVecSize;
	pDesSig->szName = pSrcSig->szName;
	pDesSig->szTypeName = pSrcSig->szTypeName;
	pDesSig->nRecordGroup = pSrcSig->nRecordGroup;

	pDesSig->nNO = pSrcSig->nNO;
	pDesSig->nCol = pSrcSig->nCol;
	pDesSig->nRow = pSrcSig->nRow;
	pDesSig->szPath = pSrcSig->szPath;
	pDesSig->szBlockType = pSrcSig->szBlockType;
	return 0;
}

int CSimuConfigParser::CopyInterParam(const ConfigInterParam_t *pSrcParam, ConfigInterParam_t *pDesParam)
{
	pDesParam->eDataType = pSrcParam->eDataType;
	pDesParam->nId = pSrcParam->nId;
	pDesParam->nModelId = pSrcParam->nModelId;
	pDesParam->nVecSize = pSrcParam->nVecSize;
	pDesParam->szName = pSrcParam->szName;
	pDesParam->szTypeName = pSrcParam->szTypeName;

	pDesParam->nNO = pSrcParam->nNO;
	pDesParam->nCol = pSrcParam->nCol;
	pDesParam->nRow = pSrcParam->nRow;
	pDesParam->szPath = pSrcParam->szPath;
	pDesParam->szBlockType = pSrcParam->szBlockType;
	pDesParam->szInitValue = pSrcParam->szInitValue;
	return 0;
}

/*配置文件转换*/
int CSimuConfigParser::Ex_GenerateModelConfig(vector<ConfigTargetModelDir_t *> *pVecCfgModel)
{
	FILE *fp = NULL;
	ConfigTargetModelDir_t *pModelDir = NULL;
	char szModelCfgFile[256];
	ConfigSimuModelInfo_t *pSimuModel = NULL;
	bool bFind = false;
	int nGenerateCount = 0;

	for(unsigned i = 0; i < pVecCfgModel->size(); i++){
		pModelDir  = pVecCfgModel->at(i);
		bFind = false;
		for(unsigned j = 0; j < m_vecModelInfo.size(); j++){
			if( pModelDir->nModelId == m_vecModelInfo.at(j)->nId ){
				bFind = true;
				pSimuModel = m_vecModelInfo.at(j);
				break;
			}
		}
		if(!bFind){
			continue;
		}
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
		sprintf(szModelCfgFile, "%s/SimuModel.config", pModelDir->szModelDir.c_str());
#else
		sprintf(szModelCfgFile, "%s\\SimuModel.config", pModelDir->szModelDir.c_str());
#endif
		fp = fopen(szModelCfgFile, "w");
		if( fp == NULL){
			continue;
		}

		if( !Ex_GenerateModelConfigToFile(fp, pSimuModel)){
			nGenerateCount++;
		}
		fclose(fp);
	}
	return nGenerateCount;
}

/*解析仿真模式节点*/
bool CSimuConfigParser::ParseSimuModeNode(TiXmlElement *pSimuModeEle)
{
	const char	*pTmpStr = NULL;

	/* 解析仿真模式 */
	pTmpStr = pSimuModeEle->Attribute("SimuMode");
	m_SimuMode.eSimuMode = (pTmpStr != NULL) ? (ConfigSyncModeTypeE)atol(pTmpStr) : CONF_SYNC_MODE_Type_software;

	pTmpStr = pSimuModeEle->Attribute("AutoRun");
	m_SimuMode.bAutoRun = (pTmpStr != NULL && atoi(pTmpStr)==1) ? true: false;

	pTmpStr = pSimuModeEle->Attribute("TestTaskID");
	m_SimuMode.nTaskId = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	pTmpStr = pSimuModeEle->Attribute("TestCaseID");
	m_SimuMode.nCaseId = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	pTmpStr = pSimuModeEle->Attribute("MasterModel");
	if( pTmpStr == NULL){
		m_szErrorMsg = "ParseSimuModelNode->MasterModel is null";
		return false;
	}
	m_SimuMode.nMasterModelId = atoi(pTmpStr);
	return true;
}

/*解析仿真模型节点*/
bool CSimuConfigParser::ParseSimuModelsNode(TiXmlElement *pRootXmlEle)
{
	TiXmlElement *pSubModelEle = NULL;
	TiXmlElement *pExModelInfoEle = NULL;
	TiXmlElement *pSimuConfigEle = NULL;
	TiXmlElement *pDeveConfigEle = NULL;
	TiXmlElement *pDataDictEle = NULL;
	const char	*pTmpStr = NULL;
	string szExModelNodeName = "";

	for( pSubModelEle = pRootXmlEle->FirstChildElement("SubModel"); pSubModelEle != NULL;
		pSubModelEle = pSubModelEle->NextSiblingElement("SubModel"))
	{
		ConfigSimuModelInfo_t *pConfModel = new ConfigSimuModelInfo_t;

		// 模型节点属性解析
		pTmpStr = pSubModelEle->Attribute("ID");
		pConfModel->nId = (pTmpStr != NULL) ?  atoi(pTmpStr) : 0;

		pTmpStr = pSubModelEle->Attribute("Name");
		pConfModel->szName = (pTmpStr != NULL) ? pTmpStr : "";

		pTmpStr = pSubModelEle->Attribute("ModelType");
		pConfModel->nModelType =(ConfigSimuModelTypeE)((pTmpStr != NULL) ? atoi(pTmpStr) : CONF_MODEL_Type_Cplusplus_Ipc);

		// ExModelInfo解析
		if( pConfModel->nModelType == CONF_MODEL_Type_Cplusplus_Ipc){
			//szExModelNodeName = "ExModelInfo_Cplusplus";
			szExModelNodeName = "ExModelInfo_Simulink";
		}else if( pConfModel->nModelType == CONF_MODEL_Type_Simulink_Ipc){
			szExModelNodeName = "ExModelInfo_Simulink";
		}else if( pConfModel->nModelType == CONF_MODEL_Type_Simulink_Fpga){
			szExModelNodeName = "ExModelInfo_Fgpa";
		}
		pExModelInfoEle = pSubModelEle->FirstChildElement(szExModelNodeName.c_str());
		if( pExModelInfoEle == NULL){
			m_szErrorMsg = szExModelNodeName + " ExModelInfo_node is null";
			FreeSimuModel(pConfModel);
			delete pConfModel;
			return false;
		}
		if( !ParseExModelInfoNode(pExModelInfoEle, pConfModel)){
			FreeSimuModel(pConfModel);
			delete pConfModel;
			return false;
		}

		// SimulationConfig解析
		pSimuConfigEle = pSubModelEle->FirstChildElement("SimulationConfig");
		if( pSimuConfigEle == NULL){
			m_szErrorMsg = "SimulationConfig Node is null";
			FreeSimuModel(pConfModel);
			delete pConfModel;
			return false;
		}
		if( !ParseSimulationCfgNode(pSimuConfigEle, pConfModel)){
			FreeSimuModel(pConfModel);
			delete pConfModel;
			return false;
		}

		// DevelopmentConfig解析
		pDeveConfigEle = pSubModelEle->FirstChildElement("DevelopmentConfig");
		if( pDeveConfigEle == NULL){
			m_szErrorMsg = "DevelopmentConfig Node is null";
			FreeSimuModel(pConfModel);
			delete pConfModel;
			return false;
		}
		if( !ParseDevelopmentCfgNode(pDeveConfigEle, pConfModel)){
			FreeSimuModel(pConfModel);
			delete pConfModel;
			return false;
		}

		// DataDictionary解析
		pDataDictEle = pSubModelEle->FirstChildElement("DataDictionary");
		if( pDeveConfigEle == NULL){
			m_szErrorMsg = "DevelopmentConfig Node is null";
			FreeSimuModel(pConfModel);
			delete pConfModel;
			return false;
		}
		if( !ParseDataDictionaryNode(pDataDictEle, pConfModel)){
			FreeSimuModel(pConfModel);
			delete pConfModel;
			return false;
		}
		
		// 解析成功
		m_vecModelInfo.push_back(pConfModel);

		/* 创建一个默认分组*/
		ConfigRecordGroup_t *pRGroup = NULL;
		char szDefault[256];

		// 新建默认分组
		pRGroup = new ConfigRecordGroup_t;
		pRGroup->nId = 1;
		pRGroup->nModelId = pConfModel->nId;
		pRGroup->nRecordTimes = 1;
		pRGroup->szName = "defaultGroup";
		sprintf(szDefault, "model%d_data_default_%d.dat", pConfModel->nId, time(NULL));
		pRGroup->szRecordFile = szDefault;
		pRGroup->szRecordPath = "";
		pRGroup->bSaveIntegerHex = false;
		pRGroup->bSaveScieNotation = false;
		pRGroup->nSaveEffectBits = 6;
		m_vecRecordGroup.push_back(pRGroup);
	}
	return true;
}

/*解析ExModelInfo Node*/
bool CSimuConfigParser::ParseExModelInfoNode(TiXmlElement *pRootXmlEle, ConfigSimuModelInfo_t *pSimuModel)
{
	TiXmlElement	*pTmpXmlEle = NULL;
	const char	*pTmpStr = NULL;

	if( pSimuModel->nModelType == CONF_MODEL_Type_Simulink_Ipc){
		// MatlabVer 
		pTmpXmlEle = pRootXmlEle->FirstChildElement("MatlabVer");
		if( pTmpXmlEle != NULL){
			pTmpStr = pTmpXmlEle->GetText();
			if( pTmpStr != NULL)
				strcpy(pSimuModel->ExModelInfo.Simulink.szMatlabVer,pTmpStr);
			else
				strcpy(pSimuModel->ExModelInfo.Simulink.szMatlabVer,"");
		}else{
			m_szErrorMsg = "MatlabVer Node is null";
			return false;
		}
		// Linked
		pTmpXmlEle = pRootXmlEle->FirstChildElement("Linked");
		if( pTmpXmlEle != NULL){
			pTmpStr = pTmpXmlEle->GetText();
			pSimuModel->ExModelInfo.Simulink.nLinked = (pTmpStr != NULL)? atoi(pTmpStr):0;
		}else{
			m_szErrorMsg = "Linked Node is null";
			return false;
		}
		// Path
		pTmpXmlEle = pRootXmlEle->FirstChildElement("Path");
		if( pTmpXmlEle != NULL){
			pTmpStr = pTmpXmlEle->GetText();
			if( pTmpStr != NULL)
				strcpy(pSimuModel->ExModelInfo.Simulink.szPath,pTmpStr);
			else 
				strcpy(pSimuModel->ExModelInfo.Simulink.szPath,"");
		}else{
			m_szErrorMsg = "Path Node is null";
			return false;
		}
	}else if( pSimuModel->nModelType == CONF_MODEL_Type_Cplusplus_Ipc){

	}else if( pSimuModel->nModelType == CONF_MODEL_Type_Simulink_Fpga){

	}
	return true;
}

/*解析SimulationConfig Node*/
bool CSimuConfigParser::ParseSimulationCfgNode(TiXmlElement *pRootXmlEle, ConfigSimuModelInfo_t *pSimuModel)
{
	TiXmlElement	*pTmpXmlEle = NULL;
	const char	*pTmpStr = NULL;

	// TargetIP
	pTmpXmlEle = pRootXmlEle->FirstChildElement("TargetIP");
	if( pTmpXmlEle == NULL){
		m_szErrorMsg = "TargetIp node is Null";
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	pSimuModel->szTargetIp = (pTmpStr != NULL)?pTmpStr :"";

	// TargetCore
	pTmpXmlEle = pRootXmlEle->FirstChildElement("TargetCore");
	if( pTmpXmlEle == NULL){
		m_szErrorMsg = "TargetCore Node is Null";
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	if( pTmpStr != NULL){/* 解析CPU分配 */
		vector<string> vecCores;
		vector<string> vecCore;
		unsigned int idx;
		int nCores = 0;
		StrToVector(pTmpStr, "()", vecCores);
		if( vecCores.size() >= 2){
			nCores = atoi(vecCores[0].c_str());
			StrToVector(vecCores[1].c_str(), ", ", vecCore);
			for( idx = 0; idx < vecCore.size(); idx++){
				pSimuModel->szCore[idx] = atoi(vecCore[idx].c_str());
				if( idx > 32 )break;
			}
			pSimuModel->nCoreNum = vecCore.size();
		}else{
			pSimuModel->nCoreNum = 0;
		}
	}else{
		pSimuModel->nCoreNum = 0;
	}
	
	// SimuStep
	pTmpXmlEle = pRootXmlEle->FirstChildElement("SimuStep");
	if( pTmpXmlEle == NULL){
		m_szErrorMsg = "SimuStep node is Null";
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	if( pTmpStr == NULL){
		return false;
	}
	pSimuModel->nRunStep = atoi(pTmpStr);
	pSimuModel->dRunStep = pSimuModel->nRunStep/1000000.0;
	//pSimuModel->dRunStep = atof(pTmpStr);
	//pSimuModel->nRunStep = pSimuModel->dRunStep * 1000000;

	// RecordRate
	pTmpXmlEle = pRootXmlEle->FirstChildElement("RecordRate");
	if( pTmpXmlEle == NULL){
		m_szErrorMsg = "RecordRate node is Null";
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	if( pTmpStr == NULL){
		return false;
	}
	pSimuModel->nRecordRate = atoi(pTmpStr);
	pSimuModel->dRecordRate = pSimuModel->nRecordRate/1000000.0;
	//pSimuModel->dRecordRate = atof(pTmpStr);
	//pSimuModel->nRecordRate = pSimuModel->dRecordRate * 1000000;

	// DisplayRate
	pTmpXmlEle = pRootXmlEle->FirstChildElement("DisplayRate");
	if( pTmpXmlEle == NULL){
		m_szErrorMsg = "DisplayRate node is Null";
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	if( pTmpStr == NULL){
		return false;
	}
	pSimuModel->nDispRate = atoi(pTmpStr);
	pSimuModel->dDispRate = pSimuModel->nDispRate/1000000.0;
	//pSimuModel->dDispRate = atof(pTmpStr);
	//pSimuModel->nDispRate = pSimuModel->dDispRate * 1000000;

	return true;
}

/*解析DevelopmentConfig Node*/
bool CSimuConfigParser::ParseDevelopmentCfgNode(TiXmlElement *pRootXmlEle, ConfigSimuModelInfo_t *pSimuModel)
{
	TiXmlElement	*pExSrcIncEle = NULL;
	TiXmlElement	*pExUserFileEle = NULL;
	TiXmlElement	*pTmpXmlEle = NULL;
	const char	*pTmpStr = NULL;

	// CompilerOpt
	pTmpXmlEle = pRootXmlEle->FirstChildElement("CompilerOpt");
	if( pTmpXmlEle == NULL){
		m_szErrorMsg = "CompilerOpt node is Null";
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	if( pTmpStr == NULL){
		pSimuModel->szCompilerOpt = "";;
	}else{
		pSimuModel->szCompilerOpt = pTmpStr;
	}

	// LinkerOpt
	pTmpXmlEle = pRootXmlEle->FirstChildElement("LinkerOpt");
	if( pTmpXmlEle == NULL){
		m_szErrorMsg = "LinkerOpt node is Null";
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	if( pTmpStr == NULL){
		pSimuModel->szLinkerOpt = "";
	}else{
		pSimuModel->szLinkerOpt = pTmpStr;
	}

	// ExSrcIncFiles
	pExSrcIncEle = pRootXmlEle->FirstChildElement("ExSrcIncFiles");
	if( pExSrcIncEle == NULL){
		m_szErrorMsg = "ExSrcIncFiles node is Null";
		return false;
	}
	for( pTmpXmlEle = pExSrcIncEle->FirstChildElement("UserFile"); pTmpXmlEle != NULL; 
		pTmpXmlEle = pTmpXmlEle->NextSiblingElement("UserFile"))
	{
		ConfigExSrcIncFile_t *pCfgFile = new ConfigExSrcIncFile_t;
		pTmpStr = pTmpXmlEle->Attribute("FileName");
		pCfgFile->szName = pTmpStr;
		pTmpStr = pTmpXmlEle->Attribute("FullPath");
		pCfgFile->szFullPath = pTmpStr;
		pSimuModel->vecSrcIncFile.push_back(pCfgFile);
	}

	// ExUserFiles
	pExUserFileEle = pRootXmlEle->FirstChildElement("ExUserFiles");
	if( pExUserFileEle == NULL){
		m_szErrorMsg = "ExUserFiles node is Null";
		return false;
	}
	for( pTmpXmlEle = pExUserFileEle->FirstChildElement("UserFile"); pTmpXmlEle != NULL; 
		pTmpXmlEle = pTmpXmlEle->NextSiblingElement("UserFile"))
	{
		ConfigExUserFile_t *pCfgFile = new ConfigExUserFile_t;
		pTmpStr = pTmpXmlEle->Attribute("FileName");
		pCfgFile->szName = pTmpStr;
		pTmpStr = pTmpXmlEle->Attribute("FullPath");
		pCfgFile->szFullPath = pTmpStr;
		pSimuModel->vecUserFile.push_back(pCfgFile);
	}
	return true;
}

/*解析DataDictionary Node*/
bool CSimuConfigParser::ParseDataDictionaryNode(TiXmlElement *pRootXmlEle, ConfigSimuModelInfo_t *pSimuModel)
{
	TiXmlElement	*pInputSigEle = NULL;
	TiXmlElement	*pOutputSigEle = NULL;
	TiXmlElement	*pInterParamEle = NULL;
	TiXmlElement	*pTmpXmlEle = NULL;
	const char	*pTmpStr = NULL;

	// InputSignals
	pInputSigEle = pRootXmlEle->FirstChildElement("InputSignals");
	if( pInputSigEle == NULL){
		m_szErrorMsg = "InputSignals node is null";
		return false;
	}

	// InputSig
	for( pTmpXmlEle = pInputSigEle->FirstChildElement("InputSig"); pTmpXmlEle != NULL;
		pTmpXmlEle = pTmpXmlEle->NextSiblingElement("InputSig"))
	{
		ConfigInputSig_t *pInput = new ConfigInputSig_t;
		pInput->nModelId = pSimuModel->nId;

		if( ParseDataDict_InputSigNode(pTmpXmlEle, pInput)){
			pSimuModel->vecInputSig.push_back(pInput);
			ConfigInputSig_t *pInput2 = new ConfigInputSig_t;
			CopyInputSig(pInput, pInput2);
			m_vecInputSig.push_back(pInput2);
		}else{
			m_szErrorMsg = "ParseDataDictNode->ParseDataDict_InputSigNode failed";
			delete pInput;
			return false;
		}
	}

	// OutputSignals
	pOutputSigEle = pRootXmlEle->FirstChildElement("OutputSignals");
	if( pOutputSigEle == NULL){
		m_szErrorMsg = "OutputSignals node is null";
		return false;
	}

	// OutputSig
	for( pTmpXmlEle = pOutputSigEle->FirstChildElement("OutputSig"); pTmpXmlEle != NULL;
		pTmpXmlEle = pTmpXmlEle->NextSiblingElement("OutputSig"))
	{
		ConfigOutputSig_t *pOutput = new ConfigOutputSig_t;
		pOutput->nModelId = pSimuModel->nId;

		if( ParseDataDict_OutputSigNode(pTmpXmlEle, pOutput)){
			pSimuModel->vecOutputSig.push_back(pOutput);
			ConfigOutputSig_t *pOutput2 = new ConfigOutputSig_t;
			CopyOutputSig(pOutput, pOutput2);
			m_vecOutputSig.push_back(pOutput2);
		}else{
			m_szErrorMsg = "ParseDataDictNode->ParseDataDict_OutputSigNode failed";
			delete pOutput;
			return false;
		}
	}

	/*解析录制组*/
	ConfigRecordGroup_t *pRGroup = NULL;
	char szDefault[256];

	// 新建默认分组
	pRGroup = new ConfigRecordGroup_t;
	pRGroup->nId = 1;
	pRGroup->nModelId = pSimuModel->nId;
	pRGroup->nRecordTimes = 1;
	pRGroup->szName = "defaultGroup";
	sprintf(szDefault, "model%d_data_default_%d.dat", pSimuModel->nId, time(NULL));
	pRGroup->szRecordFile = szDefault;
	pRGroup->szRecordPath = "";
	pRGroup->bSaveIntegerHex = false;
	pRGroup->bSaveScieNotation = false;
	pRGroup->nSaveEffectBits = 6;
	m_vecRecordGroup.push_back(pRGroup);

	//// 解析用户分组
	//for( pTmpXmlEle = pOutputSigEle->FirstChildElement("RecordGroup"); pTmpXmlEle != NULL;
	//	pTmpXmlEle = pTmpXmlEle->NextSiblingElement("RecordGroup"))
	//{
	//	pRGroup = new ConfigRecordGroup_t;
	//	pRGroup->nModelId = nModelId;

	//	if( ParseOutputRecordGroupNode(pTmpXmlEle, pRGroup)){
	//		m_vecRecordGroup.push_back(pRGroup);
	//	}else{
	//		m_szErrorMsg = "ParseRecordGroupNod failed";
	//		delete pRGroup;
	//		return false;
	//	}
	//}

	// InternalParams
	pInterParamEle = pRootXmlEle->FirstChildElement("InternalParams");
	if( pInterParamEle == NULL){
		m_szErrorMsg = "InternalParams node is null";
		return false;
	}

	// Param
	for( pTmpXmlEle = pInterParamEle->FirstChildElement("Param"); pTmpXmlEle != NULL;
		pTmpXmlEle = pTmpXmlEle->NextSiblingElement("Param"))
	{
		ConfigInterParam_t *pParam = new ConfigInterParam_t;
		pParam->nModelId = pSimuModel->nId;

		if( ParseDataDict_ParamNode(pTmpXmlEle, pParam)){
			pSimuModel->vecInteParam.push_back(pParam);
			ConfigInterParam_t *pParam2 = new ConfigInterParam_t;
			CopyInterParam(pParam, pParam2);
			m_vecParams.push_back(pParam2);
		}else{
			m_szErrorMsg = "ParseDataDictNode->ParseDataDict_ParamNode failed";
			delete pParam;
			return false;
		}
	}

	return true;
}

/*解析输入信号节点*/
bool CSimuConfigParser::ParseDataDict_InputSigNode(TiXmlElement *pRootXmlEle, ConfigInputSig_t *pInput)
{
	TiXmlElement	*pTmpXmlEle = NULL;
	const char	*pTmpStr = NULL;

	pTmpStr = pRootXmlEle->Attribute("ID");
	pInput->nId = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	//pTmpStr = pRootXmlEle->Attribute("Name");
	//pInput->szName = (pTmpStr != NULL) ? pTmpStr : "";

	pTmpStr = pRootXmlEle->Attribute("DataType");
	pInput->eDataType = (pTmpStr != NULL) ? (ConfigDataTypeE)atoi(pTmpStr): CONF_DATA_Type_unknown;

	pTmpStr = pRootXmlEle->Attribute("TypeName");
	pInput->szTypeName = (pTmpStr != NULL) ? pTmpStr : "";

	//pTmpStr = pRootXmlEle->Attribute("vecSize");
	//pInput->nVecSize = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	pTmpStr = pRootXmlEle->Attribute("AssOutputSigID");
	pInput->nAssOutSigId = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	// NO
	pTmpStr = pRootXmlEle->Attribute("NO");
	pInput->nNO = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	// Name
	pTmpXmlEle = pRootXmlEle->FirstChildElement("Name");
	if( pTmpXmlEle == NULL){
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	if( pTmpStr != NULL) pInput->szName = pTmpStr;
	else pInput->szName = "";

	// Path
	pTmpXmlEle = pRootXmlEle->FirstChildElement("Path");
	if( pTmpXmlEle == NULL){
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	if( pTmpStr != NULL) pInput->szPath = pTmpStr;
	else pInput->szPath = "";

	// BlockType
	pTmpXmlEle = pRootXmlEle->FirstChildElement("BlockType");
	if( pTmpXmlEle == NULL){
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	if( pTmpStr != NULL) pInput->szBlockType = pTmpStr;
	else pInput->szBlockType = "";	

	// Width
	pTmpStr = pRootXmlEle->Attribute("Width");
	pInput->nVecSize = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	// Row 
	pTmpStr = pRootXmlEle->Attribute("Rol");
	pInput->nRow = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	// Col 
	pTmpStr = pRootXmlEle->Attribute("Col");
	pInput->nCol = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	return true;
}

/*解析数据存储组节点*/
bool CSimuConfigParser::ParseOutputRecordGroupNode(TiXmlElement *pRootXmlEle, ConfigRecordGroup_t *pGroup)
{
	const char *pTmpStr = NULL;

	pTmpStr = pRootXmlEle->Attribute("ID");
	pGroup->nId = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	pTmpStr = pRootXmlEle->Attribute("Name");
	pGroup->szName = (pTmpStr != NULL) ? pTmpStr : "";

	pTmpStr = pRootXmlEle->Attribute("RecordTimes");
	pGroup->nRecordTimes = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	pTmpStr = pRootXmlEle->Attribute("RecordFile");
	pGroup->szRecordFile = (pTmpStr != NULL) ? pTmpStr : "";

	pTmpStr = pRootXmlEle->Attribute("RecordPath");
	pGroup->szRecordPath = (pTmpStr != NULL) ? pTmpStr : "";

	pTmpStr = pRootXmlEle->Attribute("SaveIntegerHex");
	pGroup->bSaveIntegerHex = (pTmpStr != NULL && atoi(pTmpStr) == 1) ? true: false;

	pTmpStr = pRootXmlEle->Attribute("SaveScieNotation");
	pGroup->bSaveScieNotation = (pTmpStr != NULL && atoi(pTmpStr) == 1) ? true: false;

	pTmpStr = pRootXmlEle->Attribute("SaveEffectBits");
	pGroup->nSaveEffectBits = (pTmpStr != NULL) ? atoi(pTmpStr): 6;

	return true;
}

/*解析输出信号节点*/
bool CSimuConfigParser::ParseDataDict_OutputSigNode(TiXmlElement *pRootXmlEle, ConfigOutputSig_t *pOutput)
{
	TiXmlElement	*pTmpXmlEle = NULL;
	const char	*pTmpStr = NULL;

	pTmpStr = pRootXmlEle->Attribute("ID");
	pOutput->nId = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	//pTmpStr = pRootXmlEle->Attribute("name");
	//pOutput->szName = (pTmpStr != NULL) ? pTmpStr : "";

	pTmpStr = pRootXmlEle->Attribute("DataType");
	pOutput->eDataType = (pTmpStr != NULL) ? (ConfigDataTypeE)atoi(pTmpStr): CONF_DATA_Type_unknown;

	pTmpStr = pRootXmlEle->Attribute("TypeName");
	pOutput->szTypeName = (pTmpStr != NULL) ? pTmpStr : "";

	//pTmpStr = pRootXmlEle->Attribute("vecSize");
	//pOutput->nVecSize = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	pTmpStr = pRootXmlEle->Attribute("Record");
	pOutput->bRecord = (pTmpStr != NULL && atoi(pTmpStr) == 1) ? true: false;

	/*pTmpStr = pRootXmlEle->Attribute("recordGroup");
	pOutput->nRecordGroup = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	if( pOutput->nRecordGroup <= 1){
		pOutput->nRecordGroup = 1;
	}*/
	pOutput->nRecordGroup = 1;

	pTmpStr = pRootXmlEle->Attribute("Display");
	pOutput->bDisplay = (pTmpStr != NULL && atoi(pTmpStr) == 1) ? true: false;

	pTmpStr = pRootXmlEle->Attribute("SyncOut");
	pOutput->bSyncOutput = (pTmpStr != NULL && atoi(pTmpStr) == 1) ? true: false;

	// NO
	pTmpStr = pRootXmlEle->Attribute("NO");
	pOutput->nNO = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	// Name
	pTmpXmlEle = pRootXmlEle->FirstChildElement("Name");
	if( pTmpXmlEle == NULL){
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	if( pTmpStr != NULL) pOutput->szName = pTmpStr;
	else pOutput->szName = "";

	// Path
	pTmpXmlEle = pRootXmlEle->FirstChildElement("Path");
	if( pTmpXmlEle == NULL){
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	if( pTmpStr != NULL) pOutput->szPath = pTmpStr;
	else pOutput->szPath = "";

	// BlockType
	pTmpXmlEle = pRootXmlEle->FirstChildElement("BlockType");
	if( pTmpXmlEle == NULL){
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	if( pTmpStr != NULL) pOutput->szBlockType = pTmpStr;
	else pOutput->szBlockType = "";	

	// Width
	pTmpStr = pRootXmlEle->Attribute("Width");
	pOutput->nVecSize = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	// Row 
	pTmpStr = pRootXmlEle->Attribute("Row");
	pOutput->nRow = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	// Col 
	pTmpStr = pRootXmlEle->Attribute("Col");
	pOutput->nCol = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	return true;
}

/*解析内部参数节点*/
bool CSimuConfigParser::ParseDataDict_ParamNode(TiXmlElement *pRootXmlEle, ConfigInterParam_t *pParam)
{
	TiXmlElement	*pTmpXmlEle = NULL;
	const char	*pTmpStr = NULL;

	pTmpStr = pRootXmlEle->Attribute("ID");
	pParam->nId = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	//pTmpStr = pRootXmlEle->Attribute("Name");
	//pParam->szName = (pTmpStr != NULL) ? pTmpStr : "";

	pTmpStr = pRootXmlEle->Attribute("DataType");
	pParam->eDataType = (pTmpStr != NULL) ? (ConfigDataTypeE)atoi(pTmpStr): CONF_DATA_Type_unknown;

	pTmpStr = pRootXmlEle->Attribute("TypeName");
	pParam->szTypeName = (pTmpStr != NULL) ? pTmpStr : "";

	//pTmpStr = pRootXmlEle->Attribute("vecSize");
	//pParam->nVecSize = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	// NO
	pTmpStr = pRootXmlEle->Attribute("NO");
	pParam->nNO = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	// Name
	pTmpXmlEle = pRootXmlEle->FirstChildElement("Name");
	if( pTmpXmlEle == NULL){
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	if( pTmpStr != NULL) pParam->szName = pTmpStr;
	else pParam->szName = "";

	// Path
	pTmpXmlEle = pRootXmlEle->FirstChildElement("Path");
	if( pTmpXmlEle == NULL){
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	if( pTmpStr != NULL) pParam->szPath = pTmpStr;
	else pParam->szPath = "";

	// InitValue
	pTmpXmlEle = pRootXmlEle->FirstChildElement("Value");
	if( pTmpXmlEle == NULL){
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	if( pTmpStr != NULL) pParam->szInitValue = pTmpStr;
	else pParam->szInitValue = "";	

	// BlockType
	pTmpXmlEle = pRootXmlEle->FirstChildElement("BlockType");
	if( pTmpXmlEle == NULL){
		return false;
	}
	pTmpStr = pTmpXmlEle->GetText();
	if( pTmpStr != NULL) pParam->szBlockType = pTmpStr;
	else pParam->szBlockType = "";	

	// Width
	pTmpStr = pRootXmlEle->Attribute("Width");
	pParam->nVecSize = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	// Row 
	pTmpStr = pRootXmlEle->Attribute("Row");
	pParam->nRow = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	// Col 
	pTmpStr = pRootXmlEle->Attribute("Col");
	pParam->nCol = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

	return true;
}

/*解析测试用例节点*/
bool CSimuConfigParser::ParseTestCaseNode(TiXmlElement *pRootXmlEle)
{
	TiXmlElement *pTestCaseEle = NULL;
	TiXmlElement *pTestStartEle = NULL;
	TiXmlElement *pTestEndEle = NULL;
	TiXmlElement *pActionEle = NULL;
	TiXmlElement *pEventEle = NULL;

	const char	*pTmpStr = NULL;

	for( pTestCaseEle = pRootXmlEle->FirstChildElement("TestCase"); pTestCaseEle != NULL;
		pTestCaseEle = pTestCaseEle->NextSiblingElement("TestCase"))
	{
		ConfigTestCase_t *pTestCase = new ConfigTestCase_t;

		pTmpStr = pTestCaseEle->Attribute("id");
		pTestCase->nId = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

		pTmpStr = pTestCaseEle->Attribute("name");
		pTestCase->szName = (pTmpStr != NULL) ? pTmpStr : "";

		pTmpStr = pTestCaseEle->Attribute("waitTime");
		pTestCase->nWaitTime = (pTmpStr != NULL) ? atoi(pTmpStr):0;

		/* TestStart */
		pTestStartEle = pTestCaseEle->FirstChildElement("TestStart");
		if( pTestStartEle != NULL){
			for( pActionEle = pTestStartEle->FirstChildElement("Action"); pActionEle != NULL;
				pActionEle = pActionEle->NextSiblingElement("Action"))
			{/* Action */
				ConfigTestAction_t  *pAction = new ConfigTestAction_t;
				
				pTmpStr = pActionEle->Attribute("DD_ID");
				pAction->nDDId = (pTmpStr != NULL)? atoi(pTmpStr): 0;

				pTmpStr = pActionEle->Attribute("SetValue");
				pAction->szSetValue = (pTmpStr != NULL)? pTmpStr: "";

				pTestCase->vecStartCond.push_back(pAction);
			}
		}else{
			m_szErrorMsg = "ParseTestCaseNode->TestStart is NULL";
			delete pTestCase;
			return false;
		}

		/* TestEnd */
		pTestEndEle = pTestCaseEle->FirstChildElement("TestEnd");
		if( pTestEndEle != NULL){
			for( pEventEle = pTestEndEle->FirstChildElement("Event"); pEventEle != NULL;
				pEventEle = pEventEle->NextSiblingElement("Event"))
			{/* Event */
				ConfigTestEvent_t *pEvent = new ConfigTestEvent_t;

				pTmpStr = pEventEle->Attribute("Type");
				pEvent->eType = (pTmpStr != NULL)? (ConfigEventTypeE)atoi(pTmpStr): CONF_EVENT_Type_unknown;

				pTmpStr = pEventEle->Attribute("DD_ID");
				pEvent->nDDId = (pTmpStr != NULL)? atoi(pTmpStr): 0;

				pTmpStr = pEventEle->Attribute("Sign");
				pEvent->eSign = (pTmpStr != NULL)? (ConfigValueSignTypeE)atoi(pTmpStr): CONF_VAL_SIGN_Type_unknown;

				pTmpStr = pEventEle->Attribute("ReferValue");
				pEvent->szReferValue = (pTmpStr != NULL) ? pTmpStr : "";

				pTestCase->vecEndCond.push_back(pEvent);
			}
		}else{
			m_szErrorMsg = "ParseTestCaseNode->TestEnd is NULL";
			delete pTestCase;
			return false;
		}

		m_vecTestCase.push_back(pTestCase);
	}
	return true;
}

/*解析测试任务节点*/
bool CSimuConfigParser::ParseTestTaskNode(TiXmlElement *pRootXmlEle)
{
	TiXmlElement *pTestTaskEle = NULL;
	TiXmlElement *pTestCaseEle = NULL;

	const char	*pTmpStr = NULL;

	for( pTestTaskEle = pRootXmlEle->FirstChildElement("TestTask"); pTestTaskEle != NULL;
		pTestTaskEle = pTestTaskEle->NextSiblingElement("TestTask"))
	{/* TestTask */
		ConfigTestTask_t *pTestTask = new ConfigTestTask_t;
	
		pTmpStr = pTestTaskEle->Attribute("id");
		pTestTask->nId = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

		pTmpStr = pTestTaskEle->Attribute("name");
		pTestTask->szName = (pTmpStr != NULL) ? pTmpStr : "";

		for( pTestCaseEle = pTestTaskEle->FirstChildElement("TestCase"); pTestCaseEle != NULL;
			pTestCaseEle = pTestCaseEle->NextSiblingElement("TestCase"))
		{/* TestExecCase */
			ConfigTestExecCase_t *pExecCase = new ConfigTestExecCase_t;
			
			pTmpStr = pTestCaseEle->Attribute("id");
			pExecCase->nId = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

			pTmpStr = pTestCaseEle->Attribute("execTimes");
			pExecCase->nExecTimes = (pTmpStr != NULL) ? atoi(pTmpStr): 0;

			pTestTask->vecExecCase.push_back(pExecCase);
		}
		m_vecTestTask.push_back(pTestTask);
	}

	return true;
}

bool CSimuConfigParser::CheckProject()
{
	return true;
}

bool CSimuConfigParser::CheckSimuModel()
{
	return true;
}

bool CSimuConfigParser::CheckModelInfo()
{
	return true;
}

bool CSimuConfigParser::CheckExUserFile()
{
	return true;
}

bool CSimuConfigParser::CheckInputSig()
{
	return true;
}

bool CSimuConfigParser::CheckRecordGroup()
{
	return true;
}

bool CSimuConfigParser::CheckOutputSig()
{
	return true;
}

bool CSimuConfigParser::CheckParams()
{
	return true;
}

bool CSimuConfigParser::CheckTestCase()
{
	return true;
}

bool CSimuConfigParser::CheckTestTask()
{
	return true;
}

void CSimuConfigParser::FreeSimuModel(ConfigSimuModelInfo_t *pModelInfo)
{
	for( unsigned int j = 0; j < pModelInfo->vecInputSig.size();j++)
		delete pModelInfo->vecInputSig.at(j);
	pModelInfo->vecInputSig.clear();
	for( unsigned int j = 0; j < pModelInfo->vecOutputSig.size();j++)
		delete pModelInfo->vecOutputSig.at(j);
	pModelInfo->vecOutputSig.clear();
	for( unsigned int j = 0; j < pModelInfo->vecInteParam.size();j++)
		delete pModelInfo->vecInteParam.at(j);
	pModelInfo->vecInteParam.clear();
	for( unsigned int j = 0; j < pModelInfo->vecSrcIncFile.size();j++)
		delete pModelInfo->vecSrcIncFile.at(j);
	pModelInfo->vecSrcIncFile.clear();
	for( unsigned int j = 0; j < pModelInfo->vecUserFile.size();j++)
		delete pModelInfo->vecUserFile.at(j);
	pModelInfo->vecUserFile.clear();
}

bool CSimuConfigParser::Ex_GenerateModelConfigToFile(FILE *fp , ConfigSimuModelInfo_t *pSimuModel)
{
	char szLine[1024] = {0};
	char szTmp[128] = {0};

	/****************** 写入工程信息********************/
	fprintf(fp, "[Project]\n");
	fprintf(fp, "SimuProj_ID=%s\n", m_ProjInfo.szProjId.c_str());
	fprintf(fp, "SimuProj_Name=%s\n", m_ProjInfo.szProjName.c_str());
	fprintf(fp, "SimuMode=%d\n", m_SimuMode.eSimuMode);
	fprintf(fp, "MasterModelID=%d\n", m_SimuMode.nMasterModelId);
	fprintf(fp, "\n");

	
	/***************写入模型信息***********/
	fprintf(fp, "[Model]\n");
	fprintf(fp, "ModelID=%d\n", pSimuModel->nId);
	fprintf(fp, "ModelName=%s\n", pSimuModel->szName.c_str());
	if( pSimuModel->nCoreNum <= 0){
		fprintf(fp, "TargetCore=-1\n");
	}else{
		sprintf(szLine, "TargetCore=");
		for(unsigned i = 0; i < pSimuModel->nCoreNum; i++){
			if( i == 0) sprintf(szTmp, "%d", pSimuModel->szCore[i]);
			else sprintf(szTmp, ",%d", pSimuModel->szCore[i]);
			strcat(szLine, szTmp);
		}
		fprintf(fp, "%s\n", szLine);
	}
	fprintf(fp, "SimuStep=%lf\n",pSimuModel->dRunStep);
	fprintf(fp, "RecordTime=%lf\n", pSimuModel->dRecordRate);
	fprintf(fp, "DispTime=%lf\n", pSimuModel->dDispRate);
	fprintf(fp, "\n");

	/***************写入其他模型信息***********/
	fprintf(fp, "[OtherModel]\n");
	for(unsigned i = 0; i < m_vecModelInfo.size(); i++){
		ConfigSimuModelInfo_t *pModel = m_vecModelInfo.at(i);
		if( pModel->nId == pSimuModel->nId)continue;
		fprintf(fp, "%d|%lf\n", pModel->nId, pModel->dRunStep);
	}
	fprintf(fp, "\n");
	
	/***************写入模型input***********/
	int nInputNum = 0;
	fprintf(fp, "[Input]\n");
	nInputNum = pSimuModel->vecInputSig.size();
	fprintf(fp, "Num=%d\n", nInputNum);
	fprintf(fp, "#NO|ID|Name|Width|ModleId|OutputId|Key\n");
	for(unsigned i = 0; i < pSimuModel->vecInputSig.size(); i++){
		int nkey = 0, nModelId = 0;
		ConfigSimuModelInfo_t *pModel = NULL;
		ConfigInterParam_t *pParam = NULL;
		ConfigInputSig_t *pInSig = NULL;
		
		nkey = 0; 
		nModelId = 0;
		pInSig = pSimuModel->vecInputSig.at(i);
		pParam = Ex_GetInteParamByInputSig(pSimuModel, pInSig->szPath.c_str(), "");
		if( pParam != NULL){
			nkey = atoi(pParam->szInitValue.c_str());
		}
		pModel = Ex_GetSimuModelByOutputID(pSimuModel->nId, pInSig->nAssOutSigId);
		if( pModel != NULL){
			nModelId = pModel->nId;
		}
		fprintf(fp, "%d|%d|%s|%d|%d|%d|%d\n", pInSig->nNO, pInSig->nId, pInSig->szName.c_str(), 
			pInSig->nVecSize, nModelId, pInSig->nAssOutSigId, nkey);
	}
	fprintf(fp, "\n");

	/***************写入模型output***********/
	int nOutputNum = 0;
	int nOutputStartIdx = 0;
	fprintf(fp, "[Output]\n");
	for( unsigned i = 0; i < pSimuModel->vecOutputSig.size(); i++){
		ConfigOutputSig_t *pOutSig =  pSimuModel->vecOutputSig.at(i);
		if( pOutSig->bDisplay || pOutSig->bRecord || pOutSig->bSyncOutput){
			nOutputNum++;
		}
	}
	fprintf(fp, "Num=%d\n", nOutputNum);
	fprintf(fp, "#NO|ID|Name|Width|StartIdx|bDisp|bRecord|bSync|Key|ModelID|InputId\n");
	for(unsigned i = 0; i < pSimuModel->vecOutputSig.size(); i++){
		int nkey = 0, nModelId = 0, nInputId = 0;
		ConfigOutputSig_t *pOutSig = NULL;
		ConfigInterParam_t *pParam = NULL;
		ConfigInputSig_t *pInSig = NULL;

		nkey = 0;
		nModelId = 0;
		nInputId = 0;
		pOutSig = pSimuModel->vecOutputSig.at(i);
		pInSig = Ex_GetAssInputByOutputID(pSimuModel->nId, pOutSig->nId);
		if( pInSig != NULL && pOutSig->bSyncOutput){
			pParam = Ex_GetInteParamByOutputSig(pSimuModel, pOutSig->szPath.c_str(), "");
			if( pParam != NULL)
				nkey = atoi(pParam->szInitValue.c_str());
			nModelId = pInSig->nModelId;
			nInputId = pInSig->nId;
		}
		if( pOutSig->bDisplay || pOutSig->bRecord || pOutSig->bSyncOutput){
			fprintf(fp, "%d|%d|%s|%d|%d|%d|%d|%d|%d|%d|%d\n", pOutSig->nNO, pOutSig->nId, pOutSig->szName.c_str(), 
				pOutSig->nVecSize, nOutputStartIdx, pOutSig->bDisplay, pOutSig->bRecord, pOutSig->bSyncOutput,
				nkey, nModelId, nInputId);
		}
		nOutputStartIdx += pOutSig->nVecSize;
	}
	fprintf(fp, "\n");

	/***************写入模型param***********/
	int nParamNum = 0;
	int nParamStartIdx = 0;
	fprintf(fp, "[Param]\n");
	nParamNum = pSimuModel->vecInteParam.size();
	fprintf(fp, "Num=%d\n", nParamNum);
	fprintf(fp, "#NO|ID|Name|Width|StartIdx\n");
	for(unsigned i = 0; i < pSimuModel->vecInteParam.size(); i++){
		ConfigInterParam_t *pParam = NULL;
		pParam = pSimuModel->vecInteParam.at(i);
		fprintf(fp, "%d|%d|%s|%d|%d\n", pParam->nNO, pParam->nId, pParam->szName.c_str(), 
			pParam->nVecSize, nParamStartIdx);
		nParamStartIdx += pParam->nVecSize;
	}
	fprintf(fp, "\n");

	return true;
}

ConfigInterParam_t *CSimuConfigParser::Ex_GetInteParamByInputSig(ConfigSimuModelInfo_t *pSimuModel, const char *szPath, const char *szName)
{
	ConfigInterParam_t *pParam = NULL;
	bool bFind = false;

	for( unsigned i = 0; i < pSimuModel->vecInteParam.size(); i++){
		pParam = pSimuModel->vecInteParam.at(i);
		//if( strcmp(pParam->szPath.c_str(), szPath) == 0 && strcmp(pParam->szName.c_str(), szName) == 0){
		if( strcmp(pParam->szPath.c_str(), szPath) == 0 ){
			bFind = true;
			break;
		}
	}
	if( bFind ){
		return pParam;
	}
	return NULL;
}

ConfigInterParam_t *CSimuConfigParser::Ex_GetInteParamByOutputSig(ConfigSimuModelInfo_t *pSimuModel, const char *szPath, const char *szName)
{
	ConfigInterParam_t *pParam = NULL;
	bool bFind = false;

	for( unsigned i = 0; i < pSimuModel->vecInteParam.size(); i++){
		pParam = pSimuModel->vecInteParam.at(i);
		//if( strcmp(pParam->szPath.c_str(), szPath) == 0 && strcmp(pParam->szName.c_str(), szName) == 0){
		if( strcmp(pParam->szPath.c_str(), szPath) == 0 ){
			bFind = true;
			break;
		}
	}
	if( bFind ){
		return pParam;
	}
	return NULL;
}

ConfigSimuModelInfo_t *CSimuConfigParser::Ex_GetSimuModelByOutputID(int ncurModelId, int nOutputId)
{
	ConfigSimuModelInfo_t *pSimuModel = NULL;
	bool bFind = false;

	for(unsigned i = 0; i < m_vecModelInfo.size(); i++){
		pSimuModel = m_vecModelInfo.at(i);
		if( ncurModelId == pSimuModel->nId)continue;
		for( unsigned j = 0; j < pSimuModel->vecOutputSig.size(); j++){
			if( pSimuModel->vecOutputSig.at(j)->nId == nOutputId){
				bFind = true;
				break;
			}
		}
		if( bFind){
			break;
		}
	}
	if( bFind)
		return pSimuModel;
	return NULL;
}

ConfigInputSig_t *CSimuConfigParser::Ex_GetAssInputByOutputID(int ncurModelId, int nOutputId)
{
	ConfigSimuModelInfo_t *pModel = NULL;
	ConfigInputSig_t *pInput = NULL;
	bool bFind = false;
	for( unsigned i = 0; i < m_vecModelInfo.size(); i++){
		pModel = m_vecModelInfo.at(i);
		if( pModel->nId == ncurModelId)continue;
		for(unsigned j = 0; j < pModel->vecInputSig.size(); j++){
			pInput = pModel->vecInputSig.at(j);
			if( pInput->nAssOutSigId == nOutputId){
				bFind = true;
				break;
			}
		}
		if( bFind)break;
	}
	if( bFind)return pInput;
	return NULL;
}
