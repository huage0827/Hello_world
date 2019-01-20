/*****************************************************************
 * 文件名: SimuConfigParser.h
 * 功能: 解析模型配置文件头文件
 *****************************************************************/
#pragma  once

#include <vector>	
#include <string>
#include <map>
#include <iostream>
#include "SimuConfigDef.h"
#include "tinyxml.h"
#include "tinystr.h"

using namespace std;

/****************************************************************
 * 类名 : CSimuConfigParser
 * 父类 : 无
 * 说明 : 配置文件解析类
 ****************************************************************/
class CSimuConfigParser{

public:
	/*构造函数*/
	CSimuConfigParser();
	/*析构函数*/
	~CSimuConfigParser();

	/* 加载配置文件 */
	bool  LoadCfg(const char *szCfgFile); 
	/* 释放资源 */
	void  UnLoad(); 
	/* 配置合法性检查*/
	bool  CheckCfg(); 	
	/* 获取错误信息*/
	const char *GetLastErrMsg(); 
	/* 获取配置文件*/
	const char *GetCfgFile();

	/* 数据返回 */
	const ConfigProjectInfo_t* GetProjInfo(); //返回工程配置信息
	const ConfigSimuMode_t* GetSimuMode(); //  返回仿真配置
	const vector<ConfigSimuModelInfo_t *> *GetModelInfo();//返回子模型信息列表
	//const vector<ConfigExUserFile_t *> *GetExUserFile();//返回用户额外文件列表
	const vector<ConfigInputSig_t *> *GetInputSig();//返回输入信号列表
	const vector<ConfigRecordGroup_t *> *GetRecordGroup();//返回记录组列表
	const vector<ConfigOutputSig_t *> *GetOutputSig();//返回输出信号列表
	const vector<ConfigInterParam_t *> *GetParams();//返回内部参数列表
	const vector<ConfigTestCase_t *> *GetTestCase();//返回测试用例列表
	const vector<ConfigTestTask_t *> *GetTestTask();//返回测试任务列表

	/* 打印输出 */
	void PrintAll();
	void PrintProject();
	void PrintSimuMode();
	void PrintModeInfo();
	//void PrintExUserFile();
	void PrintInputSig();
	void PrintRecordGroup();
	void PrintOutputSig();
	void PrintParams();
	void PrintTestCase();
	void PrintTestTask();

	/*根据类型返回其所占字节*/
	static int GetDataTypeByteLen(int type);

	static int CopyProjectInfo(const ConfigProjectInfo_t *pSrcProj,ConfigProjectInfo_t *pDesProj);//复制工程信息
//	static int CopyModelInfo(const ConfigSimuModelInfo_t *pSrcModel, ConfigSimuModelInfo_t *pDesModel);//复制模型信息
	static int CopyInputSig(const ConfigInputSig_t *pSrcSig, ConfigInputSig_t *pDesSig);//复制输入信号
	static int CopyOutputSig(const ConfigOutputSig_t *pSrcSig, ConfigOutputSig_t* pDesSig);//复制输出信号
	static int CopyInterParam(const ConfigInterParam_t *pSrcParam, ConfigInterParam_t *pDesParam);//复制内部参数

	/*配置文件转换*/
	int Ex_GenerateModelConfig(vector<ConfigTargetModelDir_t *> *pVecCfgModel);

private:
	/*解析仿真模式节点*/
	bool ParseSimuModeNode(TiXmlElement *pRootXmlEle);
	/*解析仿真模型节点*/
	bool ParseSimuModelsNode(TiXmlElement *pRootXmlEle);
	
	/*解析ExModelInfo Node*/
	bool ParseExModelInfoNode(TiXmlElement *pRootXmlEle, ConfigSimuModelInfo_t *pSimuModel);
	/*解析SimulationConfig Node*/
	bool ParseSimulationCfgNode(TiXmlElement *pRootXmlEle, ConfigSimuModelInfo_t *pSimuModel);
	/*解析DevelopmentConfig Node*/
	bool ParseDevelopmentCfgNode(TiXmlElement *pRootXmlEle, ConfigSimuModelInfo_t *pSimuModel);
	/*解析DataDictionary Node*/
	bool ParseDataDictionaryNode(TiXmlElement *pRootXmlEle, ConfigSimuModelInfo_t *pSimuModel);
	
	/*解析输入信号节点*/
	bool ParseDataDict_InputSigNode(TiXmlElement *pRootXmlEle, ConfigInputSig_t *pInput);
	/*解析数据存储组节点*/
	bool ParseOutputRecordGroupNode(TiXmlElement *pRootXmlEle, ConfigRecordGroup_t *pGroup);
	/*解析输出信号节点*/
	bool ParseDataDict_OutputSigNode(TiXmlElement *pRootXmlEle, ConfigOutputSig_t *pOutput);
	/*解析内部参数节点*/
	bool ParseDataDict_ParamNode(TiXmlElement *pRootXmlEle, ConfigInterParam_t *pParam);

	/*解析测试用例节点*/
	bool ParseTestCaseNode(TiXmlElement *pRootXmlEle);
	/*解析测试任务节点*/
	bool ParseTestTaskNode(TiXmlElement *pRootXmlEle);

	/* 合法性检查 */
	bool CheckProject();
	bool CheckSimuModel();
	bool CheckModelInfo();
	bool CheckExUserFile();
	bool CheckInputSig();
	bool CheckRecordGroup();
	bool CheckOutputSig();
	bool CheckParams();
	bool CheckTestCase();
	bool CheckTestTask();

	/*释放资源*/
	void FreeSimuModel(ConfigSimuModelInfo_t *pModel);

	/*构造模型配置写到文件*/
	bool Ex_GenerateModelConfigToFile(FILE *fp , ConfigSimuModelInfo_t *pSimuModel);
	ConfigInterParam_t *Ex_GetInteParamByInputSig(ConfigSimuModelInfo_t *pSimuModel, const char *szPath, const char *szName);
	ConfigInterParam_t *Ex_GetInteParamByOutputSig(ConfigSimuModelInfo_t *pSimuModel, const char *szPath, const char *szName);
	ConfigSimuModelInfo_t *Ex_GetSimuModelByOutputID(int ncurModelId, int nOutputId);
	ConfigInputSig_t *Ex_GetAssInputByOutputID(int ncurModelId, int nOutputId);

protected:
	bool    m_bLoadCfg; /*是否已经加载配置*/
	string  m_szErrorMsg; /*错误信息*/
	string  m_szCfgFile; /*配置文件*/

	ConfigProjectInfo_t m_ProjInfo;/*工程信息*/
	ConfigSimuMode_t m_SimuMode;/*仿真模式*/
	vector<ConfigSimuModelInfo_t *> m_vecModelInfo;/*模型信息*/
//	vector<ConfigExUserFile_t *> m_vecExUserFile;/*额外用户文件*/
	vector<ConfigInputSig_t *> m_vecInputSig;/*模型输入信号*/
	vector<ConfigRecordGroup_t*> m_vecRecordGroup;/*数据存储编组*/
	vector<ConfigOutputSig_t *> m_vecOutputSig;/*模型输出信号*/
	vector<ConfigInterParam_t *> m_vecParams;/*模型内部参数*/

	vector<ConfigTestCase_t *> m_vecTestCase;/*测试用例*/
	vector<ConfigTestTask_t *> m_vecTestTask;/*测试任务*/
};

