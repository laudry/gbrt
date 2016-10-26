// Copyright (c) 2016 �ʲ���All rights reserved��

#if !defined (_GB_MODULE_DATA_H_GB_20161026_)
#define _GB_MODULE_DATA_H_GB_20161026_

#pragma once

#include "gbfsminner.h"

#pragma pack(push, GBPACK)

// ��ʼ���Ǻš�
typedef enum _GBINITMARK
{
	GB_IM_NOT_INITIALIZED = 0,					// δ��ʼ����
	GB_IM_TLS_INDEX_INITIALIZED,				// TLS ������ʼ����ϡ�
	GB_IM_CSP_INITIALIZED,						// CSP �����ʼ����ϡ�
	GB_IM_FSM_ALLOC_MGR_INITIALIZED,			// FSM ��������������ʼ����ϡ�
} GBINITMARK;

// ģ�����ݡ�
class CGBModuleData
{
public:
	CGBModuleData(void);
	~CGBModuleData(void);

	// ��ʼ��ģ�����ݡ�
	BOOL Init(
		HMODULE hModule							// IN��ģ������
	);

	// �ͷ�ģ�����ݡ�
	void Release(void);

public:
	// ��Ա������
	HMODULE m_hModule;							// ģ������
	DWORD m_dwInitMark;							// ��ʼ���Ǻš�
	DWORD m_dwTlsIndex;							// TLS ������
	HCRYPTPROV m_hCSP;							// CSP �����
	GBFSMALLOCMGR m_FSMAllocMgr;				// FSM ��������������
};

#pragma pack(pop)

// Ψһģ������������
GBEXTERN CGBModuleData ext_ModuleData;

#endif
