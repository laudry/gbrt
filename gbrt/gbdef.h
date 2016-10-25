// Copyright (c) 2016 ¸Ê²¨¡£All rights reserved¡£

#if !defined (_GB_DEF_H_GB_20161025_)
#define _GB_DEF_H_GB_20161025_

#pragma once

#define GBPACK 8
#define GBPACKSHIFT 3

#define GBEXTERN_C extern "C"
#define GBEXTERN extern
#define GBSTATIC static
#define GBINLINE inline

#define GBNAKED __declspec(naked)
#define GBNOVTABLE __declspec(novtable)
#define GBUUID(_UUID) __declspec(uuid(_UUID))
#define GBINTERFACE(_UUID) struct GBNOVTABLE GBUUID(_UUID)

#define GBCDECLCALL __cdecl
#define GBSTDCALL __stdcall
#define GBFASTCALL __fastcall

#define GBANYSIZE 1

#define GBPTR2PTR(_PtrType, _Ptr) ((_PtrType)(LPVOID)(_Ptr))
#define GBINT2PTR(_PtrType, _Int) ((_PtrType)(LPVOID)(_Int))
#define GBPTR2INT(_IntType, _Ptr) ((_IntType)(LPVOID)(_Ptr))
#define GBINT2INT(_IntType, _Int) ((_IntType)(LPVOID)(_Int))

#define GBALIGNSIZE1(dwSize, dwAlign) ((((dwSize)-1)/(dwAlign)+1)*(dwAlign))
#define GBALIGNSIZE2(dwSize, dwShift) (((((dwSize)-1)>>(dwShift))+1)<<(dwShift))

#define GBHandleException(_Exception) (GetExceptionCode()==_Exception ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)

#endif
