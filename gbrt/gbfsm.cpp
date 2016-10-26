// Copyright (c) 2016 �ʲ���All rights reserved��

#include "stdafx.h"

#pragma region "����������"

//////////////////////////////////////////////////////////////////////////
// ����������

// ���� FSM ������������������
// ����ֵ������������ʧ��ʱ���� 0��
GBEXTERN_C GBAPI DWORD GBSTDCALL GBCalcMaxAllocCountOfFSMAlloc(
	DWORD dwFixedSize,							// IN���̶���С������Ϊ 0��
	DWORD dwAllocCountOnce						// IN��һ�η�����������Ϊ 0��
)
{
	// ������������
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

	// ���� FSM ���С��
	DWORD dwFSMBlockSize = sizeof(GBFSMBLOCK) + GBALIGNSIZE2(dwFixedSize, GBPACKSHIFT);

	// ����Ƿ������
	if ((GB_MAX_FSM_ALLOC_SIZE - sizeof(GBFSMPAGE)) / dwFSMBlockSize < dwAllocCountOnce)
	{
		GBSetLastError(GB_EC_OVERFLOW);
		return 0;
	}

	// ���� FSM ҳ��С��
	DWORD dwFSMPageSize = sizeof(GBFSMPAGE) + dwAllocCountOnce*dwFSMBlockSize;

	// ���� FSM ������������������
	return GB_MAX_FSM_ALLOC_SIZE / dwFSMPageSize * dwAllocCountOnce;
}

// ���� FSM ��������
// ����ֵ�� FSM �����������ʧ��ʱ���� NULL��
GBEXTERN_C GBAPI HANDLE GBSTDCALL GBCreateFSMAlloc(
	DWORD dwFixedSize,							// IN���̶���С������Ϊ 0��
	DWORD dwAllocCountOnce						// IN��һ�η�����������Ϊ 0��
)
{
	// ���� FSM ������������������
	if (0 == GBCalcMaxAllocCountOfFSMAlloc(dwFixedSize, dwAllocCountOnce))
		return NULL;

	// �����ٽ�����
	::EnterCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	// ���� FSM ��������
	LPGBFSMALLOC lpFSMAlloc = GBPTR2PTR(LPGBFSMALLOC, GBAllocFSM(GBPTR2PTR(HANDLE, &ext_ModuleData.m_FSMAllocMgr.FSMAlloc)));
	if (NULL != lpFSMAlloc)
	{
		// ��ʼ�� FSM ��������
		lpFSMAlloc->dwFSMBlockSize = sizeof(GBFSMBLOCK) + GBALIGNSIZE2(dwFixedSize, GBPACKSHIFT);
		lpFSMAlloc->dwAllocCountOnce = dwAllocCountOnce;
		lpFSMAlloc->dwAllocedCount = 0;
		lpFSMAlloc->dwFSMPageCount = 0;
		GBInitList(&lpFSMAlloc->FSMPageList);
		GBInsertAfterListItem(&ext_ModuleData.m_FSMAllocMgr.FSMAlloc.FSMAllocListItem, &lpFSMAlloc->FSMAllocListItem);
	}

	// �뿪�ٽ�����
	::LeaveCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	// ���ش����� FSM ��������
	return GBPTR2PTR(HANDLE, lpFSMAlloc);
}

// ���� FSM ��������
GBEXTERN_C GBAPI void GBSTDCALL GBDestroyFSMAlloc(
	HANDLE hFSMAlloc							// IN��FSM ��������
)
{
	// �����ٽ�����
	::EnterCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	// �ͷ� FSM ��������
	GBFreeFSMAlloc(GBPTR2PTR(LPGBFSMALLOC, hFSMAlloc));

	// �뿪�ٽ�����
	::LeaveCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);
}

// �ж��Ƿ� FSM ��������
GBEXTERN_C GBAPI BOOL GBSTDCALL GBIsFSMAlloc(
	HANDLE hFSMAlloc							// IN��FSM ��������
)
{
	// �����ٽ�����
	::EnterCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	// �ж��Ƿ� FSM ��������
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

	// �뿪�ٽ�����
	::LeaveCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	return bIsFSMAlloc;
}

// ��ȡ�ѷ��� FSM ����
GBEXTERN_C GBAPI DWORD GBSTDCALL GBGetAllocedFSMCount(
	HANDLE hFSMAlloc							// IN��FSM ��������
)
{
	return GBPTR2PTR(LPGBFSMALLOC, hFSMAlloc)->dwAllocedCount;
}

