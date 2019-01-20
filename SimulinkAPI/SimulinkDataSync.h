#pragma  once
#include "SimulinkCfgDefine.h"

int SL_InitDataSync(const SimulinkConfig_t *pSimuCfg);
int SL_UninitDataSync();

int SL_RecvSyncData(int nKey,const double *pInitValue, double *pDataAddr, int nDouNum);
int SL_SendSyncData(int nKey,const double *pDataAddr, int nDouNum);

