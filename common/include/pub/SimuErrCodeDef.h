/*****************************************************************
 * 文件名: SimuErrCodeDef.h
 * 功能: 错误码定义
 *****************************************************************/
#pragma  once


#define KL_RET_NO_ERROR 0

/*****************************************************************
 * 类型 : 控制命令返回命令错误码
 * 范围: 1 到  10000
 * 说明 : 命令错误代码已KL_CERR_CODE开头；
 * 	             命令错误信息已KL_CERR_INFO开头；
 *****************************************************************/

/////////////
#define KL_CERR_CODE_simuGuard_connect_timeout	 (1)
#define KL_CERR_CODE_simuMonitor_init_failed	 (2)
#define KL_CERR_CODE_load_simuconfig_file_failed (3)
#define KL_CERR_CODE_simuconfig_check_failed	 (4)
#define KL_CERR_CODE_simucfg_cmd_isnot_same_proj (5)
#define KL_CERR_CODE_storCommu_init_failed		 (6)
#define KL_CERR_CODE_dispCommu_init_failed		 (7)
#define KL_CERR_CODE_load_param_is_invaild		 (8)
#define KL_CERR_CODE_m_cCommu_init_failed		 (9)
#define KL_CERR_CODE_s_gCommu_init_failed		 (10)

#define KL_CERR_CODE_starup_simumonitor_failed	 (11)


////////////
#define KL_CERR_CODE_load_simu_model_failed		 (100)

/////////////
#define KL_CERR_CODE_exec_simu_model_failed      (200)

/////////////
#define KL_CERR_CODE_pause_simu_model_failed	 (300)

/////////////
#define KL_CERR_CODE_reset_simu_model_failed		(400)
#define KL_CERR_CODE_reset_simu_model_data_undealed (401)

/////////////
#define KL_CERR_CODE_set_model_param_failed		 (500)





/*****************************************************************
 * 类型 : 异常上报等级
 *****************************************************************/
#define KL_EREP_LEVEL_info	 1  /*异常信息*/
#define KL_EREP_LEVEL_warn	 2  /*异常警告*/
#define KL_EREP_LEVEL_error  3  /*异常错误*/


/*****************************************************************
 * 类型 : 异常上报码
 * 范围: 10000 以上
 * 说明 : 异常上报码已KL_EREP_CODE开头；
 * 	             异常上报码已KL_EREP_INFO开头；
 *****************************************************************/
#define KL_EREP_CODE_set_cpu_prio	10001  /*设置CPU亲和度失败*/
#define KL_EREP_INFO_set_cpu_prio	"set cpu prio failed"

