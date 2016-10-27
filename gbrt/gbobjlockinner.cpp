// Copyright (c) 2016 甘波。All rights reserved。

#include "stdafx.h"

#pragma region "内部函数。"

//////////////////////////////////////////////////////////////////////////
// 内部函数。

// 获取对象锁信息。
// 返回值：对象锁信息。失败时返回 NULL。
GBEXTERN_C LPGBOBJLOCKINFO GBSTDCALL GBGetObjLockInfo(
	LPGBOBJLOCKMGR lpObjLockMgr,				// IN：对象锁管理器。
	LPCVOID lpObj,								// IN：对象。
	BOOL bCreateNew								// IN：是否创建新对象锁信息。
)
{
	LPGBOBJLOCKINFO lpObjLockInfo;				// 返回值。

	// 进入临界区。
	::EnterCriticalSection(&lpObjLockMgr->CS);

	// 计算对象 Hash 表索引。
	DWORD dwIndex = GBCalcObjHashTableIndex(lpObj);

	// 先查找使用中对象锁信息链表。
	LPGBLIST lpList = &lpObjLockMgr->lpInUseObjLockInfoList[dwIndex];
	LPGBLIST lpListItem = lpList->lpNext;
	while (lpListItem != lpList)
	{
		lpObjLockInfo = GBPTR2PTR(LPGBOBJLOCKINFO, lpListItem);
		if (lpObjLockInfo->lpObj == lpObj)
		{
			if (bCreateNew)
				++lpObjLockInfo->wRefCount;
			goto tagOver;
		}
		lpListItem = lpListItem->lpNext;
	}

	// 这时不在使用中对象锁信息链表中，需要创建新对象锁信息，判断是否创建。
	if (!bCreateNew)
		lpObjLockInfo = NULL;
	else
	{
		if (!GBIsListEmpty(&lpObjLockMgr->FreeObjLockInfoList))
		{
			// 需要创建时如果存在空闲对象锁信息，则使用之。
			lpObjLockInfo = GBPTR2PTR(LPGBOBJLOCKINFO, lpObjLockMgr->FreeObjLockInfoList.lpNext);
			GBDelListItem(&lpObjLockInfo->ObjLockInfoListItem);
			lpObjLockInfo->lpObj = lpObj;
			lpObjLockInfo->wRefCount = 1;
			GBInsertAfterListItem(lpList, &lpObjLockInfo->ObjLockInfoListItem);
		}
		else
		{
			// 需要创建时如果不存在空闲对象锁信息，则新分配。
			lpObjLockInfo = GBPTR2PTR(LPGBOBJLOCKINFO, GBAllocFSM(lpObjLockMgr->hObjLockInfoAlloc));
			if (NULL != lpObjLockInfo)
			{
				if (!GBInitializeCriticalSection(&lpObjLockInfo->CS))
				{
					GBSetLastError(GB_EC_NO_MEMORY);
					GBFreeFSM(lpObjLockInfo);
					lpObjLockInfo = NULL;
				}
				else
				{
					lpObjLockInfo->lpObj = lpObj;
					lpObjLockInfo->wRefCount = 1;
					lpObjLockInfo->wLockCount = 0;
					lpObjLockInfo->dwOwningThreadId = 0;
					GBInsertAfterListItem(lpList, &lpObjLockInfo->ObjLockInfoListItem);
				}
			}
		}
	}

tagOver:
	// 离开临界区。
	::LeaveCriticalSection(&lpObjLockMgr->CS);

	return lpObjLockInfo;
}

