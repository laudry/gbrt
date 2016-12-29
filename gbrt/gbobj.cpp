// Copyright (c) 2016 �ʲ���All rights reserved��

#include "stdafx.h"
#include "gbobj.h"

#pragma region "����"

//////////////////////////////////////////////////////////////////////////
// ����

CGBObj::CGBObj(void)
	: m_lRefCount(0)
{
}

CGBObj::~CGBObj(void)
{
	GBASSERT(0 == GetRefCount());
}

// �������á�
LONG CGBObj::AddRef(void)
{
	GBASSERT(GetRefCount() >= 0);
	return InterlockedIncrement(&m_lRefCount);
}

// �ͷš�
LONG CGBObj::Release(void)
{
	GBASSERT(GetRefCount() > 0);
	return InterlockedDecrement(&m_lRefCount);
}

// ��ȡ���ü�����
LONG CGBObj::GetRefCount(void)
{
	return InterlockedCompareExchange(&m_lRefCount, 0, 0);
}

#pragma endregion
