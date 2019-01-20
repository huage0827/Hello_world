/*******************************************************************
 * 文件名: SimuL_SCmd.h
 * 功能: SimuLab和SimuService之间通信命令定义
 ******************************************************************/
#pragma  once

#include <vector>
#include "SimuCommunicationDef.h"

using namespace std;

/****************************************************************
 * 枚举名 : L_S_CMD
 * 说明 : SimuLab和SimuService之间通信命令字定义
 ****************************************************************/
typedef enum euL_S_CMD
{
	L_S_CMD_UNKNOWN						= -1,			/* 未知命令 */
	L_S_CMD_CONNECTTARGET				= 0x00000001,	/* 连接目标机请求 */
	L_S_CMD_CONNECTTARGET_RESP			= 0x80000001,	/* 连接目标机响应 */
	L_S_CMD_LOADSIMUMODEL				= 0x00000002,	/* 加载仿真模型 */
	L_S_CMD_LOADSIMUMODEL_RESP			= 0x80000002,	/* 加载仿真模型响应 */
	L_S_CMD_ENABLESIMUMONITOR			= 0x00000003,	/* 使能监控服务请求 */
	L_S_CMD_ENABLESIMUMONITOR_RESP		= 0x80000003,	/* 使能监控服务响应 */
	L_S_CMD_COMPILESIMUMODEL			= 0x00000004,	/* 编译模型请求 */
	L_S_CMD_COMPILESIMUMODEL_RESP		= 0x80000004,	/* 编译模型响应 */
	L_S_CMD_COMPILEINFO_REP				= 0x80000005,	/* 编译信息上报 */
} L_S_CMD;

/*BEGIN  SimuLab和SimuService之间通信命令帧各字段在帧中所处的位置以及大小定义 */
#define L_S_CMD_POS_VERSION					0
#define L_S_CMD_POS_FRAMELEN				4
#define L_S_CMD_POS_COMMAND					8
#define L_S_CMD_POS_FRAMEBODYLEN			12
#define L_S_CMD_POS_SIMUPROJECTID			16
#define L_S_CMD_POS_SIMUMODEL				52
#define L_S_CMD_POS_RESERVED2				56
#define L_S_CMD_POS_RESERVED3				60
#define L_S_CMD_POS_RESERVED4				64
#define L_S_CMD_POS_FRAMEBODY				80
#define L_S_CMD_HEADER_SIZE					80
#define L_S_CMD_BODY_SIZE(bodylen)			(bodylen)
#define L_S_CMD_SIZE(bodylen)				((L_S_CMD_HEADER_SIZE) + (L_S_CMD_BODY_SIZE(bodylen)))
#define L_S_CMD_SIZE_VERSION				4
#define L_S_CMD_SIZE_FRAMELEN				4
#define L_S_CMD_SIZE_COMMAND				4
#define L_S_CMD_SIZE_FRAMEBODYLEN			4
#define L_S_CMD_SIZE_SIMUPROJECTID			36
#define L_S_CMD_SIZE_SIMUMODEL				4
#define L_S_CMD_SIZE_RESERVED2				4
#define L_S_CMD_SIZE_RESERVED3				4
#define L_S_CMD_SIZE_RESERVED4				16
#define L_S_CMD_SIZE_FRAMEBODY(bodylen)		(bodylen)
#define NAME_LEN							64
/*BEGIN  SimuLab和SimuService之间通信命令帧各字段在帧中所处的位置以及大小定义 */


/****************************************************************
 * 类名 : CSimuL_SCmd
 * 父类 : 无
 * 说明 : SimuLab和SimuService之间通信的所有命令的父类
 ****************************************************************/
