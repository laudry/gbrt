// Copyright (c) 2016 �ʲ���All rights reserved��

#include "stdafx.h"

#pragma region "����������"

//////////////////////////////////////////////////////////////////////////
// ����������

// ��������
// ����ֵ�������������ʧ��ʱ���� NULL��
GBEXTERN_C GBAPI HANDLE GBSTDCALL GBLockObj(
	LPCVOID lpObj								// IN������
)
{
	// ��ȡ��������Ϣ��
	LPGBOBJLOCKINFO lpObjLockInfo = GBGetObjLockInfo(&ext_ModuleData.m_ObjLockMgr, lpObj, TRUE);
	if (NULL == lpObjLockInfo)
		return NULL;

	// ��������
	::EnterCriticalSection(&lpObjLockInfo->CS);

	// ���������������������ǰ��������Ϊ 0�������������߳� ID��
	if (0 == lpObjLockInfo->wLockCount++)
		InterlockedExchange(GBPTR2PTR(LONG volatile *, &lpObjLockInfo->dwOwningThreadId), (LONG)::GetCurrentThreadId());

	return GBPTR2PTR(HANDLE, lpObjLockInfo);
}

// ������������
// ����ֵ�������������ʧ��ʱ���� NULL�����������߳�����ʱ���� INVALID_HANDLE_VALUE��
GBEXTERN_C GBAPI HANDLE GBSTDCALL GBTryLockObj(
	LPCVOID lpObj								// IN������
)
{
	// ��ȡ��������Ϣ��
	LPGBOBJLOCKINFO lpObjLockInfo = GBGetObjLockInfo(&ext_ModuleData.m_ObjLockMgr, lpObj, TRUE);
	if (NULL == lpObjLockInfo)
		return NULL;

	// ������������
	if (::TryEnterCriticalSection(&lpObjLockInfo->CS))
	{
		// �����ɹ�ʱ���������������������ǰ��������Ϊ 0�������������߳� ID��
		if (0 == lpObjLockInfo->wLockCount++)
			InterlockedExchange(GBPTR2PTR(LONG volatile *, &lpObjLockInfo->dwOwningThreadId), (LONG)::GetCurrentThreadId());

		return GBPTR2PTR(HANDLE, lpObjLockInfo);
	}
	else
	{
		// ����ʧ��ʱ���������߳���������ʱ�ͷŶ�������Ϣ������ INVALID_HANDLE_VALUE��
		GBFreeObjLockInfo(&ext_ModuleData.m_ObjLockMgr, lpObjLockInfo);

		return INVALID_HANDLE_VALUE;
	}
}

// ��������
// ע���˺������ڸ��ݶ��������������������
GBEXTERN_C GBAPI void GBSTDCALL GBUnlockObj(
	LPCVOID lpObj								// IN������
)
{
	// ��ȡ��������Ϣ��
	LPGBOBJLOCKINFO lpObjLockInfo = GBGetObjLockInfo(&ext_ModuleData.m_ObjLockMgr, lpObj, FALSE);
	if (NULL == lpObjLockInfo)
		return;

	// ֻ�������߳��ܹ���������
	if (::InterlockedCompareExchange(GBPTR2PTR(LONG volatile *, &lpObjLockInfo->dwOwningThreadId), 0, 0) == (LONG)::GetCurrentThreadId())
	{
		// ��������������������ٺ���������Ϊ 0�������������߳� ID��
		if (0 == --lpObjLockInfo->wLockCount)
			InterlockedExchange(GBPTR2PTR(LONG volatile *, &lpObjLockInfo->dwOwningThreadId), 0);

		// ��������
		::LeaveCriticalSection(&lpObjLockInfo->CS);

		// �ͷŶ�������Ϣ��
		GBFreeObjLockInfo(&ext_ModuleData.m_ObjLockMgr, lpObjLockInfo);
	}
}

// ���ݶ����������������
GBEXTERN_C GBAPI void GBSTDCALL GBUnlockObjByObjLock(
	HANDLE hObjLock								// IN�������������
)
{
	// ��ȡ��������Ϣ��
	LPGBOBJLOCKINFO lpObjLockInfo = GBPTR2PTR(LPGBOBJLOCKINFO, hObjLock);

	// ֻ�������߳��ܹ���������
	if (::InterlockedCompareExchange(GBPTR2PTR(LPLONG, &lpObjLockInfo->dwOwningThreadId), 0, 0) == (LONG)::GetCurrentThreadId())
	{
		// ��������������������ٺ���������Ϊ 0�������������߳� ID��
		if (0 == --lpObjLockInfo->wLockCount)
			InterlockedExchange(GBPTR2PTR(LONG volatile *, &lpObjLockInfo->dwOwningThreadId), 0);

		// ��������
		::LeaveCriticalSection(&lpObjLockInfo->CS);

		// �ͷŶ�������Ϣ��
		GBFreeObjLockInfo(&ext_ModuleData.m_ObjLockMgr, lpObjLockInfo);
	}
}

// �ж��Ƿ��������
GBEXTERN_C GBAPI BOOL GBSTDCALL GBIsObjLock(
	HANDLE hObjLock								// IN�������������
)
{
	// �����ٽ�����
	::EnterCriticalSection(&ext_ModuleData.m_ObjLockMgr.CS);

	__try
	{
		// ������� Hash ��������
		DWORD dwIndex = GBCalcObjHashTableIndex(GBPTR2PTR(LPGBOBJLOCKINFO, hObjLock)->lpObj);

		// �ж��Ƿ��������
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

		// �뿪�ٽ�����
		::LeaveCriticalSection(&ext_ModuleData.m_ObjLockMgr.CS);

		return bIsObjLock;
	}
	__except (GBHandleException(EXCEPTION_ACCESS_VIOLATION))
	{
		// �뿪�ٽ�����
		::LeaveCriticalSection(&ext_ModuleData.m_ObjLockMgr.CS);

		return FALSE;
	}
}

// �ж϶����Ƿ�������
GBEXTERN_C GBAPI BOOL GBSTDCALL GBIsObjLocked(
	LPCVOID lpObj								// IN������
)
{
	// �����ٽ�����
	::EnterCriticalSection(&ext_ModuleData.m_ObjLockMgr.CS);

	// ������� Hash ��������
	DWORD dwIndex = GBCalcObjHashTableIndex(lpObj);

	// �ж϶����Ƿ�������
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

	// �뿪�ٽ�����
	::LeaveCriticalSection(&ext_ModuleData.m_ObjLockMgr.CS);

	return bIsObjLocked;
}

#pragma endregion
