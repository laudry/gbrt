// Copyright (c) 2016 �ʲ���All rights reserved��

#if !defined (_GB_ERROR_H_GB_20161026_)
#define _GB_ERROR_H_GB_20161026_

#pragma once

#include "gbrt.h"

#pragma pack(push, GBPACK)

// ��������
typedef enum _GBFUNCNAME
{
	GB_FN_NONE = 0,								// �޺�������
} GBFUNCNAME;

// �����롣
typedef enum _GBERRORCODE
{
	GB_EC_NO_ERROR = 0,							// û�д���
	GB_EC_API_ERROR,							// API ���󡣴�����Ϣһ��API �Ĵ����룻������Ϣ����API �ĺ�������
	GB_EC_NO_MEMORY,							// �ڴ治�㡣
	GB_EC_INVALID_PARAM,						// ��Ч������������Ϣһ����Ч������������ 1 ��ʼ��
	GB_EC_OVERFLOW,								// �����
} GBERRORCODE;

#pragma pack(pop)

// ��ȡ�����롣
GBEXTERN_C GBAPI LONG GBSTDCALL GBGetLastError(void);

// ��ȡ�����뼰������Ϣ��
// ����ֵ�������롣
GBEXTERN_C GBAPI LONG GBSTDCALL GBGetLastErrorAndMsg(
	LPLONG lpErrorMsg1,							// OUT��������Ϣһ��
	LPLONG lpErrorMsg2,							// OUT��������Ϣ����
	LPLONG lpErrorMsg3							// OUT��������Ϣ����
);

// ���ô����롣
GBEXTERN_C GBAPI void GBSTDCALL GBSetLastError(
	LONG lErrorCode								// IN�������롣
);

// ���ô����뼰������Ϣ��
GBEXTERN_C GBAPI void GBSTDCALL GBSetLastErrorAndMsg(
	LONG lErrorCode,							// IN�������롣
	LONG lErrorMsg1,							// IN��������Ϣһ��
	LONG lErrorMsg2,							// IN��������Ϣ����
	LONG lErrorMsg3								// IN��������Ϣ����
);

#endif
