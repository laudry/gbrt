// Copyright (c) 2016 �ʲ���All rights reserved��

#if !defined (_GB_THREAD_DATA_H_GB_20161026_)
#define _GB_THREAD_DATA_H_GB_20161026_

#pragma once

#include "gbrt.h"

#pragma pack(push, GBPACK)

// �߳����ݡ�
typedef struct _GBTHREADDATA
{
	LONG lErrorCode;							// �����롣
	LONG lErrorMsg1;							// ������Ϣһ��
	LONG lErrorMsg2;							// ������Ϣ����
	LONG lErrorMsg3;							// ������Ϣ����
	GBBUFFMGR BuffMgr;							// �����������
} GBTHREADDATA, *LPGBTHREADDATA;
typedef struct _GBTHREADDATA const *LPCGBTHREADDATA;

#pragma pack(pop)

// ��ȡ�߳����ݡ�
GBEXTERN_C GBAPI LPGBTHREADDATA GBSTDCALL GBGetThreadData(void);

// �ͷ��߳����ݡ�
GBEXTERN_C void GBSTDCALL GBFreeThreadData(
	LPGBTHREADDATA lpThreadData					// IN���߳����ݡ�
);

#endif
