/*****************************************************************
 * 文件名: SimuStorDataC_DCmd.h
 * 功能: SimuService,SimuGuard与SimuMonitor之间通信命令实现
 *****************************************************************/
#include "stdafx.h"
#include "SimuStorDataC_DCmd.h"
#include "Util.h"
#include <string.h>

using namespace std;

CSimuStorDataC_DCmd::CSimuStorDataC_DCmd()
{
	m_nCommand = SDATA_C_D_CMD_STOR_SIMUDATA;
	m_usModelID = 0;
	m_usDataCount = 0;
}

CSimuStorDataC_DCmd::~CSimuStorDataC_DCmd()
{
	CommuStorModelDataUnit_t *pUnit = NULL;
	for( unsigned int i = 0; i < m_vecStorData.size(); i++){
		pUnit = m_vecStorData.at(i);
		delete []pUnit->pData;
		delete pUnit;
	}
}

void CSimuStorDataC_DCmd::SetModelID(unsigned short id)
{
	m_usModelID = id;
}

unsigned short CSimuStorDataC_DCmd::GetModelID()
{
	return m_usModelID;
}

unsigned short CSimuStorDataC_DCmd::GetDataCount()
{
	return m_usDataCount;
}
unsigned long CSimuStorDataC_DCmd::GetCommand()
{
	return m_nCommand;
}

unsigned long CSimuStorDataC_DCmd::Serialize(unsigned char* szBuffer)
{
	unsigned nLen = 0;
	CommuStorModelDataUnit_t *pUnit = NULL;

	m_usDataCount = m_vecStorData.size();

	memcpy(szBuffer + nLen, &m_nCommand, 4);
	nLen += 4;
	memcpy(szBuffer + nLen, &m_usModelID, 2);
	nLen += 2;
	memcpy(szBuffer + nLen, &m_usDataCount, 2);
	nLen += 2;
	
	for( unsigned int i = 0; i < m_vecStorData.size(); i++){
		pUnit = m_vecStorData.at(i);

		memcpy(szBuffer + nLen, &pUnit->nStep, 4);
		nLen += 4;
		memcpy(szBuffer + nLen, &pUnit->bDataType, 1);
		nLen += 1;
		if( pUnit->bDataType == SDATA_C_D_Type_ModelData){
			memcpy(szBuffer +nLen, &pUnit->u.nGroupIdx, 4);
			nLen += 4;
		}else{
			unsigned long nRecordLen = strlen(pUnit->u.szRecordFile);
			memcpy(szBuffer + nLen, &nRecordLen, 4);
			nLen += 4;
			memcpy(szBuffer + nLen, pUnit->u.szRecordFile, nRecordLen);
			nLen += nRecordLen;
		}
		memcpy(szBuffer + nLen, &pUnit->nTimeStampSec, 4);
		nLen += 4;
		memcpy(szBuffer + nLen, &pUnit->nTimeStampMsec, 4);
		nLen += 4;
		memcpy(szBuffer + nLen, &pUnit->nDataLen, 4);
		nLen += 4;
		memcpy(szBuffer + nLen, pUnit->pData, pUnit->nDataLen);
		nLen += pUnit->nDataLen;
	}
	return nLen;
}

