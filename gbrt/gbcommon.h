// Copyright (c) 2016 �ʲ���All rights reserved��

#if !defined (_GB_COMMON_H_GB_20161025_)
#define _GB_COMMON_H_GB_20161025_

#pragma once

#include "gbdef.h"

#pragma pack(push, GBPACK)

// ֪ͨ������
typedef int (GBSTDCALL *GBNOTIFYPROC)(
	UINT uMsg,									// IN����Ϣ��
	UINT_PTR uParam1,							// IN������һ��
	UINT_PTR uParam2,							// IN����������
	UINT_PTR lpUserData							// IN���û����ݡ�
	);

#pragma pack(pop)

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
