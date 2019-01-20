/*****************************************************************
 * �ļ���: SimuM_CCmd.h
 * ����: SimuService��SimuGuard��SimuModel֮��ͨ�������
 *****************************************************************/
#pragma  once

#ifndef WIN_PLATFORM
#include "stdafx.h"
#endif

#include <vector>
#include "SimuCommunicationDef.h"

using namespace std;

/****************************************************************
 * ö���� : M_C_CMD
 * ˵�� : SimuService��SimuGuard��SimuModel֮��ͨ�������ֶ���
 ****************************************************************/
typedef enum euM_C_CMD
{
	M_C_CMD_Unknown				= -1 ,		  /* δ֪����*/
	M_C_CMD_ExecSimuModel		= 0x30000001, /* C->M ִ�з���ģ������ */
	M_C_CMD_PauseSimuModel		= 0x30000002, /* C->M ��ͣ����ģ������ */
	M_C_CMD_ResetSimuModel		= 0x30000003, /* C->M ���÷���ģ������ */
	M_C_CMD_SetModelParam		= 0x30000004, /* C->M ����ģ�Ͳ������� */

	M_C_CMD_RegisterModel		= 0xB0000001, /* M->C ע�����ģ������ */
	M_C_CMD_TestEventReport		= 0xB0000002, /* M->C �����¼��ϱ� */
	M_C_CMD_ExceptEventReport	= 0xB0000003, /* M->C �쳣�ϱ� */
	M_C_CMD_DispModelData		= 0xB0000004, /* M->C ��ʾģ������*/
//	M_C_CMD_DispStatusData		= 0xB0000005, /* M->C ��ʾ״̬����*/
	M_C_CMD_DispUserData		= 0xB0000006, /* M->C ��ʾ�û�����*/
	M_C_CMD_StorModelData		= 0xB0000007, /* M->C �洢ģ������*/
	M_C_CMD_StorUserData		= 0xB0000008, /* M->C �洢�û�����*/
	M_C_CMD_DispAndStorData		= 0xB0000009,
	M_C_CMD_NonRealTimeRunAdust = 0xB000000A, /* M->C ��ʵʱ����΢��*/
	M_C_CMD_BUFFERFULL			= 0xB000000B,
	M_C_CMD_UserPauseModel      = 0xB000000C, /* M->C �û���ͣģ��*/
	M_C_CMD_UserStopModel       = 0xB000000D, /* M->C �û�ֹͣģ��*/
} M_C_CMD;

#if 0

/*BEGIN  SimuService��SimuGuard��SimuModel֮��ͨ������֡���ֶ���֡��������λ���Լ���С���� */
#define M_C_CMD_POS_COMMAND				0
#define M_C_CMD_POS_SIMUMODELID			4
#define M_C_CMD_POS_FRAMELEN			8
#define M_C_CMD_POS_FRAMEBODY			12

#define M_C_CMD_HEADER_SIZE				12

#define M_C_CMD_SIZE_COMMAND			4
#define M_C_CMD_SIZE_SIMUMODELID		4
#define M_C_CMD_SIZE_FRAMELEN			4
/*END  SimuService��SimuGuard��SimuModel֮��ͨ������֡���ֶ���֡��������λ���Լ���С���� */

/****************************************************************
 * ���� : CSimuM_CCmd
 * ���� : ��
 * ˵�� : SimuService��SimuGuard��SimuModel֮��ͨ�ŵ���������ĸ���
 ****************************************************************/
class CSimuM_CCmd
{
public:
	CSimuM_CCmd(M_C_CMD sgc);
	virtual ~CSimuM_CCmd();

	/* Ԥ�������� , �����Ӧ������ */
	static M_C_CMD PreParseCmd(unsigned char* szBuffer , unsigned long nBufferLen , unsigned long& nCmdDataLen);	
	
	/* ��ȡ����ֵ */
	M_C_CMD GetCmdType() const;	
	
	/* ��ȡ����֡���� */
	unsigned long GetFrameLen();		
	
	/* ��ȡ����֡�峤�� */
	unsigned long GetFrameBodyLen();
	
	/* ����ģ��ID */
	void SetSimuModelID(unsigned long nModelId);

	/* ��ȡģ��ID */
	unsigned long GetSimuModelID();

	/* ���л����� */
	virtual unsigned long Serialize(unsigned char* szBuffer);
	
	/* �����л���������� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);

	/* �����ӡ */
	virtual void Print(char* szBuffer); 

protected:
	unsigned long CalcFrameLen();
	virtual unsigned long CalcFrameBodyLen() = 0;

