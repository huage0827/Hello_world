/* $Revision: 1.68.4.12 $
 * Copyright 1994-2005 The MathWorks, Inc.
 *
 * File    : simulab_qnx_main.c
 *
 * Abstract:
 *      A Generic "Real-Time (single tasking or pseudo-multitasking,
 *      statically allocated data)" main that runs under most
 *      operating systems.
 *
 *      This file may be a useful starting point when targeting a new
 *      processor or microcontroller.
 *
 *
 * Compiler specified defines:
 *	RT              - Required.
 *      MODEL=modelname - Required.
 *	NUMST=#         - Required. Number of sample times.
 *	NCSTATES=#      - Required. Number of continuous states.
 *      TID01EQ=1 or 0  - Optional. Only define to 1 if sample time task
 *                        id's 0 and 1 have equal rates.
 *      MULTITASKING    - Optional. (use MT for a synonym).
 *	SAVEFILE        - Optional (non-quoted) name of .mat file to create.
 *			  Default is <MODEL>.mat
 *      BORLAND         - Required if using Borland C/C++
 */

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rtwtypes.h"
# include "rtmodel.h"
#include "rt_sim.h"
#include "rt_logging.h"
#ifdef UseMMIDataLogging
#include "rt_logging_mmi.h"
#endif
#include "rt_nonfinite.h"

/* Signal Handler header */
#ifdef BORLAND
#include <signal.h>
#include <float.h>
#endif

#include "ext_work.h"

///////////////////////  SL_Add ///////////////////////////
#include "SimulinkAPI.h"
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <sys/siginfo.h>
#include <sys/syspage.h>
#include <stdint.h>
#include <errno.h>

SimulinkConfig_t g_SimuCfg;
SimulinkModel_t g_SimuModel;

//////////////////////  End SL_Add /////////////////////////


/*=========*
 * Defines *
 *=========*/

#ifndef TRUE
#define FALSE (0)
#define TRUE  (1)
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE  1
#endif
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS  0
#endif

#define QUOTE1(name) #name
#define QUOTE(name) QUOTE1(name)    /* need to expand name    */

#ifndef RT
# error "must define RT"
#endif

#ifndef MODEL
# error "must define MODEL"
#endif

#ifndef NUMST
# error "must define number of sample times, NUMST"
#endif

#ifndef NCSTATES
# error "must define NCSTATES"
#endif

#ifndef SAVEFILE
# define MATFILE2(file) #file ".mat"
# define MATFILE1(file) MATFILE2(file)
# define MATFILE MATFILE1(MODEL)
#else
# define MATFILE QUOTE(SAVEFILE)
#endif

#define RUN_FOREVER -1.0

#define EXPAND_CONCAT(name1,name2) name1 ## name2
#define CONCAT(name1,name2) EXPAND_CONCAT(name1,name2)
#define RT_MODEL            CONCAT(MODEL,_rtModel)

/*====================*
 * External functions *
 *====================*/
#ifdef __cplusplus

extern "C" {

#endif

extern RT_MODEL *MODEL(void);

extern void MdlInitializeSizes(void);
extern void MdlInitializeSampleTimes(void);
extern void MdlStart(void);
extern void MdlOutputs(int_T tid);
extern void MdlUpdate(int_T tid);
extern void MdlTerminate(void);

#ifdef __cplusplus

}
#endif

#if NCSTATES > 0
#ifdef __cplusplus

extern "C" {

#endif
  extern void rt_ODECreateIntegrationData(RTWSolverInfo *si);
  extern void rt_ODEUpdateContinuousStates(RTWSolverInfo *si);
#ifdef __cplusplus

}
#endif

# define rt_CreateIntegrationData(S) \
    rt_ODECreateIntegrationData(rtmGetRTWSolverInfo(S));
# define rt_UpdateContinuousStates(S) \
    rt_ODEUpdateContinuousStates(rtmGetRTWSolverInfo(S));
# else
# define rt_CreateIntegrationData(S)  \
      rtsiSetSolverName(rtmGetRTWSolverInfo(S),"FixedStepDiscrete");
# define rt_UpdateContinuousStates(S) /* Do Nothing */
#endif


/*==================================*
 * Global data local to this module *
 *==================================*/

