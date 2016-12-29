// Copyright (c) 2016 �ʲ���All rights reserved��

#if !defined (_GB_OBJ_H_GB_20161229_)
#define _GB_OBJ_H_GB_20161229_

#pragma once

#include "gbrt.h"

#pragma pack(push, GBPACK)

// ����
class GBCLASS CGBObj
{
public:
	CGBObj(void);
	~CGBObj(void);

public:
	// �������á�
	virtual LONG AddRef(void);

	// �ͷš�
	virtual LONG Release(void);

public:
	// ��ȡ���ü�����
	LONG GetRefCount(void);

private:
	// ��Ա������
	LONG volatile m_lRefCount;					// ���ü�����

private:
	// ���á�
	CGBObj(const CGBObj &);
	CGBObj & operator = (const CGBObj &);
};

#pragma pack(pop)

#endif
