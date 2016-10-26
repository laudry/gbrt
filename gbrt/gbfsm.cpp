// Copyright (c) 2016 甘波。All rights reserved。

#include "stdafx.h"

#pragma region "导出函数。"

//////////////////////////////////////////////////////////////////////////
// 导出函数。

// 计算 FSM 分配器的最大分配数。
// 返回值：最大分配数。失败时返回 0。
GBEXTERN_C GBAPI DWORD GBSTDCALL GBCalcMaxAllocCountOfFSMAlloc(
	DWORD dwFixedSize,							// IN：固定大小。不能为 0。
	DWORD dwAllocCountOnce						// IN：一次分配数。不能为 0。
)
{
	// 检查输入参数。
	if (0 == dwFixedSize || dwFixedSize > GB_MAX_FIXED_SIZE)
	{
		GBSetLastErrorAndMsg(GB_EC_INVALID_PARAM, 1, 0, 0);
		return 0;
	}
	if (0 == dwAllocCountOnce)
	{
		GBSetLastErrorAndMsg(GB_EC_INVALID_PARAM, 2, 0, 0);
		return 0;
	}

	// 计算 FSM 块大小。
	DWORD dwFSMBlockSize = sizeof(GBFSMBLOCK) + GBALIGNSIZE2(dwFixedSize, GBPACKSHIFT);

	// 检查是否溢出。
	if ((GB_MAX_FSM_ALLOC_SIZE - sizeof(GBFSMPAGE)) / dwFSMBlockSize < dwAllocCountOnce)
	{
		GBSetLastError(GB_EC_OVERFLOW);
		return 0;
	}

	// 计算 FSM 页大小。
	DWORD dwFSMPageSize = sizeof(GBFSMPAGE) + dwAllocCountOnce*dwFSMBlockSize;

	// 计算 FSM 分配器的最大分配数。
	return GB_MAX_FSM_ALLOC_SIZE / dwFSMPageSize * dwAllocCountOnce;
}

// 创建 FSM 分配器。
// 返回值： FSM 分配器句柄。失败时返回 NULL。
GBEXTERN_C GBAPI HANDLE GBSTDCALL GBCreateFSMAlloc(
	DWORD dwFixedSize,							// IN：固定大小。不能为 0。
	DWORD dwAllocCountOnce						// IN：一次分配数。不能为 0。
)
{
	// 计算 FSM 分配器的最大分配数。
	if (0 == GBCalcMaxAllocCountOfFSMAlloc(dwFixedSize, dwAllocCountOnce))
		return NULL;

	// 进入临界区。
	::EnterCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	// 创建 FSM 分配器。
	LPGBFSMALLOC lpFSMAlloc = GBPTR2PTR(LPGBFSMALLOC, GBAllocFSM(GBPTR2PTR(HANDLE, &ext_ModuleData.m_FSMAllocMgr.FSMAlloc)));
	if (NULL != lpFSMAlloc)
	{
		// 初始化 FSM 分配器。
		lpFSMAlloc->dwFSMBlockSize = sizeof(GBFSMBLOCK) + GBALIGNSIZE2(dwFixedSize, GBPACKSHIFT);
		lpFSMAlloc->dwAllocCountOnce = dwAllocCountOnce;
		lpFSMAlloc->dwAllocedCount = 0;
		lpFSMAlloc->dwFSMPageCount = 0;
		GBInitList(&lpFSMAlloc->FSMPageList);
		GBInsertAfterListItem(&ext_ModuleData.m_FSMAllocMgr.FSMAlloc.FSMAllocListItem, &lpFSMAlloc->FSMAllocListItem);
	}

	// 离开临界区。
	::LeaveCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	// 返回创建的 FSM 分配器。
	return GBPTR2PTR(HANDLE, lpFSMAlloc);
}

