#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

#include "SimulinkCfgParser.h"

#define SimulinkCfg_Space_Proj		 1
#define SimulinkCfg_Space_Model		 2
#define SimulinkCfg_Space_OtherModel 3
#define SimulinkCfg_Space_Input		 4		
#define SimulinkCfg_Space_Output	 5
#define SimulinkCfg_Space_Param      6

/////////////////////////////////////////////////
static int StrToList(char *szLine, const char *separateChar, char *ppList[]);
static int ParseCfgSpace_Proj(char *szLine, SimulinkConfig_t *pSimuCfg);
static int ParseCfgSpace_Model(char *szLine, SimulinkConfig_t *pSimuCfg);
static int ParseCfgSpace_OtherModel(char *szLine, SimulinkConfig_t *pSimuCfg);
static int ParseCfgSpace_Input(char *szLine, SimulinkConfig_t *pSimuCfg);
static int ParseCfgSpace_Output(char *szLine, SimulinkConfig_t *pSimuCfg);
static int ParseCfgSpace_Param(char *szLine, SimulinkConfig_t *pSimuCfg);

static char **gszVecLine = NULL;
//SimulinkConfig_t g_SimuCfg;

///////////////////////////////////////////////////
int StrToList(char *szLine, const char *separateChar, char *ppList[])
{
	char *token = NULL;
	int idx = 0;
	
	token = strtok(szLine, separateChar);
	while( token != NULL )
	{
		strcpy(ppList[idx], token);
		token = strtok( NULL , separateChar);
		idx++;
	}
	return idx;
}

int ParseCfgSpace_Proj(char *szLine, SimulinkConfig_t *pSimuCfg)
{
	int nVecNum = 0;
	int nRet = -1;
	int i = 0;

	nVecNum = StrToList(szLine, "=\n", gszVecLine);
	if( nVecNum  == 2){
		if(strcmp(gszVecLine[0], "SimuProj_ID") == 0){
			strcpy(pSimuCfg->proj.szProjId, gszVecLine[1]);
			nRet = 0;
		}else if(strcmp(gszVecLine[0], "SimuProj_Name") == 0){
			strcpy(pSimuCfg->proj.szProjName, gszVecLine[1]);
			nRet = 0;
		}else if(strcmp(gszVecLine[0], "SimuMode") == 0){
			pSimuCfg->proj.simuMode = atoi(gszVecLine[1]);
			nRet = 0;
		}else if(strcmp(gszVecLine[0], "MasterModelID") == 0){
			pSimuCfg->proj.masterId = atoi(gszVecLine[1]);
			nRet = 0;
		}
	}
	return nRet;
}

int ParseCfgSpace_Model(char *szLine, SimulinkConfig_t *pSimuCfg)
{
	int nVecNum = 0;
	int nRet = 0;

	nVecNum = StrToList(szLine, "=\n", gszVecLine);
	if( nVecNum == 2){
		if( strcmp(gszVecLine[0], "ModelID") == 0){
			pSimuCfg->curModel.id = atoi(gszVecLine[1]);
		}else if( strcmp(gszVecLine[0], "ModelName") == 0){
			strcpy(pSimuCfg->curModel.szName, gszVecLine[1]);
		}else if( strcmp(gszVecLine[0], "TargetCore") == 0){
			int nCoreNum = 0;
			char *szCore = NULL;
			szCore = strtok(gszVecLine[1], ", \t\r");
			while(szCore != NULL){
				pSimuCfg->curModel.szCore[nCoreNum] = atoi(szCore);
				nCoreNum++;
				szCore = strtok(NULL, ", \t\r");
			}
			pSimuCfg->curModel.nCoreNum = nCoreNum;
		}else if( strcmp(gszVecLine[0], "SimuStep") == 0){
			pSimuCfg->curModel.simuStep = atof(gszVecLine[1]);
		}else if( strcmp(gszVecLine[0], "RecordTime") == 0){
			pSimuCfg->curModel.recordTime = atof(gszVecLine[1]);
		}else if( strcmp(gszVecLine[0], "DispTime") == 0){
			pSimuCfg->curModel.dispTime = atof(gszVecLine[1]);
		}else{
			nRet = -1;
		}
	}else{
		nRet = -1;
	}
	return nRet;
}

