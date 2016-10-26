// Copyright (c) 2016 甘波。All rights reserved。

#include "stdafx.h"

#pragma region "导出函数。"

//////////////////////////////////////////////////////////////////////////
// 导出函数。

// 获取错误码。
GBEXTERN_C GBAPI LONG GBSTDCALL GBGetLastError(void)
{
	LPGBTHREADDATA lpThreadData = GBGetThreadData();
	if (NULL == lpThreadData)
		return GB_EC_NO_MEMORY;
	else
		return lpThreadData->lErrorCode;
}

// 获取错误码及错误信息。
// 返回值：错误码。
GBEXTERN_C GBAPI LONG GBSTDCALL GBGetLastErrorAndMsg(
	LPLONG lpErrorMsg1,							// OUT：错误信息一。
	LPLONG lpErrorMsg2,							// OUT：错误信息二。
	LPLONG lpErrorMsg3							// OUT：错误信息三。
)
{
	LPGBTHREADDATA lpThreadData = GBGetThreadData();
	if (NULL == lpThreadData)
	{
		*lpErrorMsg1 = 0;
		*lpErrorMsg2 = 0;
		*lpErrorMsg3 = 0;
		return GB_EC_NO_MEMORY;
	}
	else
	{
		*lpErrorMsg1 = lpThreadData->lErrorMsg1;
		*lpErrorMsg2 = lpThreadData->lErrorMsg2;
		*lpErrorMsg3 = lpThreadData->lErrorMsg3;
		return lpThreadData->lErrorCode;
	}
}

// 设置错误码。
GBEXTERN_C GBAPI void GBSTDCALL GBSetLastError(
	LONG lErrorCode								// IN：错误码。
)
{
	LPGBTHREADDATA lpThreadData = GBGetThreadData();
	if (NULL != lpThreadData)
	{
		lpThreadData->lErrorCode = lErrorCode;
		lpThreadData->lErrorMsg1 = 0;
		lpThreadData->lErrorMsg2 = 0;
		lpThreadData->lErrorMsg3 = 0;
	}
}

// 设置错误码及错误信息。
GBEXTERN_C GBAPI void GBSTDCALL GBSetLastErrorAndMsg(
	LONG lErrorCode,							// IN：错误码。
	LONG lErrorMsg1,							// IN：错误信息一。
	LONG lErrorMsg2,							// IN：错误信息二。
	LONG lErrorMsg3								// IN：错误信息三。
)
{
	LPGBTHREADDATA lpThreadData = GBGetThreadData();
	if (NULL != lpThreadData)
	{
		lpThreadData->lErrorCode = lErrorCode;
		lpThreadData->lErrorMsg1 = lErrorMsg1;
		lpThreadData->lErrorMsg2 = lErrorMsg2;
		lpThreadData->lErrorMsg3 = lErrorMsg3;
	}
}

#pragma endregion
