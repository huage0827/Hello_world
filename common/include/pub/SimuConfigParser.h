/*****************************************************************
 * �ļ���: SimuConfigParser.h
 * ����: ����ģ�������ļ�ͷ�ļ�
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
 * ���� : CSimuConfigParser
 * ���� : ��
 * ˵�� : �����ļ�������
 ****************************************************************/
class CSimuConfigParser{

public:
	/*���캯��*/
	CSimuConfigParser();
	/*��������*/
	~CSimuConfigParser();

	/* ���������ļ� */
	bool  LoadCfg(const char *szCfgFile); 
	/* �ͷ���Դ */
	void  UnLoad(); 
	/* ���úϷ��Լ��*/
	bool  CheckCfg(); 	
	/* ��ȡ������Ϣ*/
	const char *GetLastErrMsg(); 
	/* ��ȡ�����ļ�*/
	const char *GetCfgFile();

	/* ���ݷ��� */
	const ConfigProjectInfo_t* GetProjInfo(); //���ع���������Ϣ
	const ConfigSimuMode_t* GetSimuMode(); //  ���ط�������
	const vector<ConfigSimuModelInfo_t *> *GetModelInfo();//������ģ����Ϣ�б�
	//const vector<ConfigExUserFile_t *> *GetExUserFile();//�����û������ļ��б�
	const vector<ConfigInputSig_t *> *GetInputSig();//���������ź��б�
	const vector<ConfigRecordGroup_t *> *GetRecordGroup();//���ؼ�¼���б�
	const vector<ConfigOutputSig_t *> *GetOutputSig();//��������ź��б�
	const vector<ConfigInterParam_t *> *GetParams();//�����ڲ������б�
	const vector<ConfigTestCase_t *> *GetTestCase();//���ز��������б�
	const vector<ConfigTestTask_t *> *GetTestTask();//���ز��������б�

	/* ��ӡ��� */
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

	/*�������ͷ�������ռ�ֽ�*/
	static int GetDataTypeByteLen(int type);

	static int CopyProjectInfo(const ConfigProjectInfo_t *pSrcProj,ConfigProjectInfo_t *pDesProj);//���ƹ�����Ϣ
//	static int CopyModelInfo(const ConfigSimuModelInfo_t *pSrcModel, ConfigSimuModelInfo_t *pDesModel);//����ģ����Ϣ
	static int CopyInputSig(const ConfigInputSig_t *pSrcSig, ConfigInputSig_t *pDesSig);//���������ź�
	static int CopyOutputSig(const ConfigOutputSig_t *pSrcSig, ConfigOutputSig_t* pDesSig);//��������ź�
	static int CopyInterParam(const ConfigInterParam_t *pSrcParam, ConfigInterParam_t *pDesParam);//�����ڲ�����

	/*�����ļ�ת��*/
	int Ex_GenerateModelConfig(vector<ConfigTargetModelDir_t *> *pVecCfgModel);

private:
	/*��������ģʽ�ڵ�*/
	bool ParseSimuModeNode(TiXmlElement *pRootXmlEle);
	/*��������ģ�ͽڵ�*/
	bool ParseSimuModelsNode(TiXmlElement *pRootXmlEle);
	
	/*����ExModelInfo Node*/
	bool ParseExModelInfoNode(TiXmlElement *pRootXmlEle, ConfigSimuModelInfo_t *pSimuModel);
	/*����SimulationConfig Node*/
	bool ParseSimulationCfgNode(TiXmlElement *pRootXmlEle, ConfigSimuModelInfo_t *pSimuModel);
	/*����DevelopmentConfig Node*/
	bool ParseDevelopmentCfgNode(TiXmlElement *pRootXmlEle, ConfigSimuModelInfo_t *pSimuModel);
	/*����DataDictionary Node*/
	bool ParseDataDictionaryNode(TiXmlElement *pRootXmlEle, ConfigSimuModelInfo_t *pSimuModel);
	
	/*���������źŽڵ�*/
	bool ParseDataDict_InputSigNode(TiXmlElement *pRootXmlEle, ConfigInputSig_t *pInput);
	/*�������ݴ洢��ڵ�*/
	bool ParseOutputRecordGroupNode(TiXmlElement *pRootXmlEle, ConfigRecordGroup_t *pGroup);
	/*��������źŽڵ�*/
	bool ParseDataDict_OutputSigNode(TiXmlElement *pRootXmlEle, ConfigOutputSig_t *pOutput);
	/*�����ڲ������ڵ�*/
	bool ParseDataDict_ParamNode(TiXmlElement *pRootXmlEle, ConfigInterParam_t *pParam);

	/*�������������ڵ�*/
	bool ParseTestCaseNode(TiXmlElement *pRootXmlEle);
	/*������������ڵ�*/
	bool ParseTestTaskNode(TiXmlElement *pRootXmlEle);

	/* �Ϸ��Լ�� */
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

	/*�ͷ���Դ*/
	void FreeSimuModel(ConfigSimuModelInfo_t *pModel);

	/*����ģ������д���ļ�*/
	bool Ex_GenerateModelConfigToFile(FILE *fp , ConfigSimuModelInfo_t *pSimuModel);
	ConfigInterParam_t *Ex_GetInteParamByInputSig(ConfigSimuModelInfo_t *pSimuModel, const char *szPath, const char *szName);
	ConfigInterParam_t *Ex_GetInteParamByOutputSig(ConfigSimuModelInfo_t *pSimuModel, const char *szPath, const char *szName);
	ConfigSimuModelInfo_t *Ex_GetSimuModelByOutputID(int ncurModelId, int nOutputId);
	ConfigInputSig_t *Ex_GetAssInputByOutputID(int ncurModelId, int nOutputId);

protected:
	bool    m_bLoadCfg; /*�Ƿ��Ѿ���������*/
	string  m_szErrorMsg; /*������Ϣ*/
	string  m_szCfgFile; /*�����ļ�*/

	ConfigProjectInfo_t m_ProjInfo;/*������Ϣ*/
	ConfigSimuMode_t m_SimuMode;/*����ģʽ*/
	vector<ConfigSimuModelInfo_t *> m_vecModelInfo;/*ģ����Ϣ*/
//	vector<ConfigExUserFile_t *> m_vecExUserFile;/*�����û��ļ�*/
	vector<ConfigInputSig_t *> m_vecInputSig;/*ģ�������ź�*/
	vector<ConfigRecordGroup_t*> m_vecRecordGroup;/*���ݴ洢����*/
	vector<ConfigOutputSig_t *> m_vecOutputSig;/*ģ������ź�*/
	vector<ConfigInterParam_t *> m_vecParams;/*ģ���ڲ�����*/

	vector<ConfigTestCase_t *> m_vecTestCase;/*��������*/
	vector<ConfigTestTask_t *> m_vecTestTask;/*��������*/
};