int ParseCfgSpace_OtherModel(char *szLine, SimulinkConfig_t *pSimuCfg)
{
	int nVecNum = 0;
	int nRet = -1;

	nVecNum = StrToList(szLine, "|\t\r\n", gszVecLine);
	if( nVecNum == 2){
		pSimuCfg->otherModel[pSimuCfg->nOtherModelNum].id = atoi(gszVecLine[0]);
		pSimuCfg->otherModel[pSimuCfg->nOtherModelNum].simuStep = atof(gszVecLine[1]);
		pSimuCfg->nOtherModelNum++;
		nRet = 0;
	}
	return nRet;
}

int ParseCfgSpace_Input(char *szLine, SimulinkConfig_t *pSimuCfg)
{
	int nVecNum = 0;
	int nRet = -1;
	static int idx = 0;
	int i = 0;

	nVecNum = StrToList(szLine, "=|\n", gszVecLine);
	if( nVecNum == 2 && strcmp(gszVecLine[0], "Num") == 0){
		pSimuCfg->curModel.inputNum = atoi(gszVecLine[1]);
		if( pSimuCfg->curModel.inputNum > 0 && pSimuCfg->curModel.inputNum < 1024){
			pSimuCfg->curModel.plstInput = (SimulinkInputCfg_t *)calloc(pSimuCfg->curModel.inputNum, 
				sizeof(SimulinkInputCfg_t));
		}else{
			pSimuCfg->curModel.inputNum = 0;
		}
		for(i = 0; i < pSimuCfg->curModel.inputNum; i++){
			pSimuCfg->curModel.plstInput[i].id = 0;
		}
		nRet = 0;
	}else if( nVecNum == 7){
		if( idx < pSimuCfg->curModel.inputNum){
			pSimuCfg->curModel.plstInput[idx].no = atoi(gszVecLine[0]);
			pSimuCfg->curModel.plstInput[idx].id = atoi(gszVecLine[1]);
			strcpy(pSimuCfg->curModel.plstInput[idx].szName, gszVecLine[2]);
			pSimuCfg->curModel.plstInput[idx].width = atoi(gszVecLine[3]);
			pSimuCfg->curModel.plstInput[idx].modelId = atoi(gszVecLine[4]);
			pSimuCfg->curModel.plstInput[idx].outputId = atoi(gszVecLine[5]);
			pSimuCfg->curModel.plstInput[idx].key = atoi(gszVecLine[6]);
			idx++;
			nRet = 0;
		}
	}
	return nRet;
}

int ParseCfgSpace_Output(char *szLine, SimulinkConfig_t *pSimuCfg)
{
	int nVecNum = 0;
	int nRet = -1;
	static int idx = 0;
	int i = 0;

	nVecNum = StrToList(szLine, "=|\n", gszVecLine);
	if( nVecNum == 2 && strcmp(gszVecLine[0], "Num") == 0){
		pSimuCfg->curModel.outputNum = atoi(gszVecLine[1]);
		if( pSimuCfg->curModel.outputNum > 0 && pSimuCfg->curModel.outputNum < 1024){
			pSimuCfg->curModel.plstOutput = (SimulinkOutputCfg_t *)calloc(pSimuCfg->curModel.outputNum, 
				sizeof(SimulinkOutputCfg_t));
		}else{
			pSimuCfg->curModel.outputNum = 0;
		}
		for(i = 0; i < pSimuCfg->curModel.outputNum; i++){
			pSimuCfg->curModel.plstOutput[i].id = 0;
		}
		nRet = 0;
	}else if( nVecNum == 11){
		if( idx < pSimuCfg->curModel.outputNum){
			pSimuCfg->curModel.plstOutput[idx].no = atoi(gszVecLine[0]);
			pSimuCfg->curModel.plstOutput[idx].id = atoi(gszVecLine[1]);
			strcpy(pSimuCfg->curModel.plstOutput[idx].szName, gszVecLine[2]);
			pSimuCfg->curModel.plstOutput[idx].width = atoi(gszVecLine[3]);
			pSimuCfg->curModel.plstOutput[idx].startIdx = atoi(gszVecLine[4]);
			pSimuCfg->curModel.plstOutput[idx].bDisp = atoi(gszVecLine[5]);
			pSimuCfg->curModel.plstOutput[idx].bRecord = atoi(gszVecLine[6]);
			pSimuCfg->curModel.plstOutput[idx].bSync = atoi(gszVecLine[7]);
			pSimuCfg->curModel.plstOutput[idx].key = atoi(gszVecLine[8]);
			pSimuCfg->curModel.plstOutput[idx].modelId = atoi(gszVecLine[9]);
			pSimuCfg->curModel.plstOutput[idx].inputId = atoi(gszVecLine[10]);
			idx++;
			nRet = 0;
		}
	}
	return 0;
}

