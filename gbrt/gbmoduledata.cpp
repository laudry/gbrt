// Copyright (c) 2016 �ʲ���All rights reserved��

#include "stdafx.h"
#include "gbmoduledata.h"

#pragma region "ģ�����ݡ�"

//////////////////////////////////////////////////////////////////////////
// ģ�����ݡ�

CGBModuleData::CGBModuleData(void)
	: m_hModule(NULL), m_dwInitMark(GB_IM_NOT_INITIALIZED)
{
}

CGBModuleData::~CGBModuleData(void)
{
}

// ��ʼ��ģ�����ݡ�
BOOL CGBModuleData::Init(
	HMODULE hModule								// IN��ģ������
)
{
	m_hModule = hModule;

	// ��ʼ�� TLS ������
	m_dwTlsIndex = ::TlsAlloc();
	if (TLS_OUT_OF_INDEXES == m_dwTlsIndex)
		return FALSE;
	++m_dwInitMark;

	// ��ʼ�� CSP �����
	if (!::CryptAcquireContext(&m_hCSP, _T("gbrt"), NULL, PROV_RSA_FULL, CRYPT_MACHINE_KEYSET | CRYPT_SILENT))
	{
		if (!::CryptAcquireContext(&m_hCSP, _T("gbrt"), NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET | CRYPT_MACHINE_KEYSET | CRYPT_SILENT))
			return FALSE;
	}
	++m_dwInitMark;

	return TRUE;
}

// �ͷ�ģ�����ݡ�
void CGBModuleData::Release(void)
{
	// �жϳ�ʼ���Ǻš�
	// ע������ʼ���Ǻ����ͷų�Ա��������ʱ����Ҫ break��
	switch (m_dwInitMark)
	{
	case GB_IM_CSP_INITIALIZED:					// CSP �����ʼ����ϡ�
	{
		// �ͷ� CSP �����
		::CryptReleaseContext(m_hCSP, 0);
	}
	case GB_IM_TLS_INDEX_INITIALIZED:			// TLS ������ʼ����ϡ�
	{
		// �ͷ� TLS ������
		::TlsFree(m_dwTlsIndex);
	}
	case GB_IM_NOT_INITIALIZED:					// δ��ʼ����
	{

	}
	break;
	}
}

#pragma endregion
