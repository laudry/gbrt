// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <WinSock2.h>
#include <wincrypt.h>

#define GBAPI __declspec(dllexport)
#define GBVARIABLE __declspec(dllexport)
#define GBCLASS __declspec(dllexport)

#if !defined (_DEBUG)
	#include <malloc.h>
#else											// _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif											// _DEBUG
#include <tchar.h>

#include "gbcommon.h"
#include "gblist.h"
#include "gbrt.h"
#include "gbfsminner.h"
#include "gbfsm.h"
#include "gbobjlockinner.h"
#include "gbobjlock.h"
#include "gberror.h"
#include "gbmoduledata.h"
#include "gbassert.h"
#include "gbbuffinner.h"
#include "gbthreaddata.h"
