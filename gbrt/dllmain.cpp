// Copyright (c) 2016 �ʲ���All rights reserved��
// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣

#include "stdafx.h"

// dll ��ں�����
GBEXTERN_C BOOL APIENTRY DllMain(
	HMODULE hModule,							// IN��ģ������
	DWORD  ul_reason_for_call,					// IN��������ں������ɡ�
	LPVOID lpReserved							// IN��������
	)
{
	// �жϵ�����ں������ɡ�
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:					// ���ӵ����̡�
	{
		// ��ʼ��ģ�����ݡ�
		if (!ext_ModuleData.Init(hModule))
			return FALSE;
	}
	break;
	case DLL_THREAD_ATTACH:						// ���ӵ��̡߳�
	{
	}
	break;
	case DLL_THREAD_DETACH:						// ���̷߳��롣
	{
		// �ͷ��߳����ݡ�
		LPGBTHREADDATA lpThreadData = GBPTR2PTR(LPGBTHREADDATA, ::TlsGetValue(ext_ModuleData.m_dwTlsIndex));
		if (NULL != lpThreadData)
		{
			::TlsSetValue(ext_ModuleData.m_dwTlsIndex, NULL);
			GBFreeThreadData(lpThreadData);
		}
	}
	break;
	case DLL_PROCESS_DETACH:					// ����̷��롣
	{
		// �ͷ��߳����ݡ�
		if (ext_ModuleData.m_dwInitMark >= GB_IM_TLS_INDEX_INITIALIZED)
		{
			LPGBTHREADDATA lpThreadData = GBPTR2PTR(LPGBTHREADDATA, ::TlsGetValue(ext_ModuleData.m_dwTlsIndex));
			if (NULL != lpThreadData)
			{
				::TlsSetValue(ext_ModuleData.m_dwTlsIndex, NULL);
				GBFreeThreadData(lpThreadData);
			}
		}

		// �ͷ�ģ�����ݡ�
		ext_ModuleData.Release();
	}
	break;
	}
	return TRUE;
}
