// Copyright (c) 2016 �ʲ���All rights reserved��

#if !defined (_GB_ERROR_H_GB_20161026_)
#define _GB_ERROR_H_GB_20161026_

#pragma once

#include "gbrt.h"

#pragma pack(push, GBPACK)

// API ���ơ�
typedef enum _GBAPINAME
{
	GB_AN_NONE = 0,								// �� API ���ơ�
} GBAPINAME;

// �����롣
typedef enum _GBERRORCODE
{
	GB_EC_NO_ERROR = 0,							// û�д���
	GB_EC_API_ERROR,							// API ����
	GB_EC_NO_MEMORY,							// �ڴ治�㡣
	GB_EC_INVALID_PARAM,						// ��Ч������
	GB_EC_OVERFLOW,								// �����
	GB_EC_BUFF_INSUFFICIENT,					// ���治�㡣
} GBERRORCODE;

/*
������Ϣ��
-----------------------------------------------------------------------------------------------------------
| ������                       | ������Ϣһ             | ������Ϣ��             | ������Ϣ��             |
-----------------------------------------------------------------------------------------------------------
| GB_EC_NO_ERROR               | ��                     | ��                     | ��                     |
| GB_EC_API_ERROR              | API ������             | API ����               | ��                     |
| GB_EC_NO_MEMORY              | ��                     | ��                     | ��                     |
| GB_EC_INVALID_PARAM          | ������������ 1 ��ʼ    | ��                     | ��                     |
| GB_EC_OVERFLOW               | ��                     | ��                     | ��                     |
| GB_EC_BUFF_INSUFFICIENT      | ��                     | ��                     | ��                     |
-----------------------------------------------------------------------------------------------------------
*/

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
