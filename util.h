// Copyright (C) Anders Kjersem. Licensed under the zlib/libpng license, see License.txt for details.

#pragma once
#include "platform.h"

#ifndef _WIN64
FORCEINLINE HANDLE WINAPI GetCurrentProcess(){return ((HANDLE)(-1));}
FORCEINLINE HANDLE WINAPI GetCurrentThread(){return ((HANDLE)(-2));}
#endif


#if defined(_MSC_FULL_VER) && _MSC_VER >= 1400
#if _MSC_FULL_VER >= 140050727
#include <intrin.h>
#else
EXTERN_C void __stosb(BYTE*,BYTE,size_t);
#endif
#pragma intrinsic(__stosb)
#define MemSet(p,cb,v) __stosb((p),(v),(cb))
#else
void WINAPI MemSet(void*pMem,BYTE val,SIZE_T cb);
#endif
#define MemZero UACHlpr_MemZero
void WINAPI UACHlpr_MemZero(void*pMem,SIZE_T cb) ;
#define ZEROSTRUCT(r) MemZero(&(r),sizeof(r))
void WINAPI MemCopy(void*pD,void*pS,SIZE_T cb);
#define MySecureZeroMemory MemZero

DWORD GetSysVer(bool Major);
UINT_PTR StrToUIntPtr(LPTSTR s,bool ForceHEX=false,BOOL*pFoundBadChar=0);
inline UINT StrToUInt(LPTSTR s,bool ForceHEX=false,BOOL*pFoundBadChar=0) { return (UINT) StrToUIntPtr(s, ForceHEX, pFoundBadChar); }
LPTSTR StrSkipWhitespace(LPCTSTR s) ;
BOOL EnablePrivilege(LPCTSTR pszPrivilege,BOOL Enable,BOOL *pWasEnabled);

#define GetOSVerMaj() (GetSysVer(true))
#define GetOSVerMin() (GetSysVer(false))

inline LPTSTR StrNextChar(LPCTSTR Str) 
{ 
	return CharNext(Str); 
}

#define MyIsWindowVisible IsWindowVisible 

FORCEINLINE LRESULT SndDlgItemMsg(HWND hDlg,int id,UINT Msg,WPARAM wp=0,LPARAM lp=0) 
{
	return SendMessage(GetDlgItem(hDlg,id),Msg,wp,lp);
}

FORCEINLINE BOOL MySetDlgItemText(HWND hDlg,int id,LPCTSTR s) 
{
	return (BOOL)SndDlgItemMsg(hDlg,id,WM_SETTEXT,0,(LPARAM)s);
}

FORCEINLINE LONG_PTR WndModifyLong(HWND hwnd,int idx,LONG_PTR Mask,LONG_PTR Bits) 
{
	LONG_PTR Data=GetWindowLongPtr(hwnd,idx);
	Data=(Data&~Mask)|Bits;
	return SetWindowLongPtr(hwnd,idx,Data);
}


#if defined(_DEBUG) || defined(BUILD_DBGRELEASE)
#define ___BUILD_DBG
#define DBG_RESETDBGVIEW() do{HWND hDbgView=FindWindowA("dbgviewClass",0);PostMessage(hDbgView,WM_COMMAND,40020,0);if(0)SetForegroundWindow(hDbgView);}while(0)
#define _pp_MakeStr_(x)	#x
#define pp_MakeStr(x)	_pp_MakeStr_(x)
#define DBGONLY(_x) _x
#ifndef ASSERT
#	define ASSERT(_x) do{if(!(_x)){MessageBoxA(0,#_x##"\n\n"##__FILE__##":"## pp_MakeStr(__LINE__),"UAC SimpleAssert",MB_SYSTEMMODAL);}}while(0)
#endif
#define VERIFY(_x) ASSERT(_x)
#define TRACEA OutputDebugStringA
static void TRACEF(const char*fmt,...) 
{
	va_list a;va_start(a,fmt);
	static TCHAR b[1024*4];
	if (sizeof(TCHAR)!=sizeof(char))
	{
		static TCHAR fmtBuf[ARRAYSIZE(b)];
		VERIFY(wsprintf(fmtBuf,_T("%hs"),fmt)<ARRAYSIZE(fmtBuf));fmt=(LPCSTR)fmtBuf;
	}
	wvsprintf(b,(TCHAR*)fmt,a);OutputDebugString(b);
}
#else
#define ASSERT(x)
#define DBGONLY(_x) ASSERT(_x)
#define VERIFY(_x) ((void)(_x))
#define TRACEA
#define TRACEF TRACEA
#endif
void UAC_DbgHlpr_LoadPasswordInRunAs(HWND hDlg);
