
#ifndef SIMULINKCONFIGDEF_H_
#define SIMULINKCONFIGDEF_H_

#define SIMULINK_MAX_PROJ_ID_LEN 64
#define SIMULINK_MAX_NAME_LEN 	 64
#define SIMULINK_MAX_PATH_LEN 	 256
#define SIMULINK_MAX_BLOCK_LEN   64

typedef struct{
	char szProjId[SIMULINK_MAX_PROJ_ID_LEN];
	char szProjName[SIMULINK_MAX_NAME_LEN];
	int simuMode;
	int masterId;
}SimulinkProjCfg_t;

typedef struct{
	int no;
	int id;
	char szName[SIMULINK_MAX_NAME_LEN];
//	char szPath[SIMULINK_MAX_PATH_LEN];
//	char szBlock[SIMULINK_MAX_BLOCK_LEN];
	int width;
	int modelId;
	int outputId;
	int key;
}SimulinkInputCfg_t;

typedef struct{
	int no;
	int id;
	int startIdx;
	char szName[SIMULINK_MAX_NAME_LEN];
//	char szPath[SIMULINK_MAX_PATH_LEN];
//	char szBlock[SIMULINK_MAX_BLOCK_LEN];
	int width;
	int bDisp;
	int bRecord;
	int bSync;
	int key;
	int modelId;
	int inputId;
}SimulinkOutputCfg_t;

typedef struct{
	int no;
	int id;
	int startIdx;
	char szName[SIMULINK_MAX_NAME_LEN];
	//char szPath[SIMULINK_MAX_PATH_LEN];
	//char szBlock[SIMULINK_MAX_BLOCK_LEN];
	int width;
	//char *pInitVal;
}SimulinkParamCfg_t;

typedef struct{
	int id;
	char szName[SIMULINK_MAX_NAME_LEN];
	int  nCoreNum;
	char szCore[64];
	double simuStep;
	double recordTime;
	double dispTime;
	int inputNum;
	SimulinkInputCfg_t *plstInput;
	int outputNum;
	SimulinkOutputCfg_t *plstOutput;
	int paramNum;
	SimulinkParamCfg_t *plstParam;
}SimulinkModelCfg_t;

typedef struct{
	int id;
	double simuStep;
}SimulinkOtherModelCfg_t;

typedef struct{
	char szCofigFile[256];
	char szMonitorIp[32];
	SimulinkProjCfg_t proj;
	SimulinkModelCfg_t curModel;
	int nOtherModelNum;
	SimulinkOtherModelCfg_t otherModel[64];
}SimulinkConfig_t;

#endif
