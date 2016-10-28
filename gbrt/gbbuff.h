// Copyright (c) 2016 甘波。All rights reserved。

#if !defined (_GB_BUFF_H_GB_20161028_)
#define _GB_BUFF_H_GB_20161028_

#pragma once

#include "gblist.h"
#include "gbrt.h"

#pragma pack(push, GBPACK)

// 缓存。
typedef struct _GBBUFF
{
	GBLIST BuffListItem;						// 缓存链表项。
	ULONG uBuffCapacity;						// 缓存容量。
	WSABUF Buff;								// 缓存。
} GBBUFF, *LPGBBUFF;
typedef struct _GBBUFF const *LPCGBBUFF;

// 默认缓存容量。
#define GB_DEFAULT_BUFF_CAPACITY 8064

#pragma pack(pop)

// 分配缓存。
// 返回值：缓存。失败时返回 NULL。
// 注：缓存容量按 8 字节对齐；缓存容量小于默认缓存容量时按默认缓存容量分配。
GBEXTERN_C GBAPI LPGBBUFF GBSTDCALL GBAllocBuff(
	ULONG uBuffCapacity							// IN：缓存容量。
);

// 分配指定容量缓存。
// 返回值：缓存。失败时返回 NULL。
// 注：缓存容量按 8 字节对齐。
GBEXTERN_C GBAPI LPGBBUFF GBSTDCALL GBAllocAppointedCapacityBuff(
	ULONG uBuffCapacity							// IN：缓存容量。
);

// 释放缓存。
GBEXTERN_C GBAPI void GBSTDCALL GBFreeBuff(
	LPGBBUFF lpBuff								// IN：缓存。
);

// 批量释放缓存。
GBEXTERN_C GBAPI void GBSTDCALL GBFreeBuffInBatch(
	LPGBLIST lpBuffList							// INOUT：缓存链表。
);

#endif
