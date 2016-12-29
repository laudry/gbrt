// Copyright (c) 2016 甘波。All rights reserved。

#include "stdafx.h"
#include "gbobj.h"

#pragma region "对象。"

//////////////////////////////////////////////////////////////////////////
// 对象。

CGBObj::CGBObj(void)
	: m_lRefCount(0)
{
}

CGBObj::~CGBObj(void)
{
	GBASSERT(0 == GetRefCount());
}

// 增加引用。
LONG CGBObj::AddRef(void)
{
	GBASSERT(GetRefCount() >= 0);
	return InterlockedIncrement(&m_lRefCount);
}

// 释放。
LONG CGBObj::Release(void)
{
	GBASSERT(GetRefCount() > 0);
	return InterlockedDecrement(&m_lRefCount);
}

// 获取引用计数。
LONG CGBObj::GetRefCount(void)
{
	return InterlockedCompareExchange(&m_lRefCount, 0, 0);
}

#pragma endregion