	M_C_CMD		   m_sgc;																																													/* ������ */
	unsigned long  m_nCmd;																																										/* ������ */
	unsigned long  m_nFrameLen;																																					/* ����֡�峤�� */
	unsigned long  m_nModelId;																								/* ������ֶ�4 */
};

/****************************************************************
 * ���� : CSimuM_CExecSimuModelCmd
 * ���� : CSimuM_CCmd
 * ˵�� : 1,ִ�з���ģ��
 ****************************************************************/
class CSimuM_CExecSimuModelCmd : public CSimuM_CCmd
{
public:
	CSimuM_CExecSimuModelCmd();
	virtual ~CSimuM_CExecSimuModelCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* �����л���������� */
	virtual void Print(char* szBuffer); 

	void SetParam(int nExecMode);
	unsigned long GetParam();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long  m_nExecMode;
};

/****************************************************************
 * ���� : CSimuM_CPauseSimuModelCmd
 * ���� : CSimuM_CCmd
 * ˵�� : 2,��ͣ����ģ��
 ****************************************************************/
class CSimuM_CPauseSimuModelCmd : public CSimuM_CCmd
{
public:
	CSimuM_CPauseSimuModelCmd();
	virtual ~CSimuM_CPauseSimuModelCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* �����л���������� */
	virtual void Print(char* szBuffer); 

protected:
	virtual unsigned long CalcFrameBodyLen();

};

/****************************************************************
 * ���� : CSimuM_CResetSimuModelCmd
 * ���� : CSimuM_CCmd
 * ˵�� : 3,���÷���ģ��
 ****************************************************************/
class CSimuM_CResetSimuModelCmd : public CSimuM_CCmd
{
public:
	CSimuM_CResetSimuModelCmd();
	virtual ~CSimuM_CResetSimuModelCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* �����л���������� */
	virtual void Print(char* szBuffer); 

protected:
	virtual unsigned long CalcFrameBodyLen();

};

/****************************************************************
 * ���� : CSimuM_CSetSimuModelParamCmd
 * ���� : CSimuM_CCmd
 * ˵�� : 4,���÷���ģ�Ͳ���
 ****************************************************************/
class CSimuM_CSetSimuModelParamCmd : public CSimuM_CCmd
{
public:
	CSimuM_CSetSimuModelParamCmd();
	virtual ~CSimuM_CSetSimuModelParamCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* �����л���������� */
	virtual void Print(char* szBuffer); 

	void SetModelParam(vector<CommuSetModelParam_t *> &vecModelParam);
	const vector<CommuSetModelParam_t *> *GetModelParam();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long		m_nModelParamCount;
	vector<CommuSetModelParam_t*>	m_vecModelParam;
};

/////////////////////////////////////////////////////////////////////////////////////

/****************************************************************
 * ���� : CSimuM_CRegisterSimuModelCmd
 * ���� : CSimuM_CCmd
 * ˵�� : 1,ע�����ģ����������
 ****************************************************************/
class CSimuM_CRegisterSimuModelCmd : public CSimuM_CCmd
{
public:
	CSimuM_CRegisterSimuModelCmd();
	virtual ~CSimuM_CRegisterSimuModelCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* �����л���������� */
	virtual void Print(char* szBuffer);

protected:
	virtual unsigned long CalcFrameBodyLen();
};

/****************************************************************
 * ���� : CSimuM_CTestEventReportCmd
 * ���� : CSimuM_CCmd
 * ˵�� : 2,�����¼��ϱ�
 ****************************************************************/
class CSimuM_CTestEventReportCmd : public CSimuM_CCmd
{
public:
	CSimuM_CTestEventReportCmd();
	virtual ~CSimuM_CTestEventReportCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* �����л���������� */
	virtual void Print(char* szBuffer); 

	void SetReport(CommuTestEventReport_t *pReport);
	void SetReport(unsigned long nType, unsigned long nTaskId, unsigned long nCaseId, const char *szReport);
	const CommuTestEventReport_t *GetReport();

protected:
	virtual unsigned long CalcFrameBodyLen();

	CommuTestEventReport_t	m_Report;
};

/****************************************************************
 * ���� : CSimuM_CExceptEventReportCmd
 * ���� : CSimuM_CCmd
 * ˵�� : 3,�쳣�¼��ϱ�
 ****************************************************************/
class CSimuM_CExceptEventReportCmd : public CSimuM_CCmd
{
public:
	CSimuM_CExceptEventReportCmd();
	virtual ~CSimuM_CExceptEventReportCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* �����л���������� */
	virtual void Print(char* szBuffer); 

