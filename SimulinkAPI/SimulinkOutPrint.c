#include "SimulinkOutPrint.h"


static int g_Level = SL_Print_Level_Error;
static FILE* g_fp = NULL;

///////////////////////////////////
void SL_OutPrintSet(int key, int Level)
{
	char szFile[128];
	sprintf(szFile, "./SiimulinkModel_Log_Key%d_%d.txt", key, time(NULL));

	g_Level = Level;
	if( g_fp == NULL){
		g_fp = fopen(szFile, "w");
	}
}
void SL_OutPrint(int Level, const char *szPrint)
{
	char szFile[128];

	if( g_fp == NULL){
		sprintf(szFile, "./SiimulinkModel_Log_Key0_%d.txt", time(NULL));
		g_fp = fopen(szFile, "w");
	}

	if( g_fp != NULL){
		if( Level >= g_Level){
			fprintf(g_fp, "%s\n", szPrint);
			fflush(g_fp);
		}
	}
}

void SL_OutPrint2(int Level, const char *szFormat, ...)
{

}
