/*****************************************************************
 * 文件名: SimuL_TCmd.h
 * 功能: SimuLab 与 SimuGuard 之间通信命令定义
 *****************************************************************/
#pragma  once

#ifndef WIN_PLATFORM
#include "stdafx.h"
#endif

#include <vector>
#include "SimuCommunicationDef.h"

using namespace std;

/****************************************************************
 * 枚举名 : L_T_CMD
 * 说明 : SimuLab 与 SimuGuard 之间通信命令定义
 ****************************************************************/
typedef enum euL_T_CMD
{
	L_T_CMD_UNKNOWN						= -1 ,		 /* 未知命令*/
	L_T_CMD_TARGET_HEARTBEAT_REQ		= 0x70000001,/* 心跳检测请求 */
	L_T_CMD_TARGET_HEARTBEAT_RESP		= 0x70000002,/* 心跳检测响应 */
	L_T_CMD_GET_TARGET_INFO_REQ			= 0x70000003,/* 获取下位机信息请求 */
	L_T_CMD_GET_TARGET_INFO_RESP		= 0x70000004,/* 获取下位机信息响应 */
} L_T_CMD;


/*BEGIN  SimuLab 与 SimuGuard之间通信命令帧各字段在帧中所处的位置以及大小定义 */
#define L_T_CMD_POS_BEGAINSIGN			0
#define L_T_CMD_POS_VERSION				2
#define L_T_CMD_POS_SENDID				4
#define L_T_CMD_POS_RECVID				8
#define L_T_CMD_POS_COMMAND				12
#define L_T_CMD_POS_FRAMEBODYLEN		16
#define L_T_CMD_POS_RESERVED			20
#define L_T_CMD_POS_ENDSIGN				30

#define L_T_CMD_HEADER_SIZE				32

#define L_T_CMD_SIZE_BEGINSIGN			2
#define L_T_CMD_SIZE_VERSION			2
#define L_T_CMD_SIZE_SENDID				4
#define L_T_CMD_SIZE_RECVID				4
#define L_T_CMD_SIZE_COMMAND			4
#define L_T_CMD_SIZE_FRAMEBODYLEN		4
#define L_T_CMD_SIZE_RESERVED			10
#define L_T_CMD_SIZE_ENDSIGN			2
/*END  SimuLab 与 SimuGuard之间通信命令帧各字段在帧中所处的位置以及大小定义 */

/****************************************************************
 * 类名 : CSimuL_TCmd
 * 父类 : 无
 * 说明 : SimuLab 与 SimuGuard之间通信的所有命令的父类
 ****************************************************************/
class CSimuL_TCmd
{
public:
	CSimuL_TCmd(L_T_CMD sgc);
	virtual ~CSimuL_TCmd();

	/* 预解析数据 , 获得响应的命令 */
	static L_T_CMD PreParseCmd(unsigned char* szBuffer , unsigned long nBufferLen , unsigned long& nCmdDataLen);	
	
	/* 获取命令值 */
	L_T_CMD GetCmdType() const;		
	
	/* 获取命令帧体长度 */
	unsigned long GetFrameBodyLen();

	/* 序列化命令帧头 */
	unsigned long SerializeHeader(unsigned char* szBuffer);	
	
	/* 序列化命令 */
	virtual unsigned long Serialize(unsigned char* szBuffer);
	
	/* 反序列化传入的数据 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);

	/* 输出打印 */
	virtual void Print(char* szBuffer); 

protected:
	virtual unsigned long CalcFrameBodyLen() = 0;

	L_T_CMD		   m_sgc;  /* 命令字 */
	unsigned long  m_nCmd; /* 命令字 */
	unsigned long  m_nFrameBodyLen;	/* 命令帧体长度 */
	unsigned char  m_beginSign[2];
	unsigned char  m_endSign[2];
};

/****************************************************************
 * 类名 : CSimuL_THeartBeatCmd
 * 父类 : CSimuL_TCmd
 * 说明 : 心跳检测请求命令
 ****************************************************************/
class CSimuL_THeartBeatCmd : public CSimuL_TCmd
{
public:
	CSimuL_THeartBeatCmd();
	virtual ~CSimuL_THeartBeatCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer);

	void SetSerial(unsigned long result);
	unsigned long GetSerial();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulSerial;
};

/****************************************************************
 * 类名 : CSimuL_THeartBeatRespCmd
 * 父类 : CSimuL_TCmd
 * 说明 : 心跳检测响应命令
 ****************************************************************/
class CSimuL_THeartBeatRespCmd : public CSimuL_TCmd
{
public:
	CSimuL_THeartBeatRespCmd();
	virtual ~CSimuL_THeartBeatRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer); 

	void SetSerial(unsigned long result);
	unsigned long GetSerial();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulSerial;
};

/****************************************************************
 * 类名 : CSimuL_TGetTargetInfoCmd
 * 父类 : CSimuL_TCmd
 * 说明 : 获取下位机信息请求命令
 ****************************************************************/
class CSimuL_TGetTargetInfoCmd : public CSimuL_TCmd
{
public:
	CSimuL_TGetTargetInfoCmd();
	virtual ~CSimuL_TGetTargetInfoCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer);

protected:
	virtual unsigned long CalcFrameBodyLen();
};

/****************************************************************
 * 类名 : CSimuL_TGetTargetInfoRespCmd
 * 父类 : CSimuL_TCmd
 * 说明 : 获取下位机信息响应命令
 ****************************************************************/
class CSimuL_TGetTargetInfoRespCmd : public CSimuL_TCmd
{
public:
	CSimuL_TGetTargetInfoRespCmd();
	virtual ~CSimuL_TGetTargetInfoRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* 序列化命令 */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* 反序列化传入的数据 */
	virtual void Print(char* szBuffer); 

	void SetInfo(CommuTargetInfo_t *pInfo);
	const CommuTargetInfo_t *GetInfo();

protected:
	virtual unsigned long CalcFrameBodyLen();

	CommuTargetInfo_t m_TargetInfo;
};
