// Copyright (c) 2016 �ʲ���All rights reserved��

#include "stdafx.h"

#pragma region "�ڲ�������"

//////////////////////////////////////////////////////////////////////////
// �ڲ�������

// ��ȡ��������Ϣ��
// ����ֵ����������Ϣ��ʧ��ʱ���� NULL��
GBEXTERN_C LPGBOBJLOCKINFO GBSTDCALL GBGetObjLockInfo(
	LPGBOBJLOCKMGR lpObjLockMgr,				// IN����������������
	LPCVOID lpObj,								// IN������
	BOOL bCreateNew								// IN���Ƿ񴴽��¶�������Ϣ��
)
{
	LPGBOBJLOCKINFO lpObjLockInfo;				// ����ֵ��

	// �����ٽ�����
	::EnterCriticalSection(&lpObjLockMgr->CS);

	// ������� Hash ��������
	DWORD dwIndex = GBCalcObjHashTableIndex(lpObj);

	// �Ȳ���ʹ���ж�������Ϣ����
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

	// ��ʱ����ʹ���ж�������Ϣ�����У���Ҫ�����¶�������Ϣ���ж��Ƿ񴴽���
	if (!bCreateNew)
		lpObjLockInfo = NULL;
	else
	{
		if (!GBIsListEmpty(&lpObjLockMgr->FreeObjLockInfoList))
		{
			// ��Ҫ����ʱ������ڿ��ж�������Ϣ����ʹ��֮��
			lpObjLockInfo = GBPTR2PTR(LPGBOBJLOCKINFO, lpObjLockMgr->FreeObjLockInfoList.lpNext);
			GBDelListItem(&lpObjLockInfo->ObjLockInfoListItem);
			lpObjLockInfo->lpObj = lpObj;
			lpObjLockInfo->wRefCount = 1;
			GBInsertAfterListItem(lpList, &lpObjLockInfo->ObjLockInfoListItem);
		}
		else
		{
			// ��Ҫ����ʱ��������ڿ��ж�������Ϣ�����·��䡣
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
	// �뿪�ٽ�����
	::LeaveCriticalSection(&lpObjLockMgr->CS);

	return lpObjLockInfo;
}

// �ͷŶ�������Ϣ��
GBEXTERN_C void GBSTDCALL GBFreeObjLockInfo(
	LPGBOBJLOCKMGR lpObjLockMgr,				// IN����������������
	LPGBOBJLOCKINFO lpObjLockInfo				// IN����������Ϣ��
)
{
	// �����ٽ�����
	::EnterCriticalSection(&lpObjLockMgr->CS);

	// �������ü�����������ٺ����ü���Ϊ 0������Ҫ��ʹ���ж�������Ϣ������ɾ����
	if (0 == --lpObjLockInfo->wRefCount)
	{
		// ��ʹ���ж�������Ϣ������ɾ����
		GBDelListItem(&lpObjLockInfo->ObjLockInfoListItem);

		// ��������ж��Ǽ�����ж�������Ϣ������ֱ���ͷš�
		if (GBGetAllocedFSMCount(lpObjLockMgr->hObjLockInfoAlloc) <= 32)
		{
			// ����������Ϣ��С�ڵ���һ�η�����ʱ������ж�������Ϣ����
			GBInsertAfterListItem(&lpObjLockMgr->FreeObjLockInfoList, &lpObjLockInfo->ObjLockInfoListItem);
		}
		else
		{
			// ����ֱ���ͷš�
			::DeleteCriticalSection(&lpObjLockInfo->CS);
			GBFreeFSM(lpObjLockInfo);
		}
	}

	// �뿪�ٽ�����
	::LeaveCriticalSection(&lpObjLockMgr->CS);
}

// ��ʼ����������������
GBEXTERN_C BOOL GBSTDCALL GBInitObjLockMgr(
	LPGBOBJLOCKMGR lpObjLockMgr					// INOUT����������������
)
{
	// ��ʼ���ٽ�����
	if (!GBInitializeCriticalSection(&lpObjLockMgr->CS))
		return FALSE;

	// ������������Ϣ��������
	lpObjLockMgr->hObjLockInfoAlloc = GBCreateFSMAlloc(sizeof(GBOBJLOCKINFO), 32);
	if (NULL == lpObjLockMgr->hObjLockInfoAlloc)
	{
		::DeleteCriticalSection(&lpObjLockMgr->CS);
		return FALSE;
	}

	// ��ʼ����������
	GBInitList(&lpObjLockMgr->FreeObjLockInfoList);
	for (DWORD i = 0; i != GB_OBJ_LOCK_MGR_HASH_TABLE_SIZE; ++i)
		GBInitList(&lpObjLockMgr->lpInUseObjLockInfoList[i]);

	return TRUE;
}

// �ͷŶ�������������
GBEXTERN_C void GBSTDCALL GBReleaseObjLockMgr(
	LPGBOBJLOCKMGR lpObjLockMgr					// INOUT����������������
)
{
	// �����ٽ�����
	::EnterCriticalSection(&lpObjLockMgr->CS);

	// �ͷ�ʹ���ж�������Ϣ��
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

	// �ͷſ��ж�������Ϣ��
	LPGBLIST lpListItem = lpObjLockMgr->FreeObjLockInfoList.lpNext;
	while (lpListItem != &lpObjLockMgr->FreeObjLockInfoList)
	{
		LPGBOBJLOCKINFO lpObjLockInfo = GBPTR2PTR(LPGBOBJLOCKINFO, lpListItem);
		lpListItem = lpListItem->lpNext;
		GBDelListItem(&lpObjLockInfo->ObjLockInfoListItem);
		::DeleteCriticalSection(&lpObjLockInfo->CS);
		GBFreeFSM(lpObjLockInfo);
	}

	// ���ٶ�������Ϣ��������
	GBDestroyFSMAlloc(lpObjLockMgr->hObjLockInfoAlloc);

	// ɾ���ٽ�����
	::DeleteCriticalSection(&lpObjLockMgr->CS);
}

#pragma endregion
