// Copyright (c) 2016 甘波。All rights reserved。

#include "stdafx.h"

#pragma region "导出函数。"

//////////////////////////////////////////////////////////////////////////
// 导出函数。

// 锁定对象。
// 返回值：对象锁句柄。失败时返回 NULL。
GBEXTERN_C GBAPI HANDLE GBSTDCALL GBLockObj(
	LPCVOID lpObj								// IN：对象。
)
{
	// 获取对象锁信息。
	LPGBOBJLOCKINFO lpObjLockInfo = GBGetObjLockInfo(&ext_ModuleData.m_ObjLockMgr, lpObj, TRUE);
	if (NULL == lpObjLockInfo)
		return NULL;

	// 锁定对象。
	::EnterCriticalSection(&lpObjLockInfo->CS);

	// 增加锁定计数。如果增加前锁定计数为 0，则设置所属线程 ID。
	if (0 == lpObjLockInfo->wLockCount++)
		InterlockedExchange(GBPTR2PTR(LONG volatile *, &lpObjLockInfo->dwOwningThreadId), (LONG)::GetCurrentThreadId());

	return GBPTR2PTR(HANDLE, lpObjLockInfo);
}

// 尝试锁定对象。
// 返回值：对象锁句柄。失败时返回 NULL；对象被其它线程锁定时返回 INVALID_HANDLE_VALUE。
GBEXTERN_C GBAPI HANDLE GBSTDCALL GBTryLockObj(
	LPCVOID lpObj								// IN：对象。
)
{
	// 获取对象锁信息。
	LPGBOBJLOCKINFO lpObjLockInfo = GBGetObjLockInfo(&ext_ModuleData.m_ObjLockMgr, lpObj, TRUE);
	if (NULL == lpObjLockInfo)
		return NULL;

	// 尝试锁定对象。
	if (::TryEnterCriticalSection(&lpObjLockInfo->CS))
	{
		// 锁定成功时增加锁定计数。如果增加前锁定计数为 0，则设置所属线程 ID。
		if (0 == lpObjLockInfo->wLockCount++)
			InterlockedExchange(GBPTR2PTR(LONG volatile *, &lpObjLockInfo->dwOwningThreadId), (LONG)::GetCurrentThreadId());

		return GBPTR2PTR(HANDLE, lpObjLockInfo);
	}
	else
	{
		// 锁定失败时对象被其它线程锁定。这时释放对象锁信息并返回 INVALID_HANDLE_VALUE。
		GBFreeObjLockInfo(&ext_ModuleData.m_ObjLockMgr, lpObjLockInfo);

		return INVALID_HANDLE_VALUE;
	}
}

// 解锁对象。
// 注：此函数慢于根据对象锁句柄解锁对象函数。
GBEXTERN_C GBAPI void GBSTDCALL GBUnlockObj(
	LPCVOID lpObj								// IN：对象。
)
{
	// 获取对象锁信息。
	LPGBOBJLOCKINFO lpObjLockInfo = GBGetObjLockInfo(&ext_ModuleData.m_ObjLockMgr, lpObj, FALSE);
	if (NULL == lpObjLockInfo)
		return;

	// 只有所属线程能够解锁对象。
	if (::InterlockedCompareExchange(GBPTR2PTR(LONG volatile *, &lpObjLockInfo->dwOwningThreadId), 0, 0) == (LONG)::GetCurrentThreadId())
	{
		// 减少锁定计数。如果减少后锁定计数为 0，则重置所属线程 ID。
		if (0 == --lpObjLockInfo->wLockCount)
			InterlockedExchange(GBPTR2PTR(LONG volatile *, &lpObjLockInfo->dwOwningThreadId), 0);

		// 解锁对象。
		::LeaveCriticalSection(&lpObjLockInfo->CS);

		// 释放对象锁信息。
		GBFreeObjLockInfo(&ext_ModuleData.m_ObjLockMgr, lpObjLockInfo);
	}
}

