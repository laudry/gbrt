// Copyright (c) 2016 �ʲ���All rights reserved��

#if !defined (_GB_OBJ_LOCK_H_GB_20161027_)
#define _GB_OBJ_LOCK_H_GB_20161027_

#pragma once

#include "gbrt.h"
#include "gbassert.h"

// ��������
// ����ֵ�������������ʧ��ʱ���� NULL��
GBEXTERN_C GBAPI HANDLE GBSTDCALL GBLockObj(
	LPCVOID lpObj								// IN������
);

// ������������
// ����ֵ�������������ʧ��ʱ���� NULL�����������߳�����ʱ���� INVALID_HANDLE_VALUE��
GBEXTERN_C GBAPI HANDLE GBSTDCALL GBTryLockObj(
	LPCVOID lpObj								// IN������
);

// ��������
// ע���˺������ڸ��ݶ��������������������
GBEXTERN_C GBAPI void GBSTDCALL GBUnlockObj(
	LPCVOID lpObj								// IN������
);

// ���ݶ����������������
GBEXTERN_C GBAPI void GBSTDCALL GBUnlockObjByObjLock(
	HANDLE hObjLock								// IN�������������
);

// �ж��Ƿ��������
GBEXTERN_C GBAPI BOOL GBSTDCALL GBIsObjLock(
	HANDLE hObjLock								// IN�������������
);

// �ж϶����Ƿ�������
GBEXTERN_C GBAPI BOOL GBSTDCALL GBIsObjLocked(
	LPCVOID lpObj								// IN������
);

#pragma pack(push, GBPACK)

// ��������
class CGBObjLock
{
public:
	explicit CGBObjLock(
		LPCVOID lpObj							// IN������
	)
	{
		m_hObjLock = GBLockObj(lpObj);
		GBASSERT(NULL != m_hObjLock);
	}
	~CGBObjLock(void)
	{
		if (NULL != m_hObjLock)
		{
			GBUnlockObjByObjLock(m_hObjLock);
		}
	}

protected:
	// ������Ա������
	HANDLE m_hObjLock;							// �����������

private:
	// ���á�
	CGBObjLock(const CGBObjLock &);
	CGBObjLock & operator = (const CGBObjLock &);
};

#pragma pack(pop)

#endif
