// Copyright (c) 2016 �ʲ���All rights reserved��

#include "stdafx.h"

#pragma region "����������"

//////////////////////////////////////////////////////////////////////////
// ����������

// ��ȡ�����롣
GBEXTERN_C GBAPI LONG GBSTDCALL GBGetLastError(void)
{
	LPGBTHREADDATA lpThreadData = GBGetThreadData();
	if (NULL == lpThreadData)
		return GB_EC_NO_MEMORY;
	else
		return lpThreadData->lErrorCode;
}

// ��ȡ�����뼰������Ϣ��
// ����ֵ�������롣
GBEXTERN_C GBAPI LONG GBSTDCALL GBGetLastErrorAndMsg(
	LPLONG lpErrorMsg1,							// OUT��������Ϣһ��
	LPLONG lpErrorMsg2,							// OUT��������Ϣ����
	LPLONG lpErrorMsg3							// OUT��������Ϣ����
)
{
	LPGBTHREADDATA lpThreadData = GBGetThreadData();
	if (NULL == lpThreadData)
	{
		*lpErrorMsg1 = 0;
		*lpErrorMsg2 = 0;
		*lpErrorMsg3 = 0;
		return GB_EC_NO_MEMORY;
	}
	else
	{
		*lpErrorMsg1 = lpThreadData->lErrorMsg1;
		*lpErrorMsg2 = lpThreadData->lErrorMsg2;
		*lpErrorMsg3 = lpThreadData->lErrorMsg3;
		return lpThreadData->lErrorCode;
	}
}

// ���ô����롣
GBEXTERN_C GBAPI void GBSTDCALL GBSetLastError(
	LONG lErrorCode								// IN�������롣
)
{
	LPGBTHREADDATA lpThreadData = GBGetThreadData();
	if (NULL != lpThreadData)
	{
		lpThreadData->lErrorCode = lErrorCode;
		lpThreadData->lErrorMsg1 = 0;
		lpThreadData->lErrorMsg2 = 0;
		lpThreadData->lErrorMsg3 = 0;
	}
}

// ���ô����뼰������Ϣ��
GBEXTERN_C GBAPI void GBSTDCALL GBSetLastErrorAndMsg(
	LONG lErrorCode,							// IN�������롣
	LONG lErrorMsg1,							// IN��������Ϣһ��
	LONG lErrorMsg2,							// IN��������Ϣ����
	LONG lErrorMsg3								// IN��������Ϣ����
)
{
	LPGBTHREADDATA lpThreadData = GBGetThreadData();
	if (NULL != lpThreadData)
	{
		lpThreadData->lErrorCode = lErrorCode;
		lpThreadData->lErrorMsg1 = lErrorMsg1;
		lpThreadData->lErrorMsg2 = lErrorMsg2;
		lpThreadData->lErrorMsg3 = lErrorMsg3;
	}
}

#pragma endregion
