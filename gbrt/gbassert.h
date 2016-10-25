// Copyright (c) 2016 �ʲ���All rights reserved��

#if !defined (_GB_ASSERT_H_GB_20161025_)
#define _GB_ASSERT_H_GB_20161025_

#pragma once

#if !defined (_DEBUG)
	#define GBASSERT(_Exp) ((void)0)
	#define GBVERIFY(_Exp) (_Exp)
	#define GBASSUME(_Exp) __assume(_Exp)
#else

	#include "gbrt.h"
	
	// ���ԡ����� C ��׼��Ķ��Ժ����ڶ��̻߳�������ʱ���������Ӵ˺�����
	// ע�������� dll ����ں����е��ô˺�����
	GBEXTERN_C GBAPI void GBSTDCALL GBAssert(
		LPCWSTR lpszMsg,						// IN����Ϣ��
		LPCWSTR lpszFile,						// IN���ļ���
		UINT uLine								// IN���ڼ��С�
		);

	#define GBASSERT(_Exp) (void)((!!(_Exp)) || (GBAssert(_CRT_WIDE(#_Exp), _CRT_WIDE(__FILE__), __LINE__), 0))
	#define GBVERIFY(_Exp) GBASSERT(_Exp)
	#define GBASSUME(_Exp) GBASSERT(_Exp)
#endif

#endif
