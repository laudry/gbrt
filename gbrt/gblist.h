// Copyright (c) 2016 甘波。All rights reserved。

#if !defined (_GB_LIST_H_GB_20161025_)
#define _GB_LIST_H_GB_20161025_

#pragma once

#include "gbdef.h"

#pragma pack(push, GBPACK)

// 链表（项）。
typedef struct _GBLIST
{
	struct _GBLIST *lpNext;						// 后一链表项。
	struct _GBLIST *lpPrev;						// 前一链表项。
} GBLIST, *LPGBLIST;
typedef struct _GBLIST const *LPCGBLIST;

#pragma pack(pop)

// 把链表项插入到目的链表项后。
GBEXTERN_C GBINLINE void GBSTDCALL GBInsertAfterListItem(
	LPGBLIST lpDesListItem,						// INOUT：目的链表项。
	LPGBLIST lpListItem							// INOUT：链表项。
)
{
	lpListItem->lpNext = lpDesListItem->lpNext;
	lpListItem->lpPrev = lpDesListItem;
	lpDesListItem->lpNext->lpPrev = lpListItem;
	lpDesListItem->lpNext = lpListItem;
}

// 把链表项插入到目的链表项前。
GBEXTERN_C GBINLINE void GBSTDCALL GBInsertBeforeListItem(
	LPGBLIST lpDesListItem,						// INOUT：目的链表项。
	LPGBLIST lpListItem							// INOUT：链表项。
)
{
	lpListItem->lpPrev = lpDesListItem->lpPrev;
	lpListItem->lpNext = lpDesListItem;
	lpDesListItem->lpPrev->lpNext = lpListItem;
	lpDesListItem->lpPrev = lpListItem;
}

// 删除链表项。
GBEXTERN_C GBINLINE void GBSTDCALL GBDelListItem(
	LPGBLIST lpListItem							// INOUT：链表项。
)
{
	lpListItem->lpNext->lpPrev = lpListItem->lpPrev;
	lpListItem->lpPrev->lpNext = lpListItem->lpNext;
}

// 判断是否空链表。
#define GBIsListEmpty(lpList) ((lpList)->lpNext == (lpList))

// 初始化链表。
GBEXTERN_C GBINLINE void GBSTDCALL GBInitList(
	LPGBLIST lpList								// INOUT：链表。
)
{
	lpList->lpNext = lpList->lpPrev = lpList;
}

// 把链表插入到目的链表项后。
GBEXTERN_C GBINLINE void GBSTDCALL GBInsertAfterList(
	LPGBLIST lpDesListItem,						// INOUT：目的链表项。
	LPGBLIST lpList								// INOUT：链表。
)
{
	lpList->lpPrev->lpNext = lpDesListItem->lpNext;
	lpList->lpNext->lpPrev = lpDesListItem;
	lpDesListItem->lpNext->lpPrev = lpList->lpPrev;
	lpDesListItem->lpNext = lpList->lpNext;
	lpList->lpNext = lpList->lpPrev = lpList;
}

// 把链表插入到目的链表项前。
GBEXTERN_C GBINLINE void GBSTDCALL GBInsertBeforeList(
	LPGBLIST lpDesListItem,						// INOUT：目的链表项。
	LPGBLIST lpList								// INOUT：链表。
)
{
	lpList->lpNext->lpPrev = lpDesListItem->lpPrev;
	lpList->lpPrev->lpNext = lpDesListItem;
	lpDesListItem->lpPrev->lpNext = lpList->lpNext;
	lpDesListItem->lpPrev = lpList->lpPrev;
	lpList->lpNext = lpList->lpPrev = lpList;
}

#endif
