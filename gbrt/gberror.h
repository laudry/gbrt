// Copyright (c) 2016 甘波。All rights reserved。

#if !defined (_GB_ERROR_H_GB_20161026_)
#define _GB_ERROR_H_GB_20161026_

#pragma once

#include "gbrt.h"

#pragma pack(push, GBPACK)

// 函数名。
typedef enum _GBFUNCNAME
{
	GB_FN_NONE = 0,								// 无函数名。
} GBFUNCNAME;

// 错误码。
typedef enum _GBERRORCODE
{
	GB_EC_NO_ERROR = 0,							// 没有错误。
	GB_EC_API_ERROR,							// API 错误。错误信息一：API 的错误码；错误信息二：API 的函数名。
	GB_EC_NO_MEMORY,							// 内存不足。
	GB_EC_INVALID_PARAM,						// 无效参数。错误信息一：无效参数索引，从 1 开始。
	GB_EC_OVERFLOW,								// 溢出。
} GBERRORCODE;

#pragma pack(pop)

// 获取错误码。
GBEXTERN_C GBAPI LONG GBSTDCALL GBGetLastError(void);

// 获取错误码及错误信息。
// 返回值：错误码。
GBEXTERN_C GBAPI LONG GBSTDCALL GBGetLastErrorAndMsg(
	LPLONG lpErrorMsg1,							// OUT：错误信息一。
	LPLONG lpErrorMsg2,							// OUT：错误信息二。
	LPLONG lpErrorMsg3							// OUT：错误信息三。
);

// 设置错误码。
GBEXTERN_C GBAPI void GBSTDCALL GBSetLastError(
	LONG lErrorCode								// IN：错误码。
);

// 设置错误码及错误信息。
GBEXTERN_C GBAPI void GBSTDCALL GBSetLastErrorAndMsg(
	LONG lErrorCode,							// IN：错误码。
	LONG lErrorMsg1,							// IN：错误信息一。
	LONG lErrorMsg2,							// IN：错误信息二。
	LONG lErrorMsg3								// IN：错误信息三。
);

#endif