BOOL CSimuStorDataC_DCmd::Unserialize(unsigned char* szBuffer , unsigned long nBufferLen , unsigned long &nCmdLen)
{
	if( nBufferLen <  8 + 18)
		return FALSE;
	nCmdLen = 0;
	CommuStorModelDataUnit_t *pUnit = NULL;
	unsigned int nPos = 0;

	nPos = 0;
	memcpy(&m_nCommand, szBuffer + nPos, 4);
	nPos += 4;
	memcpy(&m_usModelID, szBuffer + nPos, 2);
	nPos += 2;
	memcpy(&m_usDataCount, szBuffer + nPos, 2);
	nPos += 2;

	if( m_nCommand != SDATA_C_D_CMD_STOR_SIMUDATA){
		return FALSE;
	}

	for( unsigned int i = 0; i < m_usDataCount; i++){
		pUnit = new CommuStorModelDataUnit_t;

		////////
		if( nPos + 10 > nBufferLen){
			delete pUnit;
			return FALSE;
		}
		////////

		memcpy(&pUnit->nStep, szBuffer + nPos, 4);
		nPos += 4;
		memcpy(&pUnit->bDataType, szBuffer + nPos, 1);
		nPos += 1;
		if( pUnit->bDataType == SDATA_C_D_Type_ModelData){
			memcpy(&pUnit->u.nGroupIdx, szBuffer + nPos, 4);
			nPos += 4;
		}else{
			unsigned long nRecordLen = 0;
			memset(pUnit->u.szRecordFile, 0x0, sizeof(pUnit->u.szRecordFile));

			memcpy(&nRecordLen, szBuffer + nPos, 4);
			nPos += 4;

			////////
			if( nPos + nRecordLen > nBufferLen){
				delete pUnit;
				return FALSE;
			}
			////////

			memcpy(pUnit->u.szRecordFile, szBuffer + nPos, nRecordLen);
			nPos += nRecordLen;
		}
		memcpy(&pUnit->nTimeStampSec, szBuffer + nPos, 4);
		nPos += 4;
		memcpy(&pUnit->nTimeStampMsec, szBuffer + nPos, 4);
		nPos += 4;
		
		////////
		if( nPos + 5 > nBufferLen){
			delete pUnit;
			return FALSE;
		}
		////////
		
		memcpy(&pUnit->nDataLen, szBuffer + nPos, 4);
		nPos += 4;
		if( pUnit->nDataLen > 1024 * 10){
			delete pUnit;
			return FALSE;
		}

		////////
		if( nPos + pUnit->nDataLen > nBufferLen){
			delete pUnit;
			return FALSE;
		}
		////////

		pUnit->pData = new unsigned char[pUnit->nDataLen + 1];
		memcpy(pUnit->pData, szBuffer + nPos, pUnit->nDataLen);
		pUnit->pData[pUnit->nDataLen] = 0;

		nPos += pUnit->nDataLen;
		m_vecStorData.push_back(pUnit);
	}

	nCmdLen = nPos;
	return TRUE;
}

void CSimuStorDataC_DCmd::Print(char* szBuffer)
{
	char szTmp[256];
	CommuStorModelDataUnit_t *pUnit = NULL; 

	szBuffer[0] = 0;

	sprintf(szTmp , "SimuStorData-ModelID:%d, DataCount:%d \n",
			m_usModelID, m_usDataCount);
	strcat(szBuffer, szTmp);

	for( unsigned int i = 0; i < m_vecStorData.size(); i++){
		pUnit = m_vecStorData.at(i);

		sprintf(szTmp , "step:%d, type:%d, timestamp:%d-%d, nDataLen:%d\n",
			pUnit->nStep, pUnit->bDataType, pUnit->nTimeStampSec,
			pUnit->nTimeStampMsec, pUnit->nDataLen);
		strcat(szBuffer, szTmp);

		if( pUnit->bDataType == SDATA_C_D_Type_UserData){
			sprintf(szTmp , "Data:%s \n",pUnit->pData);
			strcat(szBuffer, szTmp);
		}
	}
	return ;
}

//void CSimuStorDataC_DCmd::AddData(unsigned long nStep, unsigned char bType,  unsigned long ts1, unsigned long ts2, unsigned char *pData, unsigned long nDataLen)
//{
//	CommuStorModelDataUnit_t *pUnit = NULL;
//
//	pUnit = new CommuStorModelDataUnit_t;
//	pUnit->nStep = nStep;
//	pUnit->bDataType = bType;
//	pUnit->nTimeStampSec = ts1;
//	pUnit->nTimeStampMsec = ts2;
//	pUnit->nDataLen = nDataLen;
//	pUnit->pData = new unsigned char[pUnit->nDataLen + 1];
//	memcpy(pUnit->pData, pData, nDataLen);
//	pUnit->pData[nDataLen] = 0;
//
//	m_vecStorData.push_back(pUnit);
//	m_usDataCount = m_vecStorData.size();
//}

void CSimuStorDataC_DCmd::AddUserData(unsigned long nStep,const char *szFile, unsigned long ts1, unsigned long ts2, unsigned char *pData, unsigned long nDataLen)
{
	CommuStorModelDataUnit_t *pNewUnit = new CommuStorModelDataUnit_t;
	pNewUnit->nStep = nStep;
	pNewUnit->bDataType = SDATA_C_D_Type_UserData;
	strcpy(pNewUnit->u.szRecordFile, szFile);
	pNewUnit->nTimeStampSec = ts1;
	pNewUnit->nTimeStampMsec = ts2;
	pNewUnit->nDataLen = nDataLen;
	pNewUnit->pData = new unsigned char[pNewUnit->nDataLen];
	memcpy(pNewUnit->pData, pData, nDataLen);
	
	m_vecStorData.push_back(pNewUnit);	
}

