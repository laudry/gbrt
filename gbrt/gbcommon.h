// Copyright (c) 2016 甘波。All rights reserved。

#if !defined (_GB_COMMON_H_GB_20161025_)
#define _GB_COMMON_H_GB_20161025_

#pragma once

#include "gbdef.h"

#pragma pack(push, GBPACK)

// 通知函数。
typedef int (GBSTDCALL *GBNOTIFYPROC)(
	UINT uMsg,									// IN：消息。
	UINT_PTR uParam1,							// IN：参数一。
	UINT_PTR uParam2,							// IN：参数二。
	UINT_PTR lpUserData							// IN：用户数据。
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
