// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <windows.h>
#include <wincrypt.h>

#define GBAPI __declspec(dllexport)
#define GBVARIABLE __declspec(dllexport)
#define GBCLASS __declspec(dllexport)

#if !defined (_DEBUG)
	#include <malloc.h>
#else											// _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif											// _DEBUG
#include <tchar.h>

#include "gbassert.h"