class CSimuL_SCmd
{
public:
	CSimuL_SCmd(L_S_CMD slc);
	virtual ~CSimuL_SCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);	/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer); /* 输出打印 */

	static L_S_CMD PreParseCmd(unsigned char* szBuffer , unsigned long nBufferLen , unsigned long& nCmdDataLen);	/* 预解析数据 , 获得响应的命令 */
	L_S_CMD GetCmdType() const;	 /* 获取命令值 */
	void SetSimuProjectID(const unsigned char* szId); /* 设置命令所属的仿真工程ID号 */
	unsigned long GetFrameLen();	/* 获取命令帧长度 */
	unsigned long GetFrameBodyLen(); /* 获取命令帧体长度 */
	void GetSimuProjectID(unsigned char* szId); /* 获取命令所属的仿真工程ID号 */
	unsigned long SerializeHeader(unsigned char* szBuffer);/* 序列化命令帧头 */
	void SetSimuModelID(const unsigned char* szModelId); /* 设置模型ID */
	void GetSimuModelID(unsigned char *szModelId);	/* 获取模型ID */
protected:
	unsigned long CalcFrameLen();
	virtual unsigned long CalcFrameBodyLen() = 0;

	L_S_CMD m_slc;				 /* 命令字 */
	unsigned char m_szVersion[L_S_CMD_SIZE_VERSION];/* 命令版本号 */
	unsigned long m_nFrameLen;	 /* 命令帧长度*/
	unsigned long m_nCmd;		 /* 命令字 */
	unsigned long m_nFrameBodyLen; /* 命令帧体长度 */
	unsigned char  m_szSimuProjectID[L_S_CMD_SIZE_SIMUPROJECTID];	/* 命令所属的仿真工程ID号 */
	unsigned char  m_szSimuModelID[L_S_CMD_SIZE_SIMUMODEL]; /* 命令保留字段1->SimuModelID */
	unsigned char  m_szReserved2[L_S_CMD_SIZE_RESERVED2];	/* 命令保留字段2 */
	unsigned char  m_szReserved3[L_S_CMD_SIZE_RESERVED3];	/* 命令保留字段3 */
	unsigned char  m_szReserved4[L_S_CMD_SIZE_RESERVED4];	/* 命令保留字段4 */
};

/****************************************************************
 * 类名 : CSimuL_SConnectTargetCmd
 * 父类 : CSimuL_SCmd
 * 说明 : 连接到SimuService请求命令
 ****************************************************************/
class CSimuL_SConnectTargetCmd : public CSimuL_SCmd
{
public:
	CSimuL_SConnectTargetCmd();
	virtual ~CSimuL_SConnectTargetCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer); /* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen); /* 反序列化传入的数据 */
	virtual void Print(char* szBuffer);

	void GetTargetInfo(unsigned char *szTargetIP, unsigned short &usPort);
	void SetTargetInfo(const char* szTargetIP , unsigned short usPort);

	void SetConnectType(unsigned int ConnectType);
	unsigned int GetConnectType();

protected:
	virtual unsigned long CalcFrameBodyLen();

	char m_szTargetIp[4];
	unsigned short m_usPort;
	unsigned int m_ulConnectType;
};

/****************************************************************
 * 类名 : CSimuL_SConnectTargetRespCmd
 * 父类 : CSimuL_SCmd
 * 说明 : 连接到SimuService响应命令
 ****************************************************************/
class CSimuL_SConnectTargetRespCmd : public CSimuL_SCmd
{
public:
	CSimuL_SConnectTargetRespCmd();
	virtual ~CSimuL_SConnectTargetRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);	/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer);

	void SetResult(unsigned long ret);
	unsigned long GetResult();	
																																								/* 获取连接结果 */
	void SetErrInfo(unsigned char *szErrInfo, unsigned long nErrLen);
	int  GetErrInfo(unsigned char *szBuffer);
protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long	m_ulResult;		
	unsigned long   m_ulErrInfoLen;																																					/* 连接结果 */
	unsigned char   m_szErrInfo[1024];
};

