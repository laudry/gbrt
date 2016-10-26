// Copyright (c) 2016 �ʲ���All rights reserved��

#if !defined (_GB_FSM_H_GB_20161026_)
#define _GB_FSM_H_GB_20161026_

#pragma once

#include "gbrt.h"
#include "gbassert.h"
#include <set>
#include <map>

// ���� FSM ������������������
// ����ֵ������������ʧ��ʱ���� 0��
GBEXTERN_C GBAPI DWORD GBSTDCALL GBCalcMaxAllocCountOfFSMAlloc(
	DWORD dwFixedSize,							// IN���̶���С������Ϊ 0��
	DWORD dwAllocCountOnce						// IN��һ�η�����������Ϊ 0��
);

// ���� FSM ��������
// ����ֵ�� FSM �����������ʧ��ʱ���� NULL��
GBEXTERN_C GBAPI HANDLE GBSTDCALL GBCreateFSMAlloc(
	DWORD dwFixedSize,							// IN���̶���С������Ϊ 0��
	DWORD dwAllocCountOnce						// IN��һ�η�����������Ϊ 0��
);

// ���� FSM ��������
GBEXTERN_C GBAPI void GBSTDCALL GBDestroyFSMAlloc(
	HANDLE hFSMAlloc							// IN��FSM ��������
);

// �ж��Ƿ� FSM ��������
GBEXTERN_C GBAPI BOOL GBSTDCALL GBIsFSMAlloc(
	HANDLE hFSMAlloc							// IN��FSM ��������
);

// ��ȡ�ѷ��� FSM ����
GBEXTERN_C GBAPI DWORD GBSTDCALL GBGetAllocedFSMCount(
	HANDLE hFSMAlloc							// IN��FSM ��������
);

// ���� FSM��
// ����ֵ��FSM ָ�롣ʧ��ʱ���� NULL��
GBEXTERN_C GBAPI LPVOID GBSTDCALL GBAllocFSM(
	HANDLE hFSMAlloc							// IN��FSM ��������
);

// �ͷ� FSM��
GBEXTERN_C GBAPI void GBSTDCALL GBFreeFSM(
	LPVOID lpFSM								// IN��FSM��
);

// �ж��Ƿ� FSM��
GBEXTERN_C GBAPI BOOL GBSTDCALL GBIsFSM(
	LPVOID lpFSM								// IN��FSM��
);

// ��������ȡ�ѷ��� FSM ����
GBEXTERN_C GBAPI DWORD GBSTDCALL GBLockedGetAllocedFSMCount(
	HANDLE hFSMAlloc							// IN��FSM ��������
);

// ���������� FSM��
GBEXTERN_C GBAPI LPVOID GBSTDCALL GBLockedAllocFSM(
	HANDLE hFSMAlloc							// IN��FSM ��������
);

// �������ͷ� FSM��
GBEXTERN_C GBAPI void GBSTDCALL GBLockedFreeFSM(
	LPVOID lpFSM								// IN��FSM��
);

// �������ж��Ƿ� FSM��
GBEXTERN_C GBAPI BOOL GBSTDCALL GBLockedIsFSM(
	LPVOID lpFSM								// IN��FSM��
);

#endif
