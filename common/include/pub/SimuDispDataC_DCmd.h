/*****************************************************************
 * �ļ���: SimuDispDataC_DCmd.h
 * ����: SimuService,SimuGuard��SimuMonitor֮��ͨ�������
 *****************************************************************/
#pragma  once

#ifndef WIN_PLATFORM
#include "stdafx.h"
#endif

#include <vector>
#include "SimuCommunicationDef.h"

using namespace std;

/****************************************************************
 * ö���� : DispDataC_D_CMD
 * ˵�� : SimuService,SimuGuard��SimuMonitor֮��ͨ�������ֶ���
 ****************************************************************/
typedef enum euDispDataC_D_CMD
{
	DDATA_C_D_CMD_UNKNOWN			= -1 ,		 /* δ֪����*/
	DDATA_C_D_CMD_DISP_SLPRINT		= 0x50000001,/* SLPRINT�����Ϣ */
	DDATA_C_D_CMD_DISP_SIMUDATA		= 0x50000002,/* ��ʾ��ģ������ */
	DDATA_C_D_CMD_DISP_SIMUSTATUS	= 0x50000003,/* ��ʾ��״̬���� */
	
} DispDataC_D_CMD;

/*BEGIN  SimuService,SimuGuard��SimuMonitor֮��ͨ������֡���ֶ���֡��������λ���Լ���С���� */
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
/*END  SimuService,SimuGuard��SimuMonitor֮��ͨ������֡���ֶ���֡��������λ���Լ���С���� */

#define DDATA_MAX_SLPRINT_LEN			1024
#define DDATA_MAX_SIMUDATA_LEN			2048

/****************************************************************
 * ���� : CSimuDispDataC_DCmd
 * ���� : ��
 * ˵�� : SimuService,SimuGuard��SimuMonitor֮��ͨ�ŵ���������ĸ���
 ****************************************************************/
class CSimuDispDataC_DCmd
{
public:
	CSimuDispDataC_DCmd(DispDataC_D_CMD sgc);
	virtual ~CSimuDispDataC_DCmd();

	/* Ԥ�������� , �����Ӧ������ */
	static DispDataC_D_CMD PreParseCmd(const unsigned char* szBuffer , unsigned long nBufferLen , unsigned long& nCmdDataLen);	
	
	/* ��ȡ����ֵ */
	DispDataC_D_CMD GetCmdType() const;	
				
	/* ��ȡ����֡���� */
	unsigned long GetFrameLen();	

	/* ��ȡ����֡�峤�� */
	unsigned long GetFrameBodyLen();	

	/* ���л����� */
	virtual unsigned long Serialize(unsigned char* szBuffer);		
	
	/* �����л���������� */
	virtual BOOL Unserialize(const unsigned char* szBuffer , unsigned long nBufferLen);	
	
	/* ��ӡ��� */
	virtual void Print(char* szBuffer);

	/* ����ģ��ID */
	void SetModelID(unsigned short id);
	
	/* ��ȡģ��ID */
	unsigned short GetModelID();
	
	/* ����ʱ��� */
	void SetTimeStamp(unsigned long ts);
	
	/* ����ʱ��� */
	unsigned long GetTimeStamp();

protected:
	unsigned long CalcFrameLen();
	virtual unsigned long CalcFrameBodyLen() = 0;

	DispDataC_D_CMD		m_sgc;	/* ������ */

	unsigned char		m_szSign[DDATA_C_D_CMD_SIZE_SIGN];
	unsigned long		m_nCmd;				/* ������ */
	unsigned long		m_nFrameBodyLen;	/* ����֡�峤�� */
	
	unsigned long		m_nTimeStamp;		/* Unixʱ�� */
	unsigned short		m_usModelID;		/* ģ��ID */																								/* ������ֶ�4 */
};

/****************************************************************
 * ���� : CSimuDispDataC_DSLPrintCmd
 * ���� : CSimuDispDataC_DCmd
 * ˵�� : SLPrint��ӡ����
 ****************************************************************/
class CSimuDispDataC_DSLPrintCmd : public CSimuDispDataC_DCmd
{
public:
	CSimuDispDataC_DSLPrintCmd();
	virtual ~CSimuDispDataC_DSLPrintCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(const unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetStep(unsigned long nStep);
	unsigned long GetStep();

	void SetData(const unsigned char *szData, unsigned long nDataLen);
	unsigned long GetData(unsigned char *szBuff, unsigned long nBuffLen);
	unsigned long GetDataLen();     /* �������ݳ��� */
protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_nStep;
	unsigned long m_nDataLen;
	unsigned char m_szData[DDATA_MAX_SLPRINT_LEN]; /* �ַ��� */
};

/****************************************************************
 * ���� : CSimuDispDataC_DSimuDataCmd
 * ���� : CSimuDispDataC_DCmd
 * ˵�� : ��ʾ�ķ���ģ������
 ****************************************************************/
class CSimuDispDataC_DSimuDataCmd : public CSimuDispDataC_DCmd
{
public:
	CSimuDispDataC_DSimuDataCmd();
	virtual ~CSimuDispDataC_DSimuDataCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(const unsigned char* szBuffer , unsigned long nBufferLen);	
	virtual void Print(char* szBuffer);

	void SetStep(unsigned long nStep);
	unsigned long GetStep();

	void SetData(const unsigned char *szData, unsigned long nDataLen);
	unsigned long GetData(unsigned char *szBuff, unsigned long nBuffLen);
	unsigned long GetDataLen(); /* �������ݳ��� */
	unsigned char *GetData();
protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_nStep;
	unsigned long m_nDataLen;
	unsigned char m_szData[DDATA_MAX_SIMUDATA_LEN];
};

/****************************************************************
 * ���� : CSimuDispDataC_DSimuStatusCmd
 * ���� : CSimuDispDataC_DCmd
 * ˵�� : ��ʾ�ķ���״̬
 ****************************************************************/
class CSimuDispDataC_DSimuStatusCmd : public CSimuDispDataC_DCmd
{
public:
	CSimuDispDataC_DSimuStatusCmd();
	virtual ~CSimuDispDataC_DSimuStatusCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
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