int ParseCfgSpace_Param(char *szLine, SimulinkConfig_t *pSimuCfg)
{
	int nVecNum = 0;
	int nRet = -1;
	static int idx = 0;
	int i = 0;

	nVecNum = StrToList(szLine, "=|\n", gszVecLine);
	if( nVecNum == 2 && strcmp(gszVecLine[0], "Num") == 0){
		pSimuCfg->curModel.paramNum = atoi(gszVecLine[1]);
		if( pSimuCfg->curModel.paramNum > 0 && pSimuCfg->curModel.paramNum < 2048){
			pSimuCfg->curModel.plstParam = (SimulinkParamCfg_t *)calloc(pSimuCfg->curModel.paramNum, 
				sizeof(SimulinkParamCfg_t));
		}else{
			pSimuCfg->curModel.paramNum = 0;
		}
		for(i = 0; i < pSimuCfg->curModel.paramNum; i++){
			pSimuCfg->curModel.plstParam[i].id = 0;
		}
		nRet = 0;
	}else if( nVecNum == 5){
		if( idx < pSimuCfg->curModel.paramNum){
			pSimuCfg->curModel.plstParam[idx].no = atoi(gszVecLine[0]);
			pSimuCfg->curModel.plstParam[idx].id = atoi(gszVecLine[1]);
			strcpy(pSimuCfg->curModel.plstParam[idx].szName, gszVecLine[2]);
			pSimuCfg->curModel.plstParam[idx].width = atoi(gszVecLine[3]);
			pSimuCfg->curModel.plstParam[idx].startIdx = atoi(gszVecLine[4]);
			idx++;
			nRet = 0;
		}
	}
	return nRet;
}

//////////////////////////////////////////////////
int SL_ParseSimulinkCfg(const char *pszCfgFile, SimulinkConfig_t *pSimuCfg)
{
	FILE *fp = NULL;
	char szLine[1024];
	int nParseSpace = 0;
	int nErrorCount = 0;
	int i = 0;

	fp = fopen(pszCfgFile, "r");
	if( fp == NULL)return -1;

	gszVecLine = (char **)calloc(128, sizeof(char *));
	for(i = 0;  i < 128; i++){
		gszVecLine[i] = (char *)calloc(1024, sizeof(char));
	}

	while( (fgets(szLine, 1024, fp)) != NULL){
		if( szLine[0] == '#' || strlen(szLine) <= 1)continue;
		if( strcmp(szLine, "[Project]\n") == 0){
			nParseSpace = SimulinkCfg_Space_Proj;
			continue;
		}else if( strcmp(szLine, "[Model]\n") == 0){
			nParseSpace = SimulinkCfg_Space_Model;
			continue;
		}else if( strcmp(szLine, "[OtherModel]\n") == 0){
			nParseSpace = SimulinkCfg_Space_OtherModel;
			pSimuCfg->nOtherModelNum = 0;
			continue;
		}else if( strcmp(szLine, "[Input]\n") == 0 ){
			nParseSpace = SimulinkCfg_Space_Input;
			pSimuCfg->curModel.inputNum = 0;
			continue;
		}else if( strcmp(szLine, "[Output]\n") == 0){
			nParseSpace = SimulinkCfg_Space_Output;
			pSimuCfg->curModel.outputNum = 0;
			continue;
		}else if( strcmp(szLine, "[Param]\n") == 0){
			nParseSpace = SimulinkCfg_Space_Param;
			pSimuCfg->curModel.paramNum = 0;
			continue;
		}

		if( nParseSpace == SimulinkCfg_Space_Proj){
			if( ParseCfgSpace_Proj(szLine, pSimuCfg) != 0)
				nErrorCount++;
		}else if( nParseSpace == SimulinkCfg_Space_Model){
			if( ParseCfgSpace_Model(szLine, pSimuCfg) != 0)
				nErrorCount++;
		}else if( nParseSpace == SimulinkCfg_Space_OtherModel){
			if( ParseCfgSpace_OtherModel(szLine, pSimuCfg) != 0)
				nErrorCount++;
		}else if( nParseSpace == SimulinkCfg_Space_Input){
			if( ParseCfgSpace_Input(szLine, pSimuCfg) != 0)
				nErrorCount++;
		}else if( nParseSpace == SimulinkCfg_Space_Output){
			if( ParseCfgSpace_Output(szLine, pSimuCfg) != 0)
				nErrorCount++;
		}else if( nParseSpace == SimulinkCfg_Space_Param){
			if( ParseCfgSpace_Param(szLine, pSimuCfg) != 0)
				nErrorCount++;
		}
	}

	for( i = 0; i < 128; i++){
		free(gszVecLine[i]);
	}
	free(gszVecLine);

	return nErrorCount;
}

