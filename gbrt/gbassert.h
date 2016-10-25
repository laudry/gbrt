// Copyright (c) 2016 甘波。All rights reserved。

#if !defined (_GB_ASSERT_H_GB_20161025_)
#define _GB_ASSERT_H_GB_20161025_

#pragma once

#if !defined (_DEBUG)
	#define GBASSERT(_Exp) ((void)0)
	#define GBVERIFY(_Exp) (_Exp)
	#define GBASSUME(_Exp) __assume(_Exp)
#else

	#include "gbrt.h"
	
	// 断言。由于 C 标准库的断言函数在多线程环境下有时会出错，固添加此函数。
	// 注：请勿在 dll 的入口函数中调用此函数。
	GBEXTERN_C GBAPI void GBSTDCALL GBAssert(
		LPCWSTR lpszMsg,						// IN：信息。
		LPCWSTR lpszFile,						// IN：文件。
		UINT uLine								// IN：第几行。
		);

	#define GBASSERT(_Exp) (void)((!!(_Exp)) || (GBAssert(_CRT_WIDE(#_Exp), _CRT_WIDE(__FILE__), __LINE__), 0))
	#define GBVERIFY(_Exp) GBASSERT(_Exp)
	#define GBASSUME(_Exp) GBASSERT(_Exp)
#endif

#endif
