/*****************************************************************
 * 文件名: SimuD_SCmd.h
 * 功能: SimuMonitor和SimuService之间通信命令定义
 *****************************************************************/
#pragma  once

#include <vector>
#include "SimuCommunicationDef.h"

using namespace std;

/****************************************************************
 * 枚举名 : D_S_CMD
 * 说明 : SimuMonitor 和 SimuService之间通信命令字定义
 ****************************************************************/
typedef enum euD_S_CMD
{
	D_S_CMD_UNKNOWN							= -1,	
	D_S_CMD_INITMONITOR						= 0x90000001,
	D_S_CMD_INITMONITOR_RESP				= 0x10000001,
	D_S_CMD_LOADSIMUMODEL_REPORT			= 0x90000002,
	D_S_CMD_EXECSIMUMODEL					= 0x10000003,
	D_S_CMD_EXECSIMUMODEL_RESP				= 0x90000003,
	D_S_CMD_PAUSESIMUMODEL					= 0x10000004,
	D_S_CMD_PAUSESIMUMODEL_RESP				= 0x90000004,
	D_S_CMD_RESETSIMUMODEL					= 0x10000005,
	D_S_CMD_RESETSIMUMODEL_RESP				= 0x90000005,
	D_S_CMD_SETSIMUMODELPARAM				= 0x10000006,
	D_S_CMD_SETSIMUMODELPARAM_RESP			= 0x90000006,
	D_S_CMD_TESTEVENT_REPORT				= 0x90000007,
	D_S_CMD_EXCEPTEVENT_REPORT				= 0x90000008,
	D_S_CMD_ENABLESIMUMONITOR				= 0x10000009,
	D_S_CMD_ENABLESIMUMONITOR_RESP			= 0x90000009,
} D_S_CMD;

/*BEGIN  SimuMonitor和SimuService之间通信命令帧各字段在帧中所处的位置以及大小定义 */
#define D_S_CMD_POS_VERSION					0
#define D_S_CMD_POS_FRAMELEN				4
#define D_S_CMD_POS_COMMAND					8
#define D_S_CMD_POS_FRAMEBODYLEN			12
#define D_S_CMD_POS_SIMUPROJECTID			16
#define D_S_CMD_POS_SIMUMODEL				52
#define D_S_CMD_POS_RESERVED1				56
#define D_S_CMD_POS_FRAMEBODY				80
#define D_S_CMD_HEADER_SIZE					80
#define D_S_CMD_BODY_SIZE(bodylen)		(bodylen)
#define D_S_CMD_SIZE(bodylen)				((D_S_CMD_HEADER_SIZE) + (D_S_CMD_BODY_SIZE(bodylen)))
#define D_S_CMD_SIZE_VERSION				4
#define D_S_CMD_SIZE_FRAMELEN				4
#define D_S_CMD_SIZE_COMMAND				4
#define D_S_CMD_SIZE_FRAMEBODYLEN			4
#define D_S_CMD_SIZE_SUBMODELID				4
#define D_S_CMD_SIZE_RESERVED1				24
#define D_S_CMD_SIZE_FRAMEBODY(bodylen)		(bodylen)
#define D_S_CMD_SIZE_SIMUPROJECTID			36
/*END  SimuMonitor和SimuService之间通信命令帧各字段在帧中所处的位置以及大小 */

/****************************************************************
 * 类名 : CSimuD_SCmd
 * 父类 : 无
 * 说明 : SimuMonitor和SimuService之间通信的所有命令的父类
 ****************************************************************/
class CSimuD_SCmd
{
public:
	CSimuD_SCmd(D_S_CMD cmd);
	virtual ~CSimuD_SCmd();

	/* 预解析数据 , 获得响应的命令 */
	static D_S_CMD PreParseCmd(unsigned char* szBuffer , unsigned long nBufferLen , unsigned long& nCmdDataLen);	
	
	/* 获取命令值 */
	D_S_CMD GetCmdType() const;	

	/* 设置工程ID */
	void SetSimuProjectID(const unsigned char* szId);

	/* 获取工程ID */
	void GetSimuProjectID(char* szProjectID);

	/* 设置模型ID */
	void SetSimuModelID(const unsigned char *szModelID);

	/* 获取模型ID */
	void GetSimuModelID(unsigned char *szModelID);

	/* 序列化命令 */
	virtual unsigned long Serialize(unsigned char* szBuffer);
	
	/* 反序列化传入的数据 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);

	/* 打印输出 */
	virtual void Print(char* szBuffer);
	
protected:
	unsigned long CalcFrameLen();
	virtual unsigned long CalcFrameBodyLen() = 0;

