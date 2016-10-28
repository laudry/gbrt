// Copyright (c) 2016 甘波。All rights reserved。

#if !defined (_GB_COMMON_H_GB_20161025_)
#define _GB_COMMON_H_GB_20161025_

#pragma once

#include "gbdef.h"
#include <WinSock2.h>

#pragma pack(push, GBPACK)

// 通知函数。
typedef int (GBSTDCALL *GBNOTIFYPROC)(
	int nMsg,									// IN：消息。
	INT_PTR nParam1,							// IN：参数一。
	INT_PTR nParam2,							// IN：参数二。
	INT_PTR lpUserData							// IN：用户数据。
	);

#pragma pack(pop)

// 初始化临界区。
GBEXTERN_C GBINLINE BOOL GBSTDCALL GBInitializeCriticalSection(
	LPCRITICAL_SECTION lpCriticalSection		// OUT：临界区。
	)
{
	__try
	{
		::InitializeCriticalSection(lpCriticalSection);

		return TRUE;
	}
	__except (GBHandleException(STATUS_NO_MEMORY))
	{
		return FALSE;
	}
}

#endif
