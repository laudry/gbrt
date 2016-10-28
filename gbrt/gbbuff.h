// Copyright (c) 2016 �ʲ���All rights reserved��

#if !defined (_GB_BUFF_H_GB_20161028_)
#define _GB_BUFF_H_GB_20161028_

#pragma once

#include "gblist.h"
#include "gbrt.h"

#pragma pack(push, GBPACK)

// ���档
typedef struct _GBBUFF
{
	GBLIST BuffListItem;						// ���������
	ULONG uBuffCapacity;						// ����������
	WSABUF Buff;								// ���档
} GBBUFF, *LPGBBUFF;
typedef struct _GBBUFF const *LPCGBBUFF;

// Ĭ�ϻ���������
#define GB_DEFAULT_BUFF_CAPACITY 8064

#pragma pack(pop)

// ���仺�档
// ����ֵ�����档ʧ��ʱ���� NULL��
// ע������������ 8 �ֽڶ��룻��������С��Ĭ�ϻ�������ʱ��Ĭ�ϻ����������䡣
GBEXTERN_C GBAPI LPGBBUFF GBSTDCALL GBAllocBuff(
	ULONG uBuffCapacity							// IN������������
);

// ����ָ���������档
// ����ֵ�����档ʧ��ʱ���� NULL��
// ע������������ 8 �ֽڶ��롣
GBEXTERN_C GBAPI LPGBBUFF GBSTDCALL GBAllocAppointedCapacityBuff(
	ULONG uBuffCapacity							// IN������������
);

// �ͷŻ��档
GBEXTERN_C GBAPI void GBSTDCALL GBFreeBuff(
	LPGBBUFF lpBuff								// IN�����档
);

// �����ͷŻ��档
GBEXTERN_C GBAPI void GBSTDCALL GBFreeBuffInBatch(
	LPGBLIST lpBuffList							// INOUT����������
);

#endif
