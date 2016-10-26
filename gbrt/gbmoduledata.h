// Copyright (c) 2016 甘波。All rights reserved。

#if !defined (_GB_MODULE_DATA_H_GB_20161026_)
#define _GB_MODULE_DATA_H_GB_20161026_

#pragma once

#include "gbfsminner.h"

#pragma pack(push, GBPACK)

// 初始化记号。
typedef enum _GBINITMARK
{
	GB_IM_NOT_INITIALIZED = 0,					// 未初始化。
	GB_IM_TLS_INDEX_INITIALIZED,				// TLS 索引初始化完毕。
	GB_IM_CSP_INITIALIZED,						// CSP 句柄初始化完毕。
	GB_IM_FSM_ALLOC_MGR_INITIALIZED,			// FSM 分配器管理器初始化完毕。
} GBINITMARK;

// 模块数据。
class CGBModuleData
{
public:
	CGBModuleData(void);
	~CGBModuleData(void);

	// 初始化模块数据。
	BOOL Init(
		HMODULE hModule							// IN：模块句柄。
	);

	// 释放模块数据。
	void Release(void);

public:
	// 成员变量。
	HMODULE m_hModule;							// 模块句柄。
	DWORD m_dwInitMark;							// 初始化记号。
	DWORD m_dwTlsIndex;							// TLS 索引。
	HCRYPTPROV m_hCSP;							// CSP 句柄。
	GBFSMALLOCMGR m_FSMAllocMgr;				// FSM 分配器管理器。
};

#pragma pack(pop)

// 唯一模块数据声明。
GBEXTERN CGBModuleData ext_ModuleData;

#endif