static struct {
  int_T    stopExecutionFlag;
  int_T    isrOverrun;
  int_T    overrunFlags[NUMST];
  int_T    eventFlags[NUMST];
  const    char_T *errmsg;
} GBLbuf;


#ifdef EXT_MODE
#  define rtExtModeSingleTaskUpload(S)                          \
   {                                                            \
        int stIdx;                                              \
        rtExtModeUploadCheckTrigger(rtmGetNumSampleTimes(S));   \
        for (stIdx=0; stIdx<NUMST; stIdx++) {                   \
            if (rtmIsSampleHit(S, stIdx, 0 /*unused*/)) {       \
                rtExtModeUpload(stIdx,rtmGetTaskTime(S,stIdx)); \
            }                                                   \
        }                                                       \
   }
#else
#  define rtExtModeSingleTaskUpload(S) /* Do nothing */
#endif

/*=================*
 * Local functions *
 *=================*/

#ifdef BORLAND
/* Implemented for BC++ only*/

typedef void (*fptr)(int, int);

/* Function: divideByZero =====================================================
 *
 * Abstract: Traps the error Division by zero and prints a warning
 *           Also catches other FP errors, but does not identify them
 *           specifically.
 */
void divideByZero(int sigName, int sigType)
{
    signal(SIGFPE, (fptr)divideByZero);
    if ((sigType == FPE_ZERODIVIDE)||(sigType == FPE_INTDIV0)){
        printf("*** Warning: Division by zero\n\n");
        return;
    }
    else{
        printf("*** Warning: Floating Point error\n\n");
        return;
    }
} /* end divideByZero */

#endif /* BORLAND */

#if !defined(MULTITASKING)  /* SINGLETASKING */

/* Function: rtOneStep ========================================================
 *
 * Abstract:
 *      Perform one step of the model. This function is modeled such that
 *      it could be called from an interrupt service routine (ISR) with minor
 *      modifications.
 */
static void rt_OneStep(RT_MODEL *S)
{

	real_T tnext;

    /***********************************************
     * Check and see if base step time is too fast *
     ***********************************************/
    if (GBLbuf.isrOverrun++) {
        GBLbuf.stopExecutionFlag = 1;
        return;
    }

    /***********************************************
     * Check and see if error status has been set  *
     ***********************************************/
    if (rtmGetErrorStatus(S) != NULL) {
        GBLbuf.stopExecutionFlag = 1;
        return;
    }

    /* enable interrupts here */

    /*
     * In a multi-tasking environment, this would be removed from the base rate
     * and called as a "background" task.
     */
    rtExtModeOneStep(rtmGetRTWExtModeInfo(S),
                     rtmGetNumSampleTimes(S),
                     (boolean_T *)&rtmGetStopRequested(S));

    tnext = rt_SimGetNextSampleHit();
    rtsiSetSolverStopTime(rtmGetRTWSolverInfo(S),tnext);

    MdlOutputs(0);

    rtExtModeSingleTaskUpload(S);

    GBLbuf.errmsg = rt_UpdateTXYLogVars(rtmGetRTWLogInfo(S),
                                        rtmGetTPtr(S));
    if (GBLbuf.errmsg != NULL) {
        GBLbuf.stopExecutionFlag = 1;
        return;
    }

    rt_UpdateSigLogVars(rtmGetRTWLogInfo(S), rtmGetTPtr(S));

    MdlUpdate(0);
    rt_SimUpdateDiscreteTaskSampleHits(rtmGetNumSampleTimes(S),
                                       rtmGetTimingData(S),
                                       rtmGetSampleHitPtr(S),
                                       rtmGetTPtr(S));

    if (rtmGetSampleTime(S,0) == CONTINUOUS_SAMPLE_TIME) {
        rt_UpdateContinuousStates(S);
    }

    GBLbuf.isrOverrun--;

    rtExtModeCheckEndTrigger();

} /* end rtOneStep */

#else /* MULTITASKING */

# if TID01EQ == 1
#  define FIRST_TID 1
# else
#  define FIRST_TID 0
# endif

