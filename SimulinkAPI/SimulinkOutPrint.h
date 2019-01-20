
#ifndef SIMUOUTPRINT_H_
#define SIMUOUTPRINT_H_

#include <stdio.h>

typedef enum{
	SL_Print_Level_Debug = 1,
	SL_Print_Level_Info = 2,
	SL_Print_Level_Notice = 3,
	SL_Print_Level_Warn = 4,
	SL_Print_Level_Error = 5,
}SimulinkPrintLevel_e;

void SL_OutPrintSet(int key, int Level);
void SL_OutPrint(int Level, const char *szPrint);
void SL_OutPrint2(int Level, const char *szFormat, ...);

#endif
