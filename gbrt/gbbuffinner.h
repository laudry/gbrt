// Copyright (c) 2016 �ʲ���All rights reserved��

#if !defined (_GB_BUFF_INNER_H_GB_20161028_)
#define _GB_BUFF_INNER_H_GB_20161028_

#pragma once

#include "gbbuff.h"

#pragma pack(push, GBPACK)

// �����������
typedef struct _GBBUFFMGR
{
	ULONG uFreeBuffCount;						// ���л�������
	GBLIST FreeBuffList;						// ���л�������
} GBBUFFMGR, *LPGBBUFFMGR;
typedef struct _GBBUFFMGR const *LPCGBBUFFMGR;

// �����л�������
#define GB_MAX_FREE_BUFF_COUNT 32

#pragma pack(pop)

// �ӻ�����������仺�档
GBEXTERN_C LPGBBUFF GBSTDCALL GBAllocBuffFromBuffMgr(
	LPGBBUFFMGR lpBuffMgr						// IN�������������
);

// �ͷŻ��浽�����������
GBEXTERN_C void GBSTDCALL GBFreeBuffToBuffMgr(
	LPGBBUFFMGR lpBuffMgr,						// IN�������������
	LPGBBUFF lpBuff								// IN�����档
);

// ��ʼ�������������
GBEXTERN_C void GBSTDCALL GBInitBuffMgr(
	LPGBBUFFMGR lpBuffMgr						// INOUT�������������
);

// �ͷŻ����������
GBEXTERN_C void GBSTDCALL GBReleaseBuffMgr(
	LPGBBUFFMGR lpBuffMgr						// INOUT�������������
);

#endif