/* Function: rtOneStep ========================================================
 *
 * Abstract:
 *      Perform one step of the model. This function is modeled such that
 *      it could be called from an interrupt service routine (ISR) with minor
 *      modifications.
 *
 *      This routine is modeled for use in a multitasking environment and
 *	therefore needs to be fully re-entrant when it is called from an
 *	interrupt service routine.
 *
 * Note:
 *      Error checking is provided which will only be used if this routine
 *      is attached to an interrupt.
 *
 */
static void rt_OneStep(RT_MODEL *S)
{
    int_T  i;
    real_T tnext;
    int_T  *sampleHit = rtmGetSampleHitPtr(S);

    /***********************************************
     * Check and see if base step time is too fast *
     ***********************************************/
    if (GBLbuf.isrOverrun++) {
        GBLbuf.stopExecutionFlag = 1;
        return;
    }

    /***********************************************
     * Check and see if error status has been set  *
     ***********************************************/
    if (rtmGetErrorStatus(S) != NULL) {
        GBLbuf.stopExecutionFlag = 1;
        return;
    }
    /* enable interrupts here */

    /*
     * In a multi-tasking environment, this would be removed from the base rate
     * and called as a "background" task.
     */
    rtExtModeOneStep(rtmGetRTWExtModeInfo(S),
                     rtmGetNumSampleTimes(S),
                     (boolean_T *)&rtmGetStopRequested(S));

    /***********************************************
     * Update discrete events                      *
     ***********************************************/
    tnext = rt_SimUpdateDiscreteEvents(rtmGetNumSampleTimes(S),
                                       rtmGetTimingData(S),
                                       rtmGetSampleHitPtr(S),
                                       rtmGetPerTaskSampleHitsPtr(S));
    rtsiSetSolverStopTime(rtmGetRTWSolverInfo(S),tnext);
    for (i=FIRST_TID+1; i < NUMST; i++) {
        if (sampleHit[i] && GBLbuf.eventFlags[i]++) {
            GBLbuf.isrOverrun--; 
            GBLbuf.overrunFlags[i]++;    /* Are we sampling too fast for */
            GBLbuf.stopExecutionFlag=1;  /*   sample time "i"?           */
            return;
        }
    }
    /*******************************************
     * Step the model for the base sample time *
     *******************************************/
    MdlOutputs(FIRST_TID);

    rtExtModeUploadCheckTrigger(rtmGetNumSampleTimes(S));
    rtExtModeUpload(FIRST_TID,rtmGetTaskTime(S, FIRST_TID));

    GBLbuf.errmsg = rt_UpdateTXYLogVars(rtmGetRTWLogInfo(S),
                                        rtmGetTPtr(S));
    if (GBLbuf.errmsg != NULL) {
        GBLbuf.stopExecutionFlag = 1;
        return;
    }

    rt_UpdateSigLogVars(rtmGetRTWLogInfo(S), rtmGetTPtr(S));

    MdlUpdate(FIRST_TID);

    if (rtmGetSampleTime(S,0) == CONTINUOUS_SAMPLE_TIME) {
        rt_UpdateContinuousStates(S);
    }
     else {
        rt_SimUpdateDiscreteTaskTime(rtmGetTPtr(S), 
                                     rtmGetTimingData(S), 0);
    }

#if FIRST_TID == 1
    rt_SimUpdateDiscreteTaskTime(rtmGetTPtr(S), 
                                 rtmGetTimingData(S),1);
#endif


    /************************************************************************
     * Model step complete for base sample time, now it is okay to          *
     * re-interrupt this ISR.                                               *
     ************************************************************************/

    GBLbuf.isrOverrun--;


    /*********************************************
     * Step the model for any other sample times *
     *********************************************/
    for (i=FIRST_TID+1; i<NUMST; i++) {
        /* If task "i" is running, don't run any lower priority task */
        if (GBLbuf.overrunFlags[i]) return;

        if (GBLbuf.eventFlags[i]) {
            GBLbuf.overrunFlags[i]++;

            MdlOutputs(i);
 
            rtExtModeUpload(i, rtmGetTaskTime(S,i));

            MdlUpdate(i);

            rt_SimUpdateDiscreteTaskTime(rtmGetTPtr(S), 
                                         rtmGetTimingData(S),i);

            /* Indicate task complete for sample time "i" */
            GBLbuf.overrunFlags[i]--;
            GBLbuf.eventFlags[i]--;
        }
    }

    rtExtModeCheckEndTrigger();

} /* end rtOneStep */