// ���� FSM��
// ����ֵ��FSM ָ�롣ʧ��ʱ���� NULL��
GBEXTERN_C GBAPI LPVOID GBSTDCALL GBAllocFSM(
	HANDLE hFSMAlloc							// IN��FSM ��������
)
{
	// ��ȡ FSM ��������
	LPGBFSMALLOC lpFSMAlloc = GBPTR2PTR(LPGBFSMALLOC, hFSMAlloc);

	// �ж� FSM ҳ�����Ƿ������
	if (!GBIsListEmpty(&lpFSMAlloc->FSMPageList))
	{
		// ��ȡ��һ�� FSM ҳ��
		LPGBFSMPAGE lpFSMPage = GBPTR2PTR(LPGBFSMPAGE, lpFSMAlloc->FSMPageList.lpNext);

		// �жϵ�һ�� FSM ҳ�ܷ���� FSM �顣
		// ע�������ҳ���ܷ��� FSM ��Ļ��������� FSM ҳ�����ܷ��� FSM �顣
		if (lpFSMPage->dwAllocedCount != lpFSMAlloc->dwAllocCountOnce)
		{
			// �ӵ�ǰ FSM ҳ�з��� FSM �顣
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

			// �����ѷ�������
			++lpFSMAlloc->dwAllocedCount;
			if (++lpFSMPage->dwAllocedCount == lpFSMAlloc->dwAllocCountOnce)
			{
				GBDelListItem(&lpFSMPage->FSMPageListItem);
				GBInsertBeforeListItem(&lpFSMAlloc->FSMPageList, &lpFSMPage->FSMPageListItem);
			}

			// ���� FSM ���ȡ FSM��
			return GBGetFSMFromFSMBlock(lpFSMBlock);
		}
	}

	// ��ʱ���� FSM ҳ�����ܷ��� FSM �飬������ FSM ҳ��
	LPGBFSMPAGE lpFSMPage = GBPTR2PTR(LPGBFSMPAGE, malloc(sizeof(GBFSMPAGE) + lpFSMAlloc->dwAllocCountOnce*lpFSMAlloc->dwFSMBlockSize));
	if (NULL == lpFSMPage)
	{
		GBSetLastError(GB_EC_NO_MEMORY);
		return NULL;
	}

	// ��ʼ���� FSM ҳ��
	lpFSMPage->dwAllocedCount = 0;
	lpFSMPage->dwFreeFSMBlockCount = 0;
	GBInitList(&lpFSMPage->FreeFSMBlockList);
	GBInsertAfterListItem(&lpFSMAlloc->FSMPageList, &lpFSMPage->FSMPageListItem);
	++lpFSMAlloc->dwFSMPageCount;

	// ���� FSM ҳ�з��� FSM �顣
	LPGBFSMBLOCK lpFSMBlock = GBPTR2PTR(LPGBFSMBLOCK, GBPTR2PTR(LPBYTE, lpFSMPage) + sizeof(GBFSMPAGE) + lpFSMPage->dwAllocedCount*lpFSMAlloc->dwFSMBlockSize);
	lpFSMBlock->lpFSMAlloc = lpFSMAlloc;
	lpFSMBlock->lpFSMPage = lpFSMPage;

	// �����ѷ�������
	++lpFSMAlloc->dwAllocedCount;
	if (++lpFSMPage->dwAllocedCount == lpFSMAlloc->dwAllocCountOnce)
	{
		GBDelListItem(&lpFSMPage->FSMPageListItem);
		GBInsertBeforeListItem(&lpFSMAlloc->FSMPageList, &lpFSMPage->FSMPageListItem);
	}

	// ���� FSM ���ȡ FSM��
	return GBGetFSMFromFSMBlock(lpFSMBlock);
}

