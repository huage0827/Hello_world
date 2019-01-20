/*****************************************************************
 * 文件名: SimuS_GCmd.h
 * 功能: SimuService和SimuGuard之间通信命令定义
 *****************************************************************/
#pragma  once

#ifndef WIN_PLATFORM
#include "stdafx.h"
#endif

#include <vector>
#include "SimuCommunicationDef.h"

using namespace std;

/****************************************************************
 * 枚举名 : S_G_CMD
 * 说明 : SimuService和SimuGuard之间通信命令字定义
 ****************************************************************/
typedef enum euS_G_CMD
{
	S_G_CMD_UNKNOWN						= -1 ,			/* 未知命令*/
	S_G_CMD_LOADSIMUMODEL				= 0x20000001,	/* 装载仿真模型请求 */
	S_G_CMD_LOADSIMUMODEL_RESP			= 0xA0000001,	/* 装载仿真模型响应 */
	S_G_CMD_EXECSIMUMODEL				= 0x20000002,	/* 执行仿真模型请求 */
	S_G_CMD_EXECSIMUMODEL_RESP			= 0xA0000002,	/* 执行仿真模型响应 */
	S_G_CMD_PAUSESIMUMODEL				= 0x20000003,	/* 暂停仿真模型请求 */
	S_G_CMD_PAUSESIMUMODEL_RESP			= 0xA0000003,	/* 暂停仿真模型响应 */
	S_G_CMD_RESETSIMUMODEL				= 0x20000004,	/* 重置仿真模型请求 */
	S_G_CMD_RESETSIMUMODEL_RESP			= 0xA0000004,	/* 重置仿真模型响应 */
	S_G_CMD_SETMODELPARAM				= 0x20000005,	/* 设置模型参数请求 */
	S_G_CMD_SETMODELPARAM_RESP			= 0xA0000005,	/* 设置模型参数响应 */
	S_G_CMD_TESTEVENT_REPORT			= 0xA0000006,	/* 测试事件上报 */
	S_G_CMD_EXCEPTEVENT_REPORT			= 0xA0000007,	/* 异常上报 */
	S_G_CMD_COMPILESIMUMODEL			= 0x00000008,	/* 编译模型请求 */
	S_G_CMD_COMPILESIMUMODEL_RESP		= 0xA0000008,	/* 编译模型响应 */
	S_G_CMD_COMPILEINFO_REPORT			= 0xA0000009,	/* 编译信息上报 */
} S_G_CMD;


/*BEGIN  SimuService和SimuGuard之间通信命令帧各字段在帧中所处的位置以及大小定义 */
#define S_G_CMD_POS_VERSION				0
#define S_G_CMD_POS_FRAMELEN			4
#define S_G_CMD_POS_COMMAND				8
#define S_G_CMD_POS_FRAMEBODYLEN		12
#define S_G_CMD_POS_SIMUPROJECTID		16
#define S_G_CMD_POS_SIMUMODELID			52
#define S_G_CMD_POS_RESERVED2			56
#define S_G_CMD_POS_RESERVED3			60
#define S_G_CMD_POS_RESERVED4			64
#define S_G_CMD_POS_FRAMEBODY			80
#define S_G_CMD_HEADER_SIZE					80
#define S_G_CMD_BODY_SIZE(bodylen)			(bodylen)
#define S_G_CMD_SIZE(bodylen)				((S_G_CMD_HEADER_SIZE) + (S_G_CMD_BODY_SIZE(bodylen)))
#define S_G_CMD_SIZE_VERSION				4
#define S_G_CMD_SIZE_FRAMELEN				4
#define S_G_CMD_SIZE_COMMAND				4
#define S_G_CMD_SIZE_FRAMEBODYLEN			4
#define S_G_CMD_SIZE_SIMUPROJECTID			36
#define S_G_CMD_SIZE_SIMUMODELID			4
#define S_G_CMD_SIZE_RESERVED2				4
#define S_G_CMD_SIZE_RESERVED3				4
#define S_G_CMD_SIZE_RESERVED4				16
#define S_G_CMD_SIZE_FRAMEBODY(bodylen)		(bodylen)
/*END  SimuDisplay和SimuService之间通信命令帧各字段在帧中所处的位置以及大小定义 */

/****************************************************************
 * 类名 : CSimuS_GCmd
 * 父类 : 无
 * 说明 : SimuService和SimuGuard之间通信的所有命令的父类
 ****************************************************************/
class CSimuS_GCmd
{
public:
	CSimuS_GCmd(S_G_CMD sgc);
	virtual ~CSimuS_GCmd();

	/* 预解析数据 , 获得响应的命令 */
	static S_G_CMD PreParseCmd(unsigned char* szBuffer , unsigned long nBufferLen , unsigned long& nCmdDataLen);	
	
	/* 获取命令值 */
	S_G_CMD GetCmdType() const;	
	
	/* 设置仿真工程ID号 */
	void SetSimuProjectID(const unsigned char* szId);	
	
	/* 获取命令帧长度 */
	unsigned long GetFrameLen();		
	
