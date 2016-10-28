// Copyright (c) 2016 甘波。All rights reserved。

#include "stdafx.h"

#pragma region "导出函数。"

//////////////////////////////////////////////////////////////////////////
// 导出函数。

// 分配缓存。
// 返回值：缓存。失败时返回 NULL。
// 注：缓存容量按 8 字节对齐；缓存容量小于默认缓存容量时按默认缓存容量分配。
// 分配原则：
// 1、缓存容量大于默认缓存容量时分配缓存容量大小，否则分配默认缓存容量大小；
// 2、分配缓存容量大小时直接分配，分配默认缓存容量大小时从缓存管理器分配。
GBEXTERN_C GBAPI LPGBBUFF GBSTDCALL GBAllocBuff(
	ULONG uBuffCapacity							// IN：缓存容量。
)
{
	// 缓存容量按 8 字节对齐。
	uBuffCapacity = GBALIGNSIZE2(uBuffCapacity, 3);

	// 分配缓存。
	LPGBBUFF lpBuff;							// 返回值。
	if (uBuffCapacity > GB_DEFAULT_BUFF_CAPACITY)
		lpBuff = GBPTR2PTR(LPGBBUFF, malloc(sizeof(GBBUFF) + uBuffCapacity));
	else
	{
		LPGBTHREADDATA lpThreadData = GBGetThreadData();
		lpBuff = GBAllocBuffFromBuffMgr(NULL == lpThreadData ? NULL : &lpThreadData->BuffMgr);
	}
	if (NULL == lpBuff)
	{
		GBSetLastError(GB_EC_NO_MEMORY);
		return NULL;
	}

	// 初始化缓存。
	lpBuff->BuffListItem.lpNext = lpBuff->BuffListItem.lpPrev = NULL;
	lpBuff->uBuffCapacity = max(uBuffCapacity, GB_DEFAULT_BUFF_CAPACITY);
	lpBuff->Buff.len = 0;
	lpBuff->Buff.buf = GBPTR2PTR(CHAR FAR *, lpBuff) + sizeof(GBBUFF);

	return lpBuff;
}

// 分配指定容量缓存。
// 返回值：缓存。失败时返回 NULL。
// 注：缓存容量按 8 字节对齐。
// 分配原则：
// 1、缓存容量不等于默认缓存容量时分配缓存容量大小，否则分配默认缓存容量大小；
// 2、分配缓存容量大小时直接分配，分配默认缓存容量大小时从缓存管理器分配。
GBEXTERN_C GBAPI LPGBBUFF GBSTDCALL GBAllocAppointedCapacityBuff(
	ULONG uBuffCapacity							// IN：缓存容量。
)
{
	// 缓存容量按 8 字节对齐。
	uBuffCapacity = GBALIGNSIZE2(uBuffCapacity, 3);

	// 分配缓存。
	LPGBBUFF lpBuff;							// 返回值。
	if (uBuffCapacity != GB_DEFAULT_BUFF_CAPACITY)
		lpBuff = GBPTR2PTR(LPGBBUFF, malloc(sizeof(GBBUFF) + uBuffCapacity));
	else
	{
		LPGBTHREADDATA lpThreadData = GBGetThreadData();
		lpBuff = GBAllocBuffFromBuffMgr(NULL == lpThreadData ? NULL : &lpThreadData->BuffMgr);
	}
	if (NULL == lpBuff)
	{
		GBSetLastError(GB_EC_NO_MEMORY);
		return NULL;
	}

	// 初始化缓存。
	lpBuff->BuffListItem.lpNext = lpBuff->BuffListItem.lpPrev = NULL;
	lpBuff->uBuffCapacity = uBuffCapacity;
	lpBuff->Buff.len = 0;
	lpBuff->Buff.buf = GBPTR2PTR(CHAR FAR *, lpBuff) + sizeof(GBBUFF);

	return lpBuff;
}

// 释放缓存。
// 释放原则：缓存容量不等于默认缓存容量时直接释放，否则释放到缓存管理器。
GBEXTERN_C GBAPI void GBSTDCALL GBFreeBuff(
	LPGBBUFF lpBuff								// IN：缓存。
)
{
	// 释放缓存。
	if (lpBuff->uBuffCapacity != GB_DEFAULT_BUFF_CAPACITY)
		free(lpBuff);
	else
	{
		LPGBTHREADDATA lpThreadData = GBGetThreadData();
		GBFreeBuffToBuffMgr(NULL == lpThreadData ? NULL : &lpThreadData->BuffMgr, lpBuff);
	}
}

// 批量释放缓存。
// 释放原则：缓存容量不等于默认缓存容量时直接释放，否则释放到缓存管理器。
GBEXTERN_C GBAPI void GBSTDCALL GBFreeBuffInBatch(
	LPGBLIST lpBuffList							// INOUT：缓存链表。
)
{
	// 获取缓存管理器。
	LPGBTHREADDATA lpThreadData = GBGetThreadData();
	LPGBBUFFMGR lpBuffMgr = (NULL == lpThreadData ? NULL : &lpThreadData->BuffMgr);

	// 批量释放缓存。
	LPGBLIST lpListItem = lpBuffList->lpNext;
	while (lpListItem != lpBuffList)
	{
		LPGBBUFF lpBuff = GBPTR2PTR(LPGBBUFF, lpListItem);
		lpListItem = lpListItem->lpNext;
		GBDelListItem(&lpBuff->BuffListItem);
		if (lpBuff->uBuffCapacity != GB_DEFAULT_BUFF_CAPACITY)
			free(lpBuff);
		else
			GBFreeBuffToBuffMgr(lpBuffMgr, lpBuff);
	}
}

#pragma endregion