// �ͷ� FSM��
GBEXTERN_C GBAPI void GBSTDCALL GBFreeFSM(
	LPVOID lpFSM								// IN��FSM��
)
{
	// ���� FSM ��ȡ FSM �顣
	LPGBFSMBLOCK lpFSMBlock = GBGetFSMBlockFromFSM(lpFSM);

	// ��ȡ FSM �������� FSM �������� FSM ҳ��
	LPGBFSMALLOC lpFSMAlloc = lpFSMBlock->lpFSMAlloc;
	LPGBFSMPAGE lpFSMPage = lpFSMBlock->lpFSMPage;

	// �����ѷ�������
	--lpFSMAlloc->dwAllocedCount;
	--lpFSMPage->dwAllocedCount;
	GBDelListItem(&lpFSMPage->FSMPageListItem);
	GBInsertAfterListItem(&lpFSMAlloc->FSMPageList, &lpFSMPage->FSMPageListItem);

	// ���� FSM ҳ�Ŀ��� FSM ������
	GBInsertAfterListItem(&lpFSMPage->FreeFSMBlockList, &lpFSMBlock->FreeFSMBlockListItem);
	++lpFSMPage->dwFreeFSMBlockCount;

	// �� FSM ҳ���ѷ�����Ϊ 0 ʱ���жϴ� FSM ҳ�Ƿ��ܱ��ͷš�
	if (0 == lpFSMPage->dwAllocedCount)
	{
		// Ŀǰ���жϷ����ǵ�ʣ��ɷ��������ڵ���һ�η�������һ��ʱ���ܹ����ͷš�
		if ((lpFSMAlloc->dwFSMPageCount - 1)*lpFSMAlloc->dwAllocCountOnce >= ((lpFSMAlloc->dwAllocCountOnce + 1) >> 1))
		{
			GBDelListItem(&lpFSMPage->FSMPageListItem);
			--lpFSMAlloc->dwFSMPageCount;
			free(lpFSMPage);
		}
	}
}

// �ж��Ƿ� FSM��
GBEXTERN_C GBAPI BOOL GBSTDCALL GBIsFSM(
	LPVOID lpFSM								// IN��FSM��
)
{
	__try
	{
		// ���� FSM ��ȡ FSM �顣
		LPGBFSMBLOCK lpFSMBlock = GBGetFSMBlockFromFSM(lpFSM);

		// ��ȡ FSM �������� FSM �������� FSM ҳ��
		LPGBFSMALLOC lpFSMAlloc = lpFSMBlock->lpFSMAlloc;
		LPGBFSMPAGE lpFSMPage = lpFSMBlock->lpFSMPage;

		// �ж� FSM �������� FSM �������Ƿ� FSM ��������
		if (!GBIsFSMAlloc(GBPTR2PTR(HANDLE, lpFSMAlloc)))
			return FALSE;

		// �ж� FSM �������� FSM ҳ�� FSM ���������Ƿ���ڡ�
		LPGBLIST lpListItem = lpFSMAlloc->FSMPageList.lpNext;
		while (lpListItem != &lpFSMAlloc->FSMPageList)
		{
			if (GBPTR2PTR(LPGBFSMPAGE, lpListItem) == lpFSMPage)
			{
				// ����ʱ�ж� FSM ���� FSM ҳ���Ƿ���ڡ�
				// ע����ʱ�����ж� FSM ���Ƿ��� FSM ҳ�Ŀ��� FSM �������У���Ϊ���� FSM ���ָ��Ϊ���� FSM ��������ָ�롣
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

// ��������ȡ�ѷ��� FSM ����
GBEXTERN_C GBAPI DWORD GBSTDCALL GBLockedGetAllocedFSMCount(
	HANDLE hFSMAlloc							// IN��FSM ��������
)
{
	// �����ٽ�����
	::EnterCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	// ��ȡ�ѷ��� FSM ����
	DWORD dwAllocedFSMCount = GBGetAllocedFSMCount(hFSMAlloc);

	// �뿪�ٽ�����
	::LeaveCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	return dwAllocedFSMCount;
}

// ���������� FSM��
GBEXTERN_C GBAPI LPVOID GBSTDCALL GBLockedAllocFSM(
	HANDLE hFSMAlloc							// IN��FSM ��������
)
{
	// �����ٽ�����
	::EnterCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	// ���� FSM��
	LPVOID lpFSM = GBAllocFSM(hFSMAlloc);

	// �뿪�ٽ�����
	::LeaveCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	return lpFSM;
}

// �������ͷ� FSM��
GBEXTERN_C GBAPI void GBSTDCALL GBLockedFreeFSM(
	LPVOID lpFSM								// IN��FSM��
)
{
	// �����ٽ�����
	::EnterCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	// �ͷ� FSM��
	GBFreeFSM(lpFSM);

	// �뿪�ٽ�����
	::LeaveCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);
}

// �������ж��Ƿ� FSM��
GBEXTERN_C GBAPI BOOL GBSTDCALL GBLockedIsFSM(
	LPVOID lpFSM								// IN��FSM��
)
{
	// �����ٽ�����
	::EnterCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	// �ж��Ƿ� FSM��
	BOOL bIsFSM = GBIsFSM(lpFSM);

	// �뿪�ٽ�����
	::LeaveCriticalSection(&ext_ModuleData.m_FSMAllocMgr.CS);

	return bIsFSM;
}

#pragma endregion