void CSimuStorDataC_DCmd::AddModelData(unsigned long nStep,unsigned long nGroupIdx, unsigned long ts1, unsigned long ts2, unsigned char *pData, unsigned long nDataLen)
{
	CommuStorModelDataUnit_t *pNewUnit = new CommuStorModelDataUnit_t;
	pNewUnit->nStep = nStep;
	pNewUnit->bDataType = SDATA_C_D_Type_ModelData;
	pNewUnit->u.nGroupIdx = nGroupIdx;
	pNewUnit->nTimeStampSec = ts1;
	pNewUnit->nTimeStampMsec = ts2;
	pNewUnit->nDataLen = nDataLen;
	pNewUnit->pData = new unsigned char[pNewUnit->nDataLen];
	memcpy(pNewUnit->pData, pData, nDataLen);

	m_vecStorData.push_back(pNewUnit);	
}

//void CSimuStorDataC_DCmd::AddData(CommuStorModelDataUnit_t *pUnit)
//{
//	CommuStorModelDataUnit_t *pNewUnit = NULL;
//	pNewUnit = new CommuStorModelDataUnit_t;
//	pNewUnit->nStep = pUnit->nStep;
//	pNewUnit->bDataType = pUnit->bDataType;
//	if( pUnit->bDataType == SDATA_C_D_Type_ModelData){
//		pNewUnit->u.nGroupIdx = pUnit->u.nGroupIdx;
//	}else{
//		strcpy(pNewUnit->u.szRecordFile, pUnit->u.szRecordFile);
//	}
//	pNewUnit->nTimeStampSec = pUnit->nTimeStampSec;
//	pNewUnit->nTimeStampMsec = pUnit->nTimeStampMsec;
//	pNewUnit->nDataLen = pUnit->nDataLen;
//	pNewUnit->pData = new unsigned char[pNewUnit->nDataLen+1];
//	memcpy(pNewUnit->pData, pUnit->pData, pUnit->nDataLen);
//	pNewUnit->pData[pUnit->nDataLen] = 0;
//
//	m_vecStorData.push_back(pNewUnit);
//}

void CSimuStorDataC_DCmd::AddData(const vector<CommuStorModelDataUnit_t *> *pVecData)
{
	CommuStorModelDataUnit_t *pUnit = NULL;
	CommuStorModelDataUnit_t *pNewUnit = NULL;

	for( unsigned int i = 0; i < pVecData->size(); i++){
		pUnit = pVecData->at(i);

		pNewUnit = new CommuStorModelDataUnit_t;
		pNewUnit->nStep = pUnit->nStep;
		pNewUnit->bDataType = pUnit->bDataType;
		if( pUnit->bDataType == SDATA_C_D_Type_ModelData){
			pNewUnit->u.nGroupIdx = pUnit->u.nGroupIdx;
		}else{
			strcpy(pNewUnit->u.szRecordFile, pUnit->u.szRecordFile);
		}
		pNewUnit->nTimeStampSec = pUnit->nTimeStampSec;
		pNewUnit->nTimeStampMsec = pUnit->nTimeStampMsec;
		pNewUnit->nDataLen = pUnit->nDataLen;
		pNewUnit->pData = new unsigned char[pNewUnit->nDataLen+1];
		memcpy(pNewUnit->pData, pUnit->pData, pUnit->nDataLen);
		pNewUnit->pData[pUnit->nDataLen] = 0;

		m_vecStorData.push_back(pNewUnit);
	}
	m_usDataCount = m_vecStorData.size();
}

const vector<CommuStorModelDataUnit_t *> *CSimuStorDataC_DCmd::GetData()
{
	return &m_vecStorData;
}

void CSimuStorDataC_DCmd::GetData(vector<CommuStorModelDataUnit_t *> *pVecData)
{
	CommuStorModelDataUnit_t *pUnit = NULL;
	CommuStorModelDataUnit_t *pNewUnit = NULL;

	for( unsigned int i = 0; i < m_vecStorData.size(); i++){
		pUnit = m_vecStorData.at(i);

		pNewUnit = new CommuStorModelDataUnit_t;
		pNewUnit->nStep = pUnit->nStep;
		pNewUnit->bDataType = pUnit->bDataType;
		if( pUnit->bDataType == SDATA_C_D_Type_ModelData){
			pNewUnit->u.nGroupIdx = pUnit->u.nGroupIdx;
		}else{
			strcpy(pNewUnit->u.szRecordFile, pUnit->u.szRecordFile);
		}
		pNewUnit->nTimeStampSec = pUnit->nTimeStampSec;
		pNewUnit->nTimeStampMsec = pUnit->nTimeStampMsec;
		pNewUnit->nDataLen = pUnit->nDataLen;
		pNewUnit->pData = new unsigned char[pNewUnit->nDataLen+1];
		memcpy(pNewUnit->pData, pUnit->pData, pUnit->nDataLen);
		pNewUnit->pData[pUnit->nDataLen] = 0;

		pVecData->push_back(pNewUnit);
	}
}
