// Copyright (c) 2016 甘波。All rights reserved。

#include "stdafx.h"
#include <assert.h>

#pragma region "导出函数。"

//////////////////////////////////////////////////////////////////////////
// 导出函数。

#if defined (_DEBUG)

// 断言。由于 C 标准库的断言函数在多线程环境下有时会出错，固添加此函数。
// 注：请勿在 dll 的入口函数中调用此函数。
GBEXTERN_C GBAPI void GBSTDCALL GBAssert(
	LPCWSTR lpszMsg,							// IN：信息。
	LPCWSTR lpszFile,							// IN：文件。
	UINT uLine									// IN：第几行。
	)
{
	// 锁定断言函数。
//	CGBObjLock ObjLock(&GBAssert);

	// 调用 C 标准库的断言函数。
	_wassert(lpszMsg, lpszFile, uLine);
}

#endif

#pragma endregion