// 销毁 FSM 分配器。
GBEXTERN_C GBAPI void GBSTDCALL GBDestroyFSMAlloc(
	HANDLE hFSMAlloc							// IN：FSM 分配器。
)
{
	// 进入临界区。
	::EnterCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	// 释放 FSM 分配器。
	GBFreeFSMAlloc(GBPTR2PTR(LPGBFSMALLOC, hFSMAlloc));

	// 离开临界区。
	::LeaveCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);
}

// 判断是否 FSM 分配器。
GBEXTERN_C GBAPI BOOL GBSTDCALL GBIsFSMAlloc(
	HANDLE hFSMAlloc							// IN：FSM 分配器。
)
{
	// 进入临界区。
	::EnterCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	// 判断是否 FSM 分配器。
	BOOL bIsFSMAlloc = FALSE;
	LPGBLIST lpListItem = ext_ModuleData.m_FSMAllocMgr.FSMAlloc.FSMAllocListItem.lpNext;
	while (lpListItem != &ext_ModuleData.m_FSMAllocMgr.FSMAlloc.FSMAllocListItem)
	{
		if (GBPTR2PTR(HANDLE, lpListItem) == hFSMAlloc)
		{
			bIsFSMAlloc = TRUE;
			break;
		}
		lpListItem = lpListItem->lpNext;
	}

	// 离开临界区。
	::LeaveCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	return bIsFSMAlloc;
}

// 获取已分配 FSM 数。
GBEXTERN_C GBAPI DWORD GBSTDCALL GBGetAllocedFSMCount(
	HANDLE hFSMAlloc							// IN：FSM 分配器。
)
{
	return GBPTR2PTR(LPGBFSMALLOC, hFSMAlloc)->dwAllocedCount;
}

// 分配 FSM。
// 返回值：FSM 指针。失败时返回 NULL。
GBEXTERN_C GBAPI LPVOID GBSTDCALL GBAllocFSM(
	HANDLE hFSMAlloc							// IN：FSM 分配器。
)
{
	// 获取 FSM 分配器。
	LPGBFSMALLOC lpFSMAlloc = GBPTR2PTR(LPGBFSMALLOC, hFSMAlloc);

	// 判断 FSM 页链表是否空链表。
	if (!GBIsListEmpty(&lpFSMAlloc->FSMPageList))
	{
		// 获取第一个 FSM 页。
		LPGBFSMPAGE lpFSMPage = GBPTR2PTR(LPGBFSMPAGE, lpFSMAlloc->FSMPageList.lpNext);

		// 判断第一个 FSM 页能否分配 FSM 块。
		// 注：如果此页不能分配 FSM 块的话，则已有 FSM 页都不能分配 FSM 块。
		if (lpFSMPage->dwAllocedCount != lpFSMAlloc->dwAllocCountOnce)
		{
			// 从当前 FSM 页中分配 FSM 块。
			LPGBFSMBLOCK lpFSMBlock;
			if (0 == lpFSMPage->dwFreeFSMBlockCount)
			{
				lpFSMBlock = GBPTR2PTR(LPGBFSMBLOCK, GBPTR2PTR(LPBYTE, lpFSMPage) + sizeof(GBFSMPAGE) + lpFSMPage->dwAllocedCount*lpFSMAlloc->dwFSMBlockSize);
			}
			else
			{
				lpFSMBlock = GBPTR2PTR(LPGBFSMBLOCK, lpFSMPage->FreeFSMBlockList.lpNext);
				GBDelListItem(&lpFSMBlock->FreeFSMBlockListItem);
				--lpFSMPage->dwFreeFSMBlockCount;
			}
			lpFSMBlock->lpFSMAlloc = lpFSMAlloc;
			lpFSMBlock->lpFSMPage = lpFSMPage;

			// 增加已分配数。
			++lpFSMAlloc->dwAllocedCount;
			if (++lpFSMPage->dwAllocedCount == lpFSMAlloc->dwAllocCountOnce)
			{
				GBDelListItem(&lpFSMPage->FSMPageListItem);
				GBInsertBeforeListItem(&lpFSMAlloc->FSMPageList, &lpFSMPage->FSMPageListItem);
			}

			// 根据 FSM 块获取 FSM。
			return GBGetFSMFromFSMBlock(lpFSMBlock);
		}
	}

	// 这时已有 FSM 页都不能分配 FSM 块，创建新 FSM 页。
	LPGBFSMPAGE lpFSMPage = GBPTR2PTR(LPGBFSMPAGE, malloc(sizeof(GBFSMPAGE) + lpFSMAlloc->dwAllocCountOnce*lpFSMAlloc->dwFSMBlockSize));
	if (NULL == lpFSMPage)
	{
		GBSetLastError(GB_EC_NO_MEMORY);
		return NULL;
	}

	// 初始化新 FSM 页。
	lpFSMPage->dwAllocedCount = 0;
	lpFSMPage->dwFreeFSMBlockCount = 0;
	GBInitList(&lpFSMPage->FreeFSMBlockList);
	GBInsertAfterListItem(&lpFSMAlloc->FSMPageList, &lpFSMPage->FSMPageListItem);
	++lpFSMAlloc->dwFSMPageCount;

	// 从新 FSM 页中分配 FSM 块。
	LPGBFSMBLOCK lpFSMBlock = GBPTR2PTR(LPGBFSMBLOCK, GBPTR2PTR(LPBYTE, lpFSMPage) + sizeof(GBFSMPAGE) + lpFSMPage->dwAllocedCount*lpFSMAlloc->dwFSMBlockSize);
	lpFSMBlock->lpFSMAlloc = lpFSMAlloc;
	lpFSMBlock->lpFSMPage = lpFSMPage;

	// 增加已分配数。
	++lpFSMAlloc->dwAllocedCount;
	if (++lpFSMPage->dwAllocedCount == lpFSMAlloc->dwAllocCountOnce)
	{
		GBDelListItem(&lpFSMPage->FSMPageListItem);
		GBInsertBeforeListItem(&lpFSMAlloc->FSMPageList, &lpFSMPage->FSMPageListItem);
	}

	// 根据 FSM 块获取 FSM。
	return GBGetFSMFromFSMBlock(lpFSMBlock);
}

