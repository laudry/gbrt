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

#pragma pack(push, GBPACK)

// std::set、std::map 用分配器。
template<class _Type, DWORD _dwAllocCountOnce, DWORD _dwAppointedIndex=0>
class CGBAlloc
{
public:
	typedef typename _Type value_type;

	typedef value_type *pointer;
	typedef value_type & reference;
	typedef const value_type *const_pointer;
	typedef const value_type & const_reference;

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	template<class _OtherType>
	struct rebind
	{
		typedef CGBAlloc<_OtherType, _dwAllocCountOnce, _dwAppointedIndex> other;
	};

public:
	// 构造及析构函数。
	CGBAlloc(void)
		: m_dwMaxAllocCount(GBCalcMaxAllocCountOfFSMAlloc(sizeof(_Type), _dwAllocCountOnce)), m_dwIndex(0), m_hFSMAlloc(NULL), m_lpFirstAllocPtr(NULL)
	{
	}
	CGBAlloc(const CGBAlloc<_Type, _dwAllocCountOnce, _dwAppointedIndex> & OtherAlloc)
		: m_dwMaxAllocCount(GBCalcMaxAllocCountOfFSMAlloc(sizeof(_Type), _dwAllocCountOnce)), m_dwIndex(OtherAlloc.m_dwIndex + 1), m_hFSMAlloc(NULL), m_lpFirstAllocPtr(NULL)
	{
	}
	template<class _OtherType, DWORD _dwOtherAllocCountOnce, DWORD _dwOtherAppointedIndex>
	CGBAlloc(const CGBAlloc<_OtherType, _dwOtherAllocCountOnce, _dwOtherAppointedIndex> & OtherAlloc)
		: m_dwMaxAllocCount(GBCalcMaxAllocCountOfFSMAlloc(sizeof(_Type), _dwAllocCountOnce)), m_dwIndex(OtherAlloc.m_dwIndex + 1), m_hFSMAlloc(NULL), m_lpFirstAllocPtr(NULL)
	{
	}
	~CGBAlloc(void)
	{
		if (NULL != m_hFSMAlloc)
			GBDestroyFSMAlloc(m_hFSMAlloc);
		GBASSERT(NULL == m_lpFirstAllocPtr);
	}

	// 赋值函数。
	CGBAlloc<_Type, _dwAllocCountOnce, _dwAppointedIndex> & operator = (
		const CGBAlloc<_Type, _dwAllocCountOnce, _dwAppointedIndex> & OtherAlloc
		)
	{
		return (*this);
	}
	template<class _OtherType, DWORD _dwOtherAllocCountOnce, DWORD _dwOtherAppointedIndex>
	CGBAlloc<_Type, _dwAllocCountOnce, _dwAppointedIndex> & operator = (
		const CGBAlloc<_OtherType, _dwOtherAllocCountOnce, _dwOtherAppointedIndex> & OtherAlloc
		)
	{
		return (*this);
	}

	// 分配及释放函数。
	pointer allocate(size_type uCount)
	{
		GBASSERT(1 == uCount);
		if (m_dwIndex != _dwAppointedIndex)
		{
			return GBPTR2PTR(pointer, malloc(sizeof(_Type)));
		}
		else
		{
			if (NULL == m_lpFirstAllocPtr)
			{
				m_lpFirstAllocPtr = GBPTR2PTR(pointer, malloc(sizeof(_Type)));
				return m_lpFirstAllocPtr;
			}
			else if (NULL == m_hFSMAlloc)
			{
				m_hFSMAlloc = GBCreateFSMAlloc(sizeof(_Type), _dwAllocCountOnce);
				if (NULL == m_hFSMAlloc)
					return NULL;
			}
			return GBPTR2PTR(pointer, GBAllocFSM(m_hFSMAlloc));
		}
	}
	pointer allocate(size_type uCount, const void *)
	{
		return allocate(uCount);
	}
	void deallocate(pointer lpPtr, size_type uCount)
	{
		GBASSERT(1 == uCount);
		if (m_dwIndex != _dwAppointedIndex)
			free(lpPtr);
		else if (lpPtr != m_lpFirstAllocPtr)
			GBFreeFSM(lpPtr);
		else
		{
			free(lpPtr);
			m_lpFirstAllocPtr = NULL;
		}
	}

