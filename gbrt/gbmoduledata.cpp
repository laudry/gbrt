// Copyright (c) 2016 甘波。All rights reserved。

#include "stdafx.h"
#include "gbmoduledata.h"

#pragma region "模块数据。"

//////////////////////////////////////////////////////////////////////////
// 模块数据。

CGBModuleData::CGBModuleData(void)
	: m_hModule(NULL), m_dwInitMark(GB_IM_NOT_INITIALIZED)
{
}

CGBModuleData::~CGBModuleData(void)
{
}

// 初始化模块数据。
BOOL CGBModuleData::Init(
	HMODULE hModule								// IN：模块句柄。
)
{
	m_hModule = hModule;

	// 初始化 TLS 索引。
	m_dwTlsIndex = ::TlsAlloc();
	if (TLS_OUT_OF_INDEXES == m_dwTlsIndex)
		return FALSE;
	++m_dwInitMark;

	// 初始化 CSP 句柄。
	if (!::CryptAcquireContext(&m_hCSP, _T("gbrt"), NULL, PROV_RSA_FULL, CRYPT_MACHINE_KEYSET | CRYPT_SILENT))
	{
		if (!::CryptAcquireContext(&m_hCSP, _T("gbrt"), NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET | CRYPT_MACHINE_KEYSET | CRYPT_SILENT))
			return FALSE;
	}
	++m_dwInitMark;

	return TRUE;
}

// 释放模块数据。
void CGBModuleData::Release(void)
{
	// 判断初始化记号。
	// 注：按初始化记号逐步释放成员变量，这时不需要 break。
	switch (m_dwInitMark)
	{
	case GB_IM_CSP_INITIALIZED:					// CSP 句柄初始化完毕。
	{
		// 释放 CSP 句柄。
		::CryptReleaseContext(m_hCSP, 0);
	}
	case GB_IM_TLS_INDEX_INITIALIZED:			// TLS 索引初始化完毕。
	{
		// 释放 TLS 索引。
		::TlsFree(m_dwTlsIndex);
	}
	case GB_IM_NOT_INITIALIZED:					// 未初始化。
	{

	}
	break;
	}
}

#pragma endregion
