// Copyright (c) 2016 甘波。All rights reserved。

#if !defined (_GB_OBJ_LOCK_INNER_H_GB_20161027_)
#define _GB_OBJ_LOCK_INNER_H_GB_20161027_

#pragma once

#include "gblist.h"

#pragma pack(push, GBPACK)

// 对象锁信息。
typedef struct _GBOBJLOCKINFO
{
	GBLIST ObjLockInfoListItem;					// 对象锁信息链表项。
	CRITICAL_SECTION CS;						// 临界区。
	LPCVOID lpObj;								// 对象。
	DWORD dwOwningThreadId;						// 所属线程 ID。
	WORD wRefCount;								// 引用计数。
	WORD wLockCount;							// 锁定计数。注：锁定计数小于等于引用计数。
} GBOBJLOCKINFO, *LPGBOBJLOCKINFO;
typedef struct _GBOBJLOCKINFO const *LPCGBOBJLOCKINFO;

// 对象锁管理器 Hash 表大小。
#define  GB_OBJ_LOCK_MGR_HASH_TABLE_SIZE 37

// 对象锁管理器。
typedef struct _GBOBJLOCKMGR
{
	CRITICAL_SECTION CS;												// 临界区。
	HANDLE hObjLockInfoAlloc;											// 对象锁信息分配器。
	GBLIST FreeObjLockInfoList;											// 空闲对象锁信息链表。
	GBLIST lpInUseObjLockInfoList[GB_OBJ_LOCK_MGR_HASH_TABLE_SIZE];		// 使用中对象锁信息链表。
} GBOBJLOCKMGR, *LPGBOBJLOCKMGR;
typedef struct _GBOBJLOCKMGR const *LPCGBOBJLOCKMGR;

#pragma pack(pop)

// 计算对象 Hash 表索引。
#define GBCalcObjHashTableIndex(lpObj) GBINT2INT(DWORD, (GBPTR2INT(UINT_PTR, lpObj) >> 3) % GB_OBJ_LOCK_MGR_HASH_TABLE_SIZE)

// 获取对象锁信息。
// 返回值：对象锁信息。失败时返回 NULL。
GBEXTERN_C LPGBOBJLOCKINFO GBSTDCALL GBGetObjLockInfo(
	LPGBOBJLOCKMGR lpObjLockMgr,				// IN：对象锁管理器。
	LPCVOID lpObj,								// IN：对象。
	BOOL bCreateNew								// IN：是否创建新对象锁信息。
);

// 释放对象锁信息。
GBEXTERN_C void GBSTDCALL GBFreeObjLockInfo(
	LPGBOBJLOCKMGR lpObjLockMgr,				// IN：对象锁管理器。
	LPGBOBJLOCKINFO lpObjLockInfo				// IN：对象锁信息。
);

// 初始化对象锁管理器。
GBEXTERN_C BOOL GBSTDCALL GBInitObjLockMgr(
	LPGBOBJLOCKMGR lpObjLockMgr					// INOUT：对象锁管理器。
);

// 释放对象锁管理器。
GBEXTERN_C void GBSTDCALL GBReleaseObjLockMgr(
	LPGBOBJLOCKMGR lpObjLockMgr					// INOUT：对象锁管理器。
);

#endif
