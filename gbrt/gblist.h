// Copyright (c) 2016 �ʲ���All rights reserved��

#if !defined (_GB_LIST_H_GB_20161025_)
#define _GB_LIST_H_GB_20161025_

#pragma once

#include "gbdef.h"

#pragma pack(push, GBPACK)

// �������
typedef struct _GBLIST
{
	struct _GBLIST *lpNext;						// ��һ�����
	struct _GBLIST *lpPrev;						// ǰһ�����
} GBLIST, *LPGBLIST;
typedef struct _GBLIST const *LPCGBLIST;

#pragma pack(pop)

// ����������뵽Ŀ���������
GBEXTERN_C GBINLINE void GBSTDCALL GBInsertAfterListItem(
	LPGBLIST lpDesListItem,						// INOUT��Ŀ�������
	LPGBLIST lpListItem							// INOUT�������
)
{
	lpListItem->lpNext = lpDesListItem->lpNext;
	lpListItem->lpPrev = lpDesListItem;
	lpDesListItem->lpNext->lpPrev = lpListItem;
	lpDesListItem->lpNext = lpListItem;
}

// ����������뵽Ŀ��������ǰ��
GBEXTERN_C GBINLINE void GBSTDCALL GBInsertBeforeListItem(
	LPGBLIST lpDesListItem,						// INOUT��Ŀ�������
	LPGBLIST lpListItem							// INOUT�������
)
{
	lpListItem->lpPrev = lpDesListItem->lpPrev;
	lpListItem->lpNext = lpDesListItem;
	lpDesListItem->lpPrev->lpNext = lpListItem;
	lpDesListItem->lpPrev = lpListItem;
}

// ɾ�������
GBEXTERN_C GBINLINE void GBSTDCALL GBDelListItem(
	LPGBLIST lpListItem							// INOUT�������
)
{
	lpListItem->lpNext->lpPrev = lpListItem->lpPrev;
	lpListItem->lpPrev->lpNext = lpListItem->lpNext;
}

// �ж��Ƿ������
#define GBIsListEmpty(lpList) ((lpList)->lpNext == (lpList))

// ��ʼ������
GBEXTERN_C GBINLINE void GBSTDCALL GBInitList(
	LPGBLIST lpList								// INOUT������
)
{
	lpList->lpNext = lpList->lpPrev = lpList;
}

// ��������뵽Ŀ���������
GBEXTERN_C GBINLINE void GBSTDCALL GBInsertAfterList(
	LPGBLIST lpDesListItem,						// INOUT��Ŀ�������
	LPGBLIST lpList								// INOUT������
)
{
	lpList->lpPrev->lpNext = lpDesListItem->lpNext;
	lpList->lpNext->lpPrev = lpDesListItem;
	lpDesListItem->lpNext->lpPrev = lpList->lpPrev;
	lpDesListItem->lpNext = lpList->lpNext;
	lpList->lpNext = lpList->lpPrev = lpList;
}

// ��������뵽Ŀ��������ǰ��
GBEXTERN_C GBINLINE void GBSTDCALL GBInsertBeforeList(
	LPGBLIST lpDesListItem,						// INOUT��Ŀ�������
	LPGBLIST lpList								// INOUT������
)
{
	lpList->lpNext->lpPrev = lpDesListItem->lpPrev;
	lpList->lpPrev->lpNext = lpDesListItem;
	lpDesListItem->lpPrev->lpNext = lpList->lpNext;
	lpDesListItem->lpPrev = lpList->lpPrev;
	lpList->lpNext = lpList->lpPrev = lpList;
}

#endif
