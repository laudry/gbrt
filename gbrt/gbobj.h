// Copyright (c) 2016 甘波。All rights reserved。

#if !defined (_GB_OBJ_H_GB_20161229_)
#define _GB_OBJ_H_GB_20161229_

#pragma once

#include "gbrt.h"

#pragma pack(push, GBPACK)

// 对象。
class GBCLASS CGBObj
{
public:
	CGBObj(void);
	~CGBObj(void);

public:
	// 增加引用。
	virtual LONG AddRef(void);

	// 释放。
	virtual LONG Release(void);

public:
	// 获取引用计数。
	LONG GetRefCount(void);

private:
	// 成员变量。
	LONG volatile m_lRefCount;					// 引用计数。

private:
	// 禁用。
	CGBObj(const CGBObj &);
	CGBObj & operator = (const CGBObj &);
};

#pragma pack(pop)

#endif
