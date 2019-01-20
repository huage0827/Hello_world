/*******************************************************************
 * �ļ���: SimuL_SCmd.h
 * ����: SimuLab��SimuService֮��ͨ�������
 ******************************************************************/
#pragma  once

#include <vector>
#include "SimuCommunicationDef.h"

using namespace std;

/****************************************************************
 * ö���� : L_S_CMD
 * ˵�� : SimuLab��SimuService֮��ͨ�������ֶ���
 ****************************************************************/
typedef enum euL_S_CMD
{
	L_S_CMD_UNKNOWN						= -1,			/* δ֪���� */
	L_S_CMD_CONNECTTARGET				= 0x00000001,	/* ����Ŀ������� */
	L_S_CMD_CONNECTTARGET_RESP			= 0x80000001,	/* ����Ŀ�����Ӧ */
	L_S_CMD_LOADSIMUMODEL				= 0x00000002,	/* ���ط���ģ�� */
	L_S_CMD_LOADSIMUMODEL_RESP			= 0x80000002,	/* ���ط���ģ����Ӧ */
	L_S_CMD_ENABLESIMUMONITOR			= 0x00000003,	/* ʹ�ܼ�ط������� */
	L_S_CMD_ENABLESIMUMONITOR_RESP		= 0x80000003,	/* ʹ�ܼ�ط�����Ӧ */
	L_S_CMD_COMPILESIMUMODEL			= 0x00000004,	/* ����ģ������ */
	L_S_CMD_COMPILESIMUMODEL_RESP		= 0x80000004,	/* ����ģ����Ӧ */
	L_S_CMD_COMPILEINFO_REP				= 0x80000005,	/* ������Ϣ�ϱ� */
} L_S_CMD;

/*BEGIN  SimuLab��SimuService֮��ͨ������֡���ֶ���֡��������λ���Լ���С���� */
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
/*BEGIN  SimuLab��SimuService֮��ͨ������֡���ֶ���֡��������λ���Լ���С���� */


/****************************************************************
 * ���� : CSimuL_SCmd
 * ���� : ��
 * ˵�� : SimuLab��SimuService֮��ͨ�ŵ���������ĸ���
 ****************************************************************/
class CSimuL_SCmd
{
public:
	CSimuL_SCmd(L_S_CMD slc);
	virtual ~CSimuL_SCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);	/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	/* �����л���������� */
	virtual void Print(char* szBuffer); /* �����ӡ */

	static L_S_CMD PreParseCmd(unsigned char* szBuffer , unsigned long nBufferLen , unsigned long& nCmdDataLen);	/* Ԥ�������� , �����Ӧ������ */
	L_S_CMD GetCmdType() const;	 /* ��ȡ����ֵ */
	void SetSimuProjectID(const unsigned char* szId); /* �������������ķ��湤��ID�� */
	unsigned long GetFrameLen();	/* ��ȡ����֡���� */
	unsigned long GetFrameBodyLen(); /* ��ȡ����֡�峤�� */
	void GetSimuProjectID(unsigned char* szId); /* ��ȡ���������ķ��湤��ID�� */
	unsigned long SerializeHeader(unsigned char* szBuffer);/* ���л�����֡ͷ */
	void SetSimuModelID(const unsigned char* szModelId); /* ����ģ��ID */
	void GetSimuModelID(unsigned char *szModelId);	/* ��ȡģ��ID */
protected:
	unsigned long CalcFrameLen();
	virtual unsigned long CalcFrameBodyLen() = 0;

	L_S_CMD m_slc;				 /* ������ */
	unsigned char m_szVersion[L_S_CMD_SIZE_VERSION];/* ����汾�� */
	unsigned long m_nFrameLen;	 /* ����֡����*/
	unsigned long m_nCmd;		 /* ������ */
	unsigned long m_nFrameBodyLen; /* ����֡�峤�� */
	unsigned char  m_szSimuProjectID[L_S_CMD_SIZE_SIMUPROJECTID];	/* ���������ķ��湤��ID�� */
	unsigned char  m_szSimuModelID[L_S_CMD_SIZE_SIMUMODEL]; /* ������ֶ�1->SimuModelID */
	unsigned char  m_szReserved2[L_S_CMD_SIZE_RESERVED2];	/* ������ֶ�2 */
	unsigned char  m_szReserved3[L_S_CMD_SIZE_RESERVED3];	/* ������ֶ�3 */
	unsigned char  m_szReserved4[L_S_CMD_SIZE_RESERVED4];	/* ������ֶ�4 */
};

/****************************************************************
 * ���� : CSimuL_SConnectTargetCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ���ӵ�SimuService��������
 ****************************************************************/
