// Copyright (c) 2016 �ʲ���All rights reserved��

#include "stdafx.h"

#pragma region "�ڲ�������"

//////////////////////////////////////////////////////////////////////////
// �ڲ�������

// �ͷ� FSM ��������
GBEXTERN_C void GBSTDCALL GBFreeFSMAlloc(
	LPGBFSMALLOC lpFSMAlloc						// IN��FSM ��������
)
{
	// �ͷ� FSM ���������е� FSM ҳ��
	LPGBLIST lpListItem = lpFSMAlloc->FSMPageList.lpNext;
	while (lpListItem != &lpFSMAlloc->FSMPageList)
	{
		LPGBFSMPAGE lpFSMPage = GBPTR2PTR(LPGBFSMPAGE, lpListItem);
		lpListItem = lpListItem->lpNext;
		GBDelListItem(&lpFSMPage->FSMPageListItem);
		--lpFSMAlloc->dwFSMPageCount;
		free(lpFSMPage);
	}

	// �ͷ� FSM ��������
	GBDelListItem(&lpFSMAlloc->FSMAllocListItem);
	GBFreeFSM(lpFSMAlloc);
}

// ��ʼ�� FSM ��������������
GBEXTERN_C BOOL GBSTDCALL GBInitFSMAllocMgr(
	LPGBFSMALLOCMGR lpFSMAllocMgr				// IN��FSM ��������������
)
{
	// ��ʼ���ٽ�����
	if (!GBInitializeCriticalSection(&lpFSMAllocMgr->CS))
		return FALSE;

	// ��ʼ�� FSM ��������
	GBInitList(&lpFSMAllocMgr->FSMAlloc.FSMAllocListItem);
	lpFSMAllocMgr->FSMAlloc.dwFSMBlockSize = sizeof(GBFSMBLOCK) + sizeof(GBFSMALLOC);
	lpFSMAllocMgr->FSMAlloc.dwAllocCountOnce = 64;
	lpFSMAllocMgr->FSMAlloc.dwAllocedCount = 0;
	lpFSMAllocMgr->FSMAlloc.dwFSMPageCount = 0;
	GBInitList(&lpFSMAllocMgr->FSMAlloc.FSMPageList);

	return TRUE;
}

// �ͷ� FSM ��������������
GBEXTERN_C void GBSTDCALL GBReleaseFSMAllocMgr(
	LPGBFSMALLOCMGR lpFSMAllocMgr				// INOUT��FSM ��������������
)
{
	// �����ٽ�����
	::EnterCriticalSection(&lpFSMAllocMgr->CS);

	// �ͷ��Ѵ����� FSM ��������
	LPGBLIST lpListItem = lpFSMAllocMgr->FSMAlloc.FSMAllocListItem.lpNext;
	while (lpListItem != &lpFSMAllocMgr->FSMAlloc.FSMAllocListItem)
	{
		LPGBFSMALLOC lpFSMAlloc = GBPTR2PTR(LPGBFSMALLOC, lpListItem);
		lpListItem = lpListItem->lpNext;
		GBFreeFSMAlloc(lpFSMAlloc);
	}

	// �ͷ� FSM ���������е� FSM ҳ��
	lpListItem = lpFSMAllocMgr->FSMAlloc.FSMPageList.lpNext;
	while (lpListItem != &lpFSMAllocMgr->FSMAlloc.FSMPageList)
	{
		LPGBFSMPAGE lpFSMPage = GBPTR2PTR(LPGBFSMPAGE, lpListItem);
		lpListItem = lpListItem->lpNext;
		GBDelListItem(&lpFSMPage->FSMPageListItem);
		--lpFSMAllocMgr->FSMAlloc.dwFSMPageCount;
		free(lpFSMPage);
	}

	// ɾ���ٽ�����
	::DeleteCriticalSection(&lpFSMAllocMgr->CS);
}

#pragma endregion