/****************************************************************
 * 类名 : CSimuL_SLoadSimuModelCmd
 * 父类 : CSimuL_SCmd
 * 说明 : 加载仿真模型
 ****************************************************************/
class CSimuL_SLoadSimuModelCmd : public CSimuL_SCmd
{
public:
	CSimuL_SLoadSimuModelCmd();
	virtual ~CSimuL_SLoadSimuModelCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);		/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen); /* 反序列化传入的数据 */
	virtual void Print(char* szBuffer);

	void SetParam(const CommuLoadSimuModelParam_t *pParam);
	const CommuLoadSimuModelParam_t *GetParam();

protected:
	virtual unsigned long CalcFrameBodyLen();

	CommuLoadSimuModelParam_t	m_CommuParam;
};

/****************************************************************
 * 类名 : CSimuL_SLoadSimuModelRespCmd
 * 父类 : CSimuL_SCmd
 * 说明 : 加载仿真模型响应
 ****************************************************************/
class CSimuL_SLoadSimuModelRespCmd : public CSimuL_SCmd
{
public:
	CSimuL_SLoadSimuModelRespCmd();
	virtual ~CSimuL_SLoadSimuModelRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);	/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer);

	unsigned long GetResult();	
	void SetResult(unsigned long nRet);

	void SetErrInfo(unsigned char *szErrInfo, unsigned long nErrLen);
	int  GetErrInfo(unsigned char *szBuffer);
protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long	m_ulResult;		
	unsigned long   m_ulErrInfoLen;																																					/* 连接结果 */
	unsigned char   m_szErrInfo[1024];																																						/* 断开连接结果 */
};

/****************************************************************
 * 类名 : CSimuL_SEnableSimuMonitorCmd
 * 父类 : CSimuL_SCmd
 * 说明 : 使能监控命令
 ****************************************************************/
class CSimuL_SEnableSimuMonitorCmd : public CSimuL_SCmd
{
public:
	CSimuL_SEnableSimuMonitorCmd();
	virtual ~CSimuL_SEnableSimuMonitorCmd();

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
 * 类名 : CSimuL_SEnableSimuMonitorRespCmd
 * 父类 : CSimuL_SCmd
 * 说明 : 使能监控命令响应
 ****************************************************************/
class CSimuL_SEnableSimuMonitorRespCmd : public CSimuL_SCmd
{
public:
	CSimuL_SEnableSimuMonitorRespCmd();
	virtual ~CSimuL_SEnableSimuMonitorRespCmd();

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
 * 类名 : CSimuL_SCompileSimuModelCmd
 * 父类 : CSimuL_SCmd
 * 说明 : 编译模型命令
 ****************************************************************/
class CSimuL_SCompileSimuModelCmd : public CSimuL_SCmd
{
public:
	CSimuL_SCompileSimuModelCmd();
	virtual ~CSimuL_SCompileSimuModelCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	int SetCompileParam(const CommuModelCompileParam_t *pParam);
	int GetCompileParam(CommuModelCompileParam_t *pParam);

protected:
	virtual unsigned long CalcFrameBodyLen();
	CommuModelCompileParam_t m_CompileParam;
};

/****************************************************************
 * 类名 : CSimuL_SCompileSimuModelRespCmd
 * 父类 : CSimuL_SCmd
 * 说明 : 编译模型命令响应
 ****************************************************************/
class CSimuL_SCompileSimuModelRespCmd : public CSimuL_SCmd
{
public:
	CSimuL_SCompileSimuModelRespCmd();
	virtual ~CSimuL_SCompileSimuModelRespCmd();

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
 * 类名 : CSimuL_SCompileInfoRepCmd
 * 父类 : CSimuL_SCmd
 * 说明 : 编译信息上报命令
 ****************************************************************/
class CSimuL_SCompileInfoRepCmd : public CSimuL_SCmd
{
public:
	CSimuL_SCompileInfoRepCmd();
	virtual ~CSimuL_SCompileInfoRepCmd();

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