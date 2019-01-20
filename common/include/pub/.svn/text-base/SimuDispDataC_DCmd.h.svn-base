/*****************************************************************
 * 文件名: SimuDispDataC_DCmd.h
 * 功能: SimuService,SimuGuard与SimuMonitor之间通信命令定义
 *****************************************************************/
#pragma  once

#ifndef WIN_PLATFORM
#include "stdafx.h"
#endif

#include <vector>
#include "SimuCommunicationDef.h"

using namespace std;

/****************************************************************
 * 枚举名 : DispDataC_D_CMD
 * 说明 : SimuService,SimuGuard与SimuMonitor之间通信命令字定义
 ****************************************************************/
typedef enum euDispDataC_D_CMD
{
	DDATA_C_D_CMD_UNKNOWN			= -1 ,		 /* 未知命令*/
	DDATA_C_D_CMD_DISP_SLPRINT		= 0x50000001,/* SLPRINT输出信息 */
	DDATA_C_D_CMD_DISP_SIMUDATA		= 0x50000002,/* 显示的模型数据 */
	DDATA_C_D_CMD_DISP_SIMUSTATUS	= 0x50000003,/* 显示的状态数据 */
	
} DispDataC_D_CMD;

/*BEGIN  SimuService,SimuGuard与SimuMonitor之间通信命令帧各字段在帧中所处的位置以及大小定义 */
#define DDATA_C_D_CMD_POS_SIGN				0
#define DDATA_C_D_CMD_POS_MODELID			2
#define DDATA_C_D_CMD_POS_COMMAND			4
#define DDATA_C_D_CMD_POS_TIMESTAMP			8
#define DDATA_C_D_CMD_POS_FRAMEBODYLEN		12

#define DDATA_C_D_CMD_HEADER_SIZE			16

#define DDATA_C_D_CMD_SIZE_SIGN				2
#define DDATA_C_D_CMD_SIZE_MODELID			2
#define DDATA_C_D_CMD_SIZE_COMMAND			4
#define DDATA_C_D_CMD_SIZE_TIMESTAMP		4
#define DDATA_C_D_CMD_SIZE_FRAMEBODYLEN		4
/*END  SimuService,SimuGuard与SimuMonitor之间通信命令帧各字段在帧中所处的位置以及大小定义 */

#define DDATA_MAX_SLPRINT_LEN			1024
#define DDATA_MAX_SIMUDATA_LEN			2048

/****************************************************************
 * 类名 : CSimuDispDataC_DCmd
 * 父类 : 无
 * 说明 : SimuService,SimuGuard与SimuMonitor之间通信的所有命令的父类
 ****************************************************************/
class CSimuDispDataC_DCmd
{
public:
	CSimuDispDataC_DCmd(DispDataC_D_CMD sgc);
	virtual ~CSimuDispDataC_DCmd();

	/* 预解析数据 , 获得响应的命令 */
	static DispDataC_D_CMD PreParseCmd(const unsigned char* szBuffer , unsigned long nBufferLen , unsigned long& nCmdDataLen);	
	
	/* 获取命令值 */
	DispDataC_D_CMD GetCmdType() const;	
				
	/* 获取命令帧长度 */
	unsigned long GetFrameLen();	

	/* 获取命令帧体长度 */
	unsigned long GetFrameBodyLen();	

	/* 序列化命令 */
	virtual unsigned long Serialize(unsigned char* szBuffer);		
	
	/* 反序列化传入的数据 */
	virtual BOOL Unserialize(const unsigned char* szBuffer , unsigned long nBufferLen);	
	
	/* 打印输出 */
	virtual void Print(char* szBuffer);

	/* 设置模型ID */
	void SetModelID(unsigned short id);
	
	/* 获取模型ID */
	unsigned short GetModelID();
	
	/* 设置时间戳 */
	void SetTimeStamp(unsigned long ts);
	
	/* 返回时间戳 */
	unsigned long GetTimeStamp();

protected:
	unsigned long CalcFrameLen();
	virtual unsigned long CalcFrameBodyLen() = 0;

	DispDataC_D_CMD		m_sgc;	/* 命令字 */

	unsigned char		m_szSign[DDATA_C_D_CMD_SIZE_SIGN];
	unsigned long		m_nCmd;				/* 命令字 */
	unsigned long		m_nFrameBodyLen;	/* 命令帧体长度 */
	
	unsigned long		m_nTimeStamp;		/* Unix时间 */
	unsigned short		m_usModelID;		/* 模型ID */																								/* 命令保留字段4 */
};

/****************************************************************
 * 类名 : CSimuDispDataC_DSLPrintCmd
 * 父类 : CSimuDispDataC_DCmd
 * 说明 : SLPrint打印数据
 ****************************************************************/
class CSimuDispDataC_DSLPrintCmd : public CSimuDispDataC_DCmd
{
public:
	CSimuDispDataC_DSLPrintCmd();
	virtual ~CSimuDispDataC_DSLPrintCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(const unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetStep(unsigned long nStep);
	unsigned long GetStep();

	void SetData(const unsigned char *szData, unsigned long nDataLen);
	unsigned long GetData(unsigned char *szBuff, unsigned long nBuffLen);
	unsigned long GetDataLen();     /* 返回数据长度 */
protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_nStep;
	unsigned long m_nDataLen;
	unsigned char m_szData[DDATA_MAX_SLPRINT_LEN]; /* 字符串 */
};

/****************************************************************
 * 类名 : CSimuDispDataC_DSimuDataCmd
 * 父类 : CSimuDispDataC_DCmd
 * 说明 : 显示的仿真模型数据
 ****************************************************************/
class CSimuDispDataC_DSimuDataCmd : public CSimuDispDataC_DCmd
{
public:
	CSimuDispDataC_DSimuDataCmd();
	virtual ~CSimuDispDataC_DSimuDataCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(const unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetStep(unsigned long nStep);
	unsigned long GetStep();

	void SetData(const unsigned char *szData, unsigned long nDataLen);
	unsigned long GetData(unsigned char *szBuff, unsigned long nBuffLen);
	unsigned long GetDataLen(); /* 返回数据长度 */
	unsigned char *GetData();
protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_nStep;
	unsigned long m_nDataLen;
	unsigned char m_szData[DDATA_MAX_SIMUDATA_LEN];
};

/****************************************************************
 * 类名 : CSimuDispDataC_DSimuStatusCmd
 * 父类 : CSimuDispDataC_DCmd
 * 说明 : 显示的仿真状态
 ****************************************************************/
class CSimuDispDataC_DSimuStatusCmd : public CSimuDispDataC_DCmd
{
public:
	CSimuDispDataC_DSimuStatusCmd();
	virtual ~CSimuDispDataC_DSimuStatusCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(const unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetData(unsigned long nStep, unsigned long nOverRun, double execTime);
	
	unsigned long GetStep();
	unsigned long GetOverRunCount();
	double GetExecTime();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_nStep;
	unsigned long m_nOverRun;
	double		  m_ExecTime;
};