// 根据对象锁句柄解锁对象。
GBEXTERN_C GBAPI void GBSTDCALL GBUnlockObjByObjLock(
	HANDLE hObjLock								// IN：对象锁句柄。
)
{
	// 获取对象锁信息。
	LPGBOBJLOCKINFO lpObjLockInfo = GBPTR2PTR(LPGBOBJLOCKINFO, hObjLock);

	// 只有所属线程能够解锁对象。
	if (::InterlockedCompareExchange(GBPTR2PTR(LPLONG, &lpObjLockInfo->dwOwningThreadId), 0, 0) == (LONG)::GetCurrentThreadId())
	{
		// 减少锁定计数。如果减少后锁定计数为 0，则重置所属线程 ID。
		if (0 == --lpObjLockInfo->wLockCount)
			InterlockedExchange(GBPTR2PTR(LONG volatile *, &lpObjLockInfo->dwOwningThreadId), 0);

		// 解锁对象。
		::LeaveCriticalSection(&lpObjLockInfo->CS);

		// 释放对象锁信息。
		GBFreeObjLockInfo(&ext_ModuleData.m_ObjLockMgr, lpObjLockInfo);
	}
}

// 判断是否对象锁。
GBEXTERN_C GBAPI BOOL GBSTDCALL GBIsObjLock(
	HANDLE hObjLock								// IN：对象锁句柄。
)
{
	// 进入临界区。
	::EnterCriticalSection(&ext_ModuleData.m_ObjLockMgr.CS);

	__try
	{
		// 计算对象 Hash 表索引。
		DWORD dwIndex = GBCalcObjHashTableIndex(GBPTR2PTR(LPGBOBJLOCKINFO, hObjLock)->lpObj);

		// 判断是否对象锁。
		BOOL bIsObjLock = FALSE;
		LPGBLIST lpList = &ext_ModuleData.m_ObjLockMgr.lpInUseObjLockInfoList[dwIndex];
		LPGBLIST lpListItem = lpList->lpNext;
		while (lpListItem != lpList)
		{
			if (GBPTR2PTR(HANDLE, lpListItem) == hObjLock)
			{
				bIsObjLock = TRUE;
				break;
			}
			lpListItem = lpListItem->lpNext;
		}

		// 离开临界区。
		::LeaveCriticalSection(&ext_ModuleData.m_ObjLockMgr.CS);

		return bIsObjLock;
	}
	__except (GBHandleException(EXCEPTION_ACCESS_VIOLATION))
	{
		// 离开临界区。
		::LeaveCriticalSection(&ext_ModuleData.m_ObjLockMgr.CS);

		return FALSE;
	}
}

// 判断对象是否被锁定。
GBEXTERN_C GBAPI BOOL GBSTDCALL GBIsObjLocked(
	LPCVOID lpObj								// IN：对象。
)
{
	// 进入临界区。
	::EnterCriticalSection(&ext_ModuleData.m_ObjLockMgr.CS);

	// 计算对象 Hash 表索引。
	DWORD dwIndex = GBCalcObjHashTableIndex(lpObj);

	// 判断对象是否被锁定。
	BOOL bIsObjLocked = FALSE;
	LPGBLIST lpList = &ext_ModuleData.m_ObjLockMgr.lpInUseObjLockInfoList[dwIndex];
	LPGBLIST lpListItem = lpList->lpNext;
	while (lpListItem != lpList)
	{
		if (GBPTR2PTR(LPGBOBJLOCKINFO, lpListItem)->lpObj == lpObj)
		{
			bIsObjLocked = TRUE;
			break;
		}
		lpListItem = lpListItem->lpNext;
	}

	// 离开临界区。
	::LeaveCriticalSection(&ext_ModuleData.m_ObjLockMgr.CS);

	return bIsObjLocked;
}

#pragma endregion
