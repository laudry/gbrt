// Copyright (c) 2016 甘波。All rights reserved。

#if !defined (_GB_THREAD_DATA_H_GB_20161026_)
#define _GB_THREAD_DATA_H_GB_20161026_

#pragma once

#include "gbrt.h"

#pragma pack(push, GBPACK)

// 线程数据。
typedef struct _GBTHREADDATA
{
	LONG lErrorCode;							// 错误码。
	LONG lErrorMsg1;							// 错误信息一。
	LONG lErrorMsg2;							// 错误信息二。
	LONG lErrorMsg3;							// 错误信息三。
	GBBUFFMGR BuffMgr;							// 缓存管理器。
} GBTHREADDATA, *LPGBTHREADDATA;
typedef struct _GBTHREADDATA const *LPCGBTHREADDATA;

#pragma pack(pop)

// 获取线程数据。
GBEXTERN_C GBAPI LPGBTHREADDATA GBSTDCALL GBGetThreadData(void);

// 释放线程数据。
GBEXTERN_C void GBSTDCALL GBFreeThreadData(
	LPGBTHREADDATA lpThreadData					// IN：线程数据。
);

#endif
