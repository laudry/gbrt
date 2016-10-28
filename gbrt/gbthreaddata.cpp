// Copyright (c) 2016 甘波。All rights reserved。

#include "stdafx.h"

#pragma region "静态函数。"

//////////////////////////////////////////////////////////////////////////
// 静态函数。

// 初始化线程数据。
GBEXTERN_C GBSTATIC void GBSTDCALL GBInitThreadData(
	LPGBTHREADDATA lpThreadData					// OUT：线程数据。
)
{
	// 先清零。
	::ZeroMemory(lpThreadData, sizeof(GBTHREADDATA));

	// 初始化需要特别初始化的成员变量。
	GBInitBuffMgr(&lpThreadData->BuffMgr);
}

#pragma endregion

#pragma region "内部函数。"

//////////////////////////////////////////////////////////////////////////
// 内部函数。

// 释放线程数据。
GBEXTERN_C void GBSTDCALL GBFreeThreadData(
	LPGBTHREADDATA lpThreadData					// IN：线程数据。
)
{
	// 释放需要特别释放的成员变量。
	GBReleaseBuffMgr(&lpThreadData->BuffMgr);

	// 释放线程数据内存。
	free(lpThreadData);
}

#pragma endregion

#pragma region "导出函数。"

//////////////////////////////////////////////////////////////////////////
// 导出函数。

// 获取线程数据。
GBEXTERN_C GBAPI LPGBTHREADDATA GBSTDCALL GBGetThreadData(void)
{
	// 首先尝试获取已有线程数据。
	LPGBTHREADDATA lpThreadData = GBPTR2PTR(LPGBTHREADDATA, ::TlsGetValue(ext_ModuleData.m_dwTlsIndex));
	if (NULL != lpThreadData)
		return lpThreadData;

	// 当已有线程数据不存在时，创建新线程数据。
	lpThreadData = GBPTR2PTR(LPGBTHREADDATA, malloc(sizeof(GBTHREADDATA)));
	if (NULL == lpThreadData)
		return NULL;

	// 初始化新线程数据。
	GBInitThreadData(lpThreadData);

	// 保存新线程数据。
	if (::TlsSetValue(ext_ModuleData.m_dwTlsIndex, lpThreadData))
		return lpThreadData;

	// 保存新线程数据失败时释放新线程数据。
	GBFreeThreadData(lpThreadData);
	return NULL;
}

#pragma endregion