	/* 获取命令帧体长度 */
	unsigned long GetFrameBodyLen();
	
	/* 获取仿真工程ID */
	void GetSimuProjectID(unsigned char* szId);
	
	/* 设置模型ID */
	void SetSimuModelID(const unsigned char *szModelID);

	/* 获取模型ID */
	void GetSimuModelID(unsigned char *szModelID);

	/* 序列化命令帧头 */
	unsigned long SerializeHeader(unsigned char* szBuffer);	
	
	/* 序列化命令 */
	virtual unsigned long Serialize(unsigned char* szBuffer);
	
	/* 反序列化传入的数据 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);

	/* 输出打印 */
	virtual void Print(char* szBuffer); 

protected:
	unsigned long CalcFrameLen();
	virtual unsigned long CalcFrameBodyLen() = 0;

	S_G_CMD		   m_sgc;																																													/* 命令字 */
	unsigned char  m_szVersion[S_G_CMD_SIZE_VERSION];																												/* 命令版本号 */
	unsigned long  m_nFrameLen;																																							/* 命令帧长度*/
	unsigned long  m_nCmd;																																										/* 命令字 */
	unsigned long  m_nFrameBodyLen;																																					/* 命令帧体长度 */

	unsigned char  m_szSimuProjectID[S_G_CMD_SIZE_SIMUPROJECTID];																					/* 仿真工程ID号 */
	unsigned char  m_szSimuModelID[S_G_CMD_SIZE_SIMUMODELID];																									/* 命令保留字段1 */
	unsigned char  m_szReserved2[S_G_CMD_SIZE_RESERVED2];																									/* 命令保留字段2 */
	unsigned char  m_szReserved3[S_G_CMD_SIZE_RESERVED3];																									/* 命令保留字段3 */
	unsigned char  m_szReserved4[S_G_CMD_SIZE_RESERVED4];																									/* 命令保留字段4 */
};

/****************************************************************
 * 类名 : CSimuS_GLoadSimuModelCmd
 * 父类 : CSimuS_GCmd
 * 说明 : 加载仿真模型请求命令
 ****************************************************************/
class CSimuS_GLoadSimuModelCmd : public CSimuS_GCmd
{
public:
	CSimuS_GLoadSimuModelCmd();
	virtual ~CSimuS_GLoadSimuModelCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer);

	void SetParam(const CommuLoadSimuModelParam_t *pParam);
	const CommuLoadSimuModelParam_t *GetParam();

protected:
	virtual unsigned long CalcFrameBodyLen();

	CommuLoadSimuModelParam_t	m_CommuParam;
};

/****************************************************************
 * 类名 : CSimuS_GLoadSimuModelRespCmd
 * 父类 : CSimuS_GCmd
 * 说明 : 加载仿真模型响应命令
 ****************************************************************/
class CSimuS_GLoadSimuModelRespCmd : public CSimuS_GCmd
{
public:
	CSimuS_GLoadSimuModelRespCmd();
	virtual ~CSimuS_GLoadSimuModelRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer); 

	void SetResult(unsigned long result);
	unsigned long GetResult();

	void SetErrInfo(unsigned char *szErrInfo, unsigned long nErrLen);
	int  GetErrInfo(unsigned char *szBuffer);
protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long	m_ulResult;		
	unsigned long   m_ulErrInfoLen;																																					/* 连接结果 */
	unsigned char   m_szErrInfo[1024];	
};

/****************************************************************
 * 类名 : CSimuS_GExecSimuModelCmd
 * 父类 : CSimuS_GCmd
 * 说明 : 执行仿真模型
 ****************************************************************/
class CSimuS_GExecSimuModelCmd : public CSimuS_GCmd
{
public:
	CSimuS_GExecSimuModelCmd();
	virtual ~CSimuS_GExecSimuModelCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* 反序列化传入的数据 */
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
 * 类名 : CSimuS_GExecSimuModelRespCmd
 * 父类 : CSimuS_GCmd
 * 说明 : 执行仿真模型响应命令
 ****************************************************************/
class CSimuS_GExecSimuModelRespCmd : public CSimuS_GCmd
{
public:
	CSimuS_GExecSimuModelRespCmd();
	virtual ~CSimuS_GExecSimuModelRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer); 

	void SetResult(unsigned long result);
	unsigned long GetResult();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulResult;
};

/****************************************************************
 * 类名 : CSimuS_GPauseSimuModelCmd
 * 父类 : CSimuS_GCmd
 * 说明 : 暂停仿真模型
 ****************************************************************/
class CSimuS_GPauseSimuModelCmd : public CSimuS_GCmd
{
public:
	CSimuS_GPauseSimuModelCmd();
	virtual ~CSimuS_GPauseSimuModelCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer); 

protected:
	virtual unsigned long CalcFrameBodyLen();

};

/****************************************************************
 * 类名 : CSimuS_GPauseSimuModelRespCmd
 * 父类 : CSimuS_GCmd
 * 说明 : 暂停仿真模型响应命令
 ****************************************************************/
