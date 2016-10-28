// Copyright (c) 2016 �ʲ���All rights reserved��

#include "stdafx.h"

#pragma region "��̬������"

//////////////////////////////////////////////////////////////////////////
// ��̬������

// ��ʼ���߳����ݡ�
GBEXTERN_C GBSTATIC void GBSTDCALL GBInitThreadData(
	LPGBTHREADDATA lpThreadData					// OUT���߳����ݡ�
)
{
	// �����㡣
	::ZeroMemory(lpThreadData, sizeof(GBTHREADDATA));

	// ��ʼ����Ҫ�ر��ʼ���ĳ�Ա������
	GBInitBuffMgr(&lpThreadData->BuffMgr);
}

#pragma endregion

#pragma region "�ڲ�������"

//////////////////////////////////////////////////////////////////////////
// �ڲ�������

// �ͷ��߳����ݡ�
GBEXTERN_C void GBSTDCALL GBFreeThreadData(
	LPGBTHREADDATA lpThreadData					// IN���߳����ݡ�
)
{
	// �ͷ���Ҫ�ر��ͷŵĳ�Ա������
	GBReleaseBuffMgr(&lpThreadData->BuffMgr);

	// �ͷ��߳������ڴ档
	free(lpThreadData);
}

#pragma endregion

#pragma region "����������"

//////////////////////////////////////////////////////////////////////////
// ����������

// ��ȡ�߳����ݡ�
GBEXTERN_C GBAPI LPGBTHREADDATA GBSTDCALL GBGetThreadData(void)
{
	// ���ȳ��Ի�ȡ�����߳����ݡ�
	LPGBTHREADDATA lpThreadData = GBPTR2PTR(LPGBTHREADDATA, ::TlsGetValue(ext_ModuleData.m_dwTlsIndex));
	if (NULL != lpThreadData)
		return lpThreadData;

	// �������߳����ݲ�����ʱ���������߳����ݡ�
	lpThreadData = GBPTR2PTR(LPGBTHREADDATA, malloc(sizeof(GBTHREADDATA)));
	if (NULL == lpThreadData)
		return NULL;

	// ��ʼ�����߳����ݡ�
	GBInitThreadData(lpThreadData);

	// �������߳����ݡ�
	if (::TlsSetValue(ext_ModuleData.m_dwTlsIndex, lpThreadData))
		return lpThreadData;

	// �������߳�����ʧ��ʱ�ͷ����߳����ݡ�
	GBFreeThreadData(lpThreadData);
	return NULL;
}

#pragma endregion