#endif /* MULTITASKING */


static void displayUsage (void)
{
    (void) printf("usage: %s -tf <finaltime> -w -port <TCPport>\n",QUOTE(MODEL));
    (void) printf("arguments:\n");
    (void) printf("  -tf <finaltime> - overrides final time specified in "
                  "Simulink (inf for no limit).\n");
    (void) printf("  -w              - waits for Simulink to start model "
                  "in External Mode.\n");
    (void) printf("  -port <TCPport> - overrides 17725 default port in "
                  "External Mode, valid range 256 to 65535.\n");
}

/*===================*
 * Visible functions *
 *===================*/


/* Function: main =============================================================
 *
 * Abstract:
 *      Execute model on a generic target such as a workstation.
 */
int_T main(int_T argc, const char_T *argv[])
{
    RT_MODEL  *S;
    const char *status;
    real_T     finaltime = -2.0;

    int_T  oldStyle_argc;
    const char_T *oldStyle_argv[5];

	int nRet = -1;


   /******************************
     * MathError Handling for BC++ *
     ******************************/
#ifdef BORLAND
    signal(SIGFPE, (fptr)divideByZero);
#endif

    /*******************
     * Parse arguments *
     *******************/

/////////////////////// SL_Add ////////////////////////////
#if 0
////////////////////// End SL_Add ////////////////////////

    if ((argc > 1) && (argv[1][0] != '-')) {
        /* old style */
        if ( argc > 3 ) {
            displayUsage();
            exit(EXIT_FAILURE);
        }

        oldStyle_argc    = 1;
        oldStyle_argv[0] = argv[0];
    
        if (argc >= 2) {
            oldStyle_argc = 3;

            oldStyle_argv[1] = "-tf";
            oldStyle_argv[2] = argv[1];
        }

        if (argc == 3) {
            oldStyle_argc = 5;

            oldStyle_argv[3] = "-port";
            oldStyle_argv[4] = argv[2];

        }

        argc = oldStyle_argc;
        argv = oldStyle_argv;

    }

    {
        /* new style: */
        double    tmpDouble;
        char_T tmpStr2[200];
        int_T  count      = 1;
        int_T  parseError = FALSE;

        /*
         * Parse the standard RTW parameters.  Let all unrecognized parameters
         * pass through to external mode for parsing.  NULL out all args handled
         * so that the external mode parsing can ignore them.
         */
        while(count < argc) {
            const char_T *option = argv[count++];
            
            /* final time */
            if ((strcmp(option, "-tf") == 0) && (count != argc)) {
                const char_T *tfStr = argv[count++];
                
                sscanf(tfStr, "%200s", tmpStr2);
                if (strcmp(tmpStr2, "inf") == 0) {
                    tmpDouble = RUN_FOREVER;
                } else {
                    char_T tmpstr[2];

                    if ( (sscanf(tmpStr2,"%lf%1s", &tmpDouble, tmpstr) != 1) ||
                         (tmpDouble < 0.0) ) {
                        (void)printf("finaltime must be a positive, real value or inf\n");
                        parseError = TRUE;
                        break;
                    }
                }
                finaltime = (real_T) tmpDouble;

                argv[count-2] = NULL;
                argv[count-1] = NULL;
            }
        }

        if (parseError) {
            (void)printf("\nUsage: %s -option1 val1 -option2 val2 -option3 "
                         "...\n\n", QUOTE(MODEL));
            (void)printf("\t-tf 20 - sets final time to 20 seconds\n");

            exit(EXIT_FAILURE);
        }

        rtExtModeParseArgs(argc, argv, NULL);

        /*
         * Check for unprocessed ("unhandled") args.
         */
        {
            int i;
            for (i=1; i<argc; i++) {
                if (argv[i] != NULL) {
                    printf("Unexpected command line argument: %s\n",argv[i]);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

/////////////////////// SL_Add ////////////////////////////
#endif 
////////////////////// End SL_Add //////////////////////////

/////////////////////// SL_Add ////////////////////////////

	{
		unsigned long ulDispTime;
		unsigned long ulRecordTime;
		int bPause = 0;
		
		/* argv-Param 
		    argv[0] = ProgName;
		    argv[1] = ModelId;
		    argv[2] = ConfigFile;
		    argv[3] = MonitorIp;
		    argv[4] = IsPause;
		    //argv[5] = ThreadAffinity;
		*/
		if( argc != 5){
			return -1;
		}
		g_SimuModel.nID = atoi(argv[1]);
		strcpy(g_SimuCfg.szCofigFile, argv[2]);
		strcpy(g_SimuCfg.szMonitorIp, argv[3]);
		bPause = atoi(argv[4]);

		/*Init SimuM_CComm*/
		if( SL_InitM_CComm() != 0){
			return -1;
		}

		/*Send Register Cmd*/
		SL_SendRegisterModelCmd();
		
		/*Parse Config File*/
		if( SL_ParseSimulinkCfg(g_SimuCfg.szCofigFile, &g_SimuCfg) != 0){
			SL_SendExceptReportCmd(0, 0,"parse config file failed!");
			return -1;
		}
		SL_PrintSimulinkCfg(&g_SimuCfg);

		/*Copy Config*/
		if( g_SimuCfg.curModel.id != g_SimuModel.nID){
			SL_SendExceptReportCmd(0, 0,"g_SimuCfg.curModel.id != g_SimuModel.Id");
			return -1;
		}
		g_SimuModel.nStepTime = g_SimuCfg.curModel.simuStep*1000*1000*1000;
		ulDispTime = g_SimuCfg.curModel.dispTime*1000*1000*1000;
		ulRecordTime = g_SimuCfg.curModel.recordTime*1000*1000*1000;

		g_SimuModel.nDispRate = ulDispTime/g_SimuModel.nStepTime;
		g_SimuModel.nRecordRate = ulRecordTime/g_SimuModel.nStepTime;
		if( g_SimuModel.nDispRate <= 0)
			g_SimuModel.nDispRate = 1;
		if( g_SimuModel.nRecordRate <= 0)
			g_SimuModel.nRecordRate = 1;
		g_SimuModel.bPause = bPause;
		g_SimuModel.bQuit = 0;
		g_SimuModel.nCurStepCount = 0;
		g_SimuModel.nOverRunCount = 0;
		g_SimuModel.nExecMode = g_SimuCfg.proj.simuMode;
	}
////////////////////// End SL_Add //////////////////////////


    /****************************
     * Initialize global memory *
     ****************************/
    (void)memset(&GBLbuf, 0, sizeof(GBLbuf));

    /************************
     * Initialize the model *
     ************************/
    rt_InitInfAndNaN(sizeof(real_T));

    S = MODEL();
    if (rtmGetErrorStatus(S) != NULL) {
        (void)fprintf(stderr,"Error during model registration: %s\n",
                      rtmGetErrorStatus(S));
        exit(EXIT_FAILURE);
    }
    if (finaltime >= 0.0 || finaltime == RUN_FOREVER) rtmSetTFinal(S,finaltime);

    MdlInitializeSizes();
    MdlInitializeSampleTimes();
    
    status = rt_SimInitTimingEngine(rtmGetNumSampleTimes(S),
                                    rtmGetStepSize(S),
                                    rtmGetSampleTimePtr(S),
                                    rtmGetOffsetTimePtr(S),
                                    rtmGetSampleHitPtr(S),
                                    rtmGetSampleTimeTaskIDPtr(S),
                                    rtmGetTStart(S),
                                    &rtmGetSimTimeStep(S),
                                    &rtmGetTimingData(S));

    if (status != NULL) {
        (void)fprintf(stderr,
                "Failed to initialize sample time engine: %s\n", status);
        exit(EXIT_FAILURE);
    }
    rt_CreateIntegrationData(S);

#ifdef UseMMIDataLogging
    rt_FillStateSigInfoFromMMI(rtmGetRTWLogInfo(S),&rtmGetErrorStatus(S));
    rt_FillSigLogInfoFromMMI(rtmGetRTWLogInfo(S),&rtmGetErrorStatus(S));
#endif
    GBLbuf.errmsg = rt_StartDataLogging(rtmGetRTWLogInfo(S),
                                        rtmGetTFinal(S),
                                        rtmGetStepSize(S),
                                        &rtmGetErrorStatus(S));
    if (GBLbuf.errmsg != NULL) {
        (void)fprintf(stderr,"Error starting data logging: %s\n",GBLbuf.errmsg);
        return(EXIT_FAILURE);
    }

    rtExtModeCheckInit(rtmGetNumSampleTimes(S));
    rtExtModeWaitForStartPkt(rtmGetRTWExtModeInfo(S),
                             rtmGetNumSampleTimes(S),
                             (boolean_T *)&rtmGetStopRequested(S));

    (void)printf("\n** starting the model **\n");

    MdlStart();
    if (rtmGetErrorStatus(S) != NULL) {
      GBLbuf.stopExecutionFlag = 1;
    }

    /*************************************************************************
     * Execute the model.  You may attach rtOneStep to an ISR, if so replace *
     * the call to rtOneStep (below) with a call to a background task        *
     * application.                                                          *
     *************************************************************************/
    if (rtmGetTFinal(S) == RUN_FOREVER) {
        printf ("\n**May run forever. Model stop time set to infinity.**\n");
    }
    
#if 0
    while (!GBLbuf.stopExecutionFlag &&
           (rtmGetTFinal(S) == RUN_FOREVER ||
            rtmGetTFinal(S)-rtmGetT(S) > rtmGetT(S)*DBL_EPSILON)) {

        rtExtModePauseIfNeeded(rtmGetRTWExtModeInfo(S),
                               rtmGetNumSampleTimes(S),
                               (boolean_T *)&rtmGetStopRequested(S));

        if (rtmGetStopRequested(S)) break;
        rt_OneStep(S);
		
		double *ptr = rtmGetBlockIO(S);
		static int sIdx = 0;
		sIdx++;
		printf("sIdx = %d, %lf\n", sIdx, *ptr);
		//sleep(1);
    }

    if (!GBLbuf.stopExecutionFlag && !rtmGetStopRequested(S)) {
        /* Execute model last time step */
        rt_OneStep(S);
    }
#endif


/////////////////////// SL_Add ////////////////////////////
	{
		SimuClockParam_t clParam;
		int idx = 0;
		int nSize = 0;
		double *pValue = NULL;

		/* Init Clock  */
		clParam.nId = g_SimuCfg.curModel.id;
		strcpy(clParam.szName,g_SimuCfg.curModel.szName);
		//clParam.dStepSize = rtmGetStepSize(S);
		//clParam.nStepSize = rtmGetStepSize(S)*1000000000;
		clParam.dStepSize = g_SimuCfg.curModel.simuStep;
		clParam.nStepSize = g_SimuCfg.curModel.simuStep*1000*1000*1000;
		if( g_SimuCfg.proj.masterId == g_SimuCfg.curModel.id){
			clParam.bMaster = 1;
			clParam.nModelCount = g_SimuCfg.nOtherModelNum;
			for( idx = 0; idx < g_SimuCfg.nOtherModelNum; idx++){
				clParam.modelList[idx].nId = g_SimuCfg.otherModel[idx].id;
				clParam.modelList[idx].nStepSize = g_SimuCfg.otherModel[idx].simuStep*1000*1000*1000;
			}
			clParam.bUseCallback = 0;
		}else{
			clParam.bMaster = 0;
			clParam.nModelCount = 0;
			clParam.bUseCallback = 0;	
		}
		nRet = SL_ClockInit(&clParam);
		if( nRet != 0){
			SL_SendExceptReportCmd(0,0,"SL_ClockInit init failed.");
			return -1;
		}

		/*Init Disp & Record & Param*/
		nSize = rtmGetNumBlockParams(S);
		pValue = rtmGetDefaultParam(S);
		if( SL_SetModelParam(nSize, pValue) != 0){
			SL_SendExceptReportCmd(0,0,"SL_SetModelParam set failed.");
			return -1;
		}
		nSize = rtmGetNumBlockIO(S);
		pValue = rtmGetBlockIO(S);
		if( SL_SetModelOutput(nSize, pValue) != 0){
			SL_SendExceptReportCmd(0,0,"SL_SetModelOutput set failed.");
			return -1;
		}

		/*init data sync */
		if( SL_InitDataSync(&g_SimuCfg) != 0){
			SL_SendExceptReportCmd(0,0,"SL_InitDataSync init failed.");
			return -1;
		}

		/*set priority*/
		if( SL_SetPriority(32) != 0){
			SL_SendExceptReportCmd(0,0,"set priority failed.");
			return -1;
		}

		/*set thread affinity*/
		if( SL_SetThreadAffinity((unsigned char *)g_SimuCfg.curModel.szCore, g_SimuCfg.curModel.nCoreNum) != 0){
			SL_SendExceptReportCmd(0,0,"set thread affinity failed.");
			return -1;
		}
	}	


	double	userExecTime = 0.0f;
	uint64_t nCyclesPerMircoSec;
	uint64_t userStartCycle, userEndCycle;
	nCyclesPerMircoSec = SYSPAGE_ENTRY(qtime)->cycles_per_sec / (1000 * 1000);

	/* model run loop */
	while(1){
		/*clock wait*/
		nRet = SL_ClockWait(1000);

		
		/*Recv control cmd*/
		SL_RecvControlCmd();
		
		if( nRet <= 0){	 
			continue;
		}

		/*is Quit?*/
		if( g_SimuModel.bQuit == 1){
			break;
		}

		/*is Pause?*/
		if( g_SimuModel.bPause == 1){
			usleep(1);
			SL_ClockClean();
			continue;
		}

		/*step Count*/
		if( nRet - 1 > 0){
			g_SimuModel.nOverRunCount += nRet - 1;
		}
		g_SimuModel.nCurStepCount++;

		/* exec one step.*/
		userStartCycle = ClockCycles();
		rt_OneStep(S);
		userEndCycle = ClockCycles();

		userExecTime = ((double)(userEndCycle - userStartCycle)/nCyclesPerMircoSec);
		/* send disp data*/
		if( g_SimuModel.nCurStepCount == 1 || g_SimuModel.nCurStepCount % g_SimuModel.nDispRate == 0){
			SL_SendDispDataCmd(g_SimuModel.nCurStepCount, g_SimuModel.nOverRunCount, userExecTime, gPerStepExecTime);
		}

		/* send record data*/
		if( g_SimuModel.nCurStepCount == 1 || g_SimuModel.nCurStepCount % g_SimuModel.nRecordRate == 0){
			SL_SendRecordDataCmd(g_SimuModel.nCurStepCount);
		}
	}

	/*Uninit */
	SL_ClockUninit();
	SL_UninitM_CComm();
	SL_UninitDataSync();
	
////////////////////// End SL_Add //////////////////////////

    /********************
     * Cleanup and exit *
     ********************/
#ifdef UseMMIDataLogging
    rt_CleanUpForStateLogWithMMI(rtmGetRTWLogInfo(S));
    rt_CleanUpForSigLogWithMMI(rtmGetRTWLogInfo(S));
#endif
    rt_StopDataLogging(MATFILE,rtmGetRTWLogInfo(S));

    rtExtModeShutdown(rtmGetNumSampleTimes(S));

    if (GBLbuf.errmsg) {
        (void)fprintf(stderr,"%s\n",GBLbuf.errmsg);
        exit(EXIT_FAILURE);
    }

    if (rtmGetErrorStatus(S) != NULL) {
        (void)fprintf(stderr,"ErrorStatus set: \"%s\"\n", rtmGetErrorStatus(S));
        exit(EXIT_FAILURE);
    }

    if (GBLbuf.isrOverrun) {
        (void)fprintf(stderr,
                      "%s: ISR overrun - base sampling rate is too fast\n",
                      QUOTE(MODEL));
        exit(EXIT_FAILURE);
    }

#ifdef MULTITASKING
    else {
        int_T i;
        for (i=1; i<NUMST; i++) {
            if (GBLbuf.overrunFlags[i]) {
                (void)fprintf(stderr,
                        "%s ISR overrun - sampling rate is too fast for "
                        "sample time index %d\n", QUOTE(MODEL), i);
                exit(EXIT_FAILURE);
            }
        }
    }
#endif

    MdlTerminate();
    return(EXIT_SUCCESS);

} /* end main */



/* EOF: simulab_qnx_main.c */