	D_S_CMD			m_dsc;										/* 命令字 */
	unsigned char	m_szVersion[D_S_CMD_SIZE_VERSION];		    /* 命令版本号 */
	unsigned long	m_nFrameLen;								/* 命令帧长度*/
	unsigned long	m_nCmd;										/* 命令字 */
	unsigned long	m_nFrameBodyLen;							/* 命令帧体长度 */
	unsigned char	m_szSimuProjectID[D_S_CMD_SIZE_SIMUPROJECTID]; /*工程ID */
	unsigned char	m_szSimuModelID[D_S_CMD_SIZE_SUBMODELID];   /*模型ID*/
	unsigned char	m_szReserved1[D_S_CMD_SIZE_RESERVED1];	    /*命令保留字段 */
};

/****************************************************************
 * 类名 : CSimuD_SInitMonitorCmd
 * 父类 : CSimuD_SCmd
 * 说明 : 初始化监控
 ****************************************************************/
class CSimuD_SInitMonitorCmd : public CSimuD_SCmd
{
public:
	CSimuD_SInitMonitorCmd();
	virtual ~CSimuD_SInitMonitorCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void  SetData(const unsigned char *szConfPath, unsigned short nConfPathLen);
	void  GetData(unsigned char *szConfPath, unsigned short *nConfPathLen);

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned short m_usModelConfPathLen;
	unsigned char  m_szModelConfPath[256];
};

/****************************************************************
 * 类名 : CSimuD_SInitMonitorRespCmd
 * 父类 : CSimuD_SCmd
 * 说明 : 初始化监控响应
 ****************************************************************/
class CSimuD_SInitMonitorRespCmd : public CSimuD_SCmd
{
public:
	CSimuD_SInitMonitorRespCmd();
	virtual ~CSimuD_SInitMonitorRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetResult(unsigned long nResult);
	unsigned long GetResult();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulResult;
};

/****************************************************************
 * 类名 : CSimuD_SLoadSimuModelReportCmd
 * 父类 : CSimuD_SCmd
 * 说明 : 模型加载上报
 ****************************************************************/
class CSimuD_SLoadSimuModelReportCmd : public CSimuD_SCmd
{
public:
	CSimuD_SLoadSimuModelReportCmd();
	virtual ~CSimuD_SLoadSimuModelReportCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetResult(unsigned long nResult);
	unsigned long GetResult();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulResult;
};

/****************************************************************
 * 类名 : CSimuD_SExecSimuModelCmd
 * 父类 : CSimuD_SCmd
 * 说明 : 模型执行命令
 ****************************************************************/
class CSimuD_SExecSimuModelCmd : public CSimuD_SCmd
{
public:
	CSimuD_SExecSimuModelCmd();
	virtual ~CSimuD_SExecSimuModelCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetData(int nExecMode, int TestTask, int TestCase);
	unsigned long GetExecMode();
	unsigned long GetTestTask();
	unsigned long GetTestCase();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long  m_nExecMode;
	unsigned long  m_nTestTask;
	unsigned long  m_nTestCase;
};

/****************************************************************
 * 类名 : CSimuD_SExecSimuModelRespCmd
 * 父类 : CSimuD_SCmd
 * 说明 : 模型执行命令响应
 ****************************************************************/
class CSimuD_SExecSimuModelRespCmd : public CSimuD_SCmd
{
public:
	CSimuD_SExecSimuModelRespCmd();
	virtual ~CSimuD_SExecSimuModelRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetResult(unsigned long nResult);
	unsigned long GetResult();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulResult;
};

/****************************************************************
 * 类名 : CSimuD_SPauseSimuModelCmd
 * 父类 : CSimuD_SCmd
 * 说明 : 模型暂停命令
 ****************************************************************/
class CSimuD_SPauseSimuModelCmd : public CSimuD_SCmd
{
public:
	CSimuD_SPauseSimuModelCmd();
	virtual ~CSimuD_SPauseSimuModelCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

protected:
	virtual unsigned long CalcFrameBodyLen();

};

/****************************************************************
 * 类名 : CSimuD_SPauseSimuModelRespCmd
 * 父类 : CSimuD_SCmd
 * 说明 : 模型暂停命令响应
 ****************************************************************/
class CSimuD_SPauseSimuModelRespCmd : public CSimuD_SCmd
{
public:
	CSimuD_SPauseSimuModelRespCmd();
	virtual ~CSimuD_SPauseSimuModelRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetResult(unsigned long nResult);
	unsigned long GetResult();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulResult;
};

