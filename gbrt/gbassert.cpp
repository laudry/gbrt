// Copyright (c) 2016 �ʲ���All rights reserved��

#include "stdafx.h"
#include <assert.h>

#pragma region "����������"

//////////////////////////////////////////////////////////////////////////
// ����������

#if defined (_DEBUG)

// ���ԡ����� C ��׼��Ķ��Ժ����ڶ��̻߳�������ʱ���������Ӵ˺�����
// ע�������� dll ����ں����е��ô˺�����
GBEXTERN_C GBAPI void GBSTDCALL GBAssert(
	LPCWSTR lpszMsg,							// IN����Ϣ��
	LPCWSTR lpszFile,							// IN���ļ���
	UINT uLine									// IN���ڼ��С�
	)
{
	// �������Ժ�����
//	CGBObjLock ObjLock(&GBAssert);

	// ���� C ��׼��Ķ��Ժ�����
	_wassert(lpszMsg, lpszFile, uLine);
}

#endif

#pragma endregion