class CSimuS_GPauseSimuModelRespCmd : public CSimuS_GCmd
{
public:
	CSimuS_GPauseSimuModelRespCmd();
	virtual ~CSimuS_GPauseSimuModelRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer); 

	void SetResult(unsigned long result);
	unsigned long GetResult();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulResult;
};

/****************************************************************
 * 类名 : CSimuS_GResetSimuModelCmd
 * 父类 : CSimuS_GCmd
 * 说明 : 重置仿真模型
 ****************************************************************/
class CSimuS_GResetSimuModelCmd : public CSimuS_GCmd
{
public:
	CSimuS_GResetSimuModelCmd();
	virtual ~CSimuS_GResetSimuModelCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer); 

protected:
	virtual unsigned long CalcFrameBodyLen();

};

/****************************************************************
 * 类名 : CSimuS_GResetSimuModelRespCmd
 * 父类 : CSimuS_GCmd
 * 说明 : 重置仿真模型响应命令
 ****************************************************************/
class CSimuS_GResetSimuModelRespCmd : public CSimuS_GCmd
{
public:
	CSimuS_GResetSimuModelRespCmd();
	virtual ~CSimuS_GResetSimuModelRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer); 

	void SetResult(unsigned long result);
	unsigned long GetResult();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulResult;
};

/****************************************************************
 * 类名 : CSimuS_GSetSimuModelParamCmd
 * 父类 : CSimuS_GCmd
 * 说明 : 设置仿真模型参数
 ****************************************************************/
class CSimuS_GSetSimuModelParamCmd : public CSimuS_GCmd
{
public:
	CSimuS_GSetSimuModelParamCmd();
	virtual ~CSimuS_GSetSimuModelParamCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer); 

	void SetModelParam(const vector<CommuSetModelParam_t *> &vecModelParam);
	const vector<CommuSetModelParam_t *> *GetModelParam();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long		m_nModelParamCount;
	vector<CommuSetModelParam_t*>	m_vecModelParam;
};

/****************************************************************
 * 类名 : CSimuS_GSetSimuModelParamRespCmd
 * 父类 : CSimuS_GCmd
 * 说明 : 设置仿真模型参数响应命令
 ****************************************************************/
class CSimuS_GSetSimuModelParamRespCmd : public CSimuS_GCmd
{
public:
	CSimuS_GSetSimuModelParamRespCmd();
	virtual ~CSimuS_GSetSimuModelParamRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer); 

	void SetResult(unsigned long result);
	unsigned long GetResult();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulResult;
};

/****************************************************************
 * 类名 : CSimuS_GTestEventReportCmd
 * 父类 : CSimuS_GCmd
 * 说明 : 测试事件上报
 ****************************************************************/
class CSimuS_GTestEventReportCmd : public CSimuS_GCmd
{
public:
	CSimuS_GTestEventReportCmd();
	virtual ~CSimuS_GTestEventReportCmd();

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
 * 类名 : CSimuS_GExceptEventReportCmd
 * 父类 : CSimuS_GCmd
 * 说明 : 异常事件上报
 ****************************************************************/
class CSimuS_GExceptEventReportCmd : public CSimuS_GCmd
{
public:
	CSimuS_GExceptEventReportCmd();
	virtual ~CSimuS_GExceptEventReportCmd();

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
 * 类名 : CSimuS_GCompileSimuModelCmd
 * 父类 : CSimuS_GCmd
 * 说明 : 编译模型命令
 ****************************************************************/
class CSimuS_GCompileSimuModelCmd : public CSimuS_GCmd
{
public:
	CSimuS_GCompileSimuModelCmd();
	virtual ~CSimuS_GCompileSimuModelCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	int SetCompileParam(const CommuModelCompileParam_t *pParam);
	int GetCompileParam(CommuModelCompileParam_t *pParam);
	const CommuModelCompileParam_t *GetCompileParam();

protected:
	virtual unsigned long CalcFrameBodyLen();
	CommuModelCompileParam_t m_CompileParam;
};

/****************************************************************
 * 类名 : CSimuS_GCompileSimuModelRespCmd
 * 父类 : CSimuS_GCmd
 * 说明 : 编译模型命令响应
 ****************************************************************/
class CSimuS_GCompileSimuModelRespCmd : public CSimuS_GCmd
{
public:
	CSimuS_GCompileSimuModelRespCmd();
	virtual ~CSimuS_GCompileSimuModelRespCmd();

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

/****************************************************************
 * 类名 : CSimuS_GCompileInfoRepCmd
 * 父类 : CSimuS_GCmd
 * 说明 : 编译信息上报命令
 ****************************************************************/
class CSimuS_GCompileInfoRepCmd : public CSimuS_GCmd
{
public:
	CSimuS_GCompileInfoRepCmd();
	virtual ~CSimuS_GCompileInfoRepCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetCompileInfo(unsigned long ulCompileRepType, const char *szCompileInfo);
	unsigned long GetCompileInfo(unsigned long *pCompileRepType, char *pszCompileInfo, unsigned long nBuffLen);

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulCompileRepType;
	unsigned long m_ulRepInfoLen;
	char m_szCompileInfo[2048];
};
