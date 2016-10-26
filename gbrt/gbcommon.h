// Copyright (c) 2016 �ʲ���All rights reserved��

#if !defined (_GB_COMMON_H_GB_20161025_)
#define _GB_COMMON_H_GB_20161025_

#pragma once

#include "gbdef.h"

#pragma pack(push, GBPACK)

// ֪ͨ������
typedef int (GBSTDCALL *GBNOTIFYPROC)(
	int nMsg,									// IN����Ϣ��
	INT_PTR nParam1,							// IN������һ��
	INT_PTR nParam2,							// IN����������
	INT_PTR lpUserData							// IN���û����ݡ�
	);

#pragma pack(pop)

// �������
template<class _Type>
GBINLINE void GBSTDCALL GBConstruct(_Type *lpObj)
{
	::new (lpObj) _Type();
}
template<class _Type, class _OtherType>
GBINLINE void GBSTDCALL GBConstruct(_Type *lpObj, const _OtherType & Value)
{
	::new (lpObj) _Type(Value);
}

// ��������
template<class _Type>
GBINLINE void GBSTDCALL GBDestruct(_Type *lpObj)
{
	lpObj->~_Type();
}

// ��ʼ���ٽ�����
GBEXTERN_C GBINLINE BOOL GBSTDCALL GBInitializeCriticalSection(
	LPCRITICAL_SECTION lpCriticalSection		// OUT���ٽ�����
	)
{
	__try
	{
		::InitializeCriticalSection(lpCriticalSection);

		return TRUE;
	}
	__except (GBHandleException(STATUS_NO_MEMORY))
	{
		return FALSE;
	}
}

#endif
