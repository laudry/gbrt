// Copyright (c) 2016 甘波。All rights reserved。

#if !defined (_GB_FSM_H_GB_20161026_)
#define _GB_FSM_H_GB_20161026_

#pragma once

#include "gbrt.h"
#include "gbassert.h"
#include <set>
#include <map>

// 计算 FSM 分配器的最大分配数。
// 返回值：最大分配数。失败时返回 0。
GBEXTERN_C GBAPI DWORD GBSTDCALL GBCalcMaxAllocCountOfFSMAlloc(
	DWORD dwFixedSize,							// IN：固定大小。不能为 0。
	DWORD dwAllocCountOnce						// IN：一次分配数。不能为 0。
);

// 创建 FSM 分配器。
// 返回值： FSM 分配器句柄。失败时返回 NULL。
GBEXTERN_C GBAPI HANDLE GBSTDCALL GBCreateFSMAlloc(
	DWORD dwFixedSize,							// IN：固定大小。不能为 0。
	DWORD dwAllocCountOnce						// IN：一次分配数。不能为 0。
);

// 销毁 FSM 分配器。
GBEXTERN_C GBAPI void GBSTDCALL GBDestroyFSMAlloc(
	HANDLE hFSMAlloc							// IN：FSM 分配器。
);

// 判断是否 FSM 分配器。
GBEXTERN_C GBAPI BOOL GBSTDCALL GBIsFSMAlloc(
	HANDLE hFSMAlloc							// IN：FSM 分配器。
);

// 获取已分配 FSM 数。
GBEXTERN_C GBAPI DWORD GBSTDCALL GBGetAllocedFSMCount(
	HANDLE hFSMAlloc							// IN：FSM 分配器。
);

// 分配 FSM。
// 返回值：FSM 指针。失败时返回 NULL。
GBEXTERN_C GBAPI LPVOID GBSTDCALL GBAllocFSM(
	HANDLE hFSMAlloc							// IN：FSM 分配器。
);

// 释放 FSM。
GBEXTERN_C GBAPI void GBSTDCALL GBFreeFSM(
	LPVOID lpFSM								// IN：FSM。
);

// 判断是否 FSM。
GBEXTERN_C GBAPI BOOL GBSTDCALL GBIsFSM(
	LPVOID lpFSM								// IN：FSM。
);

// 锁定并获取已分配 FSM 数。
GBEXTERN_C GBAPI DWORD GBSTDCALL GBLockedGetAllocedFSMCount(
	HANDLE hFSMAlloc							// IN：FSM 分配器。
);

// 锁定并分配 FSM。
GBEXTERN_C GBAPI LPVOID GBSTDCALL GBLockedAllocFSM(
	HANDLE hFSMAlloc							// IN：FSM 分配器。
);

// 锁定并释放 FSM。
GBEXTERN_C GBAPI void GBSTDCALL GBLockedFreeFSM(
	LPVOID lpFSM								// IN：FSM。
);

// 锁定并判断是否 FSM。
GBEXTERN_C GBAPI BOOL GBSTDCALL GBLockedIsFSM(
	LPVOID lpFSM								// IN：FSM。
);

#endif
