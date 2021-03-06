/*****************************************************************
 * 文件名: SimuStorDataC_DCmd.h
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
 * 枚举名 : StorDataC_D_CMD
 * 说明 : SimuService,SimuGuard与SimuMonitor之间通信命令字定义
 ****************************************************************/
typedef enum euStorDataC_D_CMD
{
	SDATA_C_D_CMD_UNKNOWN		= -1 ,		 /* 未知命令*/
	SDATA_C_D_CMD_STOR_SIMUDATA	= 0x60000001,/* 存储数据传输命令*/
} StorDataC_D_CMD;

typedef enum euStorDataC_D_Type{

	SDATA_C_D_Type_ModelData = 1,	/* 模型数据 */
	SDATA_C_D_Type_UserData  = 2,	/* 用户数据 */
} StorDataC_D_Type;


#define SDATA_C_D_CMD_HEADER_SIZE		 8
#define SDATA_C_D_CMD_MIN_UNIT_DATA_SIZE 18


/****************************************************************
 * 类名 : CSimuStorDataC_DCmd
 * 父类 : 无
 * 说明 : SimuService,SimuGuard与SimuMonitor之间存储数据命令定义
 ****************************************************************/
class CSimuStorDataC_DCmd
{
public:
	CSimuStorDataC_DCmd();
	virtual ~CSimuStorDataC_DCmd();

	/* 设置模型ID */
	void SetModelID(unsigned short id);
	
	/* 获取模型ID */
	unsigned short GetModelID();

	/* 获取命令*/
	unsigned long GetCommand();

	/* 获取数据个数 */
	unsigned short GetDataCount();

	/* 序列化命令 */
	unsigned long Serialize(unsigned char* szBuffer);		
	
	/* 反序列化传入的数据 */
	BOOL Unserialize(unsigned char* szBuffer , unsigned long nBufferLen, unsigned long &nCmdLen);	
	
	/* 打印输出 */
	void Print(char* szBuffer);

	/* 增加一个用户数据 */
	void AddUserData(unsigned long nStep,const char *szFile, unsigned long ts1, unsigned long ts2, unsigned char *pData, unsigned long nDataLen);

	/* 增加一个模型数据*/
	void AddModelData(unsigned long nStep,unsigned long nGroupIdx, unsigned long ts1, unsigned long ts2, unsigned char *pData, unsigned long nDataLen);

	/* 增加一组数据*/
	void AddData(const vector<CommuStorModelDataUnit_t *> *pVecData);

	/* 返回数据 */
	const vector<CommuStorModelDataUnit_t *> *GetData();

	/* 获取数据 */
	void GetData(vector<CommuStorModelDataUnit_t *> *pVecData);

protected:
	unsigned long		m_nCommand; /*命令字*/
	unsigned short		m_usModelID;/*模型ID*/
	unsigned short		m_usDataCount;/*数据数目*/

	vector<CommuStorModelDataUnit_t *> m_vecStorData;/*存储数据列表*/
};
