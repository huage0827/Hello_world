/*****************************************************************
 * �ļ���: SimuStorDataC_DCmd.h
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
 * ö���� : StorDataC_D_CMD
 * ˵�� : SimuService,SimuGuard��SimuMonitor֮��ͨ�������ֶ���
 ****************************************************************/
typedef enum euStorDataC_D_CMD
{
	SDATA_C_D_CMD_UNKNOWN		= -1 ,		 /* δ֪����*/
	SDATA_C_D_CMD_STOR_SIMUDATA	= 0x60000001,/* �洢���ݴ�������*/
} StorDataC_D_CMD;

typedef enum euStorDataC_D_Type{

	SDATA_C_D_Type_ModelData = 1,	/* ģ������ */
	SDATA_C_D_Type_UserData  = 2,	/* �û����� */
} StorDataC_D_Type;


#define SDATA_C_D_CMD_HEADER_SIZE		 8
#define SDATA_C_D_CMD_MIN_UNIT_DATA_SIZE 18


/****************************************************************
 * ���� : CSimuStorDataC_DCmd
 * ���� : ��
 * ˵�� : SimuService,SimuGuard��SimuMonitor֮��洢���������
 ****************************************************************/
class CSimuStorDataC_DCmd
{
public:
	CSimuStorDataC_DCmd();
	virtual ~CSimuStorDataC_DCmd();

	/* ����ģ��ID */
	void SetModelID(unsigned short id);
	
	/* ��ȡģ��ID */
	unsigned short GetModelID();

	/* ��ȡ����*/
	unsigned long GetCommand();

	/* ��ȡ���ݸ��� */
	unsigned short GetDataCount();

	/* ���л����� */
	unsigned long Serialize(unsigned char* szBuffer);		
	
	/* �����л���������� */
	BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen, unsigned long &nCmdLen);	
	
	/* ��ӡ��� */
	void Print(char* szBuffer);

	/* ����һ���û����� */
	void AddUserData(unsigned long nStep,const char *szFile, unsigned long ts1, unsigned long ts2, unsigned char *pData, unsigned long nDataLen);

	/* ����һ��ģ������*/
	void AddModelData(unsigned long nStep,unsigned long nGroupIdx, unsigned long ts1, unsigned long ts2, unsigned char *pData, unsigned long nDataLen);

	/* ����һ������*/
	void AddData(const vector<CommuStorModelDataUnit_t *> *pVecData);

	/* �������� */
	const vector<CommuStorModelDataUnit_t *> *GetData();

	/* ��ȡ���� */
	void GetData(vector<CommuStorModelDataUnit_t *> *pVecData);

protected:
	unsigned long		m_nCommand; /*������*/
	unsigned short		m_usModelID;/*ģ��ID*/
	unsigned short		m_usDataCount;/*������Ŀ*/

	vector<CommuStorModelDataUnit_t *> m_vecStorData;/*�洢�����б�*/
};
