// Copyright (c) 2016 �ʲ���All rights reserved��

#include "stdafx.h"

#pragma region "�ڲ�������"

//////////////////////////////////////////////////////////////////////////
// �ڲ�������

// �ӻ�����������仺�档
// ����ԭ���ȳ���ʹ�û���������Ŀ��л��棬û�п��л���ʱֱ�ӷ��䡣
GBEXTERN_C LPGBBUFF GBSTDCALL GBAllocBuffFromBuffMgr(
	LPGBBUFFMGR lpBuffMgr						// IN�������������
)
{
	// ���仺�档
	LPGBBUFF lpBuff;							// ����ֵ��
	if (NULL != lpBuffMgr && 0 != lpBuffMgr->uFreeBuffCount)
	{
		lpBuff = GBPTR2PTR(LPGBBUFF, lpBuffMgr->FreeBuffList.lpNext);
		GBDelListItem(&lpBuff->BuffListItem);
		--lpBuffMgr->uFreeBuffCount;
	}
	else
		lpBuff = GBPTR2PTR(LPGBBUFF, malloc(sizeof(GBBUFF) + GB_DEFAULT_BUFF_CAPACITY));

	return lpBuff;
}

// �ͷŻ��浽�����������
// �ͷ�ԭ���ȳ��Լ��뵽����������Ŀ��л��������޷�����ʱֱ���ͷš�
GBEXTERN_C void GBSTDCALL GBFreeBuffToBuffMgr(
	LPGBBUFFMGR lpBuffMgr,						// IN�������������
	LPGBBUFF lpBuff								// IN�����档
)
{
	// �ͷŻ��档
	if (NULL != lpBuffMgr && lpBuffMgr->uFreeBuffCount < GB_MAX_FREE_BUFF_COUNT)
	{
		GBInsertAfterListItem(&lpBuffMgr->FreeBuffList, &lpBuff->BuffListItem);
		++lpBuffMgr->uFreeBuffCount;
	}
	else
		free(lpBuff);
}

// ��ʼ�������������
GBEXTERN_C void GBSTDCALL GBInitBuffMgr(
	LPGBBUFFMGR lpBuffMgr						// INOUT�������������
)
{
	// ��ʼ�������������
	lpBuffMgr->uFreeBuffCount = 0;
	GBInitList(&lpBuffMgr->FreeBuffList);
}

// �ͷŻ����������
GBEXTERN_C void GBSTDCALL GBReleaseBuffMgr(
	LPGBBUFFMGR lpBuffMgr						// INOUT�������������
)
{
	// �ͷſ��л��档
	LPGBLIST lpListItem = lpBuffMgr->FreeBuffList.lpNext;
	while (lpListItem != &lpBuffMgr->FreeBuffList)
	{
		LPGBBUFF lpBuff = GBPTR2PTR(LPGBBUFF, lpListItem);
		lpListItem = lpListItem->lpNext;
		GBDelListItem(&lpBuff->BuffListItem);
		--lpBuffMgr->uFreeBuffCount;
		free(lpBuff);
	}
}

#pragma endregion