class CSimuL_SConnectTargetCmd : public CSimuL_SCmd
{
public:
	CSimuL_SConnectTargetCmd();
	virtual ~CSimuL_SConnectTargetCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer); /* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen); /* �����л���������� */
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
 * ���� : CSimuL_SConnectTargetRespCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ���ӵ�SimuService��Ӧ����
 ****************************************************************/
class CSimuL_SConnectTargetRespCmd : public CSimuL_SCmd
{
public:
	CSimuL_SConnectTargetRespCmd();
	virtual ~CSimuL_SConnectTargetRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);	/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	/* �����л���������� */
	virtual void Print(char* szBuffer);

	void SetResult(unsigned long ret);
	unsigned long GetResult();	
																																								/* ��ȡ���ӽ�� */
	void SetErrInfo(unsigned char *szErrInfo, unsigned long nErrLen);
	int  GetErrInfo(unsigned char *szBuffer);
protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long	m_ulResult;		
	unsigned long   m_ulErrInfoLen;																																					/* ���ӽ�� */
	unsigned char   m_szErrInfo[1024];
};

/****************************************************************
 * ���� : CSimuL_SLoadSimuModelCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ���ط���ģ��
 ****************************************************************/
class CSimuL_SLoadSimuModelCmd : public CSimuL_SCmd
{
public:
	CSimuL_SLoadSimuModelCmd();
	virtual ~CSimuL_SLoadSimuModelCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);		/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen); /* �����л���������� */
	virtual void Print(char* szBuffer);

	void SetParam(const CommuLoadSimuModelParam_t *pParam);
	const CommuLoadSimuModelParam_t *GetParam();

protected:
	virtual unsigned long CalcFrameBodyLen();

	CommuLoadSimuModelParam_t	m_CommuParam;
};

/****************************************************************
 * ���� : CSimuL_SLoadSimuModelRespCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ���ط���ģ����Ӧ
 ****************************************************************/
class CSimuL_SLoadSimuModelRespCmd : public CSimuL_SCmd
{
public:
	CSimuL_SLoadSimuModelRespCmd();
	virtual ~CSimuL_SLoadSimuModelRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);	/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	/* �����л���������� */
	virtual void Print(char* szBuffer);

	unsigned long GetResult();	
	void SetResult(unsigned long nRet);

	void SetErrInfo(unsigned char *szErrInfo, unsigned long nErrLen);
	int  GetErrInfo(unsigned char *szBuffer);
protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long	m_ulResult;		
	unsigned long   m_ulErrInfoLen;																																					/* ���ӽ�� */
	unsigned char   m_szErrInfo[1024];																																						/* �Ͽ����ӽ�� */
};

/****************************************************************
 * ���� : CSimuL_SEnableSimuMonitorCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ʹ�ܼ������
 ****************************************************************/
class CSimuL_SEnableSimuMonitorCmd : public CSimuL_SCmd
{
public:
	CSimuL_SEnableSimuMonitorCmd();
	virtual ~CSimuL_SEnableSimuMonitorCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetFlag(unsigned long ulFlag);
	unsigned long GetFlag();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long  m_ulEnableFlag;
};

/****************************************************************
 * ���� : CSimuL_SEnableSimuMonitorRespCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ʹ�ܼ��������Ӧ
 ****************************************************************/
class CSimuL_SEnableSimuMonitorRespCmd : public CSimuL_SCmd
{
public:
	CSimuL_SEnableSimuMonitorRespCmd();
	virtual ~CSimuL_SEnableSimuMonitorRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
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
 * ���� : CSimuL_SCompileSimuModelCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ����ģ������
 ****************************************************************/
class CSimuL_SCompileSimuModelCmd : public CSimuL_SCmd
{
public:
	CSimuL_SCompileSimuModelCmd();
	virtual ~CSimuL_SCompileSimuModelCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	int SetCompileParam(const CommuModelCompileParam_t *pParam);
	int GetCompileParam(CommuModelCompileParam_t *pParam);

protected:
	virtual unsigned long CalcFrameBodyLen();
	CommuModelCompileParam_t m_CompileParam;
};

/****************************************************************
 * ���� : CSimuL_SCompileSimuModelRespCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ����ģ��������Ӧ
 ****************************************************************/
class CSimuL_SCompileSimuModelRespCmd : public CSimuL_SCmd
{
public:
	CSimuL_SCompileSimuModelRespCmd();
	virtual ~CSimuL_SCompileSimuModelRespCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
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
 * ���� : CSimuL_SCompileInfoRepCmd
 * ���� : CSimuL_SCmd
 * ˵�� : ������Ϣ�ϱ�����
 ****************************************************************/
class CSimuL_SCompileInfoRepCmd : public CSimuL_SCmd
{
public:
	CSimuL_SCompileInfoRepCmd();
	virtual ~CSimuL_SCompileInfoRepCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
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