// 释放 FSM。
GBEXTERN_C GBAPI void GBSTDCALL GBFreeFSM(
	LPVOID lpFSM								// IN：FSM。
)
{
	// 根据 FSM 获取 FSM 块。
	LPGBFSMBLOCK lpFSMBlock = GBGetFSMBlockFromFSM(lpFSM);

	// 获取 FSM 块所属的 FSM 分配器及 FSM 页。
	LPGBFSMALLOC lpFSMAlloc = lpFSMBlock->lpFSMAlloc;
	LPGBFSMPAGE lpFSMPage = lpFSMBlock->lpFSMPage;

	// 减少已分配数。
	--lpFSMAlloc->dwAllocedCount;
	--lpFSMPage->dwAllocedCount;
	GBDelListItem(&lpFSMPage->FSMPageListItem);
	GBInsertAfterListItem(&lpFSMAlloc->FSMPageList, &lpFSMPage->FSMPageListItem);

	// 加入 FSM 页的空闲 FSM 块链表。
	GBInsertAfterListItem(&lpFSMPage->FreeFSMBlockList, &lpFSMBlock->FreeFSMBlockListItem);
	++lpFSMPage->dwFreeFSMBlockCount;

	// 当 FSM 页的已分配数为 0 时，判断此 FSM 页是否能被释放。
	if (0 == lpFSMPage->dwAllocedCount)
	{
		// 目前的判断方法是当剩余可分配数大于等于一次分配数的一半时就能够被释放。
		if ((lpFSMAlloc->dwFSMPageCount - 1)*lpFSMAlloc->dwAllocCountOnce >= ((lpFSMAlloc->dwAllocCountOnce + 1) >> 1))
		{
			GBDelListItem(&lpFSMPage->FSMPageListItem);
			--lpFSMAlloc->dwFSMPageCount;
			free(lpFSMPage);
		}
	}
}