/****************************************************************
 * 类名 : CSimuD_SResetSimuModelCmd
 * 父类 : CSimuD_SCmd
 * 说明 : 重置模型命令
 ****************************************************************/
class CSimuD_SResetSimuModelCmd : public CSimuD_SCmd
{
public:
	CSimuD_SResetSimuModelCmd();
	virtual ~CSimuD_SResetSimuModelCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

protected:
	virtual unsigned long CalcFrameBodyLen();

};

/****************************************************************
 * 类名 : CSimuD_SResetSimuModelRespCmd
 * 父类 : CSimuD_SCmd
 * 说明 : 重置模型命令响应
 ****************************************************************/
class CSimuD_SResetSimuModelRespCmd : public CSimuD_SCmd
{
public:
	CSimuD_SResetSimuModelRespCmd();
	virtual ~CSimuD_SResetSimuModelRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetResult(unsigned long nResult);
	unsigned long GetResult();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulResult;
};

/****************************************************************
 * 类名 : CSimuD_SSetSimuModelParamCmd
 * 父类 : CSimuD_SCmd
 * 说明 : 设置模型参数
 ****************************************************************/
class CSimuD_SSetSimuModelParamCmd : public CSimuD_SCmd
{
public:
	CSimuD_SSetSimuModelParamCmd();
	virtual ~CSimuD_SSetSimuModelParamCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetData(vector<CommuSetModelParam_t *> &vecModelParam);
	const vector<CommuSetModelParam_t *> *GetData();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long		m_nModelParamCount;
	vector<CommuSetModelParam_t*>	m_vecModelParam;
};

/****************************************************************
 * 类名 : CSimuD_SSetSimuModelParamRespCmd
 * 父类 : CSimuD_SCmd
 * 说明 : 设置模型参数命令响应
 ****************************************************************/
class CSimuD_SSetSimuModelParamRespCmd : public CSimuD_SCmd
{
public:
	CSimuD_SSetSimuModelParamRespCmd();
	virtual ~CSimuD_SSetSimuModelParamRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetResult(unsigned long nResult);
	unsigned long GetResult();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulResult;
};

/****************************************************************
 * 类名 : CSimuD_STestEventReportCmd
 * 父类 : CSimuD_SCmd
 * 说明 : 测试事件上报
 ****************************************************************/
class CSimuD_STestEventReportCmd : public CSimuD_SCmd
{
public:
	CSimuD_STestEventReportCmd();
	virtual ~CSimuD_STestEventReportCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer); 

	void SetReport(const CommuTestEventReport_t *pReport);
	const CommuTestEventReport_t *GetReport();

protected:
	virtual unsigned long CalcFrameBodyLen();

	CommuTestEventReport_t	m_Report;
};

/****************************************************************
 * 类名 : CSimuD_SExceptEventReportCmd
 * 父类 : CSimuD_SCmd
 * 说明 : 异常事件上报
 ****************************************************************/
class CSimuD_SExceptEventReportCmd : public CSimuD_SCmd
{
public:
	CSimuD_SExceptEventReportCmd();
	virtual ~CSimuD_SExceptEventReportCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer); 

	void SetReport(const CommuExceptReport_t *pReport);
	const CommuExceptReport_t *GetReport();

protected:
	virtual unsigned long CalcFrameBodyLen();

	CommuExceptReport_t m_Report;
};

/****************************************************************
 * 类名 : CSimuD_SEnableSimuMonitorCmd
 * 父类 : CSimuD_SCmd
 * 说明 : 使能监控命令
 ****************************************************************/
class CSimuD_SEnableSimuMonitorCmd : public CSimuD_SCmd
{
public:
	CSimuD_SEnableSimuMonitorCmd();
	virtual ~CSimuD_SEnableSimuMonitorCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetFlag(unsigned long ulFlag);
	unsigned long GetFlag();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long  m_ulEnableFlag;
};

/****************************************************************
 * 类名 : CSimuD_SEnableSimuMonitorRespCmd
 * 父类 : CSimuD_SCmd
 * 说明 : 使能监控命令响应
 ****************************************************************/
class CSimuD_SEnableSimuMonitorRespCmd : public CSimuD_SCmd
{
public:
	CSimuD_SEnableSimuMonitorRespCmd();
	virtual ~CSimuD_SEnableSimuMonitorRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetResult(unsigned long nResult);
	unsigned long GetResult();

	void SetErrInfo(const char *szErrInfo, unsigned long ulErrLen);
	unsigned long GetErrInfo(char *pszErrInfo, unsigned long nBuffLen);

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulResult;
	unsigned long m_ulErrInfoLen;
	char m_szErrInfo[1024];
};