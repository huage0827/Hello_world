/*****************************************************************
 * �ļ���: SimuD_SCmdParser.h
 * ����: SimuMonitor��SimuService֮��ͨ������ִ�кͽ���������
 *****************************************************************/
#pragma once

#include <vector>
#include "SimuCommAPI.h"
#include "SimuD_SCmdExecutorInterface.h"

using namespace std;

/****************************************************************
 * ���� : CSimuD_SCmdParser
 * ���� : ��
 * ˵�� : SimuMonitor��SimuService֮��ͨ�����������
 ****************************************************************/
class CSimuD_SCmdParser
{
	/*��������߳�*/
	friend DWORD WINAPI SimuD_SCmdParseThread(LPVOID lpParam);
public :
	/*���캯��*/
	CSimuD_SCmdParser(CSimuD_SCmdExecutorInferface* pExecutorInterface);
	/*��������*/
	virtual ~CSimuD_SCmdParser();

	/* ��ʼ����������� */
	BOOL Initialize();																					
	
	/* ж��������������ͷ���Դ */
	void Uninitialize();
	
	/* ��ָ�������ݽ���������ִ������ִ�е����� */
	BOOL Parse(unsigned char* szData , unsigned long nDataLen);	

protected:

	/* ִ�������ѽ����ɹ������������������ĸ����̵߳��� */
	BOOL ExecAllCmd();																					

private:

	BOOL									m_bInitialize;	/* �Ƿ��ѳ�ʼ�� */
	vector<CSimuD_SCmd*>					m_vecCmd;	    /* ����� */
	CRITICAL_SECTION						m_criCmd;		/* ����������ٽ��� */
	unsigned char							*m_szBuffer;	/* ���͵�����������е����ݻ����� */
	unsigned long							m_nBufferLen;	/* ���͵�����������е����ݻ��������� */
	HANDLE									m_hParseThread;	/* ��������������̵߳ľ�� */
	HANDLE									m_hParseEvent;	/* ֹͣ��������������̵߳��¼���� */
	CSimuD_SCmdExecutorInferface*			m_pCmdExecutorInterface; /* ��Ӧ������ִ���� */
};