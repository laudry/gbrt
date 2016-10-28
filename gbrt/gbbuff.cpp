// Copyright (c) 2016 �ʲ���All rights reserved��

#include "stdafx.h"

#pragma region "����������"

//////////////////////////////////////////////////////////////////////////
// ����������

// ���仺�档
// ����ֵ�����档ʧ��ʱ���� NULL��
// ע������������ 8 �ֽڶ��룻��������С��Ĭ�ϻ�������ʱ��Ĭ�ϻ����������䡣
// ����ԭ��
// 1��������������Ĭ�ϻ�������ʱ���仺��������С���������Ĭ�ϻ���������С��
// 2�����仺��������Сʱֱ�ӷ��䣬����Ĭ�ϻ���������Сʱ�ӻ�����������䡣
GBEXTERN_C GBAPI LPGBBUFF GBSTDCALL GBAllocBuff(
	ULONG uBuffCapacity							// IN������������
)
{
	// ���������� 8 �ֽڶ��롣
	uBuffCapacity = GBALIGNSIZE2(uBuffCapacity, 3);

	// ���仺�档
	LPGBBUFF lpBuff;							// ����ֵ��
	if (uBuffCapacity > GB_DEFAULT_BUFF_CAPACITY)
		lpBuff = GBPTR2PTR(LPGBBUFF, malloc(sizeof(GBBUFF) + uBuffCapacity));
	else
	{
		LPGBTHREADDATA lpThreadData = GBGetThreadData();
		lpBuff = GBAllocBuffFromBuffMgr(NULL == lpThreadData ? NULL : &lpThreadData->BuffMgr);
	}
	if (NULL == lpBuff)
	{
		GBSetLastError(GB_EC_NO_MEMORY);
		return NULL;
	}

	// ��ʼ�����档
	lpBuff->BuffListItem.lpNext = lpBuff->BuffListItem.lpPrev = NULL;
	lpBuff->uBuffCapacity = max(uBuffCapacity, GB_DEFAULT_BUFF_CAPACITY);
	lpBuff->Buff.len = 0;
	lpBuff->Buff.buf = GBPTR2PTR(CHAR FAR *, lpBuff) + sizeof(GBBUFF);

	return lpBuff;
}

// ����ָ���������档
// ����ֵ�����档ʧ��ʱ���� NULL��
// ע������������ 8 �ֽڶ��롣
// ����ԭ��
// 1����������������Ĭ�ϻ�������ʱ���仺��������С���������Ĭ�ϻ���������С��
// 2�����仺��������Сʱֱ�ӷ��䣬����Ĭ�ϻ���������Сʱ�ӻ�����������䡣
GBEXTERN_C GBAPI LPGBBUFF GBSTDCALL GBAllocAppointedCapacityBuff(
	ULONG uBuffCapacity							// IN������������
)
{
	// ���������� 8 �ֽڶ��롣
	uBuffCapacity = GBALIGNSIZE2(uBuffCapacity, 3);

	// ���仺�档
	LPGBBUFF lpBuff;							// ����ֵ��
	if (uBuffCapacity != GB_DEFAULT_BUFF_CAPACITY)
		lpBuff = GBPTR2PTR(LPGBBUFF, malloc(sizeof(GBBUFF) + uBuffCapacity));
	else
	{
		LPGBTHREADDATA lpThreadData = GBGetThreadData();
		lpBuff = GBAllocBuffFromBuffMgr(NULL == lpThreadData ? NULL : &lpThreadData->BuffMgr);
	}
	if (NULL == lpBuff)
	{
		GBSetLastError(GB_EC_NO_MEMORY);
		return NULL;
	}

	// ��ʼ�����档
	lpBuff->BuffListItem.lpNext = lpBuff->BuffListItem.lpPrev = NULL;
	lpBuff->uBuffCapacity = uBuffCapacity;
	lpBuff->Buff.len = 0;
	lpBuff->Buff.buf = GBPTR2PTR(CHAR FAR *, lpBuff) + sizeof(GBBUFF);

	return lpBuff;
}

// �ͷŻ��档
// �ͷ�ԭ�򣺻�������������Ĭ�ϻ�������ʱֱ���ͷţ������ͷŵ������������
GBEXTERN_C GBAPI void GBSTDCALL GBFreeBuff(
	LPGBBUFF lpBuff								// IN�����档
)
{
	// �ͷŻ��档
	if (lpBuff->uBuffCapacity != GB_DEFAULT_BUFF_CAPACITY)
		free(lpBuff);
	else
	{
		LPGBTHREADDATA lpThreadData = GBGetThreadData();
		GBFreeBuffToBuffMgr(NULL == lpThreadData ? NULL : &lpThreadData->BuffMgr, lpBuff);
	}
}

// �����ͷŻ��档
// �ͷ�ԭ�򣺻�������������Ĭ�ϻ�������ʱֱ���ͷţ������ͷŵ������������
GBEXTERN_C GBAPI void GBSTDCALL GBFreeBuffInBatch(
	LPGBLIST lpBuffList							// INOUT����������
)
{
	// ��ȡ�����������
	LPGBTHREADDATA lpThreadData = GBGetThreadData();
	LPGBBUFFMGR lpBuffMgr = (NULL == lpThreadData ? NULL : &lpThreadData->BuffMgr);

	// �����ͷŻ��档
	LPGBLIST lpListItem = lpBuffList->lpNext;
	while (lpListItem != lpBuffList)
	{
		LPGBBUFF lpBuff = GBPTR2PTR(LPGBBUFF, lpListItem);
		lpListItem = lpListItem->lpNext;
		GBDelListItem(&lpBuff->BuffListItem);
		if (lpBuff->uBuffCapacity != GB_DEFAULT_BUFF_CAPACITY)
			free(lpBuff);
		else
			GBFreeBuffToBuffMgr(lpBuffMgr, lpBuff);
	}
}

#pragma endregion
