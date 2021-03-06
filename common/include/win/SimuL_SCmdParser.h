/*****************************************************************
 * 文件名: SimuL_SCmdParser.h
 * 功能: SimuLab和SimuService之间通信命令执行和解析器定义
 *****************************************************************/
#pragma once

#include <vector>
#include "SimuCommAPI.h"
#include "SimuL_SCmdExecutorInterface.h"

using namespace std;

/****************************************************************
 * 类名 : CSimuL_SCmdParser
 * 父类 : 无
 * 说明 : SimuLab和SimuService之间通信命令解析器
 ****************************************************************/
class CSimuL_SCmdParser
{
	/*解析线程*/
	friend DWORD WINAPI SimuL_SCmdParseThread(LPVOID lpParam);
public :
	/*构造函数*/
	CSimuL_SCmdParser();
	/*析构函数*/
	virtual ~CSimuL_SCmdParser();

	/* 初始化命令解析器 */
	BOOL Initialize(CSimuL_SCmdExecutorInterface* pCmdExecutorInterface);	
	
	/* 卸载命令解析器并释放资源 */
	void  Uninitialize();	
	
	/* 把指定的数据解析成命令执行器可执行的命令 */
	BOOL Parse(unsigned char* szData , unsigned long nDataLen);	

protected:

	/* 执行所有已解析成功的命令，由命令解析器的辅助线程调用 */
	BOOL ExecAllCmd();																					

private :
	BOOL							m_bInitialize;	/* 是否已初始化 */
	vector<CSimuL_SCmd*>			m_vecCmd;		/* 命令集合 */
	CRITICAL_SECTION				m_criCmd;		/* 访问命令的临界区 */
	unsigned char					*m_szBuffer;	/* 传送到命令解析器中的数据缓冲区 */
	unsigned long					m_nBufferLen;	/* 传送到命令解析器中的数据缓冲区长度 */
	HANDLE							m_hParseThread;	/* 命令解析器辅助线程的句柄 */
	HANDLE							m_hParseEvent;	/* 停止命令解析器辅助线程的事件句柄 */
	CSimuL_SCmdExecutorInterface*   m_pCmdExecutorInterface; /* 对应的命令执行器 */
};