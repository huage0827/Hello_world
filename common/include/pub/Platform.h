/*****************************************************************
 * ÎÄ¼þÃû: Platform.h
 *****************************************************************/
#pragma once

#define IN
#define OUT

#if defined(QNX_PLATFORM) // Qnx
#include "QNXPlatform.h"
#define EXPORT_DLL

#elif defined(LINUX_PLATFORM) // Linux
#include "LinuxPlatform.h"
#define EXPORT_DLL

#elif defined(WIN_PLATFORM) // Win
#include "windows.h"
#include <WinBase.h>
#include <WinError.h>
#include "winsock.h"
#include "stdio.h"
#ifdef EXPORTDLL
#define EXPORT_DLL __declspec(dllexport)
#else
#define EXPORT_DLL __declspec(dllimport)
#endif

#else

#endif