	// 构建及销毁函数。
	void construct(pointer lpPtr, const _Type & Value)
	{
		_Construct(lpPtr, Value);
	}
	void destroy(pointer lpPtr)
	{
		_Destroy(lpPtr);
	}

	// 最大分配数函数。
	size_type max_size() const
	{
		return m_dwMaxAllocCount;
	}

public:
	// 成员变量。
	const DWORD m_dwMaxAllocCount;				// 最大分配数。
	const DWORD m_dwIndex;						// 索引。
	HANDLE m_hFSMAlloc;							// FSM 分配器。
	pointer m_lpFirstAllocPtr;					// 第一次分配指针。
};

#pragma pack(pop)

// 比较函数。
template<class _Type1, DWORD _dwAllocCountOnce1, class _Type2, DWORD _dwAllocCountOnce2>
GBINLINE bool GBSTDCALL operator == (const CGBAlloc<_Type1, _dwAllocCountOnce1> & Alloc1, const CGBAlloc<_Type2, _dwAllocCountOnce2> & Alloc2)
{
	return false;
}
template<class _Type1, DWORD _dwAllocCountOnce1, class _Type2, DWORD _dwAllocCountOnce2>
GBINLINE bool GBSTDCALL operator != (const CGBAlloc<_Type1, _dwAllocCountOnce1> & Alloc1, const CGBAlloc<_Type2, _dwAllocCountOnce2> & Alloc2)
{
	return true;
}

// 生成名称。
#define GB_GEN_COMP_NAME(_CompName) CGB##_CompName##Comp
#define GB_GEN_SET_ALLOC_NAME(_Name) CGB##_Name##SetAlloc
#define GB_GEN_SET_NAME(_Name) CGB##_Name##Set
#define GB_GEN_MAP_ALLOC_NAME(_Name) CGB##_Name##MapAlloc
#define GB_GEN_MAP_NAME(_Name) CGB##_Name##Map

// 声明分配器。
#define GB_DECL_SET_ALLOC(_Name, _Type, _dwAllocCountOnce) \
	typedef CGBAlloc<_Type, _dwAllocCountOnce> GB_GEN_SET_ALLOC_NAME(_Name)
#define GB_DECL_MAP_ALLOC(_Name, _Key, _Value, _dwAllocCountOnce) \
	typedef CGBAlloc<std::pair<const _Key, _Value>, _dwAllocCountOnce> GB_GEN_MAP_ALLOC_NAME(_Name)

// 声明 std::set。
#define GB_DECL_SET(_Name, _Type, _dwAllocCountOnce) \
	GB_DECL_SET_ALLOC(_Name, _Type, _dwAllocCountOnce);\
	typedef std::set<_Type, std::less<_Type>, GB_GEN_SET_ALLOC_NAME(_Name)> GB_GEN_SET_NAME(_Name)
#define GB_DECL_SET_EX(_Name, _CompName, _Type, _dwAllocCountOnce) \
	GB_DECL_SET_ALLOC(_Name, _Type, _dwAllocCountOnce);\
	typedef std::set<_Type, GB_GEN_COMP_NAME(_CompName), GB_GEN_SET_ALLOC_NAME(_Name)> GB_GEN_SET_NAME(_Name)

// 声明 std::map。
#define GB_DECL_MAP(_Name, _Key, _Value, _dwAllocCountOnce) \
	GB_DECL_MAP_ALLOC(_Name, _Key, _Value, _dwAllocCountOnce);\
	typedef std::map<_Key, _Value, std::less<_Key>, GB_GEN_MAP_ALLOC_NAME(_Name)> GB_GEN_MAP_NAME(_Name)
#define GB_DECL_MAP_EX(_Name, _CompName, _Key, _Value, _dwAllocCountOnce) \
	GB_DECL_MAP_ALLOC(_Name, _Key, _Value, _dwAllocCountOnce);\
	typedef std::map<_Key, _Value, GB_GEN_COMP_NAME(_CompName), GB_GEN_MAP_ALLOC_NAME(_Name)> GB_GEN_MAP_NAME(_Name)

#endif