int SL_PrintSimulinkCfg(SimulinkConfig_t *pSimuCfg)
{
	unsigned i = 0;
	char szPrint[8192];
	
	printf("##########################SimulinkConfig###########################\n");

	printf("----------------------proj---------------------------\n");
	printf(" id:%s, name:%s, simuMode:%d, masterId:%d \n", pSimuCfg->proj.szProjId, pSimuCfg->proj.szProjName,
		pSimuCfg->proj.simuMode, pSimuCfg->proj.masterId);

	printf("--------------------- curModel------------------------\n");
	printf("id:%d, name:%s, simuStep:%lf, record:%lf, disp:%lf,", pSimuCfg->curModel.id,
		pSimuCfg->curModel.szName, pSimuCfg->curModel.simuStep, pSimuCfg->curModel.recordTime, pSimuCfg->curModel.dispTime);
	printf("nCore:%d, [", pSimuCfg->curModel.nCoreNum);
	for(i = 0; i < pSimuCfg->curModel.nCoreNum; i++ ){
		if( i == 0) printf("%d", pSimuCfg->curModel.szCore[i]);
		else printf(",%d", pSimuCfg->curModel.szCore[i]);
	}
	printf("]\n");

	printf("------------------- otherModel num:%d --------------------\n", pSimuCfg->nOtherModelNum);
	for( i = 0; i < pSimuCfg->nOtherModelNum; i++){
		printf("%d-%lf\n",pSimuCfg->otherModel[i].id, pSimuCfg->otherModel[i].simuStep);
	}

	printf("---------------------Input => num:%d ---------------------\n", pSimuCfg->curModel.inputNum);
	for( i = 0; i < pSimuCfg->curModel.inputNum; i++){
		printf("%d:no(%d)-id(%d)-name(%s)-width(%d)-modelid(%d)-outputid(%d)-key(%d)\n",
			i,pSimuCfg->curModel.plstInput[i].no,
			pSimuCfg->curModel.plstInput[i].id,
			pSimuCfg->curModel.plstInput[i].szName,
			pSimuCfg->curModel.plstInput[i].width,
			pSimuCfg->curModel.plstInput[i].modelId,
			pSimuCfg->curModel.plstInput[i].outputId,
			pSimuCfg->curModel.plstInput[i].key);
	}
	printf("---------------------Output => num:%d ---------------------\n", pSimuCfg->curModel.outputNum);
	for( i = 0; i < pSimuCfg->curModel.outputNum; i++){
		printf("%d:no(%d)-id(%d)-startIdx(%d)-name(%s)-width(%d)-bDisp(%d)-bRecord(%d)-bSync(%d)-key(%d)-modelid(%d)-inputid(%d)\n",
			i, pSimuCfg->curModel.plstOutput[i].id,
			pSimuCfg->curModel.plstOutput[i].no,
			pSimuCfg->curModel.plstOutput[i].startIdx,
			pSimuCfg->curModel.plstOutput[i].szName,
			pSimuCfg->curModel.plstOutput[i].width,
			pSimuCfg->curModel.plstOutput[i].bDisp,
			pSimuCfg->curModel.plstOutput[i].bRecord,
			pSimuCfg->curModel.plstOutput[i].bSync,
			pSimuCfg->curModel.plstOutput[i].key,
			pSimuCfg->curModel.plstOutput[i].modelId,
			pSimuCfg->curModel.plstOutput[i].inputId);
	}
	printf("Param => num:%d \n", pSimuCfg->curModel.paramNum);
	for( i = 0; i < pSimuCfg->curModel.paramNum; i++){
		printf("%d:no(%d)-id(%d)-startIdx(%d)-name(%s)-width(%d)\n",i,
			pSimuCfg->curModel.plstParam[i].no,
			pSimuCfg->curModel.plstParam[i].id,
			pSimuCfg->curModel.plstParam[i].startIdx,
			pSimuCfg->curModel.plstParam[i].szName,
			pSimuCfg->curModel.plstParam[i].width);
	}
	return 0;
}
