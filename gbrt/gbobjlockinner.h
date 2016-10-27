// Copyright (c) 2016 �ʲ���All rights reserved��

#if !defined (_GB_OBJ_LOCK_INNER_H_GB_20161027_)
#define _GB_OBJ_LOCK_INNER_H_GB_20161027_

#pragma once

#include "gblist.h"

#pragma pack(push, GBPACK)

// ��������Ϣ��
typedef struct _GBOBJLOCKINFO
{
	GBLIST ObjLockInfoListItem;					// ��������Ϣ�����
	CRITICAL_SECTION CS;						// �ٽ�����
	LPCVOID lpObj;								// ����
	DWORD dwOwningThreadId;						// �����߳� ID��
	WORD wRefCount;								// ���ü�����
	WORD wLockCount;							// ����������ע����������С�ڵ������ü�����
} GBOBJLOCKINFO, *LPGBOBJLOCKINFO;
typedef struct _GBOBJLOCKINFO const *LPCGBOBJLOCKINFO;

// ������������ Hash ���С��
#define  GB_OBJ_LOCK_MGR_HASH_TABLE_SIZE 37

// ��������������
typedef struct _GBOBJLOCKMGR
{
	CRITICAL_SECTION CS;												// �ٽ�����
	HANDLE hObjLockInfoAlloc;											// ��������Ϣ��������
	GBLIST FreeObjLockInfoList;											// ���ж�������Ϣ����
	GBLIST lpInUseObjLockInfoList[GB_OBJ_LOCK_MGR_HASH_TABLE_SIZE];		// ʹ���ж�������Ϣ����
} GBOBJLOCKMGR, *LPGBOBJLOCKMGR;
typedef struct _GBOBJLOCKMGR const *LPCGBOBJLOCKMGR;

#pragma pack(pop)

// ������� Hash ��������
#define GBCalcObjHashTableIndex(lpObj) GBINT2INT(DWORD, (GBPTR2INT(UINT_PTR, lpObj) >> 3) % GB_OBJ_LOCK_MGR_HASH_TABLE_SIZE)

// ��ȡ��������Ϣ��
// ����ֵ����������Ϣ��ʧ��ʱ���� NULL��
GBEXTERN_C LPGBOBJLOCKINFO GBSTDCALL GBGetObjLockInfo(
	LPGBOBJLOCKMGR lpObjLockMgr,				// IN����������������
	LPCVOID lpObj,								// IN������
	BOOL bCreateNew								// IN���Ƿ񴴽��¶�������Ϣ��
);

// �ͷŶ�������Ϣ��
GBEXTERN_C void GBSTDCALL GBFreeObjLockInfo(
	LPGBOBJLOCKMGR lpObjLockMgr,				// IN����������������
	LPGBOBJLOCKINFO lpObjLockInfo				// IN����������Ϣ��
);

// ��ʼ����������������
GBEXTERN_C BOOL GBSTDCALL GBInitObjLockMgr(
	LPGBOBJLOCKMGR lpObjLockMgr					// INOUT����������������
);

// �ͷŶ�������������
GBEXTERN_C void GBSTDCALL GBReleaseObjLockMgr(
	LPGBOBJLOCKMGR lpObjLockMgr					// INOUT����������������
);

#endif
