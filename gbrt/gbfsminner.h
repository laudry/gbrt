// Copyright (c) 2016 �ʲ���All rights reserved��

#if !defined (_GB_FSM_INNER_H_GB_20161026_)
#define _GB_FSM_INNER_H_GB_20161026_

#pragma once

#include "gblist.h"

#pragma pack(push, GBPACK)

// FSM ��������
// FSM �������Ĵ�С��Ϊ GBPACK ����������
typedef struct _GBFSMALLOC
{
	GBLIST FSMAllocListItem;					// FSM �����������
	DWORD dwFSMBlockSize;						// FSM ���С��
	DWORD dwAllocCountOnce;						// һ�η�����������Ϊ 0��
	DWORD dwAllocedCount;						// �ѷ�������
	DWORD dwFSMPageCount;						// FSM ҳ����
	GBLIST FSMPageList;							// FSM ҳ����
} GBFSMALLOC, *LPGBFSMALLOC;
typedef struct _GBFSMALLOC const *LPCGBFSMALLOC;

// FSM ҳ��
// FSM ҳ�Ĵ�С��Ϊ GBPACK ����������
typedef struct _GBFSMPAGE
{
	GBLIST FSMPageListItem;						// FSM ҳ�����
	DWORD dwAllocedCount;						// �ѷ�������
	DWORD dwFreeFSMBlockCount;					// ���� FSM ������
	GBLIST FreeFSMBlockList;					// ���� FSM ������
} GBFSMPAGE, *LPGBFSMPAGE;
typedef struct _GBFSMPAGE const *LPCGBFSMPAGE;

// FSM �顣
// FSM ��Ĵ�С��Ϊ GBPACK ����������
typedef struct _GBFSMBLOCK
{
	union
	{
		GBLIST FreeFSMBlockListItem;			// ���� FSM �������
		struct
		{
			LPGBFSMALLOC lpFSMAlloc;			// ���� FSM ��������
			LPGBFSMPAGE lpFSMPage;				// ���� FSM ҳ��
		};
	};
} GBFSMBLOCK, *LPGBFSMBLOCK;
typedef struct _GBFSMBLOCK const *LPCGBFSMBLOCK;

// FSM ��������������
typedef struct _GBFSMALLOCMGR
{
	CRITICAL_SECTION CS;						// �ٽ�����
	GBFSMALLOC FSMAlloc;						// FSM ��������
} GBFSMALLOCMGR, *LPGBFSMALLOCMGR;
typedef struct _GBFSMALLOCMGR const *LPCGBFSMALLOCMGR;

// ��� FSM ��������С��
#define GB_MAX_FSM_ALLOC_SIZE 0x10000000

// ���̶���С��
#define GB_MAX_FIXED_SIZE (GB_MAX_FSM_ALLOC_SIZE-sizeof(GBFSMPAGE)-sizeof(GBFSMBLOCK))

#pragma pack(pop)

// ���� FSM ���ȡ FSM��
#define GBGetFSMFromFSMBlock(lpFSMBlock) GBPTR2PTR(LPVOID, GBPTR2PTR(LPBYTE, lpFSMBlock)+sizeof(GBFSMBLOCK))

// ���� FSM ��ȡ FSM �顣
#define GBGetFSMBlockFromFSM(lpFSM) GBPTR2PTR(LPGBFSMBLOCK, GBPTR2PTR(LPBYTE, lpFSM)-sizeof(GBFSMBLOCK))

// �ͷ� FSM ��������
GBEXTERN_C void GBSTDCALL GBFreeFSMAlloc(
	LPGBFSMALLOC lpFSMAlloc						// IN��FSM ��������
);

// ��ʼ�� FSM ��������������
GBEXTERN_C BOOL GBSTDCALL GBInitFSMAllocMgr(
	LPGBFSMALLOCMGR lpFSMAllocMgr				// INOUT��FSM ��������������
);

// �ͷ� FSM ��������������
GBEXTERN_C void GBSTDCALL GBReleaseFSMAllocMgr(
	LPGBFSMALLOCMGR lpFSMAllocMgr				// INOUT��FSM ��������������
);

#endif
