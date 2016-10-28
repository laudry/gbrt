// Copyright (c) 2016 甘波。All rights reserved。

#include "stdafx.h"

#pragma region "内部函数。"

//////////////////////////////////////////////////////////////////////////
// 内部函数。

// 从缓存管理器分配缓存。
// 分配原则：先尝试使用缓存管理器的空闲缓存，没有空闲缓存时直接分配。
GBEXTERN_C LPGBBUFF GBSTDCALL GBAllocBuffFromBuffMgr(
	LPGBBUFFMGR lpBuffMgr						// IN：缓存管理器。
)
{
	// 分配缓存。
	LPGBBUFF lpBuff;							// 返回值。
	if (NULL != lpBuffMgr && 0 != lpBuffMgr->uFreeBuffCount)
	{
		lpBuff = GBPTR2PTR(LPGBBUFF, lpBuffMgr->FreeBuffList.lpNext);
		GBDelListItem(&lpBuff->BuffListItem);
		--lpBuffMgr->uFreeBuffCount;
	}
	else
		lpBuff = GBPTR2PTR(LPGBBUFF, malloc(sizeof(GBBUFF) + GB_DEFAULT_BUFF_CAPACITY));

	return lpBuff;
}

// 释放缓存到缓存管理器。
// 释放原则：先尝试加入到缓存管理器的空闲缓存链表，无法加入时直接释放。
GBEXTERN_C void GBSTDCALL GBFreeBuffToBuffMgr(
	LPGBBUFFMGR lpBuffMgr,						// IN：缓存管理器。
	LPGBBUFF lpBuff								// IN：缓存。
)
{
	// 释放缓存。
	if (NULL != lpBuffMgr && lpBuffMgr->uFreeBuffCount < GB_MAX_FREE_BUFF_COUNT)
	{
		GBInsertAfterListItem(&lpBuffMgr->FreeBuffList, &lpBuff->BuffListItem);
		++lpBuffMgr->uFreeBuffCount;
	}
	else
		free(lpBuff);
}

// 初始化缓存管理器。
GBEXTERN_C void GBSTDCALL GBInitBuffMgr(
	LPGBBUFFMGR lpBuffMgr						// INOUT：缓存管理器。
)
{
	// 初始化缓存管理器。
	lpBuffMgr->uFreeBuffCount = 0;
	GBInitList(&lpBuffMgr->FreeBuffList);
}

// 释放缓存管理器。
GBEXTERN_C void GBSTDCALL GBReleaseBuffMgr(
	LPGBBUFFMGR lpBuffMgr						// INOUT：缓存管理器。
)
{
	// 释放空闲缓存。
	LPGBLIST lpListItem = lpBuffMgr->FreeBuffList.lpNext;
	while (lpListItem != &lpBuffMgr->FreeBuffList)
	{
		LPGBBUFF lpBuff = GBPTR2PTR(LPGBBUFF, lpListItem);
		lpListItem = lpListItem->lpNext;
		GBDelListItem(&lpBuff->BuffListItem);
		--lpBuffMgr->uFreeBuffCount;
		free(lpBuff);
	}
}

#pragma endregion