// 判断是否 FSM。
GBEXTERN_C GBAPI BOOL GBSTDCALL GBIsFSM(
	LPVOID lpFSM								// IN：FSM。
)
{
	__try
	{
		// 根据 FSM 获取 FSM 块。
		LPGBFSMBLOCK lpFSMBlock = GBGetFSMBlockFromFSM(lpFSM);

		// 获取 FSM 块所属的 FSM 分配器及 FSM 页。
		LPGBFSMALLOC lpFSMAlloc = lpFSMBlock->lpFSMAlloc;
		LPGBFSMPAGE lpFSMPage = lpFSMBlock->lpFSMPage;

		// 判断 FSM 块所属的 FSM 分配器是否 FSM 分配器。
		if (!GBIsFSMAlloc(GBPTR2PTR(HANDLE, lpFSMAlloc)))
			return FALSE;

		// 判断 FSM 块所属的 FSM 页在 FSM 分配器中是否存在。
		LPGBLIST lpListItem = lpFSMAlloc->FSMPageList.lpNext;
		while (lpListItem != &lpFSMAlloc->FSMPageList)
		{
			if (GBPTR2PTR(LPGBFSMPAGE, lpListItem) == lpFSMPage)
			{
				// 存在时判断 FSM 块在 FSM 页中是否存在。
				// 注：这时无需判断 FSM 块是否在 FSM 页的空闲 FSM 块链表中，因为空闲 FSM 块的指针为空闲 FSM 块链表项指针。
				DWORD dwOffset = GBINT2INT(DWORD, GBPTR2PTR(LPBYTE, lpFSMBlock) - GBPTR2PTR(LPBYTE, lpFSMPage) - sizeof(GBFSMPAGE));
				return (0 == dwOffset%lpFSMAlloc->dwFSMBlockSize && dwOffset / lpFSMAlloc->dwFSMBlockSize < lpFSMPage->dwAllocedCount + lpFSMPage->dwFreeFSMBlockCount);
			}
			lpListItem = lpListItem->lpNext;
		}

		return FALSE;
	}
	__except (GBHandleException(EXCEPTION_ACCESS_VIOLATION))
	{
		return FALSE;
	}
}

// 锁定并获取已分配 FSM 数。
GBEXTERN_C GBAPI DWORD GBSTDCALL GBLockedGetAllocedFSMCount(
	HANDLE hFSMAlloc							// IN：FSM 分配器。
)
{
	// 进入临界区。
	::EnterCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	// 获取已分配 FSM 数。
	DWORD dwAllocedFSMCount = GBGetAllocedFSMCount(hFSMAlloc);

	// 离开临界区。
	::LeaveCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	return dwAllocedFSMCount;
}

// 锁定并分配 FSM。
GBEXTERN_C GBAPI LPVOID GBSTDCALL GBLockedAllocFSM(
	HANDLE hFSMAlloc							// IN：FSM 分配器。
)
{
	// 进入临界区。
	::EnterCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	// 分配 FSM。
	LPVOID lpFSM = GBAllocFSM(hFSMAlloc);

	// 离开临界区。
	::LeaveCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	return lpFSM;
}

// 锁定并释放 FSM。
GBEXTERN_C GBAPI void GBSTDCALL GBLockedFreeFSM(
	LPVOID lpFSM								// IN：FSM。
)
{
	// 进入临界区。
	::EnterCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	// 释放 FSM。
	GBFreeFSM(lpFSM);

	// 离开临界区。
	::LeaveCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);
}

// 锁定并判断是否 FSM。
GBEXTERN_C GBAPI BOOL GBSTDCALL GBLockedIsFSM(
	LPVOID lpFSM								// IN：FSM。
)
{
	// 进入临界区。
	::EnterCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	// 判断是否 FSM。
	BOOL bIsFSM = GBIsFSM(lpFSM);

	// 离开临界区。
	::LeaveCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	return bIsFSM;
}

#pragma endregion
