// Copyright (c) 2016 甘波。All rights reserved。
// dllmain.cpp : 定义 DLL 应用程序的入口点。

#include "stdafx.h"

// dll 入口函数。
GBEXTERN_C BOOL APIENTRY DllMain(
	HMODULE hModule,							// IN：模块句柄。
	DWORD  ul_reason_for_call,					// IN：调用入口函数理由。
	LPVOID lpReserved							// IN：保留。
	)
{
	// 判断调用入口函数理由。
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:					// 附加到进程。
	{
		// 初始化模块数据。
		if (!ext_ModuleData.Init(hModule))
			return FALSE;
	}
	break;
	case DLL_THREAD_ATTACH:						// 附加到线程。
	{
	}
	break;
	case DLL_THREAD_DETACH:						// 与线程分离。
	{
		// 释放线程数据。
		LPGBTHREADDATA lpThreadData = GBPTR2PTR(LPGBTHREADDATA, ::TlsGetValue(ext_ModuleData.m_dwTlsIndex));
		if (NULL != lpThreadData)
		{
			::TlsSetValue(ext_ModuleData.m_dwTlsIndex, NULL);
			GBFreeThreadData(lpThreadData);
		}
	}
	break;
	case DLL_PROCESS_DETACH:					// 与进程分离。
	{
		// 释放线程数据。
		if (ext_ModuleData.m_dwInitMark >= GB_IM_TLS_INDEX_INITIALIZED)
		{
			LPGBTHREADDATA lpThreadData = GBPTR2PTR(LPGBTHREADDATA, ::TlsGetValue(ext_ModuleData.m_dwTlsIndex));
			if (NULL != lpThreadData)
			{
				::TlsSetValue(ext_ModuleData.m_dwTlsIndex, NULL);
				GBFreeThreadData(lpThreadData);
			}
		}

		// 释放模块数据。
		ext_ModuleData.Release();
	}
	break;
	}
	return TRUE;
}
