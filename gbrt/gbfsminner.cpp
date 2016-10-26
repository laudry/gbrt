// Copyright (c) 2016 甘波。All rights reserved。

#include "stdafx.h"

#pragma region "内部函数。"

//////////////////////////////////////////////////////////////////////////
// 内部函数。

// 释放 FSM 分配器。
GBEXTERN_C void GBSTDCALL GBFreeFSMAlloc(
	LPGBFSMALLOC lpFSMAlloc						// IN：FSM 分配器。
)
{
	// 释放 FSM 分配器所有的 FSM 页。
	LPGBLIST lpListItem = lpFSMAlloc->FSMPageList.lpNext;
	while (lpListItem != &lpFSMAlloc->FSMPageList)
	{
		LPGBFSMPAGE lpFSMPage = GBPTR2PTR(LPGBFSMPAGE, lpListItem);
		lpListItem = lpListItem->lpNext;
		GBDelListItem(&lpFSMPage->FSMPageListItem);
		--lpFSMAlloc->dwFSMPageCount;
		free(lpFSMPage);
	}

	// 释放 FSM 分配器。
	GBDelListItem(&lpFSMAlloc->FSMAllocListItem);
	GBFreeFSM(lpFSMAlloc);
}

// 初始化 FSM 分配器管理器。
GBEXTERN_C BOOL GBSTDCALL GBInitFSMAllocMgr(
	LPGBFSMALLOCMGR lpFSMAllocMgr				// IN：FSM 分配器管理器。
)
{
	// 初始化临界区。
	if (!GBInitializeCriticalSection(&lpFSMAllocMgr->CS))
		return FALSE;

	// 初始化 FSM 分配器。
	GBInitList(&lpFSMAllocMgr->FSMAlloc.FSMAllocListItem);
	lpFSMAllocMgr->FSMAlloc.dwFSMBlockSize = sizeof(GBFSMBLOCK) + sizeof(GBFSMALLOC);
	lpFSMAllocMgr->FSMAlloc.dwAllocCountOnce = 64;
	lpFSMAllocMgr->FSMAlloc.dwAllocedCount = 0;
	lpFSMAllocMgr->FSMAlloc.dwFSMPageCount = 0;
	GBInitList(&lpFSMAllocMgr->FSMAlloc.FSMPageList);

	return TRUE;
}

// 释放 FSM 分配器管理器。
GBEXTERN_C void GBSTDCALL GBReleaseFSMAllocMgr(
	LPGBFSMALLOCMGR lpFSMAllocMgr				// INOUT：FSM 分配器管理器。
)
{
	// 进入临界区。
	::EnterCriticalSection(&lpFSMAllocMgr->CS);

	// 释放已创建的 FSM 分配器。
	LPGBLIST lpListItem = lpFSMAllocMgr->FSMAlloc.FSMAllocListItem.lpNext;
	while (lpListItem != &lpFSMAllocMgr->FSMAlloc.FSMAllocListItem)
	{
		LPGBFSMALLOC lpFSMAlloc = GBPTR2PTR(LPGBFSMALLOC, lpListItem);
		lpListItem = lpListItem->lpNext;
		GBFreeFSMAlloc(lpFSMAlloc);
	}

	// 释放 FSM 分配器所有的 FSM 页。
	lpListItem = lpFSMAllocMgr->FSMAlloc.FSMPageList.lpNext;
	while (lpListItem != &lpFSMAllocMgr->FSMAlloc.FSMPageList)
	{
		LPGBFSMPAGE lpFSMPage = GBPTR2PTR(LPGBFSMPAGE, lpListItem);
		lpListItem = lpListItem->lpNext;
		GBDelListItem(&lpFSMPage->FSMPageListItem);
		--lpFSMAllocMgr->FSMAlloc.dwFSMPageCount;
		free(lpFSMPage);
	}

	// 删除临界区。
	::DeleteCriticalSection(&lpFSMAllocMgr->CS);
}

#pragma endregion
