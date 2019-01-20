/*****************************************************************
 * 文件名: SimuS_GCmdParser.h
 * 功能: SimuService和SimuGuard之间通信命令执行和解析器定义
 *****************************************************************/
#pragma once

#ifndef WIN_PLATFORM
#include "stdafx.h"
#endif

#include <vector>
#include "SimuCommAPI.h"
#include "SimuS_GCmdExecutorInterface.h"

using namespace std;

class CSimuS_GComm;

/****************************************************************
 * 类名 : CSimuS_GCmdParser
 * 父类 : 无
 * 说明 : SimuService和SimuGuard之间通信命令解析器
 ****************************************************************/
class CSimuS_GCmdParser
{
#if defined(QNX_PLATFORM) || defined(LINUX_PLATFORM)
	friend void* SimuS_GCmdParseThread(LPVOID lpParam); /*解析线程*/
#else
	friend DWORD WINAPI SimuS_GCmdParseThread(LPVOID lpParam);/*解析线程*/
#endif
public :
	CSimuS_GCmdParser(KL_CLIENT_ID nClientID , CSimuS_GComm* pSimuS_GComm);/*构造函数*/
	virtual ~CSimuS_GCmdParser();/*析构函数*/

	/* 初始化命令解析器 */
	BOOL Initialize();																						
	
	/* 卸载命令解析器并释放资源 */
	void Uninitialize();
	
	/* 把指定的数据解析成命令执行器可执行的命令 */
	BOOL Parse(unsigned char* szData , unsigned long nDataLen);	

protected :

	/* 执行所有已解析的命令 */
	BOOL ExecAllCmd();																					

private :
	BOOL					m_bInitialize;	/* 是否已初始化 */
	vector<CSimuS_GCmd*>	m_vecCmd;		/* 命令集合 */
	CRITICAL_SECTION		m_criCmd;		/* 访问命令的临界区 */
	unsigned char			*m_szBuffer;	/* 传送到命令解析器中的数据缓冲区 */
	unsigned long			m_nBufferLen;	/* 传送到命令解析器中的数据缓冲区长度 */
	HANDLE					m_hParseThread;	/* 命令解析器辅助线程的句柄 */
	HANDLE					m_hParseEvent;	/* 停止命令解析器辅助线程的事件句柄 */
	CSimuS_GCmdExecutorInterface* m_pCmdExecutorInterface;	/* 对应的命令执行器 */
	CSimuS_GComm* 			m_pSimuS_GComm;	/* SimuService和SimuGuard之间通信器 */
	KL_CLIENT_ID			m_nClientID;	/* SimuService客户端ID号 */
};

