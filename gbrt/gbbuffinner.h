// Copyright (c) 2016 甘波。All rights reserved。

#if !defined (_GB_BUFF_INNER_H_GB_20161028_)
#define _GB_BUFF_INNER_H_GB_20161028_

#pragma once

#include "gbbuff.h"

#pragma pack(push, GBPACK)

// 缓存管理器。
typedef struct _GBBUFFMGR
{
	ULONG uFreeBuffCount;						// 空闲缓存数。
	GBLIST FreeBuffList;						// 空闲缓存链表。
} GBBUFFMGR, *LPGBBUFFMGR;
typedef struct _GBBUFFMGR const *LPCGBBUFFMGR;

// 最大空闲缓存数。
#define GB_MAX_FREE_BUFF_COUNT 32

#pragma pack(pop)

// 从缓存管理器分配缓存。
GBEXTERN_C LPGBBUFF GBSTDCALL GBAllocBuffFromBuffMgr(
	LPGBBUFFMGR lpBuffMgr						// IN：缓存管理器。
);

// 释放缓存到缓存管理器。
GBEXTERN_C void GBSTDCALL GBFreeBuffToBuffMgr(
	LPGBBUFFMGR lpBuffMgr,						// IN：缓存管理器。
	LPGBBUFF lpBuff								// IN：缓存。
);

// 初始化缓存管理器。
GBEXTERN_C void GBSTDCALL GBInitBuffMgr(
	LPGBBUFFMGR lpBuffMgr						// INOUT：缓存管理器。
);

// 释放缓存管理器。
GBEXTERN_C void GBSTDCALL GBReleaseBuffMgr(
	LPGBBUFFMGR lpBuffMgr						// INOUT：缓存管理器。
);

#endif