	void SetReport(CommuExceptReport_t *pReport);
	void SetReport(unsigned long ReportType, const char *szReport);
	const CommuExceptReport_t *GetReport();

protected:
	virtual unsigned long CalcFrameBodyLen();

	CommuExceptReport_t m_Report;
};

/****************************************************************
 * ���� : CSimuM_CDispModelDataCmd
 * ���� : CSimuM_CCmd
 * ˵�� : 4,��ʾģ������
 ****************************************************************/
class CSimuM_CDispModelDataCmd : public CSimuM_CCmd
{
public:
	CSimuM_CDispModelDataCmd();
	virtual ~CSimuM_CDispModelDataCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* �����л���������� */
	virtual void Print(char* szBuffer); 

	void SetData(unsigned long step, const char *pData, unsigned long nDataLen);
	void GetData(unsigned long *step, char *pData, unsigned long *nDataLen);

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulStep;
	unsigned long m_ulDataLen;
	char *m_pData;
};

/****************************************************************
 * ���� : CSimuM_CDispStatusDataCmd
 * ���� : CSimuM_CCmd
 * ˵�� : 5,��ʾ״̬���� 
 ****************************************************************/
class CSimuM_CDispStatusDataCmd : public CSimuM_CCmd
{
public:
	CSimuM_CDispStatusDataCmd();
	virtual ~CSimuM_CDispStatusDataCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* �����л���������� */
	virtual void Print(char* szBuffer); 

	void SetData(unsigned long step, unsigned long nOverRun, double execTime);
	void GetData(unsigned long *step, unsigned long *nOverRun, double *execTime);

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulStep;
	unsigned long m_ulOverRun;
	double m_execTime;
};

/****************************************************************
 * ���� : CSimuM_CDispUserDataCmd
 * ���� : CSimuM_CCmd
 * ˵�� : 6,��ʾ�û�����	
 ****************************************************************/
class CSimuM_CDispUserDataCmd : public CSimuM_CCmd
{
public:
	CSimuM_CDispUserDataCmd();
	virtual ~CSimuM_CDispUserDataCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* �����л���������� */
	virtual void Print(char* szBuffer); 

	void SetData(unsigned long step, const char *pData, unsigned long nDataLen);
	void GetData(unsigned long *step, char *pData, unsigned long *nDataLen);

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulStep;
	unsigned long m_ulDataLen;
	char *m_pData;
};

/****************************************************************
 * ���� : CSimuM_CStorModelDataCmd
 * ���� : CSimuM_CCmd
 * ˵�� : 7,�洢ģ������	
 ****************************************************************/
class CSimuM_CStorModelDataCmd : public CSimuM_CCmd
{
public:
	CSimuM_CStorModelDataCmd();
	virtual ~CSimuM_CStorModelDataCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* �����л���������� */
	virtual void Print(char* szBuffer); 

	void SetData(const CommuStorModelDataUnit_t *pData);
	void GetData(CommuStorModelDataUnit_t *pData);

protected:
	virtual unsigned long CalcFrameBodyLen();

	CommuStorModelDataUnit_t *m_pStorData;
};

/****************************************************************
 * ���� : CSimuM_CStorUserDataCmd
 * ���� : CSimuM_CCmd
 * ˵�� : 8,�洢�û�����	
 ****************************************************************/
class CSimuM_CStorUserDataCmd : public CSimuM_CCmd
{
public:
	CSimuM_CStorUserDataCmd();
	virtual ~CSimuM_CStorUserDataCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* �����л���������� */
	virtual void Print(char* szBuffer); 

	void SetData(const CommuStorModelDataUnit_t *pData);
	void GetData(CommuStorModelDataUnit_t *pData);

protected:
	virtual unsigned long CalcFrameBodyLen();

	CommuStorModelDataUnit_t *m_pStorData;
};

/****************************************************************
 * ���� : CSimuM_CNonRealTimeRunAdjustCmd
 * ���� : CSimuM_CCmd
 * ˵�� : 9,��ʵʱ����΢��	
 ****************************************************************/
class CSimuM_CNonRealTimeRunAdjustCmd : public CSimuM_CCmd
{
public:
	CSimuM_CNonRealTimeRunAdjustCmd();
	virtual ~CSimuM_CNonRealTimeRunAdjustCmd();

	virtual unsigned long Serialize(unsigned char* szBuffer);																											/* ���л����� */
	virtual BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen);																	/* �����л���������� */
	virtual void Print(char* szBuffer); 

	void SetData(unsigned long nAdjust);
	unsigned long GetData();

protected:
	virtual unsigned long CalcFrameBodyLen();

	unsigned long m_ulAdjust;
};

#endif
