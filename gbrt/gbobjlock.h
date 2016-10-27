// Copyright (c) 2016 甘波。All rights reserved。

#if !defined (_GB_OBJ_LOCK_H_GB_20161027_)
#define _GB_OBJ_LOCK_H_GB_20161027_

#pragma once

#include "gbrt.h"
#include "gbassert.h"

// 锁定对象。
// 返回值：对象锁句柄。失败时返回 NULL。
GBEXTERN_C GBAPI HANDLE GBSTDCALL GBLockObj(
	LPCVOID lpObj								// IN：对象。
);

// 尝试锁定对象。
// 返回值：对象锁句柄。失败时返回 NULL；对象被其它线程锁定时返回 INVALID_HANDLE_VALUE。
GBEXTERN_C GBAPI HANDLE GBSTDCALL GBTryLockObj(
	LPCVOID lpObj								// IN：对象。
);

// 解锁对象。
// 注：此函数慢于根据对象锁句柄解锁对象函数。
GBEXTERN_C GBAPI void GBSTDCALL GBUnlockObj(
	LPCVOID lpObj								// IN：对象。
);

// 根据对象锁句柄解锁对象。
GBEXTERN_C GBAPI void GBSTDCALL GBUnlockObjByObjLock(
	HANDLE hObjLock								// IN：对象锁句柄。
);

// 判断是否对象锁。
GBEXTERN_C GBAPI BOOL GBSTDCALL GBIsObjLock(
	HANDLE hObjLock								// IN：对象锁句柄。
);

// 判断对象是否被锁定。
GBEXTERN_C GBAPI BOOL GBSTDCALL GBIsObjLocked(
	LPCVOID lpObj								// IN：对象。
);

#pragma pack(push, GBPACK)

// 对象锁。
class CGBObjLock
{
public:
	explicit CGBObjLock(
		LPCVOID lpObj							// IN：对象。
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
	// 保护成员变量。
	HANDLE m_hObjLock;							// 对象锁句柄。

private:
	// 禁用。
	CGBObjLock(const CGBObjLock &);
	CGBObjLock & operator = (const CGBObjLock &);
};

#pragma pack(pop)

#endif
