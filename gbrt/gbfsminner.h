// Copyright (c) 2016 甘波。All rights reserved。

#if !defined (_GB_FSM_INNER_H_GB_20161026_)
#define _GB_FSM_INNER_H_GB_20161026_

#pragma once

#include "gblist.h"

#pragma pack(push, GBPACK)

// FSM 分配器。
// FSM 分配器的大小需为 GBPACK 的整数倍。
typedef struct _GBFSMALLOC
{
	GBLIST FSMAllocListItem;					// FSM 分配器链表项。
	DWORD dwFSMBlockSize;						// FSM 块大小。
	DWORD dwAllocCountOnce;						// 一次分配数。不能为 0。
	DWORD dwAllocedCount;						// 已分配数。
	DWORD dwFSMPageCount;						// FSM 页数。
	GBLIST FSMPageList;							// FSM 页链表。
} GBFSMALLOC, *LPGBFSMALLOC;
typedef struct _GBFSMALLOC const *LPCGBFSMALLOC;

// FSM 页。
// FSM 页的大小需为 GBPACK 的整数倍。
typedef struct _GBFSMPAGE
{
	GBLIST FSMPageListItem;						// FSM 页链表项。
	DWORD dwAllocedCount;						// 已分配数。
	DWORD dwFreeFSMBlockCount;					// 空闲 FSM 块数。
	GBLIST FreeFSMBlockList;					// 空闲 FSM 块链表。
} GBFSMPAGE, *LPGBFSMPAGE;
typedef struct _GBFSMPAGE const *LPCGBFSMPAGE;

// FSM 块。
// FSM 块的大小需为 GBPACK 的整数倍。
typedef struct _GBFSMBLOCK
{
	union
	{
		GBLIST FreeFSMBlockListItem;			// 空闲 FSM 块链表项。
		struct
		{
			LPGBFSMALLOC lpFSMAlloc;			// 所属 FSM 分配器。
			LPGBFSMPAGE lpFSMPage;				// 所属 FSM 页。
		};
	};
} GBFSMBLOCK, *LPGBFSMBLOCK;
typedef struct _GBFSMBLOCK const *LPCGBFSMBLOCK;

// FSM 分配器管理器。
typedef struct _GBFSMALLOCMGR
{
	CRITICAL_SECTION CS;						// 临界区。
	GBFSMALLOC FSMAlloc;						// FSM 分配器。
} GBFSMALLOCMGR, *LPGBFSMALLOCMGR;
typedef struct _GBFSMALLOCMGR const *LPCGBFSMALLOCMGR;

// 最大 FSM 分配器大小。
#define GB_MAX_FSM_ALLOC_SIZE 0x10000000

// 最大固定大小。
#define GB_MAX_FIXED_SIZE (GB_MAX_FSM_ALLOC_SIZE-sizeof(GBFSMPAGE)-sizeof(GBFSMBLOCK))

#pragma pack(pop)

// 根据 FSM 块获取 FSM。
#define GBGetFSMFromFSMBlock(lpFSMBlock) GBPTR2PTR(LPVOID, GBPTR2PTR(LPBYTE, lpFSMBlock)+sizeof(GBFSMBLOCK))

// 根据 FSM 获取 FSM 块。
#define GBGetFSMBlockFromFSM(lpFSM) GBPTR2PTR(LPGBFSMBLOCK, GBPTR2PTR(LPBYTE, lpFSM)-sizeof(GBFSMBLOCK))

// 释放 FSM 分配器。
GBEXTERN_C void GBSTDCALL GBFreeFSMAlloc(
	LPGBFSMALLOC lpFSMAlloc						// IN：FSM 分配器。
);

// 初始化 FSM 分配器管理器。
GBEXTERN_C BOOL GBSTDCALL GBInitFSMAllocMgr(
	LPGBFSMALLOCMGR lpFSMAllocMgr				// INOUT：FSM 分配器管理器。
);

// 释放 FSM 分配器管理器。
GBEXTERN_C void GBSTDCALL GBReleaseFSMAllocMgr(
	LPGBFSMALLOCMGR lpFSMAllocMgr				// INOUT：FSM 分配器管理器。
);

#endif