// 释放对象锁信息。
GBEXTERN_C void GBSTDCALL GBFreeObjLockInfo(
	LPGBOBJLOCKMGR lpObjLockMgr,				// IN：对象锁管理器。
	LPGBOBJLOCKINFO lpObjLockInfo				// IN：对象锁信息。
)
{
	// 进入临界区。
	::EnterCriticalSection(&lpObjLockMgr->CS);

	// 减少引用计数。如果减少后引用计数为 0，则需要从使用中对象锁信息链表中删除。
	if (0 == --lpObjLockInfo->wRefCount)
	{
		// 从使用中对象锁信息链表中删除。
		GBDelListItem(&lpObjLockInfo->ObjLockInfoListItem);

		// 根据情况判断是加入空闲对象锁信息链表还是直接释放。
		if (GBGetAllocedFSMCount(lpObjLockMgr->hObjLockInfoAlloc) <= 32)
		{
			// 当对象锁信息数小于等于一次分配数时加入空闲对象锁信息链表。
			GBInsertAfterListItem(&lpObjLockMgr->FreeObjLockInfoList, &lpObjLockInfo->ObjLockInfoListItem);
		}
		else
		{
			// 否则直接释放。
			::DeleteCriticalSection(&lpObjLockInfo->CS);
			GBFreeFSM(lpObjLockInfo);
		}
	}

	// 离开临界区。
	::LeaveCriticalSection(&lpObjLockMgr->CS);
}

// 初始化对象锁管理器。
GBEXTERN_C BOOL GBSTDCALL GBInitObjLockMgr(
	LPGBOBJLOCKMGR lpObjLockMgr					// INOUT：对象锁管理器。
)
{
	// 初始化临界区。
	if (!GBInitializeCriticalSection(&lpObjLockMgr->CS))
		return FALSE;

	// 创建对象锁信息分配器。
	lpObjLockMgr->hObjLockInfoAlloc = GBCreateFSMAlloc(sizeof(GBOBJLOCKINFO), 32);
	if (NULL == lpObjLockMgr->hObjLockInfoAlloc)
	{
		::DeleteCriticalSection(&lpObjLockMgr->CS);
		return FALSE;
	}

	// 初始化所有链表。
	GBInitList(&lpObjLockMgr->FreeObjLockInfoList);
	for (DWORD i = 0; i != GB_OBJ_LOCK_MGR_HASH_TABLE_SIZE; ++i)
		GBInitList(&lpObjLockMgr->lpInUseObjLockInfoList[i]);

	return TRUE;
}

// 释放对象锁管理器。
GBEXTERN_C void GBSTDCALL GBReleaseObjLockMgr(
	LPGBOBJLOCKMGR lpObjLockMgr					// INOUT：对象锁管理器。
)
{
	// 进入临界区。
	::EnterCriticalSection(&lpObjLockMgr->CS);

	// 释放使用中对象锁信息。
	for (DWORD i = 0; i != GB_OBJ_LOCK_MGR_HASH_TABLE_SIZE; ++i)
	{
		LPGBLIST lpList = &lpObjLockMgr->lpInUseObjLockInfoList[i];
		LPGBLIST lpListItem = lpList->lpNext;
		while (lpListItem != lpList)
		{
			LPGBOBJLOCKINFO lpObjLockInfo = GBPTR2PTR(LPGBOBJLOCKINFO, lpListItem);
			lpListItem = lpListItem->lpNext;
			GBDelListItem(&lpObjLockInfo->ObjLockInfoListItem);
			::DeleteCriticalSection(&lpObjLockInfo->CS);
			GBFreeFSM(lpObjLockInfo);
		}
	}

	// 释放空闲对象锁信息。
	LPGBLIST lpListItem = lpObjLockMgr->FreeObjLockInfoList.lpNext;
	while (lpListItem != &lpObjLockMgr->FreeObjLockInfoList)
	{
		LPGBOBJLOCKINFO lpObjLockInfo = GBPTR2PTR(LPGBOBJLOCKINFO, lpListItem);
		lpListItem = lpListItem->lpNext;
		GBDelListItem(&lpObjLockInfo->ObjLockInfoListItem);
		::DeleteCriticalSection(&lpObjLockInfo->CS);
		GBFreeFSM(lpObjLockInfo);
	}

	// 销毁对象锁信息分配器。
	GBDestroyFSMAlloc(lpObjLockMgr->hObjLockInfoAlloc);

	// 删除临界区。
	::DeleteCriticalSection(&lpObjLockMgr->CS);
}

#pragma endregion